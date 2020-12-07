/*
 * Author: Patrick Creighton
 * Purpose: Turn on LEDs when their corresponding button is pressed
 * Materials: 4 LEDs with 330 ohm resistors, 4 buttons with 10K ohm resistors, jumper cables
 * Note: place buttons and LEDs in the pins immediately above their start pins
 */

const int startLEDPin = 2;
const int startButtonPin = 8;
const int numLEDsAndButtons = 4;

void setup() {
  for (int led = startLEDPin; led < startLEDPin + numLEDsAndButtons; led++) {
    pinMode(led, OUTPUT);
  }
}


int buttonStates[numLEDsAndButtons]; // stores whether each button is on or off

void loop() {
  for (int button = startButtonPin; button < startButtonPin + numLEDsAndButtons; button++) {
    buttonStates[button - startButtonPin] = digitalRead(button); // converts from pin to index number
  }

  for (int index = 0; index < numLEDsAndButtons; index++) {
    if (buttonStates[index] == HIGH) {
      digitalWrite(index + startLEDPin, HIGH); 
    } else {
      digitalWrite(index + startLEDPin, LOW);
    }
  }
}
