#include "Arduino.h"
#include "Motors.h"

#include <Servo.h>

#define A 0 // FRONT LEFT
#define B 1 // FRONT RIGHT - circuit
#define C 2 // REAR RIGHT - circuit
#define D 3 // REAR LEFT - circuit

#define MOTOR_A_PIN 3
#define MOTOR_B_PIN 10
#define MOTOR_C_PIN 9
#define MOTOR_D_PIN 5



Motors::Motors(){
	pins[A] = MOTOR_A_PIN; 
	pins[B] = MOTOR_B_PIN; 
	pins[C] = MOTOR_C_PIN; 
	pins[D] = MOTOR_D_PIN; 

	for(int i=0; i<4; i++) {
		correction[i] = 0.0f; 
	}

	KP = 0.08f;
  	KI = 0.00f;
 	KD = -0.08f;

	d = (Data) {
		{ 0.0f,0.0f,0.0f,0.0f},
		{ 0.0f,0.0f,0.0f,0.0f},
		{ 0.0f,0.0f,0.0f,0.0f},
		{ 0.0f,0.0f,0.0f,0.0f}
	};
}

void Motors::init(){
	Serial.print("Motors init ...");
	for(int i=0; i<4; i++){
		pinMode(pins[i],OUTPUT); 
		servos[i].attach( pins[i] );
		servos[i].write( 10 );  
	}
	// delay( 1000 );

	// for(int i=0; i<100; i+=1){
	// 	for(int j=0; j<4; j++){
	// 		servos[j].write( i );  
	// 	}
	// 	delay( 500 );
	// 	Serial.println(i);

	// }
	// for(int j=0; j<4; j++){
	// 		servos[j].write( 0 );  
	// 	}

	// delay( 1000 );
	// Serial.println("attached"); 

	// Serial.println("Done");
}

void Motors::update(Ctrl ctrl, int throttle, int errorState){
	
	d.p[A] = ( + ctrl.error[PITCH] 			+ ctrl.error[ROLL] 			) * KP;
	d.i[A] = ( + ctrl.error_integral[PITCH] + ctrl.error_integral[ROLL] ) * KI;
	d.d[A] = ( + ctrl.rate[PITCH] 			+ ctrl.rate[ROLL] 			) * KD;
	correction[A] = d.p[A] + d.i[A] + d.d[A]; // + YAW Corr
	
	d.p[B] = ( + ctrl.error[PITCH] 			- ctrl.error[ROLL] 			) * KP;
	d.i[B] = ( + ctrl.error_integral[PITCH] - ctrl.error_integral[ROLL] ) * KI;
	d.d[B] = ( + ctrl.rate[PITCH] 			- ctrl.rate[ROLL] 			) * KD;
	correction[B] = d.p[B] + d.i[B] + d.d[B]; // + YAW Corr
	
	d.p[C] = ( - ctrl.error[PITCH] 			+ ctrl.error[ROLL] 			) * KP;
	d.i[C] = ( - ctrl.error_integral[PITCH] + ctrl.error_integral[ROLL] ) * KI;
	d.d[C] = ( - ctrl.rate[PITCH] 			+ ctrl.rate[ROLL] 			) * KD;
	correction[C] = d.p[C] + d.i[C] + d.d[C]; // + YAW Corr

	d.p[D] = ( - ctrl.error[PITCH] 			- ctrl.error[ROLL] 			) * KP;
	d.i[D] = ( - ctrl.error_integral[PITCH] - ctrl.error_integral[ROLL] ) * KI;
	d.d[D] = ( - ctrl.rate[PITCH] 			- ctrl.rate[ROLL] 			) * KD;
	correction[D] = d.p[D] + d.i[D] + d.d[D]; // + YAW Corr

	switch(errorState){
		case NOERR:
			set(throttle);
			break;
		default:
			kill();
	}
}

void Motors::set(int throttle){
 
	
	for (int i = 0; i < 4; i++) {
		command[i] = int( correction[i] ) + throttle;
		//if( command[i] > 100) command[i] = 179;
		//if( command[i] < 100) command[i] = 1;
		if( command[i] > MOTOR_MAX) command[i] = MOTOR_MAX;
		if( command[i] < MOTOR_MIN) command[i] = MOTOR_MIN;
	}

	for(int i=0; i<4; i++){
		servos[i].write( command[i] ); 
	}

}

void Motors::print(){
	for(int i=0; i<4; i++){
		Serial.print(command[i]);
		Serial.print(" ");
	}
	Serial.println();

	for(int i=0; i<4; i++){
		Serial.print(correction[i]);
		Serial.print(" ");
	}
	Serial.println();
	Serial.println();
}

void Motors::kill(){
	for(int i=0; i<4; i++){
		servos[i].write( 0 ); 
	}
	Serial.println("KILL MOTORS"); 
}