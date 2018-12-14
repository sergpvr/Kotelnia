#ifndef SerialConnector_h
#define SerialConnector_h

#ifdef __cplusplus

#include <Arduino.h>

class SerialConnector {
  public:
    void serialEvent() {
	}
  private:
    HardwareSerial *serial;
  
};

#endif // __cplusplus
#endif // SerialConnector_h