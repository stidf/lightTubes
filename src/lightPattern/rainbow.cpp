#include <arduino.h>
#include <Adafruit_WS2801.h>
#include <SPI.h>
#include "../colorHelper.h"
#include <accelerometer.h>
#include "rainbow.h"

void rainbow(int stripPeriod, Adafruit_WS2801 strip) {
  int i, j;
  int wait = stripPeriod/strip.numPixels();
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, wheel( (i + j) % 255));
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}
