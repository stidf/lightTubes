#ifndef rainbow_h
  #define rainbow_h
  #include <arduino.h>
  #include <Adafruit_WS2801.h>
  #include <SPI.h>
  #include "../colorHelper.h"
  #include <accelerometer.h>
  void rainbow(int stripPeriod, Adafruit_WS2801 strip);
  #endif
