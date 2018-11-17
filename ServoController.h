#ifndef ServoController_h
#define ServoController_h

#ifdef __cplusplus

#include <Arduino.h>
#include "ServoHerz.h"

#define alarm_on digitalWrite(LED_BUILTIN, HIGH)
#define alarm_off digitalWrite(LED_BUILTIN, LOW)
#define waterPomp_on digitalWrite(pinWaterPomp, LOW)
#define waterPomp_off digitalWrite(pinWaterPomp, HIGH)

class ServoController {
  private:
	  ServoHerz servo;
	  float *forwardTemp;
	  float *backwardTemp;
	  float forwardTempAcc;
	  float backwardTempAcc;
	  float wishedTemp;
	  uint8_t pinWaterPomp;
	  const int deviation = 1;
	  
	  void stop() {
	    waterPomp_off;
      alarm_on;
      servo.stop();
	  }
	  
	  bool checkAndSaveTemperature() {
	    if ( *forwardTemp < 0 && forwardTempAcc < 0 ) {
        return false;
	    }
	    if ( *backwardTemp < 0 && backwardTempAcc < 0 ) {
        return false;
	    }
	    if (*forwardTemp > 0 && *backwardTemp > 0 && *forwardTemp + deviation < *backwardTemp) {
        return false;
	    }
	    
	    forwardTempAcc = *forwardTemp;
      backwardTempAcc = *backwardTemp;
      return true; 
	  }

	public:
		void begin(uint8_t _pinLeft, uint8_t _pinRight, uint8_t _pinWaterPomp, float *_forwardTemp, float *_backwardTemp) {
      servo.begin(_pinLeft, _pinRight);
      pinWaterPomp = _pinWaterPomp;
      pinMode(_pinWaterPomp, OUTPUT);
      waterPomp_off;
      servo.stop();
      setWishedTemp(20.0);
    }
    
    void setWishedTemp(float temp) {
      wishedTemp = temp;
    }
    
    void process() {
      if(!checkAndSaveTemperature()) {
        stop();
        return;
      }
      if(*forwardTemp > wishedTemp + deviation) {
        servo.left();
      } else if (*forwardTemp  + deviation < wishedTemp) {
        servo.right();
      } else {
        servo.stop();
      }
      waterPomp_on;
      return;     
    }
    
};

#endif // __cplusplus
#endif // ServoController_h
