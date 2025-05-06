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
SoftwareSerial softSerial(12, 13);
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
    lSer.attach(9);
    fSer.attach(10);
    rSer.attach(11);
    neckSer.attach(3);
    resetHead();
    if (!dfpPlayer.begin(softSerial)) {
        Serial.println(F("Unable to use audio player"));
    }
    else {
        audioConnected = true;
        dfpPlayer.volume(30);
    }
}


void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        if (command == "READY") {
            Serial.println("Actuator is ready");
        }
        else if (command == "WIN") {
            Serial.println("Robot won");
            State = HAPPY;
        }
        else if (command == "Robot lost") {
            State = SAD;
        }
    }

    unsigned long currentMillis = millis();
    sensorLoop(&State, currentMillis);
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
    if (audioConnected) audioLoop(State, currentMillis);
}