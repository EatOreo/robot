bool connectAudio() {
    softSerial.begin(9600);
    if (dfPlayer.begin(softSerial, true, true)) {
        dfPlayer.volume(30);
        audioConnected = true;
        return true;
    }
    audioConnected = false;
    return false;
}

uint8_t lastAudioState = 0;

void audioLoop(uint8_t state, unsigned long currentMillis) {
    if (DEBUG && currentMillis % 1000 == 900) Serial.println("audio");
    if (lastAudioState != state) {
        lastAudioState = state;

        int toPlay = 0;
        
        switch (state) {
            case IDLE:
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
                toPlay = random(0, 3) == 0 ? 11 : 6;
                break;
            case GAMESTART:
                toPlay = 7;
                break;
            case CURIOUS:
                toPlay = 10;
                break;
            case OA1START:
                toPlay = 20;
                break;
            case OA2START:
                toPlay = 30;
                break;
            case OA1WIN:
            case OA2WIN:
                toPlay = random(0, 2) == 0 ? 23 : 24;
                break;
            case OA1LOSE:
            case OA2LOSE:
                toPlay = random(0, 2) == 0 ? 21 : 22;
                break;
            case OA1TIE:
            case OA2TIE:
                toPlay = 25;
                break;
            case INVALID:
                toPlay = 33;
                break;
            default:
                break;
        }

        if (DEBUG) {
            Serial.print("toPlay: ");
            Serial.println(toPlay);
        }
        if (toPlay > 0 && dfPlayer.readCurrentFileNumber() != toPlay) {
            if (DEBUG) Serial.println("Playing audio...");
            dfPlayer.playMp3Folder(toPlay);
        }
    }
}