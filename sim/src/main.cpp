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
static uint8_t State = LOVE; 
bool audioConnected = false;
//sensor input
const int touchPin = 2;

void setup() {
    Serial.begin(9600);
    softSerial.begin(9600);
    pinMode(2, INPUT);

    eyes.begin();
    eyes.setBrightness(25);
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
        eyeLoop(SLEEP, currentMillis, S);
        servoLoop(SLEEP, currentMillis, S);
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
    bool buttonState = digitalRead(2);
    if (buttonState == LOW && lastButtonState == HIGH) State = (State % 8) + 1;
    lastButtonState = buttonState;
    delay(10);

    unsigned long currentMillis = millis();
    //2. eye and servo movement(IDLE movement separated)
    updateBodyAndFace(State,currentMillis);
    idleMusicLoop(State == IDLE);
    
    if (audioConnected && State != lastAudioState && State != IDLE) {
        audioLoop(State, currentMillis);
        lastAudioState = State;
    }
    commServerLoop(currentMillis); //30s no interaction -> idle mode
    if (millis() - lastInteractionTime > 30000 && State != IDLE) {
        State = IDLE;
    }
}

//1. Cancelled the loop of repeating music every X seconds
//Removed if (currentMillis - lastAudioMillis >= 5000)
//Turned it into a trigger-based system, the audio plays only once when the emotional state changes
//2. Separating sleep logic from others
//
//情绪触发冷却时间，防止乱跳
//add receive -> response module eg: received "WIN" state = HAPPY
