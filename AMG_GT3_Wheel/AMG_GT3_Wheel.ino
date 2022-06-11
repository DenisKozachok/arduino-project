/*
   This example turns the ESP32 into a Bluetooth LE gamepad that presses buttons and moves axis

   At the moment we are using the default settings, but they can be canged using a BleGamepadConfig instance as parameter for the begin function.

   Possible buttons are:
   BUTTON_1 through to BUTTON_16
   (16 buttons by default. Library can be configured to use up to 128)

   Possible DPAD/HAT switch position values are:
   DPAD_CENTERED, DPAD_UP, DPAD_UP_RIGHT, DPAD_RIGHT, DPAD_DOWN_RIGHT, DPAD_DOWN, DPAD_DOWN_LEFT, DPAD_LEFT, DPAD_UP_LEFT
   (or HAT_CENTERED, HAT_UP etc)

   bleGamepad.setAxes sets all axes at once. There are a few:
   (x axis, y axis, z axis, rx axis, ry axis, rz axis, slider 1, slider 2)

   Library can also be configured to support up to 5 simulation controls
   (rudder, throttle, accelerator, brake, steering), but they are not enabled by default.

   Library can also be configured to support different function buttons
   (start, select, menu, home, back, volume increase, volume decrease, volume mute)
   start and select are enabled by default
*/

#include <Arduino.h>
#include <BleGamepad.h>

#define ARRAY_LEN 3

#define numOfButtons 10
#define numOfHatSwitches 0
#define enableX false
#define enableY false
#define enableZ false
#define enableRX false
#define enableRY false
#define enableRZ false
#define enableSlider1 false
#define enableSlider2 false
#define enableRudder false
#define enableThrottle false
#define enableAccelerator false
#define enableBrake false
#define enableSteering false

BleGamepad bleGamepad("AMG GT3 Wheel", "lemmingDev", 25);

int myPins[ARRAY_LEN] = {18, 19, 21};
byte buttonPins[numOfButtons] = {0, 35, 17, 18, 19, 23, 25, 26, 27, 32};
byte physicalButtons[numOfButtons] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};


void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(2, OUTPUT);
  for (int i = 0; i < ARRAY_LEN; i = i + 1) {
    pinMode(myPins[i], INPUT_PULLUP);
  }

  // Setup controller with 10 buttons, accelerator, brake and steering
  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setAutoReport(true);
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_JOYSTICK); // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
  bleGamepadConfig.setButtonCount(numOfButtons);
  bleGamepadConfig.setWhichAxes(enableX, enableY, enableZ, enableRX, enableRY, enableRZ, enableSlider1, enableSlider2);      // Can also be done per-axis individually. All are true by default
  bleGamepadConfig.setWhichSimulationControls(enableRudder, enableThrottle, enableAccelerator, enableBrake, enableSteering); // Can also be done per-control individually. All are false by default
  bleGamepadConfig.setHatSwitchCount(numOfHatSwitches);                                                                      // 1 by default

  bleGamepad.begin(&bleGamepadConfig);


}

void loop()
{
  if (bleGamepad.isConnected())
  {
    for (int i = 0; i < ARRAY_LEN; i++) {
      boolean button = !digitalRead(myPins[i]);
      if (button == 1) {
        bleGamepad.press(myPins[i]);
        digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(25);                     // wait for a second
        Serial.println("press button");
      }else {
        bleGamepad.release(myPins[i]);
        digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
        Serial.println("release button");
      }
    }
    delay(200);
    //    Serial.println("Press buttons 5, 16,19");
    //
    //    bleGamepad.press(16);
    //    bleGamepad.press(9);
    //    delay(1000);
    //
    //    Serial.println("release buttons 5, 19");
    //    bleGamepad.release(5);
    //    bleGamepad.release(9);
    //    bleGamepad.release(16);
    //        bleGamepad.sendReport();

  } else {
    Serial.println("BLE is not connected");
    delay(100);
  }


}
