unsigned long lastTap;
bool lastSensorState = LOW;
bool tapped = false;

void sensorLoop(uint8_t* stateAddress, unsigned long currentMillis) {
    bool sensorState = digitalRead(2);
    if (sensorState == HIGH && lastSensorState == LOW) {
        if (currentMillis - lastTap < 50) return;
        if (currentMillis - lastTap < 1000) { // double tap
            Serial.println("Double tap detected");
            *stateAddress = SILLY;
            tapped = false;
        }
        else tapped = true;
        lastTap = currentMillis;
    }
    if (tapped && lastTap + 1000 < currentMillis) {
        Serial.println("Single tap detected");
        *stateAddress = HAPPY;
        tapped = false;
    }
    lastSensorState = sensorState;
    delay(10);
}