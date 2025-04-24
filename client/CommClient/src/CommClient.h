#include <Wire.h>

enum CommunicationModuleState {
    NONE = 0,
    IDLE = 1,
    CURIOUS = 2,
    LOVE = 3,
    HAPPY = 4,
    SILLY = 5,
    ANGRY = 6,
    SAD = 7,
    SLEEP = 8
};

const uint8_t MODULE_ACTUATOR_1 = 0x08;
const uint8_t MODULE_ACTUATOR_2 = 0x09;
const uint8_t MODULE_MOVEMENT = 0x0A;
const uint8_t MODULE_POWER = 0x0B;

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