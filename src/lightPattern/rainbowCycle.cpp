#include <arduino.h>
#include <Adafruit_WS2801.h>
#include <SPI.h>
#include "../colorHelper.h"
#include <accelerometer.h>
#include "rainbowCycle.h"

void rainbowCycle(int stripPeriod, Adafruit_WS2801 strip) {
  unsigned int i, j;
  int wait = stripPeriod/strip.numPixels();
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}
