#ifndef SerialComm_h
#define SerialComm_h

#ifdef __cplusplus

#include <Arduino.h>

class SerialComm {
  public:
    SerialComm(HardwareSerial *_serial): serial(_serial) {
      // reserve 200 bytes for the inputString:
      inputString.reserve(100);

    }
    
    void serialEvent() {
      if (stringComplete == true) {
        return;
      }

      while (serial->available()) {
        // get the new byte:
        char inChar = (char)serial->read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
          stringComplete = true;
          break;
        }
      }
    }

    void process() {
      if (stringComplete) {
        serial->println(inputString);
        // clear the string:
        inputString = "";
        stringComplete = false;
      }
    }
    
  private:
    HardwareSerial *serial;
    String inputString = "";      // a String to hold incoming data
    bool stringComplete = false;  // whether the string is complete
};

#endif // __cplusplus
#endif // SerialComm_h
