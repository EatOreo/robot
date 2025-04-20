unsigned long lastAudioMillis = 0;
void audioLoop(int state, unsigned long currentMillis) {
    if (currentMillis - lastAudioMillis >= 2000) { // audio is 2 seconds long
        lastAudioMillis = currentMillis;

        switch (state) {
            case CURIOUS:
                dfpPlayer.play(1);
                break;
            case HAPPY:
                dfpPlayer.play(2);
                break;
            case SAD:
                dfpPlayer.play(3);
                break;
            case ANGRY:
                dfpPlayer.play(4);
                break;
            case SCARED:
                dfpPlayer.play(5);
                break;
            case CONFUSED:
                dfpPlayer.play(6);
                break;
            default:
                break;
        }
    }
}