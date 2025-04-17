bool approach(Servo servo, int goal, int step) {
    int current = servo.read();
    if (current == goal) return true;
    if (current < goal) {
        current += step;
        if (current > goal) current = goal;
    } else if (current > goal) {
        current -= step;
        if (current < goal) current = goal;
    }
    servo.write(current);
    return current == goal;
}

void resetServos() {
    lSer.write(0);
    fSer.write(0);
    rSer.write(0);
    neckSer.write(90);
}

unsigned long lastServoMillis = 0;

void servoLoop(unsigned int state, unsigned long currentMillis, unsigned int speed) {
    if (currentMillis - lastServoMillis >= 10 * speed) {
        lastServoMillis = currentMillis;

        switch (state) {
            case CURIOUS:
                lSer.write(0);
                fSer.write(90);
                rSer.write(180);
                approach(neckSer, 50, 5);
                break;
            case LOVE:
                break;
            case HAPPY:
                break;
            case SILLY:
                break;
            case ANGRY:
                break;
            case SAD:
                lSer.write(0);
                rSer.write(0);
                fSer.write(180);
                break;
            case SLEEP:
                break;
        }
    }
}