
int errorState = 0; 
int desired[] = {0, 0, 0, 0};

// Conflicts with Servo.h #include <TimerOne.h>


#include <MemoryFree.h>
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

//#define TIMERS false
#ifdef TIMERS
Timer loopTimer("Loop",0); 
Timer sensorTimer("Sensor",1); 
Timer motorTimer("Motor",2); 
Timer rxTimer("RX",3); 
Timer serialTimer("Serial",4); 
#endif

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

#define ERROR "ERR"
#define COMMA '.'

void setup() {


	Serial.begin(115200); 
	Serial.println("Start");
	Serial.print("freeMemory()=");
    Serial.println(freeMemory());

    amberLED.set(true); 
	errorState=0;
    sensors.init(); 
    motors.init();
  	attachInterrupt(0, interrupt, FALLING);

	greenLED.flash(100,1000,2); 
	amberLED.stop(); 

	#ifdef TIMERS
	// rxTimer.setPrint(true); 
	// serialTimer.setPrint(true); 
	sensorTimer.setPrint(true); 
	loopTimer.setPrint(true); 
	// serialTimer.setPrint(true); 
	#endif

	//motors.twitch(); 

}

void tickLEDs() {
	greenLED.tick();
	amberLED.tick();
}

void loop() {

	#ifdef TIMERS
	loopTimer.ticketytock(); 
	#endif 

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
	// acc++; q

	// if time to RX
	// updateRX(); 

	#ifdef TIMERS
	sensorTimer.tick();
	#endif
	success = sensors.read(DT, desired);

	#ifdef TIMERS 
	sensorTimer.tock(); 
	#endif

	if( success ) { 
		#ifdef TIMERS
		motorTimer.tick(); 
		#endif
		motors.update( sensors.c, desired[THROTTLE], errorState, (float)desired[YAW] ); 
		
		#ifdef TIMERS
		motorTimer.tock(); 
		#endif
	}
	else {
		Serial.println("Sensor error"); 
		amberLED.flash(100,500);
	}

	if( time.current >  time.rx ) {
		#ifdef TIMERS
		rxTimer.tick(); 
		#endif
		getRX(); 
		#ifdef TIMERS
		rxTimer.tock();
		#endif
		time.rx = millis() + 120L; 
	}

	if( time.current > ( time.tx) ){
		#ifdef TIMERS
		serialTimer.tick(); 
		#endif

		//printTimers(); 
		//sendAll();
		motors.print();
		//sensors.print(); 
		
		#ifdef TIMERS
		serialTimer.tock(); 
		#endif

		time.tx = millis() + 103L; 
	}

	tickLEDs(); 

	checkSerial();

	if(interrupted){
		Serial.println("INT"); 
		motors.kill();
		while(interrupted){
			amberLED.flash(500,500); 
		}
	}
}

// void printTimers(){
// 	Serial.println(); 
// 	loopTimer.print(); 
// 	sensorTimer.print(); 
// 	motorTimer.print(); 
// 	rxTimer.print(); 
// 	serialTimer.print(); 
// }

void getRX(){
	//comms.sendPacket(sensors.c, sensors.a, sensors.g, motors.d, desired);
	switch(errorState){
		case NOERR:
			if( !rx.updateRX( desired ) ) {
				alertError( RXERR ); 
			}
			else {
				motors.setKP( rx.getAUX1() );
				motors.setKD( rx.getAUX2() );
			}
			break;
		case RXERR:
			//Serial.println("Case:RXERR");
			if( rx.updateRX( desired ) ){
				alertError(NOERR); 
			}
			break;
		default:
			Serial.println(ERROR); 
	}
	Serial.print("freeMemory()=");
    Serial.println(freeMemory());
	//addToConsole(line, true);
	//rx.send(); 
	//sendAll();  
}

void interrupt(){

	Serial.print("INT"); 


	if(interruptedAt + 1000L < millis() ){
		Serial.println(" ed");

		rx.toggleSetKP(); 

		// interrupted = !interrupted; 
		interruptedAt = millis(); 
	}
	else {
		Serial.println(" bounce");
	}
}

void alertError(int error){
	switch(error){
		case NOERR:
			Serial.println("End"); 
			amberLED.stop();
			greenLED.flash(2); 
			errorState = NOERR;
			return;
		case RXERR:
			Serial.print("RX");
			Serial.println(ERROR); 
			amberLED.flash(500,1000);
			greenLED.stop(); 
			errorState = RXERR;
			return; 
		default:
			Serial.println(ERROR);
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
				Serial.println(ERROR);
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

	Serial.println("!"); 
}

void calcTimers(){
	#ifdef TIMERS
	loopTimer.calc();
	rxTimer.calc();
	sensorTimer.calc();
	serialTimer.calc();
	motorTimer.calc();
	#endif
}

void sendRatios(){
	#ifdef TIMERS

	Serial.print("P");
	Serial.print(COMMA); 
	Serial.print(loopTimer.ratio);
	Serial.print(COMMA); 
	Serial.print(sensorTimer.ratio);
	Serial.print(COMMA); 
	Serial.print(motorTimer.ratio);
	Serial.print(COMMA); 
	Serial.print(rxTimer.ratio);
	Serial.print(COMMA); 
	Serial.println(serialTimer.ratio);
	#endif
}

void sendTimers(){

	#ifdef TIMERS
	Serial.print("T");
	Serial.print(COMMA); 
	Serial.print(loopTimer.last);
	Serial.print(COMMA); 
	Serial.print(sensorTimer.last);
	Serial.print(COMMA); 
	Serial.print(motorTimer.last);
	Serial.print(COMMA); 
	Serial.print(rxTimer.last);
	Serial.print(COMMA); 
	Serial.println(serialTimer.last);
	#endif

}