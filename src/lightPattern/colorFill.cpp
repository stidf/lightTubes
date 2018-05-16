#include <arduino.h>
#include <Adafruit_WS2801.h>
#include <SPI.h>
#include "../colorHelper.h"
#include <accelerometer.h>
#include "colorFill.h"

void colorFill(int r, int g, int b, int stripPeriod, Adafruit_WS2801 strip){
  unsigned long c = color(r,g,b);
  unsigned int i;
  int wait = stripPeriod/strip.numPixels();
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}
