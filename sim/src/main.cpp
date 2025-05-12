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
static uint8_t InControl = SELF;
bool audioConnected = false;

void setup() {
    Serial.begin(9600);
    softSerial.begin(9600);
    pinMode(2, INPUT);
    eyes.begin();
    //eyes.setBrightness(25);
    lSer.attach(9);
    fSer.attach(10);
    rSer.attach(11);
    neckSer.attach(3);
    resetHead();

    delay(1000);
    if (!dfpPlayer.begin(softSerial)) {
        State = ERROR;
    }
    else {
        audioConnected = true;
        dfpPlayer.volume(5);
    }

    Serial.println(F("READY"));
}

unsigned long lastInteractionTime = 0;

void loop() {
    unsigned long currentMillis = millis();

    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        // TODO: recieve ack from actuator after calling them to start and set InControl
        if (command == "OA1 ACK") {
            // if (State == ONETAP) {
            State = LETSGO;
            InControl = OA1;
            // }
        }
        else if (command == "OA2 ACK") {
            // if (State == TWOTAP) {
            State = LETSGO;
            InControl = OA2;
            // }
        }
        else if (command == "WIN") {
            State = LOVE;
            InControl = SELF;
        }
        else if (command == "LOSE") {
            State = random(0, 2) == 0 ? SAD : ANGRY;
            InControl = SELF;
        }
        else if (command == "TIE") {
            State = HAPPY;
            InControl = SELF;
        }
        else {
            State = CURIOUS;
            InControl = SELF;
        }
        lastInteractionTime = currentMillis;
    }

    if (sensorLoop(&State, InControl, currentMillis)) lastInteractionTime = currentMillis;
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
    if (audioConnected) audioLoop(State);

    if (State == LETSGO && currentMillis - lastInteractionTime > 12000) {
        State = HAPPY;
        lastInteractionTime = currentMillis;
    }
    else if (InControl == SELF && currentMillis - lastInteractionTime > 30000) {
        State = IDLE;
        lastInteractionTime = currentMillis;
    }
}