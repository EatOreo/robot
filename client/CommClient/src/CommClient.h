#include <Wire.h>

enum CommunicationModuleState {
    NONE,
    CURIOUS,
    LOVE,
    HAPPY,
    SILLY,
    ANGRY,
    SAD,
    SLEEP
};

uint8_t MODULE_ACTUATOR_1 = 0x08;
uint8_t MODULE_ACTUATOR_2 = 0x09;
uint8_t MODULE_MOVEMENT = 0x0A;
uint8_t MODULE_POWER = 0x0B;

uint8_t CommState = NONE;

void requestEvent() {
    Wire.write(CommState);
}

void (*startRecieveEvent)(bool) = nullptr;

void receiveEvent(int howMany) {
    if (startRecieveEvent != nullptr) {
        while(1 < Wire.available()) {
            char c = Wire.read(); 
            if (c == 's') startRecieveEvent(true);
            else startRecieveEvent(false);
        }
    }
}

void setupCommClient(uint8_t address, void (*onStartMessage)(bool)) {
    startRecieveEvent = onStartMessage;
    Wire.begin(address);
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
}

void requestCommState(uint8_t state) {
    CommState = state;
}