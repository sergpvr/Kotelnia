#include "ServoController.h"
#include "TempCollector.h"


// Data wire is plugged into port 12 on the Arduino
#define ONE_WIRE_BUS 12


//servo
const int FIRST_FLOOR_WATER_POMP = 11;
const int SERVO_1_CLOSE = 4;
const int SERVO_1_OPEN = 5;
const int SERVO_2_CLOSE = 6;
const int SERVO_2_OPEN = 7;


ServoHerz secondFloorServo;
ServoController firsFloorController;
//


TempCollector tempCollector(ONE_WIRE_BUS);

int numberOfDevices; // Number of temperature devices found

void setup(void) {
  firsFloorController.begin(SERVO_1_CLOSE, SERVO_1_OPEN, FIRST_FLOOR_WATER_POMP,
    tempCollector.getDeviceTempPtr(TempCollector::TempDeviceEnum::firstFloorForward), 
    tempCollector.getDeviceTempPtr(TempCollector::TempDeviceEnum::firstFloorBackward));
  secondFloorServo.begin(SERVO_2_CLOSE, SERVO_2_OPEN);
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

  
  
/*
  // Start up the library
  sensors.begin();
  delay(500);
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  if(numberOfDevices < 2) {
    alarm_on;
  }

  

  */
}

void loop(void) {
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  tempCollector.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  Serial.println();

  Serial.print("firstFloorForwardTemp: ");
  Serial.println(*tempCollector.getDeviceTempPtr(TempCollector::TempDeviceEnum::firstFloorForward));
  Serial.print("firstFloorBackwardTemp: ");
  Serial.println(*tempCollector.getDeviceTempPtr(TempCollector::TempDeviceEnum::firstFloorBackward));

  //firsFloorController.process();

  delay(5000);

}


/*
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    Serial.print(" ");
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
}
*/
