/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
//byte val;
//unsigned long last_time = 0;

#define ARRAY_LEN 3

int myPins[ARRAY_LEN] = {18, 19, 21};

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  for (int i = 0; i < ARRAY_LEN; i = i + 1) {
    pinMode(myPins[i], INPUT_PULLUP);
  }
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  for (int i = 0; i < ARRAY_LEN; i++) {
    boolean button = !digitalRead(myPins[i]);
    if (button == 1) {
      Serial.print("Iteration");
      Serial.print(i);
      Serial.print("Pin num: ");
      Serial.print(myPins[i]);
      Serial.print(" , button status: ");
      Serial.println(button);
      digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);                       // wait for a second
      digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
      Serial.println("next");
    }

  }


  //  boolean button1 = !digitalRead(18);
  //  boolean button2 = !digitalRead(19);
  //  boolean button3 = !digitalRead(21);
  //  Serial.print("Button1: "); Serial.print(button1);
  //  Serial.print(", Button2: "); Serial.print(button2);
  //  Serial.print(", Button3: "); Serial.println(button3);
  //
  //  delay(1000);                       // wait for a second
}
