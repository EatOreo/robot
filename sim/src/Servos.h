const int ANGLE_MULTIPLIER = 18;

void write(Servo servo, unsigned int pos) {
    servo.write(min(pos, 10) * ANGLE_MULTIPLIER);
}

bool approach(Servo servo, unsigned int goal, unsigned int step = 2) {
    goal = min(goal, 10) * ANGLE_MULTIPLIER;
    unsigned int current = servo.read();
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
    if (currentMillis - lastServoMillis >= 5 * speed) {
        lastServoMillis = currentMillis;

        switch (state) {
            case CURIOUS:
                write(lSer, 0);
                write(fSer, 5);
                write(rSer, 10);
                approach(neckSer, 8);
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
                resetServos();
                write(fSer, 10);
                break;
            case SLEEP:
                break;
            default:
                resetServos();
                break;
        }
    }
}