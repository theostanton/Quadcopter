#ifndef Comms_h
#define Comms_h

#include "Arduino.h"
#include "Structs.h"

class Comms {

	public:
		Comms();
		void init(); 
		void send(int *desired); 
		void send(Ctrl ctrl); 
		void send(Accel accel); 
		void send(Gyro gyro); 
		void send(Data data); 
		void sendPacket(Ctrl ctrl, Accel accel, Gyro gyro, Data data, int *desired); 

};

#endif