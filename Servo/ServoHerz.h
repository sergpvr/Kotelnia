#ifndef ServoHerz_h
#define ServoHerz_h

#ifdef __cplusplus

#include <Arduino.h>

class ServoHerz {
	private:
		uint8_t pinLeft;
		uint8_t pinRight;

	public:
		  void begin(uint8_t _pinLeft, uint8_t _pinRight) {
		    pinLeft = _pinLeft;
		    pinRight = _pinRight;

		    pinMode(pinLeft, OUTPUT);
		    digitalWrite(pinLeft, HIGH);
		    pinMode(pinRight, OUTPUT);
		    digitalWrite(pinRight, HIGH);
		  }

		  void left() {
		    digitalWrite(pinRight, HIGH);
		    digitalWrite(pinLeft, LOW);
		  }

		  void right() {
		    digitalWrite(pinLeft, HIGH);
		    digitalWrite(pinRight, LOW);
		  }

		  void stop() {
		    digitalWrite(pinLeft, HIGH);
		    digitalWrite(pinRight, HIGH);
		  }

};

#endif // __cplusplus
#endif // ServoHerz_h
