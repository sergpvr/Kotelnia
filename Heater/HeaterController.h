#ifndef HeaterController_h
#define HeaterController_h

#ifdef __cplusplus

#include <Arduino.h>

#define alarm_on digitalWrite(LED_BUILTIN, HIGH)
#define alarm_off digitalWrite(LED_BUILTIN, LOW)

class HeaterController {
  public:
    void begin(uint8_t _pinBottomHeater1, uint8_t _pinBottomHeater2, uint8_t _pinMiddleHeater) {
      pinBottomHeater1 = _pinBottomHeater1;
      pinBottomHeater2 = _pinBottomHeater2;
      pinMiddleHeater = _pinMiddleHeater;
      pinMode(pinBottomHeater1, OUTPUT);
      digitalWrite(pinBottomHeater1, LOW);
      pinMode(pinBottomHeater2, OUTPUT);
      digitalWrite(pinBottomHeater2, LOW);
      pinMode(pinMiddleHeater, OUTPUT);
      digitalWrite(pinMiddleHeater, LOW);
    }

    void process(float topTemp, float bottomTemp, uint8_t hours) {
      if(!checkAndSaveTemperature(topTemp, bottomTemp)) {
        this->stopAndAlarm();
        return;
      }
      
      if (topTemp < 10) {
        return; // to miss wrong parameter
      }

      alarm_off;

      bool nightMode = hours == 23 || (hours >= 0 && hours < 7);
      
      int allowedTemp =  nightMode ? nightAllowedTemp : dayAllowedTemp;
      int bottomLevelTemp = allowedTemp - (hours == 6 ? 5 : deviation);
      
      if(topTemp > allowedTemp) {
        this->stop();
      } else {
        if (topTemp < bottomLevelTemp) {
          this->start();
        }
      }
      return;    
    }

  private:
	  float topTempAcc = 0;
    float bottomTempAcc = 0;
	  //
	  uint8_t pinBottomHeater1;
	  uint8_t pinBottomHeater2;
	  uint8_t pinMiddleHeater;
    //
	  static const int deviation = 20;
    static const int nightAllowedTemp = 80;
    static const int dayAllowedTemp = 60;
    //
    bool heat = false;
	  
	  void stopAndAlarm() {
      alarm_on;
      this->stop();
	  }
	  
	  bool checkAndSaveTemperature(float topTemp, float bottomTemp) {
	    if ( topTemp < 0 && topTempAcc < 0 ) {
        return false;
	    }
      if ( bottomTemp < 0 && bottomTempAcc < 0 ) {
        return false;
      }

      topTempAcc = topTemp;
      bottomTempAcc = bottomTemp;

      if ( topTemp > 0 && bottomTemp > 0 ) {
        if (topTemp + 1 < bottomTemp) {
          return false;
        }
      }

      return true; 
	  }
	  
	  void start() {
      if(!heat) {
  	    digitalWrite(pinBottomHeater1, HIGH);
  	    delay(1000);
  	    digitalWrite(pinBottomHeater2, HIGH);
  	    delay(1000);
  	    digitalWrite(pinMiddleHeater, HIGH);

        heat = true;
      }
	  }

    void stop() {
      if(heat) {
  	    digitalWrite(pinBottomHeater1, LOW);
  	    delay(1000);
  	    digitalWrite(pinBottomHeater2, LOW);
  	    delay(1000);
  	    digitalWrite(pinMiddleHeater, LOW);

       heat = false;
      }
    }

};

#endif // __cplusplus
#endif // HeaterController_h
