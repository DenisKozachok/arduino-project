#include <Arduino.h>
#include <BleGamepad.h>

#define numOfButtons 8
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

// BATTERY VOLTAGE
#define ADC 4

BleGamepad bleGamepad("AMG GT3 Wheel", "lemmingDev");

// BATTERY PERCENT
int batteryPercentage;

byte previousButtonStates[numOfButtons];
byte currentButtonStates[numOfButtons];
byte buttonPins[numOfButtons] = {12, 13, 14, 18, 19, 21, 26, 27};
byte physicalButtons[numOfButtons] = {1, 2, 3, 4, 5, 6, 7, 8};

int period = 10000;
unsigned long time_now = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(2, OUTPUT);
  batteryPercentage = 3350;
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
    while (millis() > time_now + period) {
      Serial.println("BLE connected");
      Serial.println("Checking Battery Level");
      batteryLevel();
      time_now = millis();
    }

    for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++) {
      currentButtonStates[currentIndex] = digitalRead(buttonPins[currentIndex]);
      //      Serial.print("buttonPins: "); Serial.print(buttonPins[currentIndex]);
      //      Serial.print("    physicalButtons: "); Serial.print(physicalButtons[currentIndex]);
      //      Serial.print("  Button status: "); Serial.println(currentButtonStates[currentIndex]);
      if (currentButtonStates[currentIndex] != previousButtonStates[currentIndex]) {
        //        Serial.print("currentButtonStates != previousButtonStates"); Serial.print(currentButtonStates[currentIndex]); Serial.println(previousButtonStates[currentIndex]);
        if (currentButtonStates[currentIndex] == LOW) {
          //          Serial.println("currentButtonStates[currentIndex] == LOW");
          bleGamepad.press(physicalButtons[currentIndex]);
        } else {
          //          Serial.println("currentButtonStates[currentIndex] == HIGH");
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
    while (millis() > time_now + period) {
      Serial.println("BLE is not connected");
      time_now = millis();
    }
  }
}

int batteryLevel() {
  if (batteryPercentage < 100) { // REMOVE
    batteryPercentage = 3300; // REMOVE
  } // REMOVE

  int sensorValue = batteryPercentage;
  float voltage = sensorValue * (5.12 / 4095.0);
  Serial.print(voltage);
  Serial.print("V ||");
  int percentage = (voltage / 4.2) * 100;
  Serial.print(percentage);
  Serial.println("%");
  bleGamepad.setBatteryLevel(percentage);
  if (percentage < 80  && percentage > 71) {
    Serial.println("LOW battery 80");
  }
  if (percentage < 70  && percentage > 61) {
    Serial.println("LOW battery 70");
  }
  if (percentage < 60  && percentage > 51) {
    Serial.println("LOW battery 60");
  }
  if (percentage < 50  && percentage > 41) {
    Serial.println("LOW battery 50");
  }
  if (percentage < 40  && percentage > 31) {
    Serial.println("LOW battery 40");
  }
  if (percentage < 30  && percentage > 21) {
    Serial.println("LOW battery 30");
  }
  if (percentage < 20  && percentage > 11) {
    Serial.println("LOW battery 20");
  }
  if (percentage < 10) {
    Serial.println("LOW battery 10");
  }

  batteryPercentage = batteryPercentage - 100; // REMOVE
  return percentage;
}
