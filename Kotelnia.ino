#include "ServoController.h"
#include "TempCollector.h"


// Data wire is plugged into port 12 on the Arduino
#define ONE_WIRE_BUS 12


//servo
const uint8_t FIRST_FLOOR_WATER_POMP = 6;
const uint8_t SERVO_1_CLOSE = 4;
const uint8_t SERVO_1_OPEN = 5;
const uint8_t SERVO_2_CLOSE = 6;
const uint8_t SERVO_2_OPEN = 7;


//ServoHerz secondFloorServo;
ServoController firsFloorController;
//

TempCollector tempCollector(ONE_WIRE_BUS);

int numberOfDevices; // Number of temperature devices found

void setup(void) {
  firsFloorController.begin(SERVO_1_CLOSE, SERVO_1_OPEN, FIRST_FLOOR_WATER_POMP);
  //secondFloorServo.begin(SERVO_2_CLOSE, SERVO_2_OPEN);
  pinMode(LED_BUILTIN, OUTPUT);
  // start serial port
  Serial.begin(9600);
  Serial.println("Kotelnia 1.0");
  delay(500);

  Serial.print("Locating devices...");
  tempCollector.begin();
  numberOfDevices = tempCollector.getDeviceCount();
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  if(numberOfDevices < 2) {
    alarm_on;
  }
  //firsFloorController.setWishedTemp(14.0);
}

void loop(void) {
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  tempCollector.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  Serial.println();

  Serial.print("firstFloorForwardTemp: ");
  Serial.println(tempCollector.getTemp(FIRST_FLOOR_FORWARD_TEMP));
  Serial.print("firstFloorBackwardTemp: ");
  Serial.println(tempCollector.getTemp(FIRST_FLOOR_BACKWARD_TEMP));

  firsFloorController.process(tempCollector.getTemp(FIRST_FLOOR_FORWARD_TEMP), tempCollector.getTemp(FIRST_FLOOR_BACKWARD_TEMP));

  delay(5000);

}
