#ifndef ACCELEROMETER_H
  #define ACCELEROMETER_H


#include <arduino.h>

class accelerometer
{
public:
  accelerometer(int xDataPin, int yDataPin, int zDataPin, int accelScale);
  double zArbitrary();
  double yArbitrary();
  double xArbitrary();
  double vectorArbitrary();
  double vectorScaled();
  double zScaled();
  double yScaled();
  double xScaled();
private:
  int _xDataPin;
  int _yDataPin;
  int _zDataPin;
  int _scale;
  double _xAccel;
  double _yAccel;
  double _zAccel;
  double _vectorAccel;
  double _zScaledAccel;
  double _yScaledAccel;
  double _xScaledAccel;
  double _vectorScaledAccel;

};

#endif
