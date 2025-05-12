unsigned long lastTap;
unsigned long lastSensorLoop;
bool lastSensorState = LOW;
bool tapped = false;

bool sensorLoop(uint8_t* stateAddress, unsigned long currentMillis) {
    if (*stateAddress > CURIOUS || currentMillis - lastSensorLoop < 20) return false;
    bool sensorState = digitalRead(2);
    if (sensorState == HIGH && lastSensorState == LOW) {
        if (currentMillis - lastTap < 50) return true;
        if (currentMillis - lastTap < 1000) { // double tap
            *stateAddress = TWOTAP;
            tapped = false;
            Serial.println("CALL OA2");
        }
        else {
            *stateAddress = ONETAP;
            tapped = true;
        }
        lastTap = currentMillis;
    }
    if (tapped && lastTap + 1000 < currentMillis) {
        *stateAddress = HAPPY;
        tapped = false;
        Serial.println("CALL OA1");
    }
    lastSensorState = sensorState;
    lastSensorLoop = currentMillis;
    return sensorState == HIGH;
}