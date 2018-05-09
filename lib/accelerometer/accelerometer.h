#ifndef ACCELEROMETER_H
  #define ACCELEROMETER_H


#include <arduino.h>
#include <math.h>
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
  void calibrationX();
  void calibrationY();
  void calibrationZ();
  void setCalibration(double rOffset, double tOffset, double pOffset);
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
  double _theta;
  double _phi;
  double _radius;
  double _radiusOffset;
  double _thetaOffset;
  double _phiOffset;
  double _cartesianToSphericalRadius(double x, double y, double z);
  double _cartesianToSphericalTheta(double x, double y, double z);
  double _cartesianToSphericalPhi(double x, double y, double z);
  double _SphericalToCartesianX(double r, double t, double p);
  double _SphericalToCartesianY(double r, double t, double p);
  double _SphericalToCartesianZ(double r, double t, double p);
};

#endif
