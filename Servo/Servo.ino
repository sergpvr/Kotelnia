#include "ServoHerz.h"

const int SERVO_1_CLOSE = 4;
const int SERVO_1_OPEN = 5;
const int SERVO_2_CLOSE = 6;
const int SERVO_2_OPEN = 7;

ServoHerz firstFloorServo, secondFloorServo;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  firstFloorServo.begin(4, 5);
  secondFloorServo.begin(6, 7);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  firstFloorServo.left();
  secondFloorServo.right();
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  firstFloorServo.right();
  secondFloorServo.left();
  delay(1000);                       // wait for a second
}
