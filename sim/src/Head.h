const int ANGLE_MULTIPLIER = 18;

void write(Servo servo, unsigned int pos) {
    servo.write(min(pos, 10) * ANGLE_MULTIPLIER);
}
bool approach(Servo servo, unsigned int goal, unsigned int step = 2) {
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
void resetHead(bool alsoResetNeck = true) {
    write(lSer, 0);
    write(fSer, 0);
    write(rSer, 0);
    if (alsoResetNeck) resetNeck();
}
bool rotateNeck(int pos, unsigned int step = 2) {
    return approach(neckSer, map(-pos, -10, 10, 0, 180), step);
}
bool moveHead(unsigned int l, unsigned int f, unsigned int r, unsigned int step = 2) {
    bool leftReached = approach(lSer, min(l, 10) * ANGLE_MULTIPLIER, step);
    bool frontReached = approach(fSer, min(f, 10) * ANGLE_MULTIPLIER, step);
    bool rightReached = approach(rSer, min(r, 10) * ANGLE_MULTIPLIER, step);
    return leftReached && frontReached && rightReached;
}

unsigned long lastHeadMillis = 0;
unsigned int sIV = 8;
unsigned int iter = 0;

void servoLoop(uint8_t state, unsigned long currentMillis, unsigned int speed) {
    if (currentMillis - lastHeadMillis >= sIV * speed) {
        lastHeadMillis = currentMillis;
        bool even = iter % 2 == 0;

        switch (state) {
            case CURIOUS:
                moveHead(2, 0, 2);
                if (rotateNeck(even ? -8 : 8)) iter++;
                sIV = 8;
                break;
            case LOVE:
                moveHead(5, 0, 5);
                rotateNeck(0);
                sIV = 8;
                break;
            case HAPPY:
                if (moveHead(even ? 3 : 0, 0, even ? 0 : 3, 8)) iter++;
                rotateNeck(even ? -1 : 1, 3);
                sIV = 8;
                break;
            case SILLY:
                if (moveHead((iter % 3 == 0) ? 3 : 0,
                    ((iter + 1) % 3 == 0) ? 3 : 0,
                    ((iter + 2) % 3 == 0) ? 3 : 0, 4)) iter++;
                rotateNeck(even ? -2 : 2, 3);
                sIV = 8;
                break;
            case ANGRY:
                if (moveHead((iter % 3 == 0) ? 5 : 1,
                    ((iter + 1) % 3 == 0) ? 5 : 1,
                    ((iter + 2) % 3 == 0) ? 5 : 1, 8)) iter++;
                rotateNeck(even ? -4 : 4, 8);
                sIV = 3;
                break;
            case SAD:
                if (rotateNeck(even ? -1 : 1, 1)) iter++;
                moveHead(0, 7, 0, 10);
                sIV = 10;
                break;
            case SLEEP:
                resetHead(true);
                sIV = 50;
                break;
            case IDLE:
                resetHead(true);
                sIV = 50;
                break;
            default:
                resetHead(true);
                sIV = 50;
                break;
        }
    }
}