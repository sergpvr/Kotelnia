#include <DS3231.h>
#include <Wire.h>
#include "TempCollector.h"
#include "HeaterController.h"
#include "HeaterSerial.h"

// Data wire is plugged into port ONE_WIRE_BUS on the Arduino
#define ONE_WIRE_BUS 7

const uint8_t BOTTOM_HEATER1 = 4;
const uint8_t BOTTOM_HEATER2 = 5;
const uint8_t MIDDLE_HEATER  = 6;

//
TempCollector tempCollector(ONE_WIRE_BUS);

HeaterSerial heaterSerial(&Serial);

int numberOfDevices; // Number of temperature devices found

HeaterController heaterController;

DS3231 Clock;

byte year;
byte month;
byte date;
byte doW;
byte hour;
byte minute;
byte second;

bool h12, PM, Century;

void setup() {
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  
  heaterController.begin(BOTTOM_HEATER1, BOTTOM_HEATER2, MIDDLE_HEATER);
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
}

void loop() {

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  tempCollector.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  Serial.println();

  Serial.print("topHeaterTemp: ");
  float topHeaterTemp = tempCollector.getTemp(TOP_HEATER_TEMP);
  Serial.println(topHeaterTemp);

  Serial.print("bottomHeaterTemp: ");
  float bottomHeaterTemp = tempCollector.getTemp(BOTTOM_HEATER_TEMP);
  Serial.println(bottomHeaterTemp);


  byte curHour = Clock.getHour(h12, PM);
  Serial.print("hour: ");
  Serial.println(curHour, DEC);

  heaterController.process(topHeaterTemp, bottomHeaterTemp, curHour);

  heaterSerial.serialEvent();
  if (heaterSerial.getDateStuff(year, month, date, doW, hour, minute, second)) {
    // The stuff coming in should be in
    // the order YYMMDDwHHMMSS, with an 'x' at the end.
    Clock.setClockMode(false);  // set to 24h

    Clock.setYear(year);
    Clock.setMonth(month);
    Clock.setDate(date);
    Clock.setDoW(doW);
    Clock.setHour(hour);
    Clock.setMinute(minute);
    Clock.setSecond(second);

    printClock();

  }

  delay(10000);

}

void printClock() {
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
}
