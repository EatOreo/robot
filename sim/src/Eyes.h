//https://xantorohara.github.io/led-matrix-editor/
const uint64_t CORNERS = 0x0042000000004200;
const uint64_t HEART_OUT = 0x1824428181819966;
const uint64_t HEART_IN = 0x00183c7e7e7e6600;
const uint64_t CRY = 0x0000000000ff8100;
const uint64_t MEAN = 0x007e3e1e0e060200;
const uint64_t MEAN_BROW = 0x0080402010080402;
const uint64_t SMILE = 0x0042424242423c00;
const uint64_t CLOSED = 0x0000003c00000000;
const uint64_t EXCLAMATION = 0x1818001818181818;
const uint64_t NUM_ONE = 0x3c10101010141810;
const uint64_t NUM_TWO = 0x3c04081020202418;
const uint64_t BLINK[] = {
	0x007e7e66667e7e00,
	0x00007e667e7e0000,
	0x0000007e7e000000,
	0x0000007e00000000,
	0x00007e667e000000
};
const uint64_t TEARS[] = {
	0x0020202060000000,
	0x2020206060000000,
	0x2060606060000000,
	0x4060606060000000,
	0x4040606060000000,
	0x0040606060000000,
	0x4040406060000000,
	0x0000406060000000
};
const uint64_t LOOK[] = {
	0x007e7e66667e7e00,
	0x007e7e4e4e7e7e00,
	0x007e7e66667e7e00,
	0x007e7e72727e7e00
};
const uint64_t CIRCLE[] = {
	0x007e7e7e72727e00,
	0x007e7e7e66667e00,
	0x007e7e7e4e4e7e00,
	0x007e7e4e4e7e7e00,
	0x007e4e4e7e7e7e00,
	0x007e66667e7e7e00,
	0x007e72727e7e7e00,
	0x007e7e72727e7e00
};
const uint64_t LETS_GO[] = {
	0x0000000000000000,
	0x8080808080808080,
	0xc040404040404040,
	0xe020202020202020,
	0xf010101010101010,
	0x7808080808080808,
	0xbc84848484848484,
	0xde424242c24242c2,
	0xef212121e12121e1,
	0xf7101010f01010f0,
	0x7b08080878080878,
	0x3d0404043c0404bc,
	0x1e0202021e0202de,
	0x8f8181818f8181ef,
	0x47404040474040f7,
	0x23202020232020fb,
	0x111010101110107d,
	0x88080888888888be,
	0xc40404c4444444df,
	0xe20202e2222222ef,
	0xf18181f1111111f7,
	0x784040780808087b,
	0x3c20203c0404043d,
	0x1e10101e0202021e,
	0x8f88888f8181818f,
	0xc7444447c04040c7,
	0xe3222223e02020e3,
	0xf1919191f01090f1,
	0x7848484878084878,
	0xbca4a4a4bc84a4bc,
	0xde5252525e4252de,
	0xef2929292f2129ef,
	0xf7949494979094f7,
	0x7b4a4a4a4b484a7b,
	0xbd25a5a5a5a4a5bd,
	0x5e1252525252525e,
	0x2f0929292929292f,
	0x1704141414141417,
	0x0b020a0a0a0a0a0b,
	0x0501050505050505,
	0x0200020202020202,
	0x0100010101010101
};

void drawLetsGo(unsigned long framenr) {
	for (int i = 0; i < 64; i++) {
		if ((LETS_GO[framenr % 42] >> i) & 1)
			eyes.setPixelColor(i, 0x39FF14);
		if ((LETS_GO[(framenr + 8) % 42] >> i) & 1) 
			eyes.setPixelColor(i + 64, 0x39FF14);
	}
}

void draw(const uint64_t frame, bool mirror = false, uint32_t color = 0x00ff00) {
	for (int i = 0; i < 64; i++) {
		if ((frame >> i) & 1) {
			eyes.setPixelColor(i, color);
			eyes.setPixelColor((mirror ? (7 - (i % 8)) + (i / 8) * 8 : i) + 64, color);
		}
	}
}

unsigned long frame = 0;
unsigned long lastEyeMillis = 0;
unsigned long eIV = 50;
int blinkFrame = -1;

void blinkWithProb(int prob) {
	if (random(0, prob) == 0) {
		blinkFrame = 0;
		eIV = 5;
	}
}

void eyeLoop(uint8_t state, unsigned long currentMillis, unsigned int speed) {
	if (currentMillis - lastEyeMillis >= eIV * speed) {
		lastEyeMillis = currentMillis;

		eyes.clear();

		// TODO: blink like this: https://www.youtube.com/watch?v=bGIOFGOlB68
		// more inspiration: https://www.adrirobot.it/occhi-per-robot-con-matrice-8x8-max7219/
		if (blinkFrame >= 0 && blinkFrame < 5) {
			draw(BLINK[blinkFrame]);
			eIV = (blinkFrame == 3) ? 20 : 7;
			blinkFrame++;
			if (blinkFrame >= 5) blinkFrame = -1;
		}
		else switch (state) {
			case IDLE:
				if (frame % 2 == 0) {
					draw(LOOK[0]);
					eIV = 40 + pow(random(0, 1000), 0.5) * 10;
				} else {
					draw(CIRCLE[random(0, 8)]);
					eIV = 20 + pow(random(0, 100), 2) / 80;
				}
				blinkWithProb(16);
				break;
			case CURIOUS:
				draw(LOOK[frame % 4]);
				eIV = 50;
				blinkWithProb(10);
				break;
			case LOVE:
				draw(HEART_OUT);
				if (frame % 2 == 0) draw(HEART_IN, false, 0xff69b4);
				eIV = frame % 2 == 0 ? 30 : 10;
				break;
			case HAPPY:
				draw(SMILE);
				eIV = 100;
				blinkWithProb(10);
				break;
			case SILLY:
				draw(CIRCLE[frame % 8], true);
				eIV = 10;
				break;
			case ANGRY:
				draw(MEAN_BROW, true);
				draw(MEAN, true, 0xff0000);
				eIV = 100;
				break;
			case SAD:
				draw(CRY);
				draw(TEARS[frame % 8], true, 0x0088ff);
				eIV = 20;
				break;
			case SLEEP:
				draw(CLOSED);
				eIV = 100;
				break;
			case ERROR:
				if (frame % 2 == 0) draw(EXCLAMATION, true, 0xff0000);
				eIV = frame % 2 == 0 ? 30 : 10;
				break;
			case ONETAP:
				draw(NUM_ONE, false, 0x0000ff);
				eIV = 100;
				break;
			case TWOTAP:
				draw(NUM_TWO, false, 0x0000ff);
				eIV = 100;
				break;
			case LETSGO:
				drawLetsGo(frame);
				eIV = 8;
				break;
			default:
				draw(CLOSED);
				eIV = 100;
				break;
		}
		eyes.show();
		frame++;
    }
}