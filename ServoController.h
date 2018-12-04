#ifndef ServoController_h
#define ServoController_h

#ifdef __cplusplus

#include <Arduino.h>
#include "ServoHerz.h"

#define alarm_on digitalWrite(LED_BUILTIN, HIGH)
#define alarm_off digitalWrite(LED_BUILTIN, LOW)

class ServoController {
  public:
    void begin(uint8_t _pinLeft, uint8_t _pinRight, uint8_t _pinWaterPomp) {
      servo.begin(_pinLeft, _pinRight);
      pinWaterPomp = _pinWaterPomp;
      pinMode(_pinWaterPomp, OUTPUT);
      waterPomp(false);
      servo.stop();
      wishedTemp = defaultWishedTemp;
    }

    void setWishedTemp(float temp) {
      wishedTemp = temp;
    }
    
    void process(float forwardTemp, float backwardTemp) {
      if(!checkAndSaveTemperature(forwardTemp, backwardTemp)) {
        this->stopAndAlarm();
        return;
      }
      if (forwardTemp < 0 || forwardTemp > 84) {
        return; // to miss wrong parameter
      }
      if(forwardTemp > wishedTemp + deviation) {
        this->left();
      } else if (forwardTemp  + deviation < wishedTemp) {
        this->right();
      } else {
        this->stop();
      }
      waterPomp(true);
      return;    
    }

  private:
	  ServoHerz servo;
	  float forwardTempAcc = 0;
	  float backwardTempAcc = 0;
	  float wishedTemp;
	  uint8_t pinWaterPomp;
   //
	  const int deviation = 1;
    static const int defaultWishedTemp = 20;
    //
    static const long interval = 150000; //150 sec
    unsigned long previousMillis = 0;
    bool movingLeftFlag = false;
    bool movingRightFlag = false;
    //
	  
	  void stopAndAlarm() {
	    waterPomp(false);
      alarm_on;
      this->stop();
	  }
	  
	  bool checkAndSaveTemperature(float forwardTemp, float backwardTemp) {
	    if ( forwardTemp < 0 && forwardTempAcc < 0 ) {
        return false;
	    }
	    if ( backwardTemp < 0 && backwardTempAcc < 0 ) {
        return false;
	    }
	    /*if ( forwardTemp > 0 && backwardTemp > 0 && forwardTemp + 2*deviation < backwardTemp ) {
        return false;
	    }*/
	    
	    forwardTempAcc = forwardTemp;
      backwardTempAcc = backwardTemp;
      return true; 
	  }

    void left() {
      unsigned long currentMillis = millis();
      //overflow (go back to zero), after approximately 50 days
      //check overflow:
      if(previousMillis > currentMillis) {
        previousMillis = currentMillis;
      }
      
      if(movingLeftFlag) {
        if (currentMillis - previousMillis >= interval) {
          servo.stop();
        }
      } else {
        servo.left();
        movingLeftFlag = true;
        movingRightFlag = false;
        previousMillis = currentMillis;
      }  
    }

    void right() {
      unsigned long currentMillis = millis();
      //overflow (go back to zero), after approximately 50 days
      //check overflow:
      if(previousMillis > currentMillis) {
        previousMillis = currentMillis;
      }
      
      if(movingRightFlag) {
        if (currentMillis - previousMillis >= interval) {
          servo.stop();
        }
      } else {
        servo.right();
        movingLeftFlag = false;
        movingRightFlag = true;
        previousMillis = currentMillis;      
      }
    }

    void stop() {
      servo.stop();
      movingLeftFlag = false;
      movingRightFlag = false;
    }

    void waterPomp(bool stream) {
      if(stream) {
        digitalWrite(pinWaterPomp, HIGH);
      } else {
        digitalWrite(pinWaterPomp, LOW);
      }
    }
    


};

#endif // __cplusplus
#endif // ServoController_h
