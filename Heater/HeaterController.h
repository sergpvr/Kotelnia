#ifndef HeaterController_h
#define HeaterController_h

#ifdef __cplusplus

#include <Arduino.h>
#include "DigitalFilter.h"

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

    void process(int8_t _topTemp, int8_t _bottomTemp, uint8_t hours) {
      // it's temporary to fix sensor's bugs
      if (_topTemp < 15 || _topTemp > 90) {
        if ( ++wrongTopValue > 3 ) {
          this->stopAndAlarm();
          return;
        }
        return;
      } else {
        wrongTopValue = 0;
      }

      if (_bottomTemp < 15 || _bottomTemp > 90 ) {
        if ( ++wrongBottomValue > 3 ) {
          this->stopAndAlarm();
          return;
        }
        return;
      } else {
        wrongBottomValue = 0;
      }
      //

      int8_t topTemp = dFilterTop.doFilter(_topTemp);
      int8_t bottomTemp = dFilterBottom.doFilter(_bottomTemp);
      if(!checkAndSaveTemperature(topTemp, bottomTemp)) {
        this->stopAndAlarm();
        return;
      }

      alarm_off;

      bool nightTime = hours == 23 || (hours >= 0 && hours < 7); // from 11 PM to 7 AM
      bool lastPartOfNight = hours >= 4 && hours < 7; // from 4 AM to 7 AM

      if (!nightTime && !heatDuringTheDay) {
        this->stop();
        return;
      }

      int allowedTemp =  lastPartOfNight ? nightAllowedTemp : dayAllowedTemp;
      int bottomLevelTemp = allowedTemp - (hours == 6 ? 5 : (lastPartOfNight? nightDeviation : deviation));
      
      if(topTemp > allowedTemp) {
        this->stop();
      } else {
        if (topTemp < bottomLevelTemp) {
          if(nightTime || heatDuringTheDay) { // switch ON only at night if nightHeaterOnly mode
            this->start();
          }
        }
      }
      return;    
    }

  private:
    DigitalFilter dFilterTop = DigitalFilter(50);
    DigitalFilter dFilterBottom = DigitalFilter(50);
    uint8_t wrongTopValue = 0;
    uint8_t wrongBottomValue = 0;

	  //
	  uint8_t pinBottomHeater1;
	  uint8_t pinBottomHeater2;
	  uint8_t pinMiddleHeater;
    //
	  static const int8_t deviation = 18;
    static const int8_t nightDeviation = 28;
    static const int8_t nightAllowedTemp = 70;//85
    static const int8_t dayAllowedTemp = 60;
    static const boolean heatDuringTheDay = false;
    //
    bool heat = false;
	  
	  void stopAndAlarm() {
      alarm_on;
      this->stop();
	  }
	  
	  bool checkAndSaveTemperature(int8_t topTemp, int8_t bottomTemp) {
	    if ( topTemp < 0 ) {
        return false;
	    }
      if ( bottomTemp < 0 ) {
        return false;
      }

      if (topTemp + 10 < bottomTemp) {
        return false;
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
