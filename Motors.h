#ifndef Motors_h
#define Motors_h

#include "Arduino.h"
#include <Servo.h>

#include "Structs.h"

class Motors {

	public:
		Motors();
		void init(); 
		void update(Ctrl ctrl,int throttle,int errorState); 
		void kill();
		void print(); 

		int command[4];
		float KP;
		float KI;
		float KD;
		Data d; 

	private:
		void set(int throttle); 
		float correction[4]; 
		int pins[4]; 
		Servo servos[4];  

};


#endif