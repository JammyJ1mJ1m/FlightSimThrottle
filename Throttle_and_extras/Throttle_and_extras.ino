#include <Joystick.h>
#include <easySwitch.h>

Joystick_ Joystick(0x15, JOYSTICK_TYPE_JOYSTICK,
                   12, 0,                 // Button Count, Hat Switch Count
                   false, false, true,   // X and Y, but no Z Axis
                   false, false, false,  // No Rx, Ry, or Rz
                   false, true,          // No rudder or throttle
                   true, false, true     // No accelerator, brake, or steering
);
const bool initAutoSendState = true;

// APU panel
easySwitch APUmasterSwitch(2);
easySwitch ENGLmasterSwitch(3);
easySwitch ENGRmasterSwitch(4);

// Master arm panel
easySwitch EmergJettSwitch(13);
easySwitch MasterArmSwitch(8);

easySwitch AAButton(6);
easySwitch AGButton(7);

easySwitch SpoilerOn(9);
easySwitch SpoilerOFF(10);

// throttle designator controller setup
int TDCPinIn = A3;
int TDCPinLeft = A4;
int TDCPinRight = A5;
easySwitch TDCUp(0);
easySwitch TDCDown(1);
easySwitch TDCLeft(TDCPinLeft);
easySwitch TDCRight(TDCPinRight);
easySwitch TDCIn(TDCPinIn);

int potPinLeft = A0;
int potPinRight = A1;

void setup() {
  Joystick.begin();
  pinMode(potPinLeft, INPUT);     // left Pot pin
  pinMode(potPinRight, INPUT);    // right Pot pin

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  // master arm panel
  pinMode(13, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  // airbrakes
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  // TDC
  pinMode(1, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(TDCPinIn, INPUT_PULLUP);
  pinMode(TDCPinLeft, INPUT_PULLUP);
  pinMode(TDCPinRight, INPUT_PULLUP);

  // APU panel
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);

  Joystick.setThrottleRange(0, 255);
  Joystick.setAcceleratorRange(0, 255);
  Joystick.setSteeringRange(0, 1023);
  Serial.begin(9600);
}

// reads serial packet sent from the FS2020TA app
/* 
// currently unused - might re implement at a later point
void readPacket() {
  String input = "@";
  float value;
  String index;

  String newVal;
  float floatVal;

  if (Serial.available() > 0) {
    Serial.readStringUntil('@');
    input += Serial.readStringUntil('/') + "/";
    index = Serial.readStringUntil('=');
    value = Serial.readStringUntil('$').toFloat();

    if (input == ("@117/")) {
      if (value == 1) {
        digitalWrite(12, HIGH);
      }
      if (value == 0) {
        digitalWrite(12, LOW);
      }
    }

    if (input == ("@320/") && index == "0") {
      controlAnalogLED(value, 0.9f, 11);
    }

    if (input == ("@320/") && index == "1") {
      controlAnalogLED(value, 0.9f, 10);
    }

    if (input == ("@320/") && index == "2") {
      controlAnalogLED(value, 0.9f, 9);
    }

    if (input == ("@58/")) {
      controlAnalogLED(value, 0.9f, 5);
    }
  }
}
*/

// clamps a value based on the current value and the limit value
int clampValue(int pVal, int pLimit) {
  if (pVal > pLimit) {
    pVal = pLimit;
  }
  return pVal;
}

// returns the value of a potentiometer from the given pin value
int readPotValue(int pPin) {
  return analogRead(pPin);
}

// returns a set value to be used when setting a joystick / throttle axis
int setAxis(int pPin) {
  int value = readPotValue(pPin);
  value = clampValue(value, 1022);
  value = map(value, 0, 1023, 0, 255);
  return value;
}

// sets the value of a switch
void setSwitch(easySwitch pSwitch, int pIndex) {
  int pin_value = digitalRead(pSwitch.getSwitchPin());
  pin_value = !pin_value;
  if (pin_value != pSwitch.getSwitchState()) {
    pSwitch.toggleSwitch();
  }
  Joystick.setButton(pIndex, pSwitch.getSwitchState());
}

void setButton(easySwitch& pSwitch, int pIndex) {
  int pin_value = digitalRead(pSwitch.getSwitchPin());
  pin_value = !pin_value;

  if (pSwitch.getLastSwitchState() == false && pin_value == 1) {

    if (pSwitch.getSwitchState() == 0) {
      pSwitch.setSwitchOn();

    } else {
      pSwitch.setSwitchOff();
    }

  }

  pSwitch.setLastState(pin_value);
  
  Joystick.setButton(pIndex, pSwitch.getSwitchState());
}

// main arduino Loop
void loop() {
  // APU panel
  setSwitch(APUmasterSwitch, 0);
  setSwitch(ENGLmasterSwitch, 1);
  setSwitch(ENGRmasterSwitch, 2);
  // Master arm panel
  setSwitch(EmergJettSwitch, 3);
  setSwitch(MasterArmSwitch, 4);
  // A/A
  setSwitch(AAButton,5);
  // A/G
  setSwitch(AGButton,6);

  // Speed brakes
  setSwitch(SpoilerOn, 7);
  setSwitch(SpoilerOFF, 8);

  Joystick.setThrottle(setAxis(potPinLeft));
  Joystick.setAccelerator(setAxis(potPinRight));
}