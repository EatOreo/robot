#include <Wire.h>

uint8_t MODULE_ACTUATOR_1 = 0x08;
uint8_t MODULE_ACTUATOR_2 = 0x09;
uint8_t MODULE_MOVEMENT = 0x0A;
uint8_t MODULE_POWER = 0x0B;

uint8_t* StatePointer;

void setupCommServer(uint8_t* stateAddress) {
    StatePointer = stateAddress;
    Wire.begin();
}

unsigned long lastServerMillis = 0;

void commServerLoop(unsigned long currentMillis) {
    if (currentMillis - lastServerMillis >= 400) {
        lastServerMillis = currentMillis;
        for (int i = 0; i < 2; i++) {
            Wire.requestFrom(0x08 + i, 1);
            if (Wire.available()) {
                uint8_t state = Wire.read();
                Serial.print("Received state: ");
                Serial.println(state);
                if (state != NONE) *StatePointer = state;
            }
        }
    }
}