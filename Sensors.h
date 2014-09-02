/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/



#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"
#include "Structs.h"



class Sensors {

	public:
		Sensors();
		void init(); 
		void setup(); 
		void read(float DT, int *desired);
		void print();
		Accel a; 
		Gyro  g; 
		Ctrl  c; 

	private:
		int val; 
		void setupAccel(); 
		void setupGyro(); 
		void writeRegister(int deviceAddress, byte address, byte val);
		int readRegister(int deviceAddress, byte address);

};

#endif