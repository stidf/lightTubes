#include <arduino.h>
#include <Adafruit_WS2801.h>
#include <SPI.h>

#include "../config/micro.h"
#include "../config/hardwareConfiguration.h"
#include "colorHelper.h"
#include <accelerometer.h>

void switchPress();
void statusSerialPrintout();

int stripPeriod = 250; //refresh rate of strip in ms
int marchLowerPeriodLimit = 100; //ms
int const maxModeTypes = 10;

volatile int lightMode = 0;
volatile int operatingMode = lightMode;
volatile unsigned long buttonStamp = 0;

Adafruit_WS2801 strip = Adafruit_WS2801(LEDCount, dataPin, clockPin);
accelerometer topAccel(xDataPinTop,yDataPinTop,zDataPinTop,accelScale);
accelerometer botAccel(xDataPinBot,yDataPinBot,zDataPinBot,accelScale);

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
      rainbowCycle(stripPeriod, strip);
    }
    break;

    case 2:{
      operatingMode = lightMode;
      rainbow(stripPeriod, strip);
    }
    break;

    case 3:{
      operatingMode = lightMode;
      // randomColorMarch(stripPeriod);
    }
    break;

    case 4:{
      operatingMode = lightMode;
      // accelRainbow(stripPeriod);
    }
    break;

    case 5:{
      operatingMode = lightMode;
      // translatingDot(stripPeriod);
    }
    break;

    case 6:
    {
      operatingMode = lightMode;
      // lightSaber(stripPeriod);
    }
    break;

    case 7:{
      operatingMode = lightMode;
      // translatingDotRandom(stripPeriod);
    }
    break;

    case 8:{
      operatingMode = lightMode;
      // stripTest(stripPeriod);
    }
    break;

    case 9:{
      operatingMode = lightMode;
      colorWipe(0,0,255,stripPeriod, strip);
      delay(stripPeriod);
      colorFill(255,0,0,stripPeriod, strip);
      delay(stripPeriod);
    }
    break;

    case 10:{
      operatingMode = lightMode;
      colorFill(255,0,0,stripPeriod,strip);
      delay(stripPeriod);
      colorFill(0,255,0,stripPeriod,strip);
      delay(stripPeriod);
      colorFill(0,0,255,stripPeriod, strip);
      delay(stripPeriod);
    }
    break;

    case 0:{
      operatingMode = lightMode;
      colorFill(0,0,0,stripPeriod,strip);
    }
    break;

    default:{
      operatingMode = lightMode;
      colorFill(0,0,0,stripPeriod,strip);
    }
    break;
  }
}

/* Helper functions */

/* Helper functions */

void switchPress(){
  unsigned long timeStamp = 0;
  timeStamp = millis();
  if(timeStamp-buttonStamp>500){
    lightMode++;
    if(lightMode>maxModeTypes){
      lightMode = 0;
    }
    Serial.print("button pressed: ");
    Serial.print(timeStamp);
    Serial.print(" | Operating Mode: ");
    Serial.println(lightMode);
  }
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


  xScaledAccelTop = topAccel.xScaled();
  yScaledAccelTop = topAccel.yScaled();
  zScaledAccelTop = topAccel.zScaled();
  vectorScaledAccelTop = topAccel.vectorScaled();
  xScaledAccelBot = botAccel.xScaled();
  yScaledAccelBot = botAccel.yScaled();
  zScaledAccelBot = botAccel.zScaled();
  vectorScaledAccelBot = botAccel.vectorScaled();


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
