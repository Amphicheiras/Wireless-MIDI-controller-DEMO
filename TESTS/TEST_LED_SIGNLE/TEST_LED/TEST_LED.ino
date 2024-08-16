#include <Adafruit_NeoPixel.h>

#define PIN 4
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

void loop() {
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
  pixels.clear();
    pixels.setPixelColor(i, pixels.Color(153, 0, 255));
    pixels.setBrightness(255);
    pixels.show();
  delay(100);
  }
}