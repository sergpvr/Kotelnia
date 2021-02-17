#ifndef HeaterSerial_h
#define HeaterSerial_h

#ifdef __cplusplus

#include <Arduino.h>

class HeaterSerial {
  public:
    HeaterSerial(HardwareSerial *_serial): serial(_serial) {
      // reserve 100 bytes for the inputString:
      inputString.reserve(100);

    }

    void serialEvent() {
      if (stringComplete == true) {
        return;
      }
      while (serial->available()) {
        // get the new byte:
        char inChar = (char)serial->read();

        if (inChar == '\n') {
          inputString = "";
          stringComplete = false;
          break;
        }
        
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == 'x') {
          stringComplete = true;
          break;
        }
      }
    }

    bool getDateStuff(byte& year, byte& month, byte& day, byte& doW, 
      byte& hour, byte& minute, byte& second) {
      // The stuff coming in should be in 
      // the order YYMMDDwHHMMSS, with an 'x' at the end.
      
      if (stringComplete) {
        serial->println(inputString);

        byte temp1, temp2;
        //char InString[20];

        // Read Year first
        temp1 = (byte)inputString[0] - 48;
        temp2 = (byte)inputString[1] - 48;
        year = temp1*10 + temp2;
        // now month
        temp1 = (byte)inputString[2] - 48;
        temp2 = (byte)inputString[3] - 48;
        month = temp1*10 + temp2;
        // now date
        temp1 = (byte)inputString[4] - 48;
        temp2 = (byte)inputString[5] - 48;
        day = temp1*10 + temp2;
        // now Day of Week
        doW = (byte)inputString[6] - 48;
        // now Hour
        temp1 = (byte)inputString[7] - 48;
        temp2 = (byte)inputString[8] - 48;
        hour = temp1*10 + temp2;
        // now Minute
        temp1 = (byte)inputString[9] - 48;
        temp2 = (byte)inputString[10] - 48;
        minute = temp1*10 + temp2;
        // now Second
        temp1 = (byte)inputString[11] - 48;
        temp2 = (byte)inputString[12] - 48;
        second = temp1*10 + temp2;

        // clear the string:
        inputString = "";
        stringComplete = false;
        //
        return true;
      }
      return false;
    }

  private:
    HardwareSerial *serial;
    String inputString = "";      // a String to hold incoming data
    bool stringComplete = false;  // whether the string is complete
};

#endif // __cplusplus
#endif // HeaterSerial_h
