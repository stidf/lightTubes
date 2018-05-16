#ifndef COLORFILL_H
  #define COLORFILL_H
  #include <arduino.h>
  #include <Adafruit_WS2801.h>
  #include <SPI.h>
  #include "../colorHelper.h"
  #include <accelerometer.h>
  void colorFill(int r, int g, int b,int stripPeriod, Adafruit_WS2801 strip);
#endif
