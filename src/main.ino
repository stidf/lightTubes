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
int const modeSwitchPin = 7;
int const xDataPinTop = A0;
int const yDataPinTop = A1;
int const zDataPinTop = A2;
int const xDataPinBot = A3;
int const yDataPinBot = A4;
int const zDataPinBot = A5;
int const maxModeTypes = 10;


int accelScale=16;
int stripPeriod=250; //refresh rate of strip in ms
int knobPotResistance = 10000;
int knobVoltDividerResistance = 10000;
int marchLowerPeriodLimit = 100;
int LEDCount = 32;
Adafruit_WS2801 strip = Adafruit_WS2801(LEDCount, dataPin, clockPin);
int volatile lightMode = 0;
int volatile operatingMode = lightMode;
unsigned long volatile buttonStamp = 0;

void setup() {
  analogReference(EXTERNAL);
  pinMode(modeSwitchPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(modeSwitchPin), switchPress, RISING);
  strip.begin();
  Serial.begin(115200);
  // Update LED contents, to start they are all 'off'
  strip.show();
}


void loop() {
  // Some example procedures showing how to display to the pixels
  statusSerialPrintout();


  switch (lightMode){
    case 1:{
      operatingMode = lightMode;
      rainbowCycle(stripPeriod);
    }
    break;

    case 2:{
      operatingMode = lightMode;
      rainbow(stripPeriod);
    }
    break;

    case 3:{
      operatingMode = lightMode;
      randomColorMarch(stripPeriod);
    }
    break;

    case 4:{
      operatingMode = lightMode;
      accelRainbow(stripPeriod);
    }
    break;

    case 5:{
      operatingMode = lightMode;
      translatingDot(stripPeriod);
    }
    break;

    case 6:
    {
      operatingMode = lightMode;
      lightSaber(stripPeriod);
    }
    break;

    case 7:{
      operatingMode = lightMode;
      translatingDotRandom(stripPeriod);
    }
    break;

    case 8:{
      operatingMode = lightMode;
      stripTest(stripPeriod);
    }
    break;

    case 9:{
      operatingMode = lightMode;
      colorWipe(Color(0,0,255),stripPeriod);
      delay(stripPeriod);
      colorFill(Color(255,0,0),stripPeriod);
      delay(stripPeriod);
    }
    break;

    case 10:{
      operatingMode = lightMode;
      colorFill(Color(255,0,0),stripPeriod);
      delay(stripPeriod);
      colorFill(Color(0,255,0),stripPeriod);
      delay(stripPeriod);
      colorFill(Color(0,0,255),stripPeriod);
      delay(stripPeriod);
    }
    break;

    default:{
      operatingMode = lightMode;
      colorFill(Color(0,0,0),stripPeriod);
    }
    break;
  }
}
/* Helper functions */

void switchPress(){
  unsigned long timeStamp = 0;
  timeStamp = millis();
  if(timeStamp-buttonStamp>1000){
    lightMode++;
  }
  if(lightMode>maxModeTypes){
    lightMode = 1;
  }
  Serial.print("button pressed: ");
  Serial.print(timeStamp);
  Serial.print(" | Operating Mode: ");
  Serial.println(lightMode);
}

void statusSerialPrintout(){
  double xScaledAccelTop=0;
  double yScaledAccelTop=0;
  double zScaledAccelTop=0;
  double vectorScaledAccelTop=0;
  double xScaledAccelBot=0;
  double yScaledAccelBot=0;
  double zScaledAccelBot=0;
  double vectorScaledAccelBot=0;


  xScaledAccelTop = xScaledAccelerationTop();
  yScaledAccelTop = yScaledAccelerationTop();
  zScaledAccelTop = zScaledAccelerationTop();
  vectorScaledAccelTop = vectorScaledAccelerationTop();
  xScaledAccelBot = xScaledAccelerationBot();
  yScaledAccelBot = yScaledAccelerationBot();
  zScaledAccelBot = zScaledAccelerationBot();
  vectorScaledAccelBot = vectorScaledAccelerationBot();


  Serial.print("Top, X:");
  Serial.print(xScaledAccelTop);
  Serial.print(", Y:");
  Serial.print(yScaledAccelTop);
  Serial.print(", Z:");
  Serial.print(zScaledAccelTop);
  Serial.print(", |a|:");
  Serial.print(vectorScaledAccelTop);
  Serial.print(",| ");
  Serial.print("Bot, X:");
  Serial.print(xScaledAccelBot);
  Serial.print(", Y:");
  Serial.print(yScaledAccelBot);
  Serial.print(", Z:");
  Serial.print(zScaledAccelBot);
  Serial.print(", |a|:");
  Serial.print(vectorScaledAccelBot);
  Serial.print(",| ");
  Serial.print(operatingMode);
  Serial.println(", ");

}
// Top accelerometer functions

double zArbatraryAccelerationTop (){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccelTop=0;
  zAccel = analogRead(zDataPinTop);
  zScaledAccelTop = zAccel/1028*2-1;
  return zScaledAccelTop;
}

double yArbatraryAccelerationTop (){
  //resturns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccelTop=0;
  yAccel = analogRead(yDataPinTop);
  yScaledAccelTop = yAccel/1028*2-1;
  return yScaledAccelTop;
}

double xArbatraryAccelerationTop (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccelTop=0;
  xAccel = analogRead(xDataPinTop);
  xScaledAccelTop = xAccel/1028*2-1;
  return xScaledAccelTop;
}

double trueArbatraryAccelerationTop(){
  double xAccel = 0;
  double yAccel = 0;
  double zAccel = 0;
  double trueAccel = 0;
  xAccel = xArbatraryAccelerationTop();
  yAccel = yArbatraryAccelerationTop();
  zAccel = zArbatraryAccelerationTop();
  trueAccel = sqrt(pow(xAccel,2) + pow(yAccel,2) + pow(zAccel,2));
  return trueAccel;
}

double vectorScaledAccelerationTop(){
  double xAccel = 0;
  double yAccel = 0;
  double zAccel = 0;
  double trueAccel = 0;
  xAccel = xScaledAccelerationTop();
  yAccel = yScaledAccelerationTop();
  zAccel = zScaledAccelerationTop();
  trueAccel = sqrt(pow(xAccel,2) + pow(yAccel,2) + pow(zAccel,2));
  return trueAccel;
}

double zScaledAccelerationTop (){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccelTop=0;
  zAccel = analogRead(zDataPinTop);
  zScaledAccelTop = zAccel/1028*2*accelScale-accelScale;
  return zScaledAccelTop;
}

double yScaledAccelerationTop (){
  //returns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccelTop=0;
  yAccel = analogRead(yDataPinTop);
  yScaledAccelTop = yAccel/1028*2*accelScale-accelScale;
  return yScaledAccelTop;
}

double xScaledAccelerationTop (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccelTop=0;
  xAccel = analogRead(xDataPinTop);
  xScaledAccelTop = xAccel/1028*2*accelScale-accelScale;
  return xScaledAccelTop;
}
//End top accelerometer functions


//Bottom accelerometer functions


double zArbatraryAccelerationBot (){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccelBot=0;
  zAccel = analogRead(zDataPinBot);
  zScaledAccelBot = zAccel/1028*2-1;
  return zScaledAccelBot;
}

double yArbatraryAccelerationBot (){
  //resturns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccelBot=0;
  yAccel = analogRead(yDataPinBot);
  yScaledAccelBot = yAccel/1028*2-1;
  return yScaledAccelBot;
}

double xArbatraryAccelerationBot (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccelBot=0;
  xAccel = analogRead(xDataPinBot);
  xScaledAccelBot = xAccel/1028*2-1;
  return xScaledAccelBot;
}

double trueArbatraryAccelerationBot(){
  double xAccel = 0;
  double yAccel = 0;
  double zAccel = 0;
  double trueAccel = 0;
  xAccel = xArbatraryAccelerationBot();
  yAccel = yArbatraryAccelerationBot();
  zAccel = zArbatraryAccelerationBot();
  trueAccel = sqrt(pow(xAccel,2) + pow(yAccel,2) + pow(zAccel,2));
  return trueAccel;
}

double vectorScaledAccelerationBot(){
  double xAccel = 0;
  double yAccel = 0;
  double zAccel = 0;
  double trueAccel = 0;
  xAccel = xScaledAccelerationBot();
  yAccel = yScaledAccelerationBot();
  zAccel = zScaledAccelerationBot();
  trueAccel = sqrt(pow(xAccel,2) + pow(yAccel,2) + pow(zAccel,2));
  return trueAccel;
}

double zScaledAccelerationBot (){
  //resturns a value between -1 and 1 for the current G reading.
  double zAccel=0;
  double zScaledAccelBot=0;
  zAccel = analogRead(zDataPinBot);
  zScaledAccelBot = zAccel/1028*2*accelScale-accelScale;
  return zScaledAccelBot;
}

double yScaledAccelerationBot (){
  //returns a value between -1 and 1 for the current G reading.
  double yAccel=0;
  double yScaledAccelBot=0;
  yAccel = analogRead(yDataPinBot);
  yScaledAccelBot = yAccel/1028*2*accelScale-accelScale;
  return yScaledAccelBot;
}

double xScaledAccelerationBot (){
  //resturns a value between -1 and 1 for the current G reading.
  double xAccel=0;
  double xScaledAccelBot=0;
  xAccel = analogRead(xDataPinBot);
  xScaledAccelBot = xAccel/1028*2*accelScale-accelScale;
  return xScaledAccelBot;
}

//end bottom accelerometer functions



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

void stripTest(int stripPeriod){
  //Sets all colors to green and then marches one set of RB down the track
  int wait = stripPeriod/4;
  int tick = 1;
  boolean running = true;
  int lightStringArray[strip.numPixels()];

  while(running){
    Serial.print("Strip Test | Mode: ");
    Serial.print(operatingMode);
    Serial.println(", ");
    if(operatingMode!=lightMode){
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


void translatingDot(int stripPeriod){
  //produces a rainbow of colors that marches down the strip. The color can be shifted by the accelerometer up or down the color wheel.
  int wait = stripPeriod/strip.numPixels();
  if (wait < marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
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
    if(operatingMode!=lightMode){
      running=false;
    }
    Serial.print("Translating Dot | Mode: ");
    Serial.print(operatingMode);
    Serial.print(", | ");
    /* read accelerometer get values
    modify the color that is coming up*/
    deltaZ=zScaledAccelerationTop();
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
    if(operatingMode!=lightMode){
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

void translatingDotRandom(int stripPeriod){
  //produces a rainbow of colors that marches down the strip. The color can be shifted by the accelerometer up or down the color wheel.
  int wait = stripPeriod/strip.numPixels();
  if (wait < marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
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
    if(operatingMode!=lightMode){
      running=false;
    }
    Serial.print("Translating Dot Random | Mode: ");
    Serial.print(operatingMode);
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
    if(operatingMode!=lightMode){
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


void accelRainbow(int stripPeriod){
  //produces a rainbow of colors that marches down the strip. The color can be shifted by the accelerometer up or down the color wheel.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
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
    if(operatingMode!=lightMode){
      running=false;
    }
    Serial.print("Accelleration Rainbow | Mode: ");
    Serial.print(operatingMode);
    Serial.print(", | ");
    /* read accelerometer get values
    modify the color that is coming up*/
    deltaZ=vectorScaledAccelerationTop();
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
    if(operatingMode!=lightMode){
      running=false;
    }
    delay(wait);
    for (int i=0; i < strip.numPixels(); i++) {
      //moves all of the color pixels down by one.
        lightStringArray[i]=lightStringArray[i+1];
    }
  }
}

void randomColorMarch(int stripPeriod){
  //picks a random color and marches it down the strand.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  boolean running = true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    lightStringArray[i]=random(255);
  }
  while(running){
    Serial.print("Random Color March | Mode: ");
    Serial.print(operatingMode);
    Serial.println(", ");
    if(operatingMode!=lightMode){
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
      if(operatingMode!=lightMode){
        //running=false;
      }
      delay(wait);
    }
  }
}

void studderRainbow(int stripPeriod){
  //fills the strand with random colors, then does a stutter step down the rainbow.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
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
    Serial.print("Studder Rainbow | Mode: ");
    Serial.print(operatingMode);
    Serial.print(", ");
    if(operatingMode!=lightMode){
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
      if(operatingMode!=lightMode){
        running=false;
      }
      delay(wait);
    }
  }
}


void lightSaber(int stripPeriod){
  //picks a random color and marches it down the strand.
  int wait = stripPeriod/strip.numPixels();
  int bladeColor = 185;
  int bladeColorVibrate = 4;
  int bladeColorState = bladeColor;
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
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
    if(operatingMode!=lightMode){
      //checks that the button hasn't been pressed or knob hasn't been turned.
      running=false;
    }
    bladeColorState = bladeColor + random(bladeColorVibrate * (-1), bladeColorVibrate) + bladeColorVibrate * (vectorScaledAccelerationTop()-1);
    for(int j=0; j< strip.numPixels(); j++){
      //runs through LED array and converts them with the color wheel and then assigns them to the strip.
      lightStringArray[j] = bladeColorState;
      c=Wheel(lightStringArray[j]);
      strip.setPixelColor(j, c);
    }
    //displays the strip with the new array data
    strip.show();
    Serial.print("Lightsaber | Mode: ");
    Serial.print(operatingMode);
    Serial.print(", Blade Color: ");
    Serial.println(bladeColorState);
    delay(wait);
  }
}


/*
void genericMode(int stripPeriod){
  //picks a random color and marches it down the strand.
  int wait = stripPeriod/strip.numPixels();
  if (wait<marchLowerPeriodLimit){
    wait = marchLowerPeriodLimit;
  }
  boolean running=true;
  int lightStringArray[strip.numPixels()];
  unsigned long c = Color(0,0,0);
  for(int i=0;i<strip.numPixels();i++){
    //sets the strip to the the initial state
    lightStringArray[i]=1;
  }
  while(running){
    Serial.print("generic Mode Name | Mode: ")
    Serial.print(operatingMode);
    Serial.println(", ");
    if(operatingMode!=lightMode){
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
