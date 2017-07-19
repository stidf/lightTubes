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
for use with a Micro and a 32 bit strip of WS2801



*/


int const dataPin  = 2;
int const clockPin = 3;
int const modeSwitchPin = A0;
int const xDataPinTop = A1;
int const yDataPinTop = A2;
int const zDataPinTop = A3;
int const xDataPinBot = A4;
int const yDataPinBot = A5;
int const zDataPinBot = A6;
int const maxModeTypes = 11;

Adafruit_WS2801 strip = Adafruit_WS2801(33, dataPin, clockPin);

int accelScale=16;
int stripPeriod=250; //refresh rate of strip in ms
int knobPotResistance = 10000;
int knobVoltDividerResistance = 10000;
int marchLowerPeriodLimit = 100;

int volatile lightMode = 3;

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
    rainbowCycle(stripPeriod);
  }
  else if(currentMode==2){
    rainbow(stripPeriod);
  }
  else if(currentMode==3){
    randomColorMarch(currentMode, stripPeriod);
  }
  else if(currentMode==4){
    accelRainbow(currentMode, stripPeriod);
  }
  else if(currentMode==5){
    translatingDot(currentMode, stripPeriod);
  }
  else if(currentMode==6){
    lightSaber(currentMode, stripPeriod);
  }
  else if(currentMode==7){
    translatingDotRandom(currentMode, stripPeriod);
  }
  else if(currentMode==8){
    stripTest(currentMode, stripPeriod);
  }
  else if(currentMode==9){
    colorWipe(Color(0,0,255),stripPeriod);
    delay(stripPeriod);
    colorFill(Color(255,0,0),stripPeriod);
    delay(stripPeriod);
  }
  else if(currentMode==10){
    colorFill(Color(255,0,0),stripPeriod);
    delay(stripPeriod);
    colorFill(Color(0,255,0),stripPeriod);
    delay(stripPeriod);
    colorFill(Color(0,0,255),stripPeriod);
    delay(stripPeriod);
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
  zAccel = analogRead(zDataPinTop);
  zScaledAccel = zAccel/1028*2-1;
  return zScaledAccel;
}

double yArbatraryAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccel=0;
  yAccel = analogRead(yDataPinTop);
  yScaledAccel = yAccel/1028*2-1;
  return yScaledAccel;
}

double xArbatraryAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccel=0;
  xAccel = analogRead(xDataPinTop);
  xScaledAccel = xAccel/1028*2-1;
  return xScaledAccel;
}

double zScaledAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccel=0;
  zAccel = analogRead(zDataPinTop);
  zScaledAccel = zAccel/1028*2*accelScale-accelScale;
  return zScaledAccel;
}

double yScaledAcceleration (){
  //returns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccel=0;
  yAccel = analogRead(yDataPinTop);
  yScaledAccel = yAccel/1028*2*accelScale-accelScale;
  return yScaledAccel;
}

double xScaledAcceleration (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccel=0;
  xAccel = analogRead(xDataPinTop);
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


void rainbow(int stripPeriod) {
  int i, j;
  int wait = stripPeriod/strip.numPixels();
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
void rainbowCycle(int stripPeriod) {
  int i, j;
  int wait = stripPeriod/strip.numPixels();
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
void colorWipe(unsigned long c, int stripPeriod) {
  int i;
  int wait = stripPeriod/strip.numPixels();
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorFill(unsigned long c, int stripPeriod){
  int i;
  int wait = stripPeriod/strip.numPixels();
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

void stripTest(int currentMode, int stripPeriod){
  //Sets all colors to green and then marches one set of RB down the track
  int wait = stripPeriod/4;
  int modeValue = currentMode;
  int tick = 1;
  boolean running = true;
  int lightStringArray[strip.numPixels()];

  while(running){
    currentMode=displayMode();
    Serial.print("Strip Test | Mode: ");
    Serial.print(currentMode);
    Serial.println(", ");
    if(currentMode!=modeValue){
      running=false;
    }
    for(int i=0;i<strip.numPixels();i++){
      lightStringArray[i]=1;
    }
    for(int x=0; x<strip.numPixels();x++){
      if (x==tick){
        lightStringArray[x]=170;
      }
      if(x==tick-1){
        lightStringArray[x]=85;
      }
      strip.setPixelColor(x,Wheel(lightStringArray[x]));
    }
    strip.show();
    delay(wait);
    tick=(tick+1)%strip.numPixels();
  }
}


void translatingDot(int currentMode, int stripPeriod){
  //produces a rainbow of colors that marches down the strip. The color can be shifted by the accelerometer up or down the color wheel.
  int wait = stripPeriod/strip.numPixels();
  if (wait < marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  int modeValue = currentMode;
  int startColor = random(255);
  int startDotPosition = random(strip.numPixels()-1);
  int colorShift;
  boolean running = true;
  int lightStringArray[strip.numPixels()];
  double deltaZ;
  int dotWidth = 3;//strip.numPixels()/10;;
  int dotPosition = startDotPosition;

  double shiftFactor=5;
  double shiftAmplification = 16;
  int lastShift=0;

  unsigned long c = Color(255,255,255);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=(startColor+i)%256;
  }
  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    Serial.print("Translating Dot | Mode: ");
    Serial.print(currentMode);
    Serial.print(", | ");
    /* read accelerometer get values
    modify the color that is coming up*/
    deltaZ=zScaledAcceleration();
    if(deltaZ>0){
      lastShift = 1;
      dotPosition++;
      if(dotPosition > strip.numPixels()- 1 - dotWidth/2){
        dotPosition = strip.numPixels()-1 - dotWidth/2;
      }
    }
    else if(deltaZ<0){
      lastShift = -1;
      dotPosition--;
      if(dotPosition < 0 + dotWidth/2){
        dotPosition = 0 + dotWidth/2;
      }
    }
    else{
      lastShift = 0;
    }
    Serial.print("Zaccel: ");
    Serial.print(deltaZ);
    Serial.print(", | Dot Position: ");
    Serial.print(dotPosition);
    Serial.print(", Last Shift: ");
    Serial.println(lastShift);
    for(int j=0; j< strip.numPixels(); j++){
        //writes all the color pixels
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    if(dotPosition>0){
      strip.setPixelColor(dotPosition-1,Color(200,200,200));
    }
    strip.setPixelColor(dotPosition,Color(255,255,255));
    if(dotPosition<strip.numPixels()-2){
      strip.setPixelColor(dotPosition+1,Color(200,200,200));
    }
    strip.show();
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    delay(wait);
    for (int i=0; i < strip.numPixels(); i++) {
      //moves all of the color pixels down by one.
        lightStringArray[i]=lightStringArray[i+1];
    }
    lightStringArray[strip.numPixels()-1] = (lightStringArray[strip.numPixels()-2]+1)%256;
  }
}

void translatingDotRandom(int currentMode, int stripPeriod){
  //produces a rainbow of colors that marches down the strip. The color can be shifted by the accelerometer up or down the color wheel.
  int wait = stripPeriod/strip.numPixels();
  if (wait < marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  int modeValue = currentMode;
  int startColor = random(255);
  int startDotPosition = random(strip.numPixels()-1);
  int colorShift;
  boolean running = true;
  int lightStringArray[strip.numPixels()];
  double deltaZ;
  int dotWidth = 3;//strip.numPixels()/10;
  int dotPosition = startDotPosition;

  double shiftFactor=5;
  double shiftAmplification = 16;
  int lastShift=0;

  unsigned long c = Color(255,255,255);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=(startColor+i)%256;
  }
  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    Serial.print("Translating Dot Random | Mode: ");
    Serial.print(currentMode);
    Serial.print(", | ");
    /* read accelerometer get values
    modify the color that is coming up*/
    deltaZ=random(-1,1);
    if(deltaZ>0){
      lastShift = 1;
      dotPosition++;
      if(dotPosition > strip.numPixels()-1){
        dotPosition = strip.numPixels()-1;
      }
    }
    else if(deltaZ<0){
      lastShift = -1;
      dotPosition--;
      if(dotPosition < 0){
        dotPosition = 0;
      }
    }
    else{
      lastShift = 0;
    }
    Serial.print("Random: ");
    Serial.print(deltaZ);
    Serial.print(", | Dot Position: ");
    Serial.print(dotPosition);
    Serial.print(", Last Shift: ");
    Serial.println(lastShift);
    for(int j=0; j< strip.numPixels(); j++){
        //writes all the color pixels
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    if(dotPosition>0){
      strip.setPixelColor(dotPosition-1,Color(200,200,200));
    }
    strip.setPixelColor(dotPosition,Color(255,255,255));
    if(dotPosition<strip.numPixels()-2){
      strip.setPixelColor(dotPosition+1,Color(200,200,200));
    }
    strip.show();
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    delay(wait);
    for (int i=0; i < strip.numPixels(); i++) {
      //moves all of the color pixels down by one.
        lightStringArray[i]=lightStringArray[i+1];
    }
    lightStringArray[strip.numPixels()-1] = (lightStringArray[strip.numPixels()-2]+1)%256;
  }
}


void accelRainbow(int currentMode, int stripPeriod){
  //produces a rainbow of colors that marches down the strip. The color can be shifted by the accelerometer up or down the color wheel.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  int modeValue = currentMode;
  int startColor = random(255);
  int colorShift;
  double deltaZ;
  double shiftFactor=5;
  double shiftAmplification = 16;
  int lastShift=0;
  boolean running = true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(255,255,255);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=startColor;
  }
  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    Serial.print("Accelleration Rainbow | Mode: ");
    Serial.print(currentMode);
    Serial.print(", | ");
    /* read accelerometer get values
    modify the color that is coming up*/
    deltaZ=zScaledAcceleration();
    colorShift=(deltaZ-1)*shiftAmplification+shiftFactor;
    if(abs(colorShift)-abs(lastShift)<shiftFactor){
      lastShift=colorShift;
      colorShift=shiftFactor;
    }
    else {
      lastShift=colorShift;
    }

    Serial.print("Zaccel: ");
    Serial.print(deltaZ);
    Serial.print(", | Color Shift: ");
    Serial.print(colorShift);
    Serial.print(", Last Shift: ");
    Serial.println(lastShift);
    startColor=(startColor+colorShift)%256;
    lightStringArray[strip.numPixels()-1]=startColor; // sets new color to end of line
    for(int j=0; j< strip.numPixels(); j++){
        //writes all the color pixels
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    strip.show();
    currentMode=displayMode();
    if(currentMode!=modeValue){
      running=false;
    }
    delay(wait);
    for (int i=0; i < strip.numPixels(); i++) {
      //moves all of the color pixels down by one.
        lightStringArray[i]=lightStringArray[i+1];
    }
  }
}

void randomColorMarch(int currentMode, int stripPeriod){
  //picks a random color and marches it down the strand.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  int modeValue = currentMode;
  boolean running = true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=random(255);
  }
  while(running){
    currentMode=displayMode();
    Serial.print("Random Color March | Mode: ");
    Serial.print(currentMode);
    Serial.println(", ");
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
      currentMode=displayMode();
      if(currentMode!=modeValue){
        //running=false;
      }
      delay(wait);
    }
  }
}

void studderRainbow(int currentMode, int stripPeriod){
  //fills the strand with random colors, then does a stutter step down the rainbow.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  int modeValue = currentMode;
  boolean running = true;

  int lightStringArray[strip.numPixels()];
  int lastColor = 0;
  int stepValue = 0;
  int colorStep = 5;
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=random(255);
  }
  lastColor = lightStringArray[strip.numPixels()-1];
  while(running){
    currentMode=displayMode();
    Serial.print("Studder Rainbow | Mode: ");
    Serial.print(currentMode);
    Serial.print(", ");
    if(currentMode!=modeValue){
      running=false;
    }
    for (int i=0; i < strip.numPixels(); i++) {
      stepValue = random(colorStep*(-1),colorStep);
      Serial.print("Step: ");
      Serial.println(stepValue);
      lightStringArray[strip.numPixels()-1]=lastColor+stepValue;
      for(int j=0; j< strip.numPixels(); j++){
        c=Wheel(lightStringArray[j]);
        strip.setPixelColor(j, c);
      }
      strip.show();
      for(int k = 0; k<strip.numPixels()-1;k++){
        lightStringArray[k]=lightStringArray[k+1];
      }
      currentMode=displayMode();
      if(currentMode!=modeValue){
        running=false;
      }
      delay(wait);
    }
  }
}


void lightSaber(int currentMode, int stripPeriod){
  //picks a random color and marches it down the strand.
  int wait = stripPeriod/strip.numPixels();
  int bladeColor = 185;
  int bladeColorVibrate = 2;
  int bladeColorState = bladeColor;
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  int modeValue = currentMode;
  boolean running=true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    //sets the strip to the the initial state
    lightStringArray[i] = bladeColor;
    strip.show();
    delay(50);
  }
  while(running){
    currentMode=displayMode();
    if(currentMode!=modeValue){
      //checks that the button hasn't been pressed or knob hasn't been turned.
      running=false;
    }
    bladeColorState = bladeColor + random(bladeColorVibrate * (-1), bladeColorVibrate) + bladeColorVibrate * (zScaledAcceleration()-1);
    for(int j=0; j< strip.numPixels(); j++){
      //runs through LED array and converts them with the color wheel and then assigns them to the strip.
      lightStringArray[j] = bladeColorState;
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    //displays the strip with the new array data
    strip.show();
    Serial.print("Lightsaber | Mode: ");
    Serial.print(currentMode);
    Serial.print(", Blade Color: ");
    Serial.println(bladeColorState);
    delay(wait);
  }
}


/*
void genericMode(int currentMode, int stripPeriod){
  //picks a random color and marches it down the strand.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  int modeValue = currentMode;
  boolean running=true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    //sets the strip to the the initial state
    lightStringArray[i]=1;
  }
  while(running){
    currentMode=displayMode();
    Serial.print("generic Mode Name | Mode: ")
    Serial.print(currentMode);
    Serial.println(", ");
    if(currentMode!=modeValue){
      //checks that the button hasn't been pressed or knob hasn't been turned.
      running=false;
    }
    lightStringArray[strip.numPixels()-1]=random(256);
    for(int j=0; j< strip.numPixels(); j++){
      //runs through LED array and converts them with the color wheel and then assigns them to the strip.
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    //displays the strip with the new array data
    strip.show();
    for(int k = 0; k<strip.numPixels()-1;k++){
      //marches everything down the strip by one LED
      lightStringArray[k]=lightStringArray[k+1];
    }
    delay(wait);
  }
}
*/
