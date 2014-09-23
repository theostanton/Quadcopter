
int errorState = 0; 
int desired[] = {0, 0, 0, 0};

// Conflicts with Servo.h #include <TimerOne.h>

#include <Structs.h>
#include <Servo.h>
#include <Sensors.h>
#include <Motors.h>
#include <Comms.h>
#include <RX.h>
#include <LED.h>
#include <Timer.h>

LED amberLED(A2,"Amber"); 
LED greenLED(A3,"Green"); 


Timer loopTimer("Loop",0); 
Timer sensorTimer("Sensor",1); 
Timer motorTimer("Motor",2); 
Timer rxTimer("RX",3); 
Timer serialTimer("Serial",4); 

Sensors sensors; 
Motors motors; 
Comms comms; 
RX rx; 

bool success = false; 

int acc = 0; 

// ROLL PITCH YAW THROTTLE

volatile boolean interrupted = false; 
volatile long interruptedAt = 0L; 
    
float DT = 0.0f;

struct time {
  unsigned long current;
  unsigned long currentMicros;
  unsigned long previousMicros;
  unsigned long previous;
  unsigned long tx;
  unsigned long rx;
  unsigned long tick;
}
time = {0L, 0L, 0L, 0L, 0L, 0L, 0L}; 


void setup() {


	Serial.begin(115200); 
	Serial.println("Start");

    amberLED.set(true); 
	errorState=0;
    sensors.init(); 
    motors.init();
  	attachInterrupt(0, interrupt, FALLING);

	greenLED.flash(100,1000,2); 
	amberLED.stop(); 

	// rxTimer.setPrint(true); 
	// serialTimer.setPrint(true); 
	// sensorTimer.setPrint(true); 
	// loopTimer.setPrint(true); 
	// serialTimer.setPrint(true); 

	//motors.twitch(); 

}

void tickLEDs() {
	greenLED.tick();
	amberLED.tick();
}

void loop() {
	loopTimer.ticketytock(); 

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

	sensorTimer.tick();
	success = sensors.read(DT, desired); 
	sensorTimer.tock(); 

	if( success ) { 
		motorTimer.tick(); 
		motors.update( sensors.c, desired[THROTTLE], errorState ); 
		motorTimer.tock(); 
	}
	else {
		Serial.println("Sensor error"); 
		amberLED.flash(100,500);
	}

	if( time.current >  time.rx ) {
		rxTimer.tick(); 
		getRX(); 
		rxTimer.tock();
		time.rx = millis() + 120L; 
	}

	if( time.current > ( time.tx) ){
		serialTimer.tick(); 
		//printTimers(); 
		sendAll();
		serialTimer.tock(); 
		time.tx = millis() + 103L; 
	}

	tickLEDs(); 

	checkSerial();

	if(interrupted){
		Serial.println("Interrupted"); 
		motors.kill();
		while(interrupted){
			amberLED.flash(500,500); 
		}
	}
}

void printTimers(){
	Serial.println(); 
	loopTimer.print(); 
	sensorTimer.print(); 
	motorTimer.print(); 
	rxTimer.print(); 
	serialTimer.print(); 
}

void getRX(){
	//comms.sendPacket(sensors.c, sensors.a, sensors.g, motors.d, desired);
	switch(errorState){
		case NOERR:
			if( !rx.updateRX( desired ) ) {
				alertError( RXERR ); 
			}
			else {
				motors.setKD( rx.getKD() );
				motors.setKP( rx.getKP() );
			}
			break;
		case RXERR:
			//Serial.println("Case:RXERR");
			if( rx.updateRX( desired ) ){
				alertError(NOERR); 
			}
			break;
		default:
			Serial.println("Error missed"); 
	}
	//rx.send(); 
	//sendAll();  
}

void interrupt(){

	Serial.print("interrupt"); 


	if(interruptedAt + 1000L < millis() ){
		Serial.println(" accept");

		rx.toggleSetKP(); 

		// interrupted = !interrupted; 
		interruptedAt = millis(); 
	}
	else {
		Serial.println(" bounced");
	}
}

void alertError(int error){
	switch(error){
		case NOERR:
			Serial.println("Exit error"); 
			amberLED.stop();
			greenLED.flash(2); 
			errorState = NOERR;
			return;
		case RXERR:
			Serial.println("RX ERR"); 
			amberLED.flash(500,1000);
			greenLED.stop(); 
			errorState = RXERR;
			return; 
		default:
			Serial.println("Other error");
			amberLED.flash(2);
			greenLED.stop(); 
			errorState = 3; 
			return; 

	}
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
	rx.send(); 
	comms.send( sensors.c );
	comms.send( sensors.a );
	comms.send( sensors.g );
	motors.send( );
	comms.send( desired );

	calcTimers(); 
	sendTimers();
	sendRatios();  
}

void calcTimers(){
	loopTimer.calc();
	rxTimer.calc();
	sensorTimer.calc();
	serialTimer.calc();
	motorTimer.calc();
}

void sendRatios(){

	Serial.print("P");
	Serial.print(","); 
	Serial.print(loopTimer.ratio);
	Serial.print(","); 
	Serial.print(sensorTimer.ratio);
	Serial.print(","); 
	Serial.print(motorTimer.ratio);
	Serial.print(","); 
	Serial.print(rxTimer.ratio);
	Serial.print(","); 
	Serial.println(serialTimer.ratio);
}

void sendTimers(){

	Serial.print("T");
	Serial.print(","); 
	Serial.print(loopTimer.last);
	Serial.print(","); 
	Serial.print(sensorTimer.last);
	Serial.print(","); 
	Serial.print(motorTimer.last);
	Serial.print(","); 
	Serial.print(rxTimer.last);
	Serial.print(","); 
	Serial.println(serialTimer.last);

}