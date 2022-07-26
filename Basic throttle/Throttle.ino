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

void setup()
{
  Joystick.begin();
  pinMode(potPinLeft, INPUT);
  pinMode(potPinRight, INPUT);

  Serial.begin(9600);
  Joystick.setThrottleRange( 0,  1023);
  Joystick.setAcceleratorRange(0, 1023);
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
}

void setAcceleratorPercentage( int pPin)
{
  int value = analogRead(pPin);
  value = clampValue(value);
  Joystick.setAccelerator( value);
}

void loop()
{
  setThrottlePercentage(potPinLeft);
  setAcceleratorPercentage(potPinRight);
}