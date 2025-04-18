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

void resetNeck() {
    neckSer.write(90);
}

void rotateNeck(unsigned int pos) {
    //TODO
}

void resetHead(bool alsoResetNeck = true) {
    write(lSer, 0);
    write(fSer, 0);
    write(rSer, 0);
    if (alsoResetNeck) resetNeck();
}

unsigned long lastServoMillis = 0;
unsigned int sIV = 5;

void servoLoop(unsigned int state, unsigned long currentMillis, unsigned int speed) {
    if (currentMillis - lastServoMillis >= sIV * speed) {
        lastServoMillis = currentMillis;

        switch (state) {
            case CURIOUS:
                write(lSer, 0);
                write(fSer, 5);
                write(rSer, 10);
                approach(neckSer, 8); //TODO: replace with rotateNeck()
                break;
            case LOVE:
                approach(neckSer, 5); //TODO: replace with rotateNeck()
                break;
            case HAPPY:
                break;
            case SILLY:
                break;
            case ANGRY:
                break;
            case SAD:
                resetHead();
                break;
            case SLEEP:
                break;
            default:
                resetHead();
                break;
        }
    }
}