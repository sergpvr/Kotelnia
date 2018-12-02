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

#define TOP_HEATER_TEMP       0
#define BOTTOM_HEATER_TEMP    1

//28 32 88 77 91 10 02 95
static const DeviceAddress topHeaterTempAddr  = { 0x28, 0x32, 0x88, 0x77, 0x91, 0x10, 0x02, 0x95 };

class TempCollector {
  private:
    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire;
    // Pass our oneWire reference to Dallas Temperature. 
    DallasTemperature sensors;

    const TempDevice tempDeviceList[1];

	public:
    TempCollector(uint8_t pin): oneWire(pin), sensors(&oneWire),
      tempDeviceList{
          TempDevice(topHeaterTempAddr)
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
      delay(500);
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
