#ifndef COLORHELPER_H
  #define COLORHELPER_H
  #include <arduino.h>
  #include <Adafruit_WS2801.h>
  #include <SPI.h>
  #include "./lightPattern/rainbowCycle.h"
  #include "./lightPattern/rainbow.h"
  #include "./lightPattern/colorFill.h"
  #include "./lightPattern/colorWipe.h"
  #include "./lightPattern/genericPattern.h"
  extern int brightnessLevel;
  unsigned long wheel(byte WheelPos);
  unsigned long color(byte r, byte g, byte b);

#endif
