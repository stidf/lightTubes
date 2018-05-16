#ifndef COLORWIPE_H
  #define COLORWIPE_H
  #include <arduino.h>
  #include <Adafruit_WS2801.h>
  #include <SPI.h>
  #include "../colorHelper.h"
  #include <accelerometer.h>
  void colorWipe(int r, int g, int b, int stripPeriod, Adafruit_WS2801 strip);
#endif
