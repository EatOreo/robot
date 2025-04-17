#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <State.h>
#include <Eyes.h>

const int S = 10;

void setup() {
    Serial.begin(9600);
    lEye.begin();
    rEye.begin();
}

unsigned long previousMillis = 0;
unsigned long iv = 50;
unsigned long frame = 0;
int state = CURIOUS; 

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= iv * S) {
        previousMillis = currentMillis;
        clear();

        switch (state) {
			case CURIOUS:
				iv = 80;
				draw(LOOK[frame % 4] ^ CORNERS, 0x00ff00);
				break;
            case CRY:
				iv = 20;
                draw(CLOSE, 0x00ff00);
				draw(TEARS[frame % 8], 0x0088ff, true);
                break;
            case LOVE:
                draw(HEART_OUT, 0x00ff00);
				iv = 30;
                if (frame % 2 == 0) draw(HEART_IN, 0xff69b4);
				else iv = 10;
                break;
            case SILLY:
                iv = 10;
                draw(CIRCLE[frame % 8] ^ CORNERS, 0x00ff00, true);
                break;
        }

        show();
        frame++;

        if (frame == 30) state = CRY;
    }
}

