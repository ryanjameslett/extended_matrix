/**
 Extended matrix program.

 Builds a matrix out of a 32x8 neopixel grid
 and extends the matrix to include 2 1x58?
 neopixel strips.

 The idea is the panel is hanging from a pack
 and the two strands are attached to the straps.

 TODO:
 * button to switch programs
 * twist control to change brightness
 * implement snake game using buttons
 * phase all pixels same color program
 TODO org:
 * don't pass counter values -- closures work here.
 * see if we can use 8-bit ints to save space
 * pass strip number through display code
 * add frame counter
 * make it so we can draw backwards
    * wipe code should be able to switch back and forward
*/
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

#define DELAY 10
#define COLOR_WHEEL 1
#define SCREEN_WIPE 2


// Global variables

byte g_program_id = 1;
int16_t g_pixel = 0;
int16_t g_row = 0;
int16_t shift_speed = 1;


// Initialize Neopixels

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

// COLOR STUFF
void stripColor(int16_t x_pos, int16_t pixel) {
    int16_t skip = 255 / STRIP_LENGTH;
    int16_t color = (pixel + (x_pos * skip)) % 255;
    strip.setPixelColor(x_pos, strip_color_wheel(color));
}

void gridColor(int16_t x_pos, int16_t y_pos, int16_t pixel_val) {
    matrix.drawPixel(x_pos, y_pos, grid_color_wheel(pixel_val));
}


// WIPE
void stripWipe(int16_t x_pos, int16_t pixel) {
  if (g_row >= GRID_LENGTH) {
    if (x_pos == g_row - GRID_LENGTH) {
      strip.setPixelColor(x_pos, strip.Color(255, 255, 255));
    }
  }
}

void gridWipe(int16_t x_pos, int16_t y_pos, int16_t pixel_val) {
  if (x_pos == (g_row)) {
    matrix.drawPixel(x_pos, y_pos, matrix.Color(255, 255, 255));
  }
}

// GENERIC STUFF
/**
 Draw to a neopixel strand

 TODO:
 * take strand object as param?
 */
void drawStrip(int16_t x_pos, int16_t y_pos, int16_t pixel) {
  switch (g_program_id) {
    case COLOR_WHEEL:
        stripColor(x_pos, 0);
        break;
    case SCREEN_WIPE:
        stripWipe(x_pos, pixel);
        break;
  }
}

void drawGrid(int16_t x_pos, int16_t y_pos, int16_t pixel) {
  switch (g_program_id) {
    case COLOR_WHEEL:
      gridColor(x_pos, y_pos, pixel);
      break;
    case SCREEN_WIPE:
      gridWipe(x_pos, y_pos, pixel);
      break;
  }
}

// MAIN

void loop() {
  matrix.fillScreen(0);
  strip.fill(0);

  for (int16_t x = 0; x < GRID_LENGTH + STRIP_LENGTH ; x++) { 
    for (int16_t y = 0; y < GRID_HEIGHT; y++) {
        if (x < GRID_LENGTH) {
            drawGrid(x, y, g_pixel);
        }
        else if (y == 0) {
          drawStrip(
            x - GRID_LENGTH, // x position shifted past the end of the grid
            y,
            g_pixel // current logical pixel count
          );
        }
        g_pixel++;
    }
  }

  matrix.show();
  strip.show();
  delay(DELAY);
  g_pixel = 0;

  g_row = g_row + 1;
  if (g_row > (GRID_LENGTH + STRIP_LENGTH)) {
    g_row = 0;
  }
}
