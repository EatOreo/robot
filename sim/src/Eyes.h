Adafruit_NeoPixel lEye(64, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rEye(64, 3, NEO_GRB + NEO_KHZ800);

//https://xantorohara.github.io/led-matrix-editor/
const uint64_t CORNERS = 0x0042000000004200;
const uint64_t HEART_OUT = 0x1824428181819966;
const uint64_t HEART_IN = 0x00183c7e7e7e6600;
const uint64_t CLOSE = 0x0000000000ff8100;
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

void draw(const uint64_t frame, uint32_t color, bool mirror = false) {
	for (int i = 0; i < 64; i++) {
		if ((frame >> i) & 1) {
			lEye.setPixelColor(i, color);
			rEye.setPixelColor(mirror ? (7 - (i % 8)) + (i / 8) * 8 : i, color);
		}
	}
}
void clear() {
	lEye.clear();
	rEye.clear();
}
void show() {
	lEye.show();
	rEye.show();
}