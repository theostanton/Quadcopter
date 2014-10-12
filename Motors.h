#ifndef Motors_h
#define Motors_h

#include "Arduino.h"
#include <Servo.h>

#include "Structs.h"

class Motors {

	public:
		Motors();
		void init(); 
		void update(Ctrl ctrl,int throttle,int errorState, float rxYaw); 
		void kill();
		void print(); 
		void send(); 
		void twitch(); 

		void setKD( float kd );
		void setKP( float kp );

		void setYD( float yd );
		void setYP( float yp );

		int command[4];
		float KP;
		float KI;
		float KD;
		float YP;
		float YD;
		Data d; 

	private:
		void set(int throttle); 
		float correction[4]; 
		int pins[4]; 
		Servo servos[4];  

};


#endif