#ifndef TempCollector_h
#define TempCollector_h

#ifdef __cplusplus

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define TEMPERATURE_PRECISION 10 // 9, 10, 11, 12

struct TempDevice {
  const uint8_t* deviceAddress;
  mutable float temp;
  TempDevice(const uint8_t* _deviceAddress) {deviceAddress = _deviceAddress;}
};

#define FIRST_FLOOR_FORWARD_TEMP    0
#define FIRST_FLOOR_BACKWARD_TEMP   1
#define SECOND_FLOOR_FORWARD_TEMP   2
#define SECOND_FLOOR_BACKWARD_TEMP  3

static const DeviceAddress firstFloorForwardTempAddr  = { 0x28, 0x13, 0x5E, 0xB6, 0x33, 0x14, 0x01, 0x00 };
static const DeviceAddress firstFloorBackwardTempAddr = { 0x28, 0xCF, 0x23, 0xE1, 0x32, 0x14, 0x01, 0xCD };
static const DeviceAddress secondFloorForwardTempAddr  = { 0x28, 0x7B, 0x73, 0xDB, 0x32, 0x14, 0x01, 0xD0 };
static const DeviceAddress secondFloorBackwardTempAddr = { 0x28, 0x9C, 0x18, 0xD2, 0x32, 0x14, 0x01, 0x8E };

class TempCollector {
  private:
    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire;
    // Pass our oneWire reference to Dallas Temperature. 
    DallasTemperature sensors;

    const TempDevice tempDeviceList[4];

	public:
    TempCollector(uint8_t pin): oneWire(pin), sensors(&oneWire),
      tempDeviceList{
          TempDevice(firstFloorForwardTempAddr),
          TempDevice(firstFloorBackwardTempAddr),
          TempDevice(secondFloorForwardTempAddr),
          TempDevice(secondFloorBackwardTempAddr)
        } {}

    void begin() {
        // Start up the library
        sensors.begin();
        delay(500);
        // Loop through each device
        DeviceAddress tempDeviceAddress;
        for(uint8_t i=0; i<sensors.getDeviceCount(); i++)
        {
          // Search the wire for address
          if(sensors.getAddress(tempDeviceAddress, i))
          {
            // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
            sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
            
          }
        }
    }

    int getDeviceCount() {
      // Grab a count of devices on the wire
      return sensors.getDeviceCount();
    }

    float getTemp(uint8_t deviceNumber) {
      return tempDeviceList[deviceNumber].temp;
    }

    void requestTemperatures() {
      sensors.requestTemperatures();
      delay(100);
      const uint8_t n = sizeof(tempDeviceList)/ sizeof( TempDevice );
      TempDevice* td;
      for(uint8_t i = 0; i < n ; i++){
        const TempDevice* td = &tempDeviceList[i];
        td->temp = sensors.getTempC(td->deviceAddress);
      }
    }

    
};

#endif // __cplusplus
#endif // TempCollector_h
