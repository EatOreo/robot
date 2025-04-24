//https://xantorohara.github.io/led-matrix-editor/
const uint64_t CORNERS = 0x0042000000004200;
const uint64_t HEART_OUT = 0x1824428181819966;
const uint64_t HEART_IN = 0x00183c7e7e7e6600;
const uint64_t CRY = 0x0000000000ff8100;
const uint64_t MEAN = 0x007e3e1e0e060200;
const uint64_t MEAN_BROW = 0x0080402010080402;
const uint64_t SMILE = 0x0042424242423c00;
const uint64_t CLOSED = 0x0000003c00000000;
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

void draw(const uint64_t frame, bool mirror = false, uint32_t color = 0x00ff00) {
	for (int i = 0; i < 64; i++) {
		if ((frame >> i) & 1) {
			lEye.setPixelColor(i, color);
			rEye.setPixelColor(mirror ? (7 - (i % 8)) + (i / 8) * 8 : i, color);
		}
	}
}

unsigned long frame = 0;
unsigned long lastEyeMillis = 0;
unsigned long eIV = 50;
int blinkFrame = 0;

void eyeLoop(uint8_t state, unsigned long currentMillis, unsigned int speed) {
	if (currentMillis - lastEyeMillis >= eIV * speed) {
		lastEyeMillis = currentMillis;

		lEye.clear();
		rEye.clear();

		// TODO: blink like this: https://www.youtube.com/watch?v=bGIOFGOlB68
		// more inspiration: https://www.adrirobot.it/occhi-per-robot-con-matrice-8x8-max7219/
		if (blinkFrame > 0) {
			draw(BLINK[blinkFrame - 1]);
			if (blinkFrame == 4) eIV = 20;
			else eIV = 5;
			blinkFrame = (blinkFrame + 1) % 6;
		}
		else switch (state) {
			case IDLE:
				if (frame % 2 == 0) {
					draw(LOOK[0]);
					eIV = 40 + pow(random(0, 1000), 0.5) * 10; // Skewed random interval
				} else {
					draw(CIRCLE[random(0, 8)]);
					eIV = 20 + pow(random(0, 100), 2) / 80; // Skewed random interval
				}
				if (random(0, 8) == 0)
				{
					blinkFrame = 1;
					eIV = 5;
				}
				break;
			case CURIOUS:
				draw(LOOK[frame % 4]);
				eIV = 50;
				break;
			case LOVE:
				draw(HEART_OUT);
				if (frame % 2 == 0) draw(HEART_IN, false, 0xff69b4);
				eIV = frame % 2 == 0 ? 30 : 10;
				break;
			case HAPPY:
				draw(SMILE);
				eIV = 100;
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
			default:
				draw(CLOSED);
				eIV = 100;
				break;
		}

		lEye.show();
		rEye.show();
		frame++;
    }
}