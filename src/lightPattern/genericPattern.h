#ifndef genericPattern_h
  #define genericPattern_h
  #include <arduino.h>
  #include <SPI.h>
  #include <Adafruit_WS2801.h>
  #include "../colorHelper.h"
  #include "../accelerometer.h"

  void genericPattern(int stripPeriod, int lightArray, Adafruit_WS2801 strip, accelerometer topAccel, accelerometer botAccel, bool isOn);
  #endif
