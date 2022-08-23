
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define BRIGHTNESS 50
#define GRID_PIN 3
#define GRID_HEIGHT 8
#define GRID_LENGTH 32
#define STRIP_PIN 5
#define STRIP_LENGTH 58

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    GRID_LENGTH,
    GRID_HEIGHT,
    GRID_PIN,
    NEO_MATRIX_RIGHT     + NEO_MATRIX_BOTTOM +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
    NEO_GRB            + NEO_KHZ800
);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(
    STRIP_LENGTH,
    STRIP_PIN,
    NEO_GRB + NEO_KHZ800
);



void setup() {
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
}


uint16_t color_wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;

  if (WheelPos < 85) {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


byte counter = 0;
byte shift_speed = 15;

void loop() {

  matrix.fillScreen(0);
  strip.fill(0);

  for (int16_t x = 0; x < GRID_LENGTH + STRIP_LENGTH ; x++) {

    for (int16_t y=0; y < GRID_HEIGHT; y++) { 

        if (y == 0) {
            if (x >= GRID_LENGTH) {
                strip.setPixelColor(x - GRID_LENGTH, color_wheel(counter));
            }
        }

        if (x < GRID_LENGTH) {
            matrix.drawPixel(x, y, color_wheel(counter));
        }
        
        counter++;
    }
  }
  matrix.show();
  strip.show();
  delay(100);
  counter = 0;
  // counter = counter - shift_speed;
}
