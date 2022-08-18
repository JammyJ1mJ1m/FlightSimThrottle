#include <Joystick.h>
#include <easySwitch.h>

Joystick_ Joystick(0x15, JOYSTICK_TYPE_JOYSTICK,
                   3, 0,                // Button Count, Hat Switch Count
                   false, false, true,  // X and Y, but no Z Axis
                   false, false, false, // No Rx, Ry, or Rz
                   false, true,         // No rudder or throttle
                   true, false, true    // No accelerator, brake, or steering
);
const bool initAutoSendState = true;

easySwitch APUmasterSwitch(2);
easySwitch ENGLmasterSwitch(3);
easySwitch ENGRmasterSwitch(4);

int brightness;
int potPinLeft = A0;
int potPinRight = A1;
int brightnessPot = A2;


// initial setup of variables
void setup()
{
  Joystick.begin();
  pinMode(potPinLeft, INPUT);    // left Pot pin
  pinMode(potPinRight, INPUT);   // right Pot pin
  pinMode(brightnessPot, INPUT); // brightness Pot pin

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);

  Joystick.setThrottleRange(0, 255);
  Joystick.setAcceleratorRange(0, 255);
  Joystick.setSteeringRange(0, 1023);
  brightness = 0;

  Serial.begin(9600);
}

// reads serial packet sent from the FS2020TA app
void readPacket()
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

    if (input == ("@117/"))
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

    if (input == ("@320/") && index == "0")
    {
      controlAnalogLED(value, 0.9f, 11);
    }

    if (input == ("@320/") && index == "1")
    {
      controlAnalogLED(value, 0.9f, 10);
    }

    if (input == ("@320/") && index == "2")
    {
      controlAnalogLED(value, 0.9f, 9);
    }

    if (input == ("@58/"))
    {
      controlAnalogLED(value, 0.9f, 5);
    }
  }
}

// sets the brightness value of an led based on the given values passed to the method
void controlAnalogLED(float pValue, float pLimit, int pPin)
{
  if (pValue >= pLimit)
  {
    analogWrite(pPin, brightness);
  }
  if (pValue == 0)
  {
    analogWrite(pPin, 0);
  }
}

// clamps a value based on the current value and the limit value 
int clampValue(int pVal, int pLimit)
{
  if (pVal > pLimit)
  {
    pVal = pLimit;
  }
  return pVal;
}

// returns the value of a potentiometer from the given pin value
int readPotValue(int pPin)
{
  return analogRead(pPin);
}

// returns a set value to be used when setting a joystick / throttle axis
int setAxis(int pPin)
{
  int value = readPotValue(pPin);
  value = clampValue(value, 1022);
  value = map(value, 0, 1023,0,255);
  return value;
}

// sets the brightness of LED brightness variable based on a potentiometers current value
void setBrightnessLevel(int pPot)
{
  int value = readPotValue(pPot);
  value = map(value, 0, 1023, 0, 255);
  analogWrite(8, value);
  brightness = value;
}

// sets the value of a switch 
void setSwitch(easySwitch pSwitch, int pIndex)
{
  int pin_value = digitalRead(pSwitch.getSwitchPin());
  pin_value = !pin_value;
  if (pin_value != pSwitch.getSwitchState())
  {
    pSwitch.toggleSwitch();
  }
  Joystick.setButton(pIndex, pSwitch.getSwitchState());
}

// main arduino Loop
void loop()
{
  setSwitch(APUmasterSwitch, 0);
  setSwitch(ENGLmasterSwitch, 1);
  setSwitch(ENGRmasterSwitch, 2);
  setBrightnessLevel(brightnessPot);
  Joystick.setThrottle(setAxis(potPinLeft));
  Joystick.setAccelerator(setAxis(potPinRight));
  Joystick.setSteering(setAxis(brightnessPot));
  readPacket();
}