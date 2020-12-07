/*
 * Author: Patrick Creighton
 * Purpose: Prototype alarm system for a bedroom, will put a computer 
 *          to sleep if one of the various triggers (line 90) are tripped.
 * Materials: Servo motor, active buzzer, buttons(x4), 10k Ohm resistors(x5), 
 *            330 Ohm resistors(x3), tilt sensor, PIR motion sensor, RGB LED
 * Notes: Adjust the sensitivity and delay time potentiometers on the PIR
 *        motion sensor for your room
 */

#include <Servo.h>

Servo myservo; // attached to pin 3

// connect devices to their corresponding digital pin
const int tiltSense = 2;
const int failSafe = 4;
const int motionSense = 5;
const int button1 = 6;
const int button2 = 7;
const int button3 = 8;
const int button4 = 9;
const int redPin = 10;
const int greenPin = 11;
const int bluePin = 12;
const int buzzer = 13;

// constants used
#define START_POS 100
#define END_POS 170
#define ONE_SEC 1000
#define MS_TO_S 1000.0
#define S_TO_MIN 60.0
#define PSW_LENGTH 5
#define NUM_ATTEMPTS 3
#define ALARM_LENGTH 20 // in seconds

// initializing variables used
unsigned long detectedTime;
int index;
int checkEntry;

// array containing disarm password
int password[PSW_LENGTH] = {button2, button3, button1, button3, button4};

void setup()
{
  Serial.begin(9600); // initializing serial monitor
  Serial.println("Security system initializing");
  
  myservo.attach(3); // initializing servo motor at pin 3
  myservo.write(START_POS);

  // initializing input devices
  pinMode(tiltSense, INPUT);
  pinMode(failSafe, INPUT);
  pinMode(motionSense, INPUT);

  // initializing output devices
  for(int pin = button1; pin <= buzzer; pin++) {
    pinMode(pin, OUTPUT);
  }

  Serial.println("12 seconds until system is armed");
  countdown();
  Serial.println("System armed");
  alert();  

  // waiting until motion is detected
  while(digitalRead(motionSense) == LOW) {
    continue;
  }

  // recording time that motion was detected
  detectedTime = millis();
  Serial.print("Motion detected ");
  Serial.print(detectedTime / MS_TO_S / S_TO_MIN);
  Serial.println(" minutes from start");    

  // starting alarm
  setColour(HIGH, HIGH, LOW); // yellow
  digitalWrite(buzzer, HIGH);

  // "password attempts" loop
  for(int count = 0; count < NUM_ATTEMPTS; count++) {
    // reset for each attempt
    index = 0; // used to compare values to the password array
    checkEntry = true; // flag for incorrect button press

    /* Boolean expressions are the triggers:
     *    1) 20 seconds has passed
     *    2) tilt sensor on arduino has moved
     *    3) buzzer was removed
     *    4) 3 incorrect password attempts
     *    (last expression is for when the password is entered correctly)
     */
    while(millis() - detectedTime <= ALARM_LENGTH * ONE_SEC && digitalRead(tiltSense) == HIGH && digitalRead(failSafe) == HIGH && checkEntry && index < PSW_LENGTH) {
      // scanning all butons for input
      for (int button = button1; button <= button4; button++) {
        if (digitalRead(button) == HIGH) {
          while (digitalRead(button) == HIGH) {
            continue;
          }
          delay(5); //improves stability
          
          if(button != password[index]) {
            // stopping alarm to play alert
            digitalWrite(buzzer, LOW);
            setColour(LOW, LOW, LOW);
            delay(ONE_SEC / 2);
            
            alert();

            // restarting alarm after alert
            delay(ONE_SEC / 2);
            setColour(HIGH, HIGH, LOW);
            digitalWrite(buzzer, HIGH);
            
            checkEntry = false; // if incorrect button pressed, attempt has failed
            break;
          }
          
          index++; // if correct button is pressed, moves on to next value of password array
        }
      }
    }
    // if one of the triggers are tripped, break out of the "password attempts" loop
    if(millis() - detectedTime > ALARM_LENGTH * ONE_SEC || digitalRead(tiltSense) == LOW || digitalRead(failSafe) == LOW || index == PSW_LENGTH) {
      break;
    }
  }
  // cases where one of the triggers were tripped
  if(millis() - detectedTime > ALARM_LENGTH * ONE_SEC || digitalRead(tiltSense) == LOW || digitalRead(failSafe) == LOW || !checkEntry) {
    // prints to serial monitor which trigger was tripped
    if(millis() - detectedTime > ALARM_LENGTH * ONE_SEC) {
      Serial.println("Time limit surpassed");
    }
    else if(digitalRead(tiltSense) == LOW) {
      Serial.println("Tampered with arduino");
    }
    else if(digitalRead(failSafe) == LOW) {
      Serial.println("Took out buzzer");
    }
    else {
      Serial.println("Out of password attempts");
    }

    // turn off alarm
    digitalWrite(buzzer, LOW);
    setColour(LOW, LOW, LOW);

    // presses computer power button...
    for (int pos = START_POS; pos <= END_POS; pos += 1) {
    myservo.write(pos);
    delay(10);
    }
    // ... then releases it, turning computer to sleep
    for (int pos = END_POS; pos >= START_POS; pos -= 1) { 
      myservo.write(pos);
      delay(10);
    }
  }
  // case where correct password is entered
  else { 
    // turn off alarm
    digitalWrite(buzzer, LOW);
    setColour(LOW, LOW, LOW);
    delay(ONE_SEC / 5);

    Serial.println("Password accepted");
    disarmAlert();    
  }
}

void countdown()
{
  for(int pause = ONE_SEC; pause > 50; pause /= 1.3) {
    tone(buzzer, 2800, pause / 2);
    setColour(HIGH, LOW, LOW); // red
    delay(pause);
    tone(buzzer, 2800, pause / 2);
    setColour(LOW, HIGH, LOW); // blue
    delay(pause);
    tone(buzzer, 2800, pause / 2);
    setColour(LOW, LOW, HIGH); // green
    delay(pause);
  }
  setColour(LOW, LOW, LOW); // off
}
  
void alert()
{
  for(int count = 0; count < 3; count++) {
    digitalWrite(buzzer, HIGH);
    setColour(HIGH, LOW, LOW); // red
    delay(ONE_SEC / 15);
    digitalWrite(buzzer, LOW);
    setColour(LOW, LOW, LOW);
    delay(ONE_SEC / 15);
  }  
}

void disarmAlert()
{
  setColour(0, 255, 0);
    digitalWrite(buzzer, HIGH);
    delay(ONE_SEC / 5);
    setColour(0, 0, 0);
    digitalWrite(buzzer, LOW);
    delay(ONE_SEC / 5);
    setColour(0, 255, 0);
    digitalWrite(buzzer, HIGH);
    delay(ONE_SEC / 5);
    setColour(0, 0, 0);
    digitalWrite(buzzer, LOW);
}

void setColour(int red, int green, int blue)
{
  digitalWrite(redPin, red);
  digitalWrite(greenPin, green);
  digitalWrite(bluePin, blue);
}

void loop()
{
}
