unsigned long lastAudioMillis = 0;
void audioLoop(int state, unsigned long currentMillis) {
    if (currentMillis - lastAudioMillis >= 2000) { // audio is 2 seconds long
        lastAudioMillis = currentMillis;

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
                dfpPlayer.play(5);
                break;
            case ANGRY:
                dfpPlayer.play(6);
                break;
            case SAD:
                dfpPlayer.play(7);
                break;
            case SLEEP:
                dfpPlayer.play(8);
                break;
            default:
                break;
        }
    }
}