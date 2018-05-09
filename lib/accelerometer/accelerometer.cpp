#include <arduino.h>
#include <math.h>
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
  _theta=0;
  _phi=0;
  _radius=0;
  _radiusOffset=0;
  _thetaOffset=0;
  _phiOffset=0;
  pinMode(_xDataPin, INPUT);
  pinMode(_yDataPin, INPUT);
  pinMode(_zDataPin, INPUT);
}

double accelerometer::zArbitrary(){
  //resturns a value between -1 and 1 for the current G reading.
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
  _xAccel = accelerometer::xArbitrary();
  _yAccel = accelerometer::yArbitrary();
  _zAccel = accelerometer::zArbitrary();
  _radius = _cartesianToSphericalRadius(_xAccel,_yAccel,_zAccel)+_radiusOffset;
  _theta = _cartesianToSphericalTheta(_xAccel,  _yAccel, _zAccel)+_thetaOffset;
  _phi = _cartesianToSphericalPhi(_xAccel, _yAccel, _zAccel)+_phiOffset;
  _zScaledAccel = _SphericalToCartesianZ(_radius, _theta, _phi)*_scale;
  return _zScaledAccel;
}

double accelerometer::yScaled(){
  //returns a value between -1 and 1 for the current G reading.
  _xAccel = accelerometer::xArbitrary();
  _yAccel = accelerometer::yArbitrary();
  _zAccel = accelerometer::zArbitrary();
  _radius = _cartesianToSphericalRadius(_xAccel,_yAccel,_zAccel)+_radiusOffset;
  _theta = _cartesianToSphericalTheta(_xAccel,  _yAccel, _zAccel)+_thetaOffset;
  _phi = _cartesianToSphericalPhi(_xAccel, _yAccel, _zAccel)+_phiOffset;
  _yScaledAccel = _SphericalToCartesianZ(_radius, _theta, _phi)*_scale;
  return _yScaledAccel;
}

double accelerometer::xScaled(){
  //resturns a value between -1 and 1 for the current G reading.
  _xAccel = accelerometer::xArbitrary();
  _yAccel = accelerometer::yArbitrary();
  _zAccel = accelerometer::zArbitrary();
  _radius = _cartesianToSphericalRadius(_xAccel,_yAccel,_zAccel)+_radiusOffset;
  _theta = _cartesianToSphericalTheta(_xAccel,  _yAccel, _zAccel)+_thetaOffset;
  _phi = _cartesianToSphericalPhi(_xAccel, _yAccel, _zAccel)+_phiOffset;
  _xScaledAccel = _SphericalToCartesianZ(_radius, _theta, _phi)*_scale;
  return _xScaledAccel;
}

double accelerometer::_cartesianToSphericalRadius(double x, double y, double z){
  double r=0;
  r=pow((pow(x, 2)+pow(y, 2)+pow(z, 2)), 0.5);
  return(r);
}

double accelerometer::_cartesianToSphericalTheta(double x, double y, double z){
  double t=0;
  t = acos(z/_cartesianToSphericalRadius(x,y,z));
  return(t);
}

double accelerometer::_cartesianToSphericalPhi(double x, double y, double z){
  double p=0;
  if(x!=0&&y!=0){
    p=atan2(y, x);

  }
  return(p);
}

double accelerometer::_SphericalToCartesianX(double r, double t, double p){
  double x=0;
  x = r*sin(t)*cos(p);
  return(x);
}

double accelerometer::_SphericalToCartesianY(double r, double t, double p){
  double y=0;
  y = r*sin(t)*sin(p);
  return(y);
}

double accelerometer::_SphericalToCartesianZ(double r, double t, double p){
  double z = 0;
  z = r*cos(t);
  return(z);
}

void accelerometer::calibrationX(){
  _xAccel = accelerometer::xArbitrary()*_scale;
  _yAccel = accelerometer::yArbitrary()*_scale;
  _zAccel = accelerometer::zArbitrary()*_scale;
  _radiusOffset = 1-_cartesianToSphericalRadius(_xAccel,_yAccel,_zAccel);
  _thetaOffset = M_PI_4-_cartesianToSphericalTheta(_xAccel,  _yAccel, _zAccel);
  _phiOffset = 0-_cartesianToSphericalPhi(_xAccel, _yAccel, _zAccel);

}

void accelerometer::calibrationY(){
  _xAccel = accelerometer::xArbitrary()*_scale;
  _yAccel = accelerometer::yArbitrary()*_scale;
  _zAccel = accelerometer::zArbitrary()*_scale;
  _radiusOffset = 1-_cartesianToSphericalRadius(_xAccel,_yAccel,_zAccel);
  _thetaOffset = M_PI_4-_cartesianToSphericalTheta(_xAccel,  _yAccel, _zAccel);
  _phiOffset = M_PI_4-_cartesianToSphericalPhi(_xAccel, _yAccel, _zAccel);

}

void accelerometer::calibrationZ(){
  _xAccel = accelerometer::xArbitrary()*_scale;
  _yAccel = accelerometer::yArbitrary()*_scale;
  _zAccel = accelerometer::zArbitrary()*_scale;
  _radiusOffset = 1-_cartesianToSphericalRadius(_xAccel,_yAccel,_zAccel);
  _thetaOffset = 0-_cartesianToSphericalTheta(_xAccel,  _yAccel, _zAccel);
  _phiOffset = 0-_cartesianToSphericalPhi(_xAccel, _yAccel, _zAccel);

}

void accelerometer::setCalibration(double rOffset, double tOffset, double pOffset){
  _radiusOffset = rOffset;
  _thetaOffset = tOffset;
  _phiOffset = pOffset;
}
