#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <State.h>
#include <Eyes.h>

const int S = 10;

bool lastButtonState = HIGH;

void setup() {
    Serial.begin(9600);
    lEye.begin();
    rEye.begin();
    pinMode(13, INPUT_PULLUP);
}

unsigned long lastMillis = 0;
unsigned long lastBlink = 0;
unsigned long iv = 50;
unsigned long frame = 0;
int state = CURIOUS; 

void loop() {
    bool buttonState = digitalRead(13);
    if (buttonState == LOW && lastButtonState == HIGH) state = (state + 1) % 7;
    lastButtonState = buttonState;
    delay(10);

    unsigned long currentMillis = millis();
    if (currentMillis - lastMillis >= iv * S) {
        lastMillis = currentMillis;
        clear();

        if (currentMillis - lastBlink >= (500 + static_cast<unsigned long>(random(1000))) * S) {
            lastBlink = currentMillis;
            draw(CLOSED);
            iv = 20;
        }
        else switch (state) {
			case CURIOUS:
				draw(LOOK[frame % 4]);
				iv = 50;
				break;
            case SAD:
                draw(CRY);
				draw(TEARS[frame % 8], true, 0x0088ff);
				iv = 20;
                break;
            case LOVE:
                draw(HEART_OUT);
                if (frame % 2 == 0) draw(HEART_IN, false, 0xff69b4);
				iv = frame % 2 == 0 ? 30 : 10;
                break;
            case SILLY:
                draw(CIRCLE[frame % 8], true);
                iv = 10;
                break;
            case ANGRY:
                draw(MEAN_OUT, true);
                draw(MEAN, true, 0xff0000);
                iv = 100;
                break;
            case HAPPY:
                draw(SMILE, true);
                iv = 100;
                break;
            case SLEEP:
                draw(CLOSED);
                iv = 100;
                break;
        }

        show();
        frame++;
    }
}