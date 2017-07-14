double zArbatraryAcceleration (int zDataPin){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccel=0;
  zAccel = analogRead(zDataPin);
  zScaledAccel = zAccel/1028*2-1;
  return zScaledAccel;
}

double yArbatraryAcceleration (int yDataPin){
  //resturns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccel=0;
  yAccel = analogRead(yDataPin);
  yScaledAccel = yAccel/1028*2-1;
  return yScaledAccel;
}

double xArbatraryAcceleration (int xDataPin){
  int xDataPinTop = A1;
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccel=0;
  xAccel = analogRead(xDataPin);
  xScaledAccel = xAccel/1028*2-1;
  return xScaledAccel;
}

double zScaledAcceleration (int zDataPin){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccel=0;
  int accelScale=16;
  zAccel = analogRead(zDataPin);
  zScaledAccel = zAccel/1028*2*accelScale-accelScale;
  return zScaledAccel;
}

double yScaledAcceleration (int yDataPin){
  //returns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccel=0;
  int accelScale=16;
  yAccel = analogRead(yDataPin);
  yScaledAccel = yAccel/1028*2*accelScale-accelScale;
  return yScaledAccel;
}

double xScaledAcceleration (int xDataPin){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccel=0;
  int accelScale=16;
  xAccel = analogRead(xDataPin);
  xScaledAccel = xAccel/1028*2*accelScale-accelScale;
  return xScaledAccel;
}
