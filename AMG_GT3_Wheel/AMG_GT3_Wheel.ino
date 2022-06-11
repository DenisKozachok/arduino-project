#include <Arduino.h>
#include <BleGamepad.h>

#define numOfButtons 3
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

byte previousButtonStates[numOfButtons];
byte currentButtonStates[numOfButtons];
byte buttonPins[numOfButtons] = {18, 19, 21};
byte physicalButtons[numOfButtons] = {1, 2, 3};
//byte buttonPins[numOfButtons] = {0, 35, 17, 18, 19, 23, 25, 26, 27, 32};
//byte physicalButtons[numOfButtons] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(2, OUTPUT);

  for (byte currentPinIndex = 0; currentPinIndex < numOfButtons; currentPinIndex++)    {
    pinMode(buttonPins[currentPinIndex], INPUT_PULLUP);
    previousButtonStates[currentPinIndex] = HIGH;
    currentButtonStates[currentPinIndex] = HIGH;
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

void loop() {
  if (bleGamepad.isConnected())  {
    Serial.println("BLE connected");
    for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++) {
      currentButtonStates[currentIndex] = digitalRead(buttonPins[currentIndex]);
      Serial.print("buttonPins: "); Serial.print(buttonPins[currentIndex]);
      Serial.print("    physicalButtons: "); Serial.print(physicalButtons[currentIndex]);
      Serial.print("  Button status: "); Serial.println(currentButtonStates[currentIndex]);
      if (currentButtonStates[currentIndex] != previousButtonStates[currentIndex]) {
        Serial.print("currentButtonStates != previousButtonStates"); Serial.print(currentButtonStates[currentIndex]); Serial.println(previousButtonStates[currentIndex]);
        if (currentButtonStates[currentIndex] == LOW) {
          Serial.println("currentButtonStates[currentIndex] == LOW");
          bleGamepad.press(physicalButtons[currentIndex]);
        } else {
          Serial.println("currentButtonStates[currentIndex] == HIGH");
          bleGamepad.release(physicalButtons[currentIndex]);
        }
      }
    }
    if (currentButtonStates != previousButtonStates) {
      for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++) {
        previousButtonStates[currentIndex] = currentButtonStates[currentIndex];
      }
      bleGamepad.sendReport();
    }
    delay(20);
  } else {
    Serial.println("BLE is not connected");
    delay(100);
  }
}
