#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
const bool DEBUG = false;
#include <StateQueue.h>
#include <State.h>
Adafruit_NeoPixel eyes(128, 5, NEO_GRB + NEO_KHZ800);
#include <Eyes.h>
Servo lSer;
Servo bSer;
Servo rSer;
Servo neckSer;
#include <Head.h>
SoftwareSerial softSerial(7, 8);
DFRobotDFPlayerMini dfPlayer;
bool audioConnected = false;
#include <Audio.h>
#include <Sensor.h>

const unsigned int S = 10;
static uint8_t State = IDLE; 
static uint8_t SelectedActuator = 0;

void setup() {
    Serial.begin(9600);
    pinMode(2, INPUT);
    eyes.begin();
    draw(CLOSED);
    eyes.show();
    lSer.attach(9);
    bSer.attach(10);
    rSer.attach(11);
    neckSer.attach(3);
    resetHead();

    delay(1000);
    if (!connectAudio()) State = ERROR;

    Serial.println(F("READY"));
}

unsigned long lastInteractionTime = 0;

void loop() {
    delay(1);
    unsigned long currentMillis = millis();

    if (DEBUG && currentMillis % 1000 == 0) {
        Serial.print("State: ");
        Serial.println(State);
    }

    if (Serial.available() > 0) {
        stateQueue.clear();
        String command = Serial.readStringUntil('\n');
        if (DEBUG) {
            Serial.print("Command: ");
            Serial.println(command);
        }
        if (command.startsWith("FOUND")) {
            int firstSpace = command.indexOf(' ');
            int secondSpace = command.indexOf(' ', firstSpace + 1);
            int people = 0, angle = 0;
            if (firstSpace != -1 && secondSpace != -1) {
                people = command.substring(firstSpace + 1, secondSpace).toInt();
                angle = command.substring(secondSpace + 1).toInt();
                neckSer.write(angle);
            }
            if (DEBUG) {
                Serial.print("FOUND people: ");
                Serial.print(people);
                Serial.print(", angle: ");
                Serial.println(angle);
            }
            State = CURIOUS;
        }
        else if (command == "WIN" && (SelectedActuator == 1 || SelectedActuator == 2)) {
            if (SelectedActuator == 1) State = OA1WIN;
            else if (SelectedActuator == 2) State = OA2WIN;
            scheduleState(LOVE, 5000);
        }
        else if (command == "LOSE" && (SelectedActuator == 1 || SelectedActuator == 2)) {
            if (SelectedActuator == 1) State = OA1LOSE;
            else if (SelectedActuator == 2) State = OA2LOSE;
            scheduleState(random(0, 2) == 0 ? SAD : ANGRY, 5000);
        }
        else if (command == "TIE" && (SelectedActuator == 1 || SelectedActuator == 2)) {
            if (SelectedActuator == 1) State = OA1TIE;
            else if (SelectedActuator == 2) State = OA2TIE;
            scheduleState(SILLY, 8000);
        }
        else if (command.startsWith("OA2")) {
            int toPlay = command.substring(3).toInt() + 30;
            if (DEBUG) {
                Serial.print("toPlay: ");
                Serial.println(toPlay);
            }
            if (toPlay > 0 && audioConnected && dfPlayer.readCurrentFileNumber() != toPlay) {
                if (DEBUG) Serial.println("Playing audio...");
                dfPlayer.playMp3Folder(toPlay);
            }
        }
        lastInteractionTime = currentMillis;
        if (!audioConnected) connectAudio();
    }

    if (sensorLoop(&State, currentMillis, &SelectedActuator)) {
        if (State == GAMESTART) lastInteractionTime = currentMillis;
    }
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
    if (audioConnected) audioLoop(State, currentMillis);

    while (stateQueue.count() > 0 && currentMillis >= stateQueue.peek().time) {
        State = stateQueue.pop().state;
        lastInteractionTime = currentMillis;
        if (DEBUG) {
            Serial.print("Transitioned to scheduled state: ");
            Serial.println(State);
        }
    }
    if (currentMillis - lastInteractionTime > 30000) {
        State = IDLE;
        lastInteractionTime = currentMillis;
        SelectedActuator = 0;
        Serial.println(F("START MOVE"));
    }
}