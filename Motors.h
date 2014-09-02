#ifndef Motors_h
#define Motors_h

#include "Arduino.h"
#include <Servo.h>

#include "Structs.h"

class Motors {

	public:
		Motors();
		void init(); 
		void update(Ctrl ctrl,int throttle); 
		void kill();
		void set(int throttle); 

		float KP;
		float KI;
		float KD;
		Data d; 

	private:
		float correction[4]; 
		int pins[4]; 
		Servo servos[4];  

};


#endif