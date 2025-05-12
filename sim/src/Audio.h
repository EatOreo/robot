uint8_t lastAudioState = 0;
void audioLoop(uint8_t state) {
    if (lastAudioState != state) {
        lastAudioState = state;

        switch (state) {
            case CURIOUS:
                dfpPlayer.play(1);
                break;
            case LOVE:
                dfpPlayer.play(2);
                break;
            case HAPPY:
                dfpPlayer.play(3);
                break;
            case SILLY:
                dfpPlayer.play(4);
                break;
            case ANGRY:
                dfpPlayer.play(5);
                break;
            case SAD:
                dfpPlayer.play(6);
                break;
            case SLEEP:
                dfpPlayer.play(7);
                break;
            default:
                break;
        }
    }
}