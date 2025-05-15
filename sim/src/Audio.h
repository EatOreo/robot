bool connectAudio() {
    softSerial.begin(9600);
    if (dfPlayer.begin(softSerial, true, true)) {
        dfPlayer.volume(25);
        dfPlayer.play(1);
        audioConnected = true;
        return true;
    }
    audioConnected = false;
    return false;
}

uint8_t lastAudioState = 0;
static unsigned long lastPlayTime = 0;
const unsigned long minPlayDuration = 3000; 

void audioLoop(uint8_t state, unsigned long currentMillis) {
    if (DEBUG && currentMillis % 1000 == 900) Serial.println("audio");
    if (lastAudioState != state) {
        lastAudioState = state;

        uint8_t toPlay = 0;

        switch (state) {
            case CURIOUS:
                toPlay = 1;
                break;
            case LOVE:
                toPlay = 2;
                break;
            case HAPPY:
                toPlay = 3;
                break;
            case SILLY:
                toPlay = 4;
                break;
            case ANGRY:
                toPlay = 5;
                break;
            case SAD:
                toPlay = 6;
                break;
            case SLEEP:
                toPlay = 7;
                break;
            default:
                break;
        }
        if (currentMillis - lastPlayTime > minPlayDuration) {
            if (toPlay != 0 && toPlay != dfPlayer.readCurrentFileNumber()) {
                dfPlayer.play(toPlay);
                lastPlayTime = currentMillis;
            }
        }
    }
}