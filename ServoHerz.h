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
		    digitalWrite(pinLeft, LOW);
		    pinMode(pinRight, OUTPUT);
		    digitalWrite(pinRight, LOW);
		  }

		  void left() {
		    digitalWrite(pinRight, LOW);
		    digitalWrite(pinLeft, HIGH);
		  }

		  void right() {
		    digitalWrite(pinLeft, LOW);
		    digitalWrite(pinRight, HIGH);
		  }

		  void stop() {
		    digitalWrite(pinLeft, LOW);
		    digitalWrite(pinRight, LOW);
		  }

};

#endif // __cplusplus
#endif // ServoHerz_h
