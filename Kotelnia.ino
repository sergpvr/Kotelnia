#include <OneWire.h>
#include <DallasTemperature.h>
#include "ServoController.h"



// Data wire is plugged into port 12 on the Arduino
#define ONE_WIRE_BUS 12
#define TEMPERATURE_PRECISION 10 // Lower resolution

//servo
const int FIRST_FLOOR_WATER_POMP = 11;
const int SERVO_1_CLOSE = 4;
const int SERVO_1_OPEN = 5;
const int SERVO_2_CLOSE = 6;
const int SERVO_2_OPEN = 7;


ServoHerz secondFloorServo;
ServoController firsFloorController;
//

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address
DeviceAddress firstFloorForwardTempAddr  = { 0x28, 0x13, 0x5E, 0xB6, 0x33, 0x14, 0x01, 0x00 };
DeviceAddress firstFloorBackwardTempAddr = { 0x28, 0xCF, 0x23, 0xE1, 0x32, 0x14, 0x01, 0xCD };

void setup(void) {
  firsFloorController.begin(SERVO_1_CLOSE, SERVO_1_OPEN, FIRST_FLOOR_WATER_POMP, /*TODO*/);
  secondFloorServo.begin(SERVO_2_CLOSE, SERVO_2_OPEN);
  pinMode(LED_BUILTIN, OUTPUT);
  // start serial port
  Serial.begin(9600);
  Serial.println("Kotelnia 1.0");
  delay(1000);

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

  
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
    {
      // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
      sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
      
    }
  }
}

void loop(void) {
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  delay(500);
  Serial.println("DONE");
  Serial.println();

  Serial.print("firstFloorForwardTempAddr: ");
  printTemperature(firstFloorForwardTempAddr);
  Serial.println();
  Serial.print("firstFloorBackwardTempAddr: ");
  printTemperature(firstFloorBackwardTempAddr);
  Serial.println();
  
  // Loop through each device, print out temperature data
  /*for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
      {
        // Output the device ID
        Serial.print("Temperature for device: ");
        Serial.print(i,DEC);
        Serial.print(" Address: ");
        printAddress(tempDeviceAddress);
        Serial.print(" ");
        // It responds almost immediately. Let's print out the data
        printTemperature(tempDeviceAddress); // Use a simple function to print out the data
       Serial.println();
      } 
  //else ghost device! Check your power requirements and cabling
  
  }*/
  delay(5000);

}

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
