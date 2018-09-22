#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRBW + NEO_KHZ800);   //first number is number of leds


void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(5);
}

bool alternate = true;

void loop() {
  if (alternate) {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, 0, 0, 255);
      strip.show();
      delay(50);
    }
    alternate = false;
  }
  else {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, 255, 0, 0);
      strip.show();
      delay(50);
      }
    alternate = true;
  }
  //strip.show();
  delay(100);

}


//use strip.setPixelColor(n, red, green, blue); to set colour of the nth LED
