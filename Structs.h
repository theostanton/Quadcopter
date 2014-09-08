#ifndef Structs_h
#define Structs_h

#include "Arduino.h"
#include "Structs.h"

struct Accel { 
      float raw[3];
      float zero[3]; // no need
      float prev[3];
      float angle[3]; // send
};

struct Gyro { 
      float raw[3];
      float zero[3];
      float rate[3];
      float angle[3]; // send
};

struct Ctrl { 
      float angle[3]; // send
      float error[3]; // send
      float des[3]; 
      float error_integral[3];
      float rate[3];
};

struct Data {
		float p[4];
		float i[4];
		float d[4];
		float t[4];
};

#define NOERR 0
#define RXERR 1
#define SNSER 2
#define SRLER 3

#define X 0
#define Y 1
#define Z 2
#define ROLL 0
#define PITCH 1
#define YAW 2 
#define THROTTLE 3

#define rxROLL 0
#define rxTHROTTLE 1
#define rxPITCH 2
#define rxYAW 3

#define ROLL_PIN 11 // CIRCUIT
#define THROTTLE_PIN 4 // CIRCUIT
#define PITCH_PIN 12 // CIRCUIT 
#define YAW_PIN 6 // CIRCUIT
#define AUX1_PIN 7 // CIRCUIT
#define AUX2_PIN 8 // CIRCUIT

#define MOTOR_MIN 1 // minimum throttle servo command angle
#define MOTOR_MAX 179 // max throttle servo angle command

#endif