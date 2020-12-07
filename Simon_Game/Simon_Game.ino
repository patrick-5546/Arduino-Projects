/*
 * Author: Patrick Creighton
 * Purpose: To test one's memory by lighting LEDS in a random pattern of user-given length
 *          and asking the user to press the corresponding buttons in that order
 * Materials: 4 LEDs with 330 ohm resistors, 4 buttons with 10K ohm resistors, jumper cables
 * Note: place buttons and LEDs in the pins immediately above their start pins
 */

const int startLEDPin = 2;
const int startButtonPin = 8;
const int numLEDsAndButtons = 4;
int patternLength;
int flag;
int index;

void setup() {
  Serial.begin(9600);
  
  for (int led = startLEDPin; led < startLEDPin + numLEDsAndButtons; led++) {
    pinMode(led, OUTPUT);
  }

  randomSeed(analogRead(0));

  Serial.println("Simon Game:");
  Serial.println("\tEnter your chosen pattern length into the serial monitor");
  Serial.println("\tTry and memorize the pattern that the LEDs light in");
  Serial.println("\tEnter the pattern using the corresponding buttons");
  Serial.println("\tBlinks twice for correct or once for incorrect\n");

  Serial.print("Enter pattern length: ");
  while (Serial.available() == 0) {
    continue;
  }
  patternLength = Serial.readString().toInt();
  Serial.println(patternLength);
  Serial.println("\n");
}

void loop() {
  flag = 0;
  int recitedPattern[patternLength];

  // making an array of randomly generated button pin numbers
  int randomPattern[patternLength];
  for (int i = 0; i < patternLength; i++) {
    randomPattern[i] = random(startButtonPin, startButtonPin + numLEDsAndButtons);
  }

  Serial.println("Press the left-most button to begin:");
  while (digitalRead(startButtonPin) == LOW) {
    continue;
  }

  // lights LEDs in the randomly-generated pattern pattern
  for(int i = 0; i < patternLength; i++) {
    delay(500);
    digitalWrite(randomPattern[i] - (startButtonPin - startLEDPin), HIGH);
    delay(500);
    digitalWrite(randomPattern[i] - (startButtonPin - startLEDPin), LOW);
  }

  Serial.println("Press the buttons in the correct order:");
  index = 0;
  while (index < patternLength) {
    for (int button = startButtonPin; button < startButtonPin + numLEDsAndButtons; button++) {
      if (digitalRead(button) == HIGH) {
        digitalWrite(button - (startButtonPin - startLEDPin), HIGH); // lights the corresponding LED
        while (digitalRead(button) == HIGH) {
          continue;
        }
        digitalWrite(button - (startButtonPin - startLEDPin), LOW);
        delay(50); // improves stability
        
        recitedPattern[index] = button;
        
        index++;
        break;
      }
    }
  }

  // compares recited pattern to the given pattern
  for(int i = 0; i < patternLength; i++) {
    if (recitedPattern[i] != randomPattern[i]) {
      flag = 1;
      break;
    }
  }
  
  Serial.println("Pattern:");
  for(int i = 0; i < patternLength; i++) {
    Serial.print(randomPattern[i] - (startButtonPin - 1));
    Serial.print(" ");
  }

  Serial.println("\nYou inputted:");
  for(int i = 0; i < patternLength; i++) {
    Serial.print(recitedPattern[i] - (startButtonPin - 1));
    Serial.print(" ");
  }
  Serial.println("");

  delay(500);
  
  // if correct button pressed, left-most button blinks twice
  // if not, left-most button blinks once
  if (flag == 1) {
    Serial.println("--> Incorrect!\n");
    digitalWrite(startLEDPin, HIGH);
    delay(1500);
    digitalWrite(startLEDPin, LOW);
  } else {
    Serial.println("--> Correct!\n");
    digitalWrite(startLEDPin, HIGH);
    delay(500);
    digitalWrite(startLEDPin, LOW);
    delay(500);
    digitalWrite(startLEDPin, HIGH);
    delay(500);
    digitalWrite(startLEDPin, LOW);
  }
}
