#ifndef TempCollector_h
#define TempCollector_h

#ifdef __cplusplus

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define TEMPERATURE_PRECISION 9 // 9, 10, 11, 12

struct TempDevice {
  const uint8_t* deviceAddress;
  mutable float temp;
  TempDevice(const uint8_t* _deviceAddress) {deviceAddress = _deviceAddress;}
};

#define TOP_HEATER_TEMP       0
#define BOTTOM_HEATER_TEMP    1

//28 A6 AA 77 91 0D 02 E6  - sensor B5  <1C
//28 FE 0C 77 91 04 02 74  - sensor B6  <0.8C

static const DeviceAddress topHeaterTempAddr    = { 0x28, 0xFE, 0x0C, 0x77, 0x91, 0x04, 0x02, 0x74 };
static const DeviceAddress bottomHeaterTempAddr = { 0x28, 0xA6, 0xAA, 0x77, 0x91, 0x0D, 0x02, 0xE6 };

class TempCollector {
  private:
    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire;
    // Pass our oneWire reference to Dallas Temperature. 
    DallasTemperature sensors;

    const TempDevice tempDeviceList[2];

	public:
    TempCollector(uint8_t pin): oneWire(pin), sensors(&oneWire),
      tempDeviceList{
          TempDevice(topHeaterTempAddr),
          TempDevice(bottomHeaterTempAddr)
        } {}

    void begin() {
        // Start up the library
        sensors.begin();
        delay(50);
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
      delay(50);
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
