/*
 * Author: Patrick Creighton
 * Purpose: To light up LEDs that each correspond to a button, and
 *          to play back a pattern given by the user using the buttons
 *           - Further details given in the instructions
 * Materials: 4 LEDs with 330 ohm resistors, 4 buttons with 10K ohm resistors, jumper cables
 * Note: place buttons and LEDs in the pins immediately above their start pins
 */

// initializing constants
const int startLEDPin = 2;
const int startButtonPin = 8;
const int numLEDsAndButtons = 4; // place buttons and LEDs in the pins immediately above their start pins
const int longestPattern = 100; // max length that will play back

void setup() {
  Serial.begin(9600);

  for (int led = startLEDPin; led < startLEDPin + numLEDsAndButtons; led++) {
    pinMode(led, OUTPUT);
  }

  Serial.println("Play Back:");
  Serial.println("\tWill only play back up to 100 presses");
  Serial.println("\tStops automatically after 3 seconds without a button press");
  Serial.println("\tOnly press one button at a time\n");
}

// declaring variables used in the loop
int start;
unsigned long currentTime; // integer range is too small, use unsigned long to record time in milliseconds
int index;
int flag;

void loop() {
  int buttonPins[longestPattern]; // order of button presses (pin #)
  int pressPeriods[longestPattern]; // duration of button presses (ms)
  int delayPeriods[longestPattern]; // how long in between button presses (ms)
  start = false;
  index = 0;
  flag = 0;

  Serial.println("Enter a pattern using the buttons to begin:");
  while (!start) { // will not start recording until first button is pressed
    for (int button = startButtonPin; button < startButtonPin + numLEDsAndButtons; button++) {
      if (digitalRead(button) == HIGH) {
        Serial.println("Started recording...");
        
        currentTime = millis();
        while (digitalRead(button) == HIGH) {
          continue;
        }
        pressPeriods[index] = millis() - currentTime;
        
        currentTime = millis();
        buttonPins[index] = button;
        
        start = true;        
        index++;        
        break;
      }
    }
  }
  
  while (index < 100 && flag == 0) { // will not continue until arrays are full or 3 seconds pass
    delay(5); // to increase stability
    for (int button = startButtonPin; button < startButtonPin + numLEDsAndButtons; button++) {
      if (digitalRead(button) == HIGH) {
        delayPeriods[index - 1] = millis() - currentTime; // first period recorded after first button press
        
        currentTime = millis();
        while (digitalRead(button) == HIGH) {
          continue;
        }
        pressPeriods[index] = millis() - currentTime;
        
        currentTime = millis();
        buttonPins[index] = button;
        
        index++;
        break; // breaks out of loop as soon as button is pressed
      } else if (millis() - currentTime > 3000) {
        flag = 1;
        break; // breaks out of loop as soon as 3 seconds pass
      }
    }
  }

  index--; // so that index is equal to last index stored in array
  
  Serial.println("Playing Back...\n");

  for (int i = 0; i <= index; i++) {
    digitalWrite(buttonPins[i] - (startButtonPin - startLEDPin), HIGH); // converts from button to LED pins
    delay(pressPeriods[i]);
    digitalWrite(buttonPins[i] - (startButtonPin - startLEDPin), LOW);
    
    if (i != index) { // last period recorded is before last button press
      delay(delayPeriods[i]);
    }
  }
}
