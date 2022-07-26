#include <Joystick.h>
Joystick_ Joystick(0x15, JOYSTICK_TYPE_JOYSTICK, 
  0, 0,                 // Button Count, Hat Switch Count
  false, false, true,  // X and Y, but no Z Axis
  false, false, false,  // No Rx, Ry, or Rz
  false, true,          // No rudder or throttle
  true, false, true);  // No accelerator, brake, or steering

//Analog Read with Serial Monitor
int potPinLeft = A0;
int potPinRight = A1;
int brightnessPot = A2;

int leftValue;
int rightValue;

int leftPercentage;
int rightPercentage;
bool isDebugEnabled;
int brightness;

const bool initAutoSendState = true;

void setup()
{
  Joystick.begin();
  pinMode(potPinLeft, INPUT);
  pinMode(potPinRight, INPUT);
  pinMode(brightnessPot, INPUT);
  
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  
  Serial.begin(9600);
  Joystick.setThrottleRange( 0,  1023);
  Joystick.setAcceleratorRange(0, 1023);
  Joystick.setSteeringRange(0, 1023);
  brightness = 0;
  isDebugEnabled = false;
}

//0.9999981



void readParkingPacket()
{
  String input = "@";
  float value;
  String index;

  String newVal;
  float floatVal;

  if (Serial.available() > 0)
  {
    Serial.readStringUntil('@');
    input += Serial.readStringUntil('/') + "/";
    index = Serial.readStringUntil('=');
    value = Serial.readStringUntil('$').toFloat();
    
    
    if (input == ("@117/") )
    {
      if (value == 1)
      {
        digitalWrite(12, HIGH);
      }
      if (value == 0)
      {
        digitalWrite(12, LOW);
      }
    }
    
  
  if (input == ("@320/") && index == "0" )
    {
      //value = readBetweenValues(floatVal, 0.9, 1);
      if (value >= 0.9)
        {
          analogWrite(11, brightness);
        }
      if (value == 0)
        {
          analogWrite(11, 0);
        }

      //setLight(value,11, 0, 0.9); 
    }
    
    
    if (input == ("@320/") && index == "1" )
    {
      if (value >= 0.9)
        {
          analogWrite(10, brightness);
        }
      if (value == 0)
        {
          analogWrite(10, 0);
        }
    }


    if (input == ("@320/") && index == "2" )
    {
      if (value >= 0.9)
        {
          analogWrite(9, brightness);
        }
      if (value == 0)
        {
          analogWrite(9, 0);
        }
    }
   
  }
}

String getIndex()
{
  String val = Serial.readStringUntil('=');
  
  return val;
}

void setLight(int pVal, int pPin, int pLow, int pbrightness)
{
  
  if (pVal >= pbrightness)
  {
    analogWrite(pPin, brightness);
  }
  if (pVal == pLow)
  {
    analogWrite(pPin, 0);    
  }
}

float readBetweenValues(float pVal, int pLow, int pbrightness)
{
  
  if(pVal > pLow )
  {
    pVal = 1;
  }
  return pVal;
}

int clampValue(int pVal)
{
  if (pVal > 1022)
  {
    pVal = 1022;
  }
  return pVal;
}

void setThrottlePercentage( int pPin)
{
  int value = analogRead(pPin);
  value = clampValue(value);
  Joystick.setThrottle( value);
  //Serial.println(value);
  //delay(100);
}

void setAcceleratorPercentage( int pPin)
{
  int value = analogRead(pPin);
  value = clampValue(value);
  Joystick.setAccelerator( value);
}

int readPotValue(int pPin)
{
  return analogRead(pPin);
  
}

void setRZAxis(int pPot)
{
  Joystick.setSteering(readPotValue(pPot)); 
}

int setBrightnessLevel(int pPot)
{
  int value = readPotValue(pPot);
  value = map(value, 0, 1023,0,255);
  return value;
}

void loop()
{
  brightness = setBrightnessLevel(brightnessPot);
  setThrottlePercentage(potPinLeft);
  setAcceleratorPercentage(potPinRight);
  setRZAxis(brightnessPot);
  readParkingPacket();
}