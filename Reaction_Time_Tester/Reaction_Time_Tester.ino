/*
 * Author: Patrick Creighton
 * Purpose: To test your reaction time when a LED lights by pressing the corresponding button
 *          keeps track of the your high score for the round and average
 * Materials: 4 LEDs with 330 ohm resistors, 4 buttons with 10K ohm resistors, jumper cables
 * Note: place buttons and LEDs in the pins immediately above their start pins
 */

const int startLEDPin = 2;
const int startButtonPin = 8;
const int numLEDsAndButtons = 4;

void setup() {
  Serial.begin(9600);

  for (int led = startLEDPin; led < startLEDPin + numLEDsAndButtons; led++) {
    pinMode(led, OUTPUT);
  }

  randomSeed(analogRead(A0));
  
  Serial.println("Reaction Time Test:");
  Serial.println("\tWhen you are ready, press the left-most button");
  Serial.println("\tWithin the next 3 seconds, a random LED will be lit");
  Serial.println("\tPress the corresponding button as fast as possible!");
  Serial.println("\tBlinks twice for correct or once for incorrect\n");
}


int delayTime;
int litLED;
unsigned long currentTime;
int start;
int pressedButton;
int reactionTime;
int highscore = 0;
int sum = 0;
int count = 0;
double average;

void loop() {
  // generate random values for delay time and LED to light up
  delayTime = random(250, 3000); // 250ms buffer after pressing start button
  litLED = random(startLEDPin, startLEDPin + numLEDsAndButtons);
  
  Serial.println("Press the left-most button to begin:");
  while (digitalRead(startButtonPin) == LOW) {
    continue;
  }

  delay(delayTime);
  digitalWrite(litLED, HIGH);
  currentTime = millis();
  
  // will not start recording until first button is pressed
  start = false;
  while (!start) { 
    for (int button = startButtonPin; button < startButtonPin + numLEDsAndButtons; button++) {
      if (digitalRead(button) == HIGH) {
        start = true;
        pressedButton = button;
        break;
      }
    }
  }

  // Doesn't count if wrong button is pressed first
  if (pressedButton != litLED - (startLEDPin - startButtonPin)) { // converts from LED to button pin #
    Serial.println("Wrong button pressed, try again\n");
  } else {
    reactionTime = millis() - currentTime;

    // determining high score
    if (highscore == 0) {
      highscore = reactionTime; // first entry is high score
    } else if (highscore > reactionTime) {
      highscore = reactionTime;
    }
  
    // calculating average
    sum += reactionTime;
    count++;
    average = (double)sum / count;
  
    // printing results to serial monitor
    Serial.print("Reaction time: ");
    Serial.print(reactionTime);
    Serial.println("ms");
    Serial.print("Highscore: ");
    Serial.print(highscore);
    Serial.println("ms");
    Serial.print("Average: ");
    Serial.print(average);
    Serial.println("ms\n");
  }
  
  delay(500);
  digitalWrite(litLED, LOW); // turns LED off after 500ms
  delay(500);

  // if correct button pressed, left-most button blinks twice
  // if not, left-most button blinks once
  if (pressedButton != litLED - (startLEDPin - startButtonPin)) {
      digitalWrite(startLEDPin, HIGH);
      delay(1500);
      digitalWrite(startLEDPin, LOW);
  } else {
      digitalWrite(startLEDPin, HIGH);
      delay(500);
      digitalWrite(startLEDPin, LOW);
      delay(500);
      digitalWrite(startLEDPin, HIGH);
      delay(500);
      digitalWrite(startLEDPin, LOW);
  }
}
