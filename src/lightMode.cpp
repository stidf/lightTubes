#include<arduino.h>
#include "colorHelper.h"
#include "lightMode.h"

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
    deltaZ=zScaledAcceleration(zDataPinTop);
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
    deltaZ=zScaledAcceleration(zDataPinTop);
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
        running=false;
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
    bladeColorState = bladeColor + random(bladeColorVibrate * (-1), bladeColorVibrate) + bladeColorVibrate * (zScaledAcceleration(zDataPinTop)-1);
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
