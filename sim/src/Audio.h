uint8_t lastAudioState = 0;
void audioLoop(unsigned long currentMillis, uint8_t state) {
    if (lastAudioState != state) {
        lastAudioState = state;

        switch (state) {
            case CURIOUS:
                dfpPlayer.loop(1);
                break;
            case LOVE:
                dfpPlayer.loop(2);
                break;
            case HAPPY:
                dfpPlayer.loop(3);
                break;
            case SILLY:
                dfpPlayer.loop(4);
                break;
            case ANGRY:
                dfpPlayer.loop(5);
                break;
            case SAD:
                dfpPlayer.loop(6);
                break;
            case SLEEP:
                dfpPlayer.loop(7);
                break;
            default:
                break;
        }
    }
}