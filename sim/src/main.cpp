#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <State.h>
Adafruit_NeoPixel eyes(128, 5, NEO_GRB + NEO_KHZ800);
#include <Eyes.h>
Servo lSer;
Servo fSer;
Servo rSer;
Servo neckSer;
#include <Head.h>
SoftwareSerial softSerial(7, 8);
DFRobotDFPlayerMini dfpPlayer;
#include <Audio.h>
#include <Sensor.h>

const unsigned int S = 10;
static uint8_t State = IDLE; 
bool audioConnected = false;

void setup() {
    Serial.begin(9600);
    softSerial.begin(9600);
    pinMode(2, INPUT);
    eyes.begin();
    //eyes.setBrightness(25);
    draw(SLEEP);
    lSer.attach(9);
    fSer.attach(10);
    rSer.attach(11);
    neckSer.attach(3);
    resetHead();

    //TODO: Why is this below breaking the servos (and eyes?)?
    delay(1000);
    unsigned long startTime = millis();
    while (!dfpPlayer.begin(softSerial)) {
        if (millis() - startTime > 5000) {
            State = ERROR;
            break;
        }
        State = ERROR;
    }

    if (dfpPlayer.begin(softSerial, true, true)) {
        audioConnected = true;
        dfpPlayer.volume(25);
        dfpPlayer.play(1);
    }

    Serial.println(F("READY"));
}

unsigned long lastInteractionTime = 0;

void loop() {
    unsigned long currentMillis = millis();

    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        if (command == "WIN")
            State = LOVE;
        else if (command == "LOSE")
            State = random(0, 2) == 0 ? SAD : ANGRY;
        else if (command == "TIE")
            State = HAPPY;
        else
            State = CURIOUS;
        lastInteractionTime = currentMillis;
    }

    if (sensorLoop(&State, currentMillis)) lastInteractionTime = currentMillis;
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
    if (audioConnected) audioLoop(State);

    if (State == GAMESTART && currentMillis - lastInteractionTime > 5000) {
        State = HAPPY;
        lastInteractionTime = currentMillis;
    }
    else if (currentMillis - lastInteractionTime > 30000) {
        State = IDLE;
        lastInteractionTime = currentMillis;
    }
}