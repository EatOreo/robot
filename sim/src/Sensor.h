unsigned long lastTap;
unsigned long lastSensorLoop;
bool lastSensorState = LOW;
bool tapped = false;

bool sensorLoop(uint8_t* state, unsigned long currentMillis, uint8_t* selectedActuator) {
    if (DEBUG && currentMillis % 1000 == 600) Serial.println("sensors");
    if (*state >= 50 || currentMillis - lastSensorLoop < 20) return false;
    if ((*state == ONETAP || *state == TWOTAP) && currentMillis - lastTap > 1000) {
        *state = GAMESTART;
        scheduleState(OA1START, 5000);
        scheduleState(HAPPY, 12000);
    }
    bool sensorState = digitalRead(2);
    if (*state != TWOTAP && sensorState == HIGH && lastSensorState == LOW) {
        if (currentMillis - lastTap < 50) return true;
        if (currentMillis - lastTap < 1000) { // double tap
            *state = TWOTAP;
            tapped = false;
            Serial.println("CALL OA2");
            *selectedActuator = 2;
        }
        else {
            *state = ONETAP;
            tapped = true;
        }
        lastTap = currentMillis;
    }
    if (tapped && lastTap + 1000 < currentMillis) {
        tapped = false;
        Serial.println("CALL OA1");
        *selectedActuator = 1;
    }
    lastSensorState = sensorState;
    lastSensorLoop = currentMillis;
    return sensorState == HIGH;
}