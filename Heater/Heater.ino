#include <DS3231.h>
#include <Wire.h>
#include "TempCollector.h"
#include "HeaterController.h"

// Data wire is plugged into port ONE_WIRE_BUS on the Arduino
#define ONE_WIRE_BUS 10

const uint8_t BOTTOM_HEATER1 = 3;
const uint8_t BOTTOM_HEATER2 = 4;
const uint8_t MIDDLE_HEATER  = 5;

//
TempCollector tempCollector(ONE_WIRE_BUS);

int numberOfDevices; // Number of temperature devices found

HeaterController heaterController;

DS3231 Clock;

bool h12, PM, Century;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // start serial port
  Serial.begin(9600);
  Serial.println("Heater 1.0");
  delay(500);

  Serial.print("Locating devices...");
  tempCollector.begin();
  numberOfDevices = tempCollector.getDeviceCount();
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  if(numberOfDevices < 1) {
    alarm_on;
  }

  // Start the I2C interface
  Wire.begin();

  heaterController.begin(BOTTOM_HEATER1, BOTTOM_HEATER2, MIDDLE_HEATER);

}

void loop() {

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  tempCollector.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  Serial.println();

  Serial.print("middleHeaterTemp: ");
  Serial.println(tempCollector.getTemp(TOP_HEATER_TEMP));

  byte curHour = Clock.getHour(h12, PM);
  Serial.print("hour: ");
  Serial.println(Clock.getHour(h12, PM), DEC);

  heaterController.process(tempCollector.getTemp(TOP_HEATER_TEMP), curHour);
  
  delay(5000);
  /*
  Serial.print(Clock.getYear(), DEC);
  Serial.print("-");
  Serial.print(Clock.getMonth(Century), DEC);
  Serial.print("-");
  Serial.print(Clock.getDate(), DEC);
  Serial.print(" ");
  Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
  Serial.print(":");
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(Clock.getSecond(), DEC);
  */
}
