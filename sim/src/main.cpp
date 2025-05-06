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
//sensor input
const int touchPin = 2;

void setup() {
    Serial.begin(9600);
    softSerial.begin(9600);
    Serial.println("Setup started.");
    pinMode(2, INPUT);

    eyes.begin();
    //eyes.setBrightness(25);

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
        dfpPlayer.volume(25);
        Serial.println(F("Able to use audio player"));
        //delay enough time to allow music finish
    }
}

bool lastButtonState = HIGH;
uint8_t lastAudioState = 0;
unsigned long lastInteractionTime = 0;

void updateBodyAndFace(uint8_t state, unsigned long currentMillis) {
    if (state == IDLE) {
        eyeLoop(IDLE, currentMillis, S);
        servoLoop(IDLE, currentMillis, S);
    } else {
        eyeLoop(state, currentMillis, S);
        servoLoop(state, currentMillis, S);
    }
}

void idleMusicLoop(bool isIdle) { //for idle music
    static bool isPlaying = false;
    if (isIdle && !isPlaying) {
        dfpPlayer.loop(10);  // put a number 10(?) music file, state matches music number
        isPlaying = true;
    } else if (!isIdle && isPlaying) {
        dfpPlayer.stop();
        isPlaying = false;
    }
}

void loop() {
    Serial.println("Loop running");
    delay(1000);

    Serial.print("Current State = ");
    Serial.println(State);

    sensorLoop(&State, millis());
    unsigned long currentMillis = millis();
    Serial.println("sensorLoop finished");

    updateBodyAndFace(State,currentMillis);
    Serial.println("updateBodyAndFace finished");
    idleMusicLoop(State == IDLE);
    Serial.println("idleMusicLoop checked");
    
    if (audioConnected && State != lastAudioState && State != IDLE) {
        if (audioConnected && State != lastAudioState && State != IDLE) {
        audioLoop(State, currentMillis);
        lastAudioState = State;
        Serial.println("audioLoop triggered");
    }

    if (millis() - lastInteractionTime > 30000 && State != IDLE) {
        State = IDLE;
        Serial.println("Idle mode activated");
    }
}

//1. Cancelled the loop of repeating music every X seconds
//Removed if (currentMillis - lastAudioMillis >= 5000)
//Turned it into a trigger-based system, the audio plays only once when the emotional state changes
//2. Separating idle logic from others
//3. [Doing]Wait one single emotion/music till the end or else it changes really quick
//4. Add receive -> response module eg: received "WIN" state = HAPPY
