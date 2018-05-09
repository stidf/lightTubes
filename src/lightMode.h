#ifndef LIGHTMODES_H
#define LIGHTMODES_H

void rainbow(int stripPeriod);
void rainbowCycle(int stripPeriod);
void colorWipe(unsigned long c, int stripPeriod);
void colorFill(unsigned long c, int stripPeriod);
void stripTest(int currentMode, int stripPeriod);
void translatingDot(int currentMode, int stripPeriod);
void translatingDotRandom(int currentMode, int stripPeriod);
void accelRainbow(int currentMode, int stripPeriod);
void randomColorMarch(int currentMode, int stripPeriod);
void studderRainbow(int currentMode, int stripPeriod);
void lightSaber(int currentMode, int stripPeriod);


#endif
