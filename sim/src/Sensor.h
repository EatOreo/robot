unsigned long lastTap;
unsigned long lastSensorLoop;
bool lastSensorState = LOW;
bool tapped = false;

bool sensorLoop(uint8_t* state, unsigned long currentMillis) {
    if (currentMillis - lastSensorLoop < 20) return false;
    if ((*state == ONETAP || *state == TWOTAP) && currentMillis - lastTap > 1000) *state = GAMESTART;
    bool sensorState = digitalRead(2);
    if (*state != TWOTAP && sensorState == HIGH && lastSensorState == LOW) {
        if (currentMillis - lastTap < 50) return true;
        if (currentMillis - lastTap < 1000) { // double tap
            *state = TWOTAP;
            tapped = false;
            Serial.println("CALL OA2");
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
    }
    lastSensorState = sensorState;
    lastSensorLoop = currentMillis;
    return sensorState == HIGH;
}