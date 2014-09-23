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

	KP = 0.9f;
  	KI = 0.00f;
 	KD = 0.0f;

	d = (Data) {
		{ 0.0f,0.0f,0.0f,0.0f},
		{ 0.0f,0.0f,0.0f,0.0f},
		{ 0.0f,0.0f,0.0f,0.0f},
		{ 0.0f,0.0f,0.0f,0.0f}
	};
}

void Motors::setKD( float kd ){
	KD = kd; 
}

void Motors::setKP( float kp ){
	KP = kp; 
}

void Motors::init(){
	Serial.print("Motors init ...");
	for(int i=0; i<4; i++){
		pinMode(pins[i],OUTPUT); 
		servos[i].attach( pins[i] );
		servos[i].write( 0 );  
	}
	delay(2000); 

	for(int i=0; i<4; i++){
		servos[i].write(12); 
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
	
	d.p[A] = ( - ctrl.error[PITCH] 			- ctrl.error[ROLL] 			) * KP;
	d.i[A] = ( + ctrl.error_integral[PITCH] + ctrl.error_integral[ROLL] ) * KI;
	d.d[A] = ( - ctrl.rate[PITCH] 			+ ctrl.rate[ROLL] 			) * KD;
	correction[A] = d.p[A] + d.i[A] + d.d[A]; // + YAW Corr
	
	d.p[B] = ( - ctrl.error[PITCH] 			+ ctrl.error[ROLL] 			) * KP;
	d.i[B] = ( + ctrl.error_integral[PITCH] - ctrl.error_integral[ROLL] ) * KI;
	d.d[B] = ( - ctrl.rate[PITCH] 			- ctrl.rate[ROLL] 			) * KD;
	correction[B] = d.p[B] + d.i[B] + d.d[B]; // + YAW Corr
	
	d.p[C] = ( + ctrl.error[PITCH] 			+ ctrl.error[ROLL] 			) * KP;
	d.i[C] = ( - ctrl.error_integral[PITCH] - ctrl.error_integral[ROLL] ) * KI;
	d.d[C] = ( + ctrl.rate[PITCH] 			- ctrl.rate[ROLL] 			) * KD;
	correction[C] = d.p[C] + d.i[C] + d.d[C]; // + YAW Corr

	d.p[D] = ( + ctrl.error[PITCH] 			- ctrl.error[ROLL] 			) * KP;
	d.i[D] = ( - ctrl.error_integral[PITCH] + ctrl.error_integral[ROLL] ) * KI;
	d.d[D] = (  ctrl.rate[PITCH] 			+ ctrl.rate[ROLL] 			) * KD;
	correction[D] = d.p[D] + d.i[D] + d.d[D]; // + YAW Corr


	if(errorState == NOERR){
		set(throttle);
	}
	else {
		kill(); 
	}
}

void Motors::twitch(){

	Serial.println(); 
	Serial.println("Twitch"); 

	for(int i=0; i<4; i++){
		servos[i].write(0);
	}
	delay(3000); 

	for(int i=0; i<4; i++){
		Serial.print("Pin "); 
		Serial.println(pins[i]); 
		for(int j=0; j<50; j+=1){
			if(j%5==0){
				Serial.print("  ");
				Serial.print(j); 
			}
			servos[i].write(j); 
			delay(33); 
		}
		// servos[i].write(10);
		// delay(1000);
		// servos[i].write(179);
		// delay(300);
		servos[i].write(1); 
		Serial.println(); 

	}

	for(int i=0; i<4; i++){
		servos[i].write(0);
	}
	delay(5000); 

}

void Motors::set(int throttle){

	// bool calibrate = false; 
	// if(calibrate){
	// 	Serial.print("Calibrate ");
	// 	if(throttle > 100){
	// 		Serial.println(MOTOR_MAX); 
	// 		for(int i=0; i<4; i++){
	// 			servos[i].write( MOTOR_MAX ); 
	// 		}
	// 	}
	// 	else {
	// 		Serial.println(MOTOR_MIN); 
	// 		for(int i=0; i<4; i++){
	// 			servos[i].write( 12 ); 
	// 		}
	// 	}
	// 	return; 
	// }

	// int max = 0; 
	// int mot = -1; 


	if(throttle < MOTOR_MIN){
		for(int i=0; i<4; i++){
			servos[i].write( 0 ); 
		}
	} 
	else {
		for (int i = 0; i < 4; i++) {
			command[i] = int( correction[i] ) + throttle;

			// if(command[i]>max){
			// 	max = command[i];
			// 	mot = i; 
			// }

			if( command[i] > MOTOR_MAX) command[i] = MOTOR_MAX;
			if( command[i] < MOTOR_MIN) command[i] = MOTOR_MIN;
			//if( command[i] < 100) command[i] = 0;
			// else {
			// 	Serial.println(); 
			// }
		}




		for(int i=0; i<4; i++){
			// Serial.print(command[i]);
			// Serial.print(" ");
			servos[i].write( command[i] ); 
		}
		// Serial.println(); 
	}

	// switch(mot){
	// 	case 0: 
	// 		Serial.print("A"); 
	// 		break;
	// 	case 1: 
	// 		Serial.print("B"); 
	// 		break;
	// 	case 2: 
	// 		Serial.print("C"); 
	// 		break;
	// 	case 3: 
	// 		Serial.print("D"); 
	// 		break;
	// 	}
	// 	Serial.println(max); 

}



void Motors::send(){
	char lab[] = {'a','b','c','d'};
	for( int i=0; i<4; i++){
		Serial.print(lab[i]);
		Serial.print(",");
		Serial.print( int(d.p[i]*1000.0f) );
		Serial.print(",");
		Serial.print( int(d.i[i]*1000.0f) );
		Serial.print(",");
		Serial.print( int(d.d[i]*1000.0f) );
		Serial.print(",");
		Serial.print( int(correction[i]) ); 
		Serial.println(); 
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
		servos[i].write( 1 ); 
	}
	//Serial.println("KILL MOTORS"); 
}