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
static uint8_t State = ANGRY; 
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
        Serial.println(F("Unable to use audio player"));  
    }
    else {
        audioConnected = true;
        dfpPlayer.volume(25);
        Serial.println(F("Able to use audio player"));
    }
}

unsigned long lastInteractionTime = 0;

void loop() {
    unsigned long currentMillis = millis();

    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        if (command == "READY") {
            Serial.println("Actuator is ready");
        }
        else if (command == "WIN") {
            Serial.println("Robot won");
            State = HAPPY;
        }
        else if (command == "LOSE") {
            Serial.println("Robot lost");
            State = random(0, 2) == 0 ? SAD : ANGRY;
        }
        else if (command == "DRAW") {
            Serial.println("Robot draw");
            State = HAPPY;//adjust a comparative audio ask for one more round
        }
        else if (command == "INVALID") {
            Serial.println("Invalid gesture");
            State = CURIOUS;//adjust a comparative audio
        }
        lastInteractionTime = currentMillis;
    }

    if (sensorLoop(&State, currentMillis)) lastInteractionTime = currentMillis;
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
    if (audioConnected) audioLoop(State, currentMillis);

    if (currentMillis - lastInteractionTime > 60000) {
        State = IDLE;
        lastInteractionTime = currentMillis;
    }
}