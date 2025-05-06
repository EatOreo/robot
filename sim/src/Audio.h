unsigned long lastAudioMillis = 0;
void audioLoop(unsigned long currentMillis, uint8_t state) {
    //if (currentMillis - lastAudioMillis >= 5000) { // audio is 20 seconds long
        //lastAudioMillis = currentMillis;
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
//change the numbers of certain music
//add idle music into mode SLEEP