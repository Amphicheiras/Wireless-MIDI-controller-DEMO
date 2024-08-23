#include <Adafruit_NeoPixel.h>

#define PIN 4
#define NUMPIXELS 8

#define ROOT_BUTTON_LED_PIN ?
#define SCALE_BUTTON_LED_PIN ?
#define CHORD_BUTTON_LED_PIN ?
#define ARP_BUTTON_LED_PIN 12
#define ON_OFF_BUTTON_LED_PIN 14
#define TOGGLE_MOUSE_BUTTON_LED_PIN 27
#define SOLO_BUTTON_LED_PIN 26
#define VG_BUTTON_LED_PIN 25

#define ROOT_BUTTON_LED_POSITION 0
#define SCALE_BUTTON_LED_POSITION 1
#define CHORD_BUTTON_LED_POSITION 2
#define ARP_BUTTON_LED_POSITION 3
#define ON_OFF_BUTTON_LED_POSITION 4
#define TOGGLE_MOUSE_BUTTON_LED_POSITION 5
#define SOLO_BUTTON_LED_POSITION 6
#define VG_BUTTON_LED_POSITION 7

#define ROOT_BUTTON_LED_COLOR 0
#define SCALE_BUTTON_LED_COLOR 1
#define CHORD_BUTTON_LED_COLOR 2
#define ARP_BUTTON_LED_COLOR 3
#define ON_OFF_BUTTON_LED_COLOR 4
#define TOGGLE_MOUSE_BUTTON_LED_COLOR 5
#define SOLO_BUTTON_LED_COLOR 6
#define VG_BUTTON_LED_COLOR 7

/* EACH LED HAS:
    PIN
    POSITION
    COLOR
    BRIGHTNESS?
    FPS?
*/

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


bool  LED_0_active,
      LED_1_active,
      LED_2_active,
      LED_3_active,
      LED_4_active,
      LED_5_active,
      LED_6_active,
      LED_7_active;

void setup() {
  pixels.begin();
}

void loop() {
  pixels.clear();
  if (LED_0_active) pixels.setPixelColor(0, pixels.Color(153, 0, 255));
  if (LED_1_active) pixels.setPixelColor(1, pixels.Color(153, 0, 255));
  if (LED_2_active) pixels.setPixelColor(2, pixels.Color(153, 0, 255));
  if (LED_3_active) pixels.setPixelColor(3, pixels.Color(153, 0, 255));
  if (LED_4_active) pixels.setPixelColor(4, pixels.Color(153, 0, 255));
  if (LED_5_active) pixels.setPixelColor(5, pixels.Color(153, 0, 255));
  if (LED_6_active) pixels.setPixelColor(6, pixels.Color(153, 0, 255));
  if (LED_7_active) pixels.setPixelColor(7, pixels.Color(153, 0, 255));
  pixels.setBrightness(10);
  pixels.show();
  delay(300);
}