#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <State.h>
Adafruit_NeoPixel lEye(64, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rEye(64, 3, NEO_GRB + NEO_KHZ800);
#include <Eyes.h>

const int S = 10;
int state = CURIOUS; 

void setup() {
    Serial.begin(9600);
    pinMode(13, INPUT_PULLUP);
    lEye.begin();
    rEye.begin();
}

bool lastButtonState = HIGH;

void loop() {
    bool buttonState = digitalRead(13);
    if (buttonState == LOW && lastButtonState == HIGH) state = (state + 1) % 7;
    lastButtonState = buttonState;
    delay(10);

    eyeLoop(state, millis(), S);
}