#ifndef rainbowCycle_h
  #define rainbowCycle_h
  #include <arduino.h>
  #include <SPI.h>
  #include <Adafruit_WS2801.h>
  #include "../colorHelper.h"
  #include <accelerometer.h>
  void rainbowCycle(int stripPeriod, Adafruit_WS2801 strip);
  #endif
