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
      requiredTemp = defaultRequiredTemp;
    }

    void setRequiredTemp(float temp) {
      requiredTemp = temp;
    }
    
    void process(float forwardTemp, float backwardTemp) {
      if(!checkAndSaveTemperature(forwardTemp, backwardTemp)) {
        this->stopAndAlarm();
        return;
      }
      if (forwardTemp < 0) {
        return; // to miss one wrong parameter
      }

      if (waitingHigherTemp) {
        if ( forwardTemp > 40 || backwardTemp > 40 ) {
          this->stopServoWaterPompOn();
          waitingHigherTemp = false;
        }
      }

      if(forwardTemp > requiredTemp + deviation) {
        this->left();
      } else if (forwardTemp + deviation < requiredTemp) {
        this->right();
      } else {
        this->stopServoWaterPompOn();
      }
      return;    
    }

  private:
    ServoHerz servo;
    float forwardTempAcc = 0;
    float backwardTempAcc = 0;
    float requiredTemp;
    uint8_t pinWaterPomp;
    //
    const int deviation = 1;
    static const int defaultRequiredTemp = 20;
    //
    static const long interval = 150000; //150 sec
    unsigned long servoRegulatorStartMovementTime = 0;
    unsigned long waterPompStartTime = 0;
    bool movingLeftFlag = false;
    bool movingRightFlag = false;
    bool waterPompFlag = false;
    bool waitingHigherTemp = false;
    //

    void stopAndAlarm() {
      this->stopServoWaterPompOn(); // it must be first because this method sets waterPompFlag to true
      this->waterPomp(false);
      alarm_on;
    }

    bool checkAndSaveTemperature(float forwardTemp, float backwardTemp) {
      if ( forwardTemp < 0 && forwardTempAcc < 0 ) {
        return false;
      }
      if ( backwardTemp < 0 && backwardTempAcc < 0 ) {
        return false;
      }
      if(waterPompFlag) {
        if (outOfTimeInterval(millis(), waterPompStartTime, interval)) {
          // forward stream temperature should be less then backward stream
          if ( forwardTemp > 0 && backwardTemp > 0 && forwardTemp + 2*deviation < backwardTemp ) {
            return false;
          }
          // the temperature of the water floor should be less then 45
          if( forwardTemp > 45 && forwardTempAcc > 45) {
            return false;
          }
        }
      }

      forwardTempAcc = forwardTemp;
      backwardTempAcc = backwardTemp;
      return true; 
    }

    void left() {
      if(movingLeftFlag) {
        if (outOfTimeInterval(millis(), servoRegulatorStartMovementTime, interval)) {
          // regulator is in a minimal position at this moment
          servo.stop();
          // but temperature is sill high
          // stoping waterPomp to floor protect
          waterPomp(false);
        }
      } else {
        waterPomp(true);
        servo.left();
        movingLeftFlag = true;
        movingRightFlag = false;
        servoRegulatorStartMovementTime = millis();
      }  
    }

    void right() {
      if(movingRightFlag) {
        if (outOfTimeInterval(millis(), servoRegulatorStartMovementTime, interval)) {
          // regulator is in a maximum position  at this moment
          servo.stop();
          // but temperature is sill low
          // stoping waterPomp to keep floor heated
          waterPomp(false);
          waitingHigherTemp = true;
        }
      } else {
        waterPomp(true);
        servo.right();
        movingLeftFlag = false;
        movingRightFlag = true;
        servoRegulatorStartMovementTime = millis();
      }
    }

    void stopServoWaterPompOn() {
      // temperature of the water stream is close to required
      // keeping regulator in this position
      servo.stop();
      movingLeftFlag = false;
      movingRightFlag = false;
      waterPomp(true);
    }

    void waterPomp(bool stream) {
      if(!waterPompFlag && stream) {
        // it is first command to start
        waterPompStartTime = millis();
      }
      waterPompFlag = stream;
      if(stream) {
        digitalWrite(pinWaterPomp, HIGH);
      } else {
        digitalWrite(pinWaterPomp, LOW);
      }
    }

    bool outOfTimeInterval(const unsigned long currentMillis, unsigned long & startedTime, const long interval) {
        //overflow (go back to zero), after approximately 50 days
        //check overflow:
        if(startedTime > currentMillis) {
          startedTime = currentMillis;
        }
        return currentMillis - startedTime > interval;
    }

};

#endif // __cplusplus
#endif // ServoController_h
