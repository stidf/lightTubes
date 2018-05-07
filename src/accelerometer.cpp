#include <arduino.h>
#include "accelerometer.h"


// Top accelerometer functions
accelerometer::accelerometer(int xDataPin, int yDataPin, int zDataPin, int accelScale){
  _xDataPin = xDataPin;
  _yDataPin = yDataPin;
  _zDataPin = zDataPin;
  _scale = accelScale;
  _zAccel=0;
  _zScaledAccel=0;
  _yAccel=0;
  _yScaledAccel=0;
  _xAccel=0;
  _xScaledAccel=0;
  pinMode(_xDataPin, INPUT);
  pinMode(_yDataPin, INPUT);
  pinMode(_zDataPin, INPUT);
}

double accelerometer::zArbitrary(){
  //resturns a value between -1 and 1 for the current G reading.
  _zAccel=0;
  _zScaledAccel=0;
  _zAccel = analogRead(_zDataPin);
  _zScaledAccel = _zAccel/1028*2-1;
  return _zScaledAccel;
}

double accelerometer::yArbitrary(){
  //resturns a value between -1 and 1 for the current G reading.
  _yAccel=0;
  _yScaledAccel=0;
  _yAccel = analogRead(_yDataPin);
  _yScaledAccel = _yAccel/1028*2-1;
  return _yScaledAccel;
}

double accelerometer::xArbitrary(){
  //resturns a value between -1 and 1 for the current G reading.
  _xAccel=0;
  _xScaledAccel=0;
  _xAccel = analogRead(_xDataPin);
  _xScaledAccel = _xAccel/1028*2-1;
  return _xScaledAccel;
}

double accelerometer::vectorArbitrary(){
  _xAccel = 0;
  _yAccel = 0;
  _zAccel = 0;
  _vectorAccel = 0;
  _xAccel = accelerometer::xArbitrary();
  _yAccel = accelerometer::yArbitrary();
  _zAccel = accelerometer::zArbitrary();
  _vectorAccel = sqrt(pow(_xAccel,2) + pow(_yAccel,2) + pow(_zAccel,2));
  return _vectorAccel;
}

double accelerometer::vectorScaled(){
  _xAccel = 0;
  _yAccel = 0;
  _zAccel = 0;
  _vectorAccel = 0;
  _xAccel = accelerometer::xScaled();
  _yAccel = accelerometer::yScaled();
  _zAccel = accelerometer::zScaled();
  _vectorScaledAccel = sqrt(pow(_xAccel,2) + pow(_yAccel,2) + pow(_zAccel,2));
  return _vectorScaledAccel;
}

double accelerometer::zScaled(){
  //resturns a value between -1 and 1 for the current G reading.
  _zAccel=0;
  _zScaledAccel=0;
  _zAccel = analogRead(_zDataPin);
  _zScaledAccel = _zAccel/1028*2*_scale-_scale;
  return _zScaledAccel;
}

double accelerometer::yScaled(){
  //returns a value between -1 and 1 for the current G reading.
  _yAccel=0;
  _yScaledAccel=0;
  _yAccel = analogRead(_yDataPin);
  _yScaledAccel = _yAccel/1028*2*_scale-_scale;
  return _yScaledAccel;
}

double accelerometer::xScaled(){
  //resturns a value between -1 and 1 for the current G reading.
  _xAccel=0;
  _xScaledAccel=0;
  _xAccel = analogRead(_xDataPin);
  _xScaledAccel = _xAccel/1028*2*_scale-_scale;
  return _xScaledAccel;
}
//End top accelerometer functions
