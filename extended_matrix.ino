
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define BRIGHTNESS 30
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
  Serial.begin(9600);
}

uint16_t grid_color_wheel(int16_t WheelPos) {
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

uint32_t strip_color_wheel(int16_t WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void drawStrip(int16_t x_pos, int16_t y_pos, int16_t counter_val) {
    // int16_t color = counter_val - (x_pos * 8) - 255;
    int16_t skip = 255 / STRIP_LENGTH;
    int16_t color = counter_val - 256 - (x_pos * 8) + (x_pos * skip);
    
   
    // Serial.println("x: ", x_pos, " count: ", counter_val, " color: ", color);
    Serial.print(" x: ");
    Serial.print(x_pos);
    Serial.print(" y: ");
    Serial.print(y_pos);
    Serial.print(" count: ");
    Serial.print(counter_val);
    Serial.print(" skip: ");
    Serial.print(skip);
    Serial.print(" color:");
    Serial.println(color);
    Serial.println("-- End Loop");
    strip.setPixelColor(x_pos, strip_color_wheel(color));
}

void drawGrid(int16_t x_pos, int16_t y_pos, int16_t counter_val) {
    matrix.drawPixel(x_pos, y_pos, grid_color_wheel(counter_val));
}


int16_t counter = 0;
int16_t shift_speed = 15;

void loop() {

  matrix.fillScreen(0);
  strip.fill(0);

  for (int16_t x = 0; x < GRID_LENGTH + STRIP_LENGTH ; x++) { 

    for (int16_t y=0; y < GRID_HEIGHT; y++) {
        // Serial.println("test: " + x);

        if (y == 0) {
            if (x >= GRID_LENGTH) {
                drawStrip(x - GRID_LENGTH, y, counter);
            }
        }

        if (x < GRID_LENGTH) {
            drawGrid(x, y, counter);
        }
        
        counter++;
    }
  }

  matrix.show();
  strip.show();
  delay(100);
  counter = 0;
  //counter = 0 + shift_speed;
}
