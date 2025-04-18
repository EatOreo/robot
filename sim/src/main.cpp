#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <State.h>
Adafruit_NeoPixel lEye(64, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rEye(64, 6, NEO_GRB + NEO_KHZ800);
#include <Eyes.h>
Servo lSer;
Servo fSer;
Servo rSer;
Servo neckSer;
#include <Head.h>

const unsigned int S = 10;
static unsigned int State = CURIOUS; 

void setup() {
    Serial.begin(9600);
    pinMode(13, INPUT_PULLUP);

    lEye.begin();
    rEye.begin();
    lSer.attach(9);
    fSer.attach(10);
    rSer.attach(11);
    neckSer.attach(3);
    resetHead();
}

bool lastButtonState = HIGH;

void loop() {
    bool buttonState = digitalRead(13);
    if (buttonState == LOW && lastButtonState == HIGH) State = (State + 1) % 7;
    lastButtonState = buttonState;
    delay(10);

    unsigned long currentMillis = millis();
    eyeLoop(State, currentMillis, S);
    servoLoop(State, currentMillis, S);
}