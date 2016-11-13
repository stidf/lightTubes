#include "Arduino.h"
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif
//#include "blinkyPatterns.h"

/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

/* Adapted by Andrew Gasperini




*/


int const dataPin  = 2;
int const clockPin = 3;
int const xDataPin = A0;
int const yDataPin = A1;
int const zDataPin = A2;
int const modeSwitchPin = A3;
int const maxModeTypes=6;

Adafruit_WS2801 strip = Adafruit_WS2801(32, dataPin, clockPin);

int accelScale=16;
int stripPeriod=500;
int knobPotResistance = 10000;
int knobVoltDividerResistance = 10000;

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  analogReference(EXTERNAL);
  strip.begin();
  Serial.begin(9600);
  // Update LED contents, to start they are all 'off'
  strip.show();
}


void loop() {
  // Some example procedures showing how to display to the pixels
  double xScaledAccel=0;
  double yScaledAccel=0;
  double zScaledAccel=0;
  int currentMode=0;

  xScaledAccel = xScaledAcceleration();
  yScaledAccel = yScaledAcceleration();
  zScaledAccel = zScaledAcceleration();
  Serial.print(xScaledAccel);
  Serial.print(", ");
  Serial.print(yScaledAccel);
  Serial.print(", ");
  Serial.print(zScaledAccel);
  Serial.print(",| ");
  currentMode=displayMode();
  Serial.print(currentMode);
  Serial.println(", ");

  if(currentMode==1){
    rainbowCycle(stripPeriod/50);
  }
  else if(currentMode==2){
    rainbow(stripPeriod/10);
  }
  else if(currentMode==3){
    randomColorMarch(currentMode, stripPeriod);
  }
  else if(currentMode==4){
    accelRainbow(currentMode, stripPeriod);
  }
  else if(currentMode==5){
    colorWipe(Color(255,0,0),stripPeriod/3);
    colorWipe(Color(0,255,0),stripPeriod/3);
    colorWipe(Color(0,0,255),stripPeriod/3);
  }
  else if(currentMode==6){
    stripTest(currentMode, stripPeriod);
  }
  else if(currentMode==7){
    colorWipe(Color(255,0,0),stripPeriod);
  }
  else if(currentMode==8){
    colorWipe(Color(0,255,0),stripPeriod);

  }
  else if(currentMode==9){
    colorWipe(Color(0,0,255),stripPeriod);
  }
  else if(currentMode==10){

  }
}
/* Helper functions */

int displayMode(){
  int value=0;
  int g=0;
  g=analogRead(modeSwitchPin);
  value=map(g,512,1027,1,maxModeTypes);
//value =1027-1028*knobPotResistance/(knobPotResistance+knobVoltDividerResistance)
  return value;
}

double zArbatraryAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccel=0;
  zAccel = analogRead(zDataPin);
  zScaledAccel = zAccel/1028*2-1;
  return zScaledAccel;
}

double yArbatraryAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccel=0;
  yAccel = analogRead(yDataPin);
  yScaledAccel = yAccel/1028*2-1;
  return yScaledAccel;
}

double xArbatraryAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccel=0;
  xAccel = analogRead(xDataPin);
  xScaledAccel = xAccel/1028*2-1;
  return xScaledAccel;
}

double zScaledAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccel=0;
  zAccel = analogRead(zDataPin);
  zScaledAccel = zAccel/1028*2*accelScale-accelScale;
  return zScaledAccel;
}

double yScaledAcceleration (){
  //returns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccel=0;
  yAccel = analogRead(yDataPin);
  yScaledAccel = yAccel/1028*2*accelScale-accelScale;
  return yScaledAccel;
}

double xScaledAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccel=0;
  xAccel = analogRead(xDataPin);
  xScaledAccel = xAccel/1028*2*accelScale-accelScale;
  return xScaledAccel;
}


// Create a 24 bit color value from R,G,B
unsigned long Color(byte r, byte g, byte b)
{
  unsigned long c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned long Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}





/////End help function sections


void rainbow(int wait) {
  int i, j;

  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed
// along the chain
void rainbowCycle(int wait) {
  int i, j;

  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(unsigned long c, int wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void stripTest(int currentMode, int wait){
  //Sets all colors to green and then marches one set of RB down the track
  int modeValue = currentMode;
  int tick = 1;
  boolean running = true;
  int lightStringArray[strip.numPixels()];

  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    for(int i=0;i<strip.numPixels();i++){
      lightStringArray[i]=1;
    }
    for(int x=0; x<strip.numPixels();x++){
      if (x==tick){
        lightStringArray[x]=150;
      }
      if(x==tick-1){
        lightStringArray[x]=255;
      }
      strip.setPixelColor(x,Wheel(lightStringArray[x]));
    }
    strip.show();
    delay(wait);
    tick=(tick+1)%strip.numPixels();
  }
}



void randomColorMarch(int currentMode,int wait){
  //picks a random color and marches it down the strand.
  int modeValue = currentMode;
  boolean running = true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=1;
  }
  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    for (int i=0; i < strip.numPixels(); i++) {
      lightStringArray[strip.numPixels()-1]=random(256);
      for(int j=0; j< strip.numPixels(); j++){
        c=Wheel(lightStringArray[j]);
        strip.setPixelColor(j, c);
      }
        strip.show();
        for(int k = 0; k<strip.numPixels()-1;k++){
          lightStringArray[k]=lightStringArray[k+1];
        }
        delay(wait);
    }
  }
}

void accelRainbow(int currentMode, int wait){
  //produces a rainbow of colors that marches down the strip. The color can be shifted by the accelerometer up or down the color wheel.
  int modeValue = currentMode;
  int startColor = 0;
  int deltaZ;
  double shiftFactor=.5;
  int lastShift=0;
  boolean running=true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=1;
  }
  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    /* read accelerometer get values
    modify the color that is coming up*/
    deltaZ=(zArbatraryAcceleration()*accelScale-1)/accelScale*255*shiftFactor;
    lastShift=deltaZ;
    Serial.print("color shift: ");
    Serial.println(deltaZ);
    startColor=(startColor+1+deltaZ)%255;
    lightStringArray[strip.numPixels()-1]=startColor; // sets new color to end of line
    for(int j=0; j< strip.numPixels(); j++){
        //writes all the color pixels
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    strip.show();
    delay(wait);
    for (int i=0; i < strip.numPixels(); i++) {
      //moves all of the color pixels down by one.
        lightStringArray[i]=lightStringArray[i+1];
    }
  }
}

void randomRainbowMarch(int currentMode, int wait){
  //picks a random color and marches it down the strand.
  int modeValue = currentMode;
  boolean running=true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=1;
  }
  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    lightStringArray[strip.numPixels()-1]=random(256);
    for(int j=0; j< strip.numPixels(); j++){
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    strip.show();
    for(int k = 0; k<strip.numPixels()-1;k++){
      lightStringArray[k]=lightStringArray[k+1];
    }
    delay(wait);
  }
}
