#include <Adafruit_NeoPixel.h>
#include <Servo.h>

int S = 10;

Servo servo1;
Servo servo2;
Servo servo3;
Servo neck;

Adafruit_NeoPixel leftEye(64, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightEye(64, 3, NEO_GRB + NEO_KHZ800);

void setup() {
  leftEye.begin();
  leftEye.clear();
  rightEye.begin();
  rightEye.clear();
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
  neck.attach(12);
  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
  neck.write(90);
}

void loop() {
  look_around();
  happy();
  for (int i = 0; i < 5; i++)
  {
    heart_eyes();
    delay(30 * S);
    clear();
    show();
    delay(10 * S);
  }
  
  angry();
  sad();
}

int getPixelIndex(int x, int y) {
  return (y * 8) + x;
}

void clear() {
  leftEye.clear();
  rightEye.clear();
}

void show() {
  leftEye.show();
  rightEye.show();
}

void bigEyes(int dir) {
  const int eye0[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,1,1,2,2,0},
    {0,2,2,1,1,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,0,0,0,0,0,0,0}
  };

  const int eye1[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,1,1,2,0},
    {0,2,2,2,1,1,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,0,0,0,0,0,0,0}
  };

  const int eye2[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,1,1,2,2,2,0},
    {0,2,1,1,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,0,0,0,0,0,0,0}
  };

  const int eye3[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,2,2,1,1,2,2,0},
    {0,2,2,1,1,2,2,0},
    {0,2,2,2,2,2,2,0},
    {0,0,0,0,0,0,0,0}
  };

  const int (*selectedEye)[8];

  switch (dir) {
    case 1: selectedEye = eye1; break;
    case 2: selectedEye = eye2; break;
    case 3: selectedEye = eye3; break;
    default: selectedEye = eye0; break;
  }

  leftEye.clear();
  rightEye.clear();

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (selectedEye[j][i] == 1) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(0, 255, 0));
        rightEye.setPixelColor(getPixelIndex(i, j), rightEye.Color(0, 255, 0));
      }
      if (selectedEye[j][i] == 2) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(0, 0, 255));
        rightEye.setPixelColor(getPixelIndex(i, j), rightEye.Color(0, 0, 255));
      }
    }
  }

  leftEye.show();
  rightEye.show();
}

void blink() {
  int eye[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
  };

  leftEye.clear();
  rightEye.clear();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (eye[j][i]) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(0, 0, 255));
        rightEye.setPixelColor(getPixelIndex(7-i, j), rightEye.Color(0, 0, 255));
      }
    }
  }
  leftEye.show();
  rightEye.show();
}


void heart_eyes() {
  int eye[8][8] = {
    {0,2,2,0,0,2,2,0},
    {2,1,1,2,2,1,1,2},
    {2,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,2},
    {2,1,1,1,1,1,1,2},
    {0,2,1,1,1,1,2,0},
    {0,0,2,1,1,2,0,0},
    {0,0,0,2,2,0,0,0},
  };


  clear();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (eye[j][i] == 1) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(255, 0, 100));
        rightEye.setPixelColor(getPixelIndex(7-i, j), rightEye.Color(255, 0, 100));
      }
      else if (eye[j][i] == 2) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(255, 0, 0));
        rightEye.setPixelColor(getPixelIndex(7-i, j), rightEye.Color(255, 0, 0));
      }
    }
  }
  show();
}

void looking_eyes() {
  int eye[8][8] = {
    {0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0}
  };
  clear();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (eye[j][i] == 1) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(0, 0, 255));
        rightEye.setPixelColor(getPixelIndex(7-i, j), rightEye.Color(0, 0, 255));
      }
    }
  }
  show();
}


void happy_eyes() {
  int eye[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
  };
  clear();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (eye[j][i] == 1) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(0, 0, 255));
        rightEye.setPixelColor(getPixelIndex(7-i, j), rightEye.Color(0, 0, 255));
      }
    }
  }
  show();
}


void angry_eyes() {
  int eye[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,1,0,0,0,0},
    {0,1,1,1,1,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0},
  };
  clear();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (eye[j][i] == 1) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(255, 0, 0));
        rightEye.setPixelColor(getPixelIndex(7-i, j), rightEye.Color(255, 0, 0));
      }
    }
  }
  show();
}

void happy() {
  bigEyes(0);
  delay(50 * S);
  bigEyes(1);
  delay(50 * S);
  bigEyes(2); 
  delay(50 * S);
  bigEyes(3);
  delay(50 * S);
  happy_eyes();
  servo1.write(0);
  delay(150 * S);
  servo1.write(180);
  servo3.write(0);
  delay(150 * S);
  servo2.write(0);
  servo3.write(180);
  delay(140 * S);
  blink();
  delay(10 * S);
  bigEyes(0);
  servo2.write(180);
  delay(50 * S);
  happy_eyes();
  delay(150 * S);
}

void sad() {
  bigEyes(3);
  servo2.write(0);
  delay(300 * S);
  servo2.write(180);
}

void look_around() {
  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
  looking_eyes();
  neck.write(30);
  delay(150 * S);
  blink();
  delay(10 * S);
  looking_eyes();
  neck.write(150);
  blink();
  delay(10 * S);
  looking_eyes();
  delay(150 * S);
  neck.write(90);
  delay(200 * S);

}

void angry() {
  angry_eyes();
  for (int i = 0; i < 5; i++) {
    servo2.write(40);
    delay(20 * S);
    servo2.write(0);
    delay(20 * S);
  }
}

uint8_t HEART_OUTSIDE[8] = {
  0b01100110,
  0b10011001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000
};

uint8_t HEART_INSIDE[8] = {
  0b00000000,
  0b01100110,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00000000
};

uint8_t LOOKING_LEFT[8] = {
  0b00000000,
  0b10000001,
  0b11111111,
  0b00000110,
  0b00000110,
  0b00000110,
  0b00000110,
  0b00000110,
};