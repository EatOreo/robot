#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <State.h>
Adafruit_NeoPixel lEye(64, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rEye(64, 6, NEO_GRB + NEO_KHZ800);
#include <Eyes.h>
Servo lSer;
Servo fSer;
Servo rSer;
Servo neckSer;
#include <Head.h>
SoftwareSerial softSerial(/*rx =*/12, /*tx =*/13);
DFRobotDFPlayerMini dfpPlayer;
#include <Audio.h>

const unsigned int S = 10;
static unsigned int State = CURIOUS; 

void setup() {
    Serial.begin(9600);
    pinMode(2, INPUT_PULLUP);

    lEye.begin();
    rEye.begin();
    lSer.attach(9);
    fSer.attach(10);
    rSer.attach(11);
    neckSer.attach(3);
    resetHead();
    dfpPlayer.volume(30); //0 to 30
}

bool lastButtonState = HIGH;

void loop() {
    bool buttonState = digitalRead(2);
    if (buttonState == LOW && lastButtonState == HIGH) State = (State + 1) % 7;
    lastButtonState = buttonState;
    delay(10);

    unsigned long currentMillis = millis();
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
    audioLoop(State, currentMillis);
}