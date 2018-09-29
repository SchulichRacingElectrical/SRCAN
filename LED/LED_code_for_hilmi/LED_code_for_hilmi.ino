#include <Adafruit_NeoPixel.h>

#define PIN 6
#define DRIVERWANTSBLOCK 1                   //0 for constant increment and 1 for block increment for the last 6 pixels

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRBW + NEO_KHZ800);   //first number is number of leds

                          
void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(5);
}

int rpm = 0;

void loop() {                               //for testing
  if (rpm < 14000) {
    rpm += 400;
  } else {
    rpm = 0;
  }
  LED(rpm);
  delay(50);
}




//max rpm: 12500, driver change: 10500, each light has a value: 657.

void blockIncrementLED() {
  for (int i = 10; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 255);
  }
}
void LED (int rpm) {
  int numberoflights = (int)(rpm * 0.00152);        //0.00152 = 1/657
  if (numberoflights > strip.numPixels()) {
    numberoflights = strip.numPixels();
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    if (i < numberoflights && i < 6) {
      strip.setPixelColor(i, 0, 255, 0);
    } else if (i < numberoflights && i < 10) {
      strip.setPixelColor(i, 255, 0, 0);
    } else if (i < numberoflights && i < 16 && DRIVERWANTSBLOCK) {
      blockIncrementLED();
      break;
    } else if (i < numberoflights && i < 16) {
      strip.setPixelColor(i, 0, 0, 255);
    } else {
      strip.setPixelColor(i, 0);
    }
  }
  strip.show();
}

void LEDBlink() {                                               //not used in this code
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
  delay(100);                                                   //delay to make blink slower
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}
