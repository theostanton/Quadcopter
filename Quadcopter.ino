
#include <Servo.h>
#include <Sensors.h>
#include <Motors.h>
#include <Comms.h>
#include <RX.h>

Sensors sensors; 
Motors motors; 
Comms comms; 

int acc = 0; 

// ROLL PITCH YAW THROTTLE
int desired[4] = { 0, 0, 0, 0 }; 

// Time Structure -----------------------------------------
    
float DT = 0.0f;

struct time {
  unsigned long current;
  unsigned long currentMicros;
  unsigned long previousMicros;
  unsigned long previous;
  unsigned long tx;
}
time = {0L, 0L, 0L, 0L, 0L}; 

void setup() {
    sensors.init(); 
    motors.init(); 
	Serial.begin(38400); 
	Serial.println("Start"); 
}

void loop() {

	time.previousMicros = time.currentMicros;
	time.currentMicros  = micros();
	time.previous       = time.current;
	time.current        = millis();

	// DT to be used by sensors
	DT = (float)( time.currentMicros - time.previousMicros );

	// if( acc > 200) {
	// 	Serial.println(DT); 
	// 	acc = 0;
	// }
	// acc++; 

	// if time to RX
	// updateRX(); 

	sensors.read(DT, desired);
	motors.update( sensors.c, desired[THROTTLE] ); 
	//sensors.print(); 

	if( time.current > ( 50 + time.tx )) {
		comms.sendPacket(sensors.c, sensors.a, sensors.g, motors.d, desired);
		//sendAll(); 
		time.tx = time.current; 
	}

	//updateRX();
	checkSerial();
}

/*
P - KP 0-1000 > 0. - 1.
I - KI 0-1000 > 0. - 1.
D - KD 0-1000 > 0. - 1.

t - Throttle 0-180 > 0>180
y - YAW -45>45 -45>45
p - Pitch
r

*/

#define KPid 1
#define KIid 2
#define KDid 3
#define RATEid 4
#define COMPid 5
#define MAXINTEGRALid 6
#define PINGid 7
#define RXPINGid 8
#define THROTTLEid 29
#define YAWid 30
#define PITCHLid 31
#define ROLLid 32

void checkSerial(){
	while( Serial.available() ){
		
		byte a = Serial.read();
		int in = -2; 
		if( Serial.available() ) {
			byte MSB = Serial.read();
			byte LSB = Serial.read();
			in = (MSB << 8) | LSB;
		}
		switch(a){

			case ROLLid:
				desired[ROLL] = in;
				break; 

			case PITCHLid:
				desired[PITCH] = in;
				break; 

			case YAWid:
				desired[YAW] = in;
				break; 

			case THROTTLEid:
				in *= -1;
				in += 45;
				in *= 2;
				desired[THROTTLE] = in;
				Serial.print("Thr");
				Serial.println(desired[THROTTLE]);
				break; 

			case KPid:
				motors.KP = (float)in / 1000.0f;
				break; 

			case KIid:
				motors.KI = (float)in / 1000.0f; 
				break; 

			case KDid:
				motors.KD = (float)in / 1000.0f; 
				break; 

			default :
				Serial.println(" Something else ");
				Serial.println(String("a " + a)); 
				Serial.println(String("in " + in) ); 
		}
	}
}

void sendAll(){
	comms.send( sensors.c );
	comms.send( sensors.a );
	comms.send( sensors.g );
	comms.send( motors.d );
	// comms.send <<DESIRED>>
}

