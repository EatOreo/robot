#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
const bool DEBUG = true;
#include <State.h>
Adafruit_NeoPixel eyes(128, 5, NEO_GRB + NEO_KHZ800);
#include <Eyes.h>
Servo lSer;
Servo fSer;
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

void setup() {
    Serial.begin(9600);
    pinMode(2, INPUT);
    eyes.begin();
    //eyes.setBrightness(25);
    draw(CLOSED);
    eyes.show();
    lSer.attach(9);
    fSer.attach(10);
    rSer.attach(11);
    neckSer.attach(3);
    resetHead();

    //TODO: Why is this below breaking the servos (and eyes?)?
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

    //TODO: take message when person found and look suddenly at them
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        if (DEBUG) {
            Serial.print("Command: ");
            Serial.println(command);
        }
        if (command == "WIN")
            State = LOVE;
        else if (command == "LOSE")
            State = random(0, 2) == 0 ? SAD : ANGRY;
        else if (command == "TIE")
            State = HAPPY;
        lastInteractionTime = currentMillis;
        if (!audioConnected) connectAudio();
    }

    if (sensorLoop(&State, currentMillis)) {
        lastInteractionTime = currentMillis;
    }
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
    if (audioConnected) audioLoop(State, currentMillis);

    if (State == GAMESTART && currentMillis - lastInteractionTime > 6000) {
        State = HAPPY;
        lastInteractionTime = currentMillis;
    }
    else if (currentMillis - lastInteractionTime > 10000) {
        State = IDLE;
        lastInteractionTime = currentMillis;
    }
}