#include <Adafruit_NeoPixel.h>
#include <Servo.h>
//speaker setup
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

int S = 10;

Servo servo1;
Servo servo2;
Servo servo3;

Adafruit_NeoPixel leftEye(64, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightEye(64, 3, NEO_GRB + NEO_KHZ800);

// Setup DFPlayer with software serial on pins 12 and 13
SoftwareSerial mySerial(12, 13); 
DFRobotDFPlayerMini myDFPlayer;


void setup() {
  leftEye.begin();
  leftEye.clear();
  rightEye.begin();
  rightEye.clear();

  // Initialize DFPlayer
  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {
    while (true); // Halt if DFPlayer fails
  }
  myDFPlayer.volume(20);  // Set default volume (0–30)

  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
}

void loop() {
  for (int i = 0; i < 8; i++)
  {
    heart();
    delay(30 * S);
    clear();
    show();
    delay(10 * S);
  }
  
  happy();
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
    {0,0,2,2,2,2,0,0},
    {0,2,0,0,0,0,2,0},
    {2,0,0,1,1,0,0,2},
    {2,0,1,1,1,1,0,2},
    {0,2,1,1,1,1,2,0},
    {0,0,2,2,2,2,0,0},
    {0,0,0,0,0,0,0,0}
  };

  const int eye1[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,2,2,2,2,0,0},
    {0,2,0,0,0,1,2,0},
    {2,0,0,0,1,1,1,2},
    {2,0,0,0,1,1,1,2},
    {0,2,0,0,0,1,2,0},
    {0,0,2,2,2,2,0,0},
    {0,0,0,0,0,0,0,0}
  };

  const int eye2[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,2,2,2,2,0,0},
    {0,2,1,1,1,1,2,0},
    {2,0,1,1,1,1,0,2},
    {2,0,0,1,1,0,0,2},
    {0,2,0,0,0,0,2,0},
    {0,0,2,2,2,2,0,0},
    {0,0,0,0,0,0,0,0}
  };

  const int eye3[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,2,2,2,2,0,0},
    {0,2,1,0,0,0,2,0},
    {2,1,1,1,0,0,0,2},
    {2,1,1,1,0,0,0,2},
    {0,2,1,0,0,0,2,0},
    {0,0,2,2,2,2,0,0},
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
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(255, 0, 255));
        rightEye.setPixelColor(getPixelIndex(7 - i, j), rightEye.Color(255, 0, 255));
      }
      else if (selectedEye[j][i] == 2) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(255, 255, 255));
        rightEye.setPixelColor(getPixelIndex(7 - i, j), rightEye.Color(255, 255, 255));
      }
    }
  }

  leftEye.show();
  rightEye.show();
}

void blink() {
  int eye[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  };

  leftEye.clear();
  rightEye.clear();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (eye[j][i]) {
        leftEye.setPixelColor(getPixelIndex(i, j), leftEye.Color(255, 0, 255));
        rightEye.setPixelColor(getPixelIndex(7-i, j), rightEye.Color(255, 0, 255));
      }
    }
  }
  leftEye.show();
  rightEye.show();
}


void heart() {
  myDFPlayer.play(0003);  // Play heart sound
  waitForMusicToEnd();

  int eye[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 2, 2, 0, 2, 2, 0},
    {0, 2, 1, 1, 2, 1, 1, 2},
    {0, 2, 1, 1, 1, 1, 1, 2},
    {0, 2, 1, 1, 1, 1, 1, 2},
    {0, 0, 2, 1, 1, 1, 2, 0},
    {0, 0, 0, 2, 1, 2, 0, 0},
    {0, 0, 0, 0, 2, 0, 0, 0},
  };


  leftEye.clear();
  rightEye.clear();
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
  leftEye.show();
  rightEye.show();
}

void happy() {
  myDFPlayer.play(0001);  // Play happy sound
  waitForMusicToEnd();
  
  bigEyes(0);
  delay(5 * S);
  bigEyes(1);
  delay(5 * S);
  bigEyes(2); 
  delay(5 * S);
  bigEyes(3);
  delay(5 * S);
  bigEyes(0);
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
  delay(200 * S);
}

void sad() {
  myDFPlayer.play(0002);  // 😢 Play sad sound
  waitForMusicToEnd();

  bigEyes(0);
  servo2.write(0);
  delay(300 * S);
  servo2.write(180);
}

// Wait for current music to finish before continuing
void waitForMusicToEnd() {
  while (myDFPlayer.isPlaying()) {
    delay(100);
  }
}
