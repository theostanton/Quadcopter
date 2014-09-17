
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



LED amberLED(A2,"Amber"); 
LED greenLED(A3,"Green"); 

Sensors sensors; 
Motors motors; 
Comms comms; 
RX rx; 

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
  unsigned long tick;
}
time = {0L, 0L, 0L, 0L, 0L, 0L}; 

void setup() {


	Serial.begin(38400); 
	Serial.println("Start");


  
	Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	Timer1.attachInterrupt( tickLEDs ); // attach the service routine here
	amberLED.flash(100,100); 

	errorState=0;

    sensors.init(); 
    motors.init(); 



    while(true){
		Serial.print("!");
		delay(100);
    }


  	// attachInterrupt(0, interrupt, FALLING);


	delay(1000); 
	greenLED.flash(100,900); 
	amberLED.stop(); 


}

void tickLEDs() {
	// Interrupt via timerOne
	Serial.println("tickLEDs");
	greenLED.tick();
	amberLED.tick();
	// Timer1.restart();
	// Timer1.start();
}

void loop(){
	Serial.print(",");
	delay(100);
}

void laoop() {

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

	if( sensors.read(DT, desired) ) { 
		motors.update( sensors.c, desired[THROTTLE], errorState ); 
	}
	else {
		Serial.println("Sensor error"); 
		amberLED.flash(100,500);
	}
	if( false && time.current > ( 50L + time.tx )) {
		//comms.sendPacket(sensors.c, sensors.a, sensors.g, motors.d, desired);
		switch(errorState){
			case NOERR:
				if( !rx.updateRX( desired ) ) {
					Serial.println("RX error"); 
					amberLED.flash(500,100); 
					errorState = RXERR; 
				}
				break;
			case RXERR:
				Serial.println("Case:RXERR");
				if( rx.updateRX( desired ) ){
					amberLED.stop(); 
					errorState = NOERR; 
					Serial.println("Set error to : NOERR");
				}
				break;
			default:
				Serial.println("Error missed"); 
		}
		//rx.send(); 
		//sendAll(); 
		time.tx = time.current; 

	}

Serial.print("."); 

	checkSerial();

	if(interrupted){
		Serial.println("Interrupted"); 
		motors.kill();
		while(interrupted){
			amberLED.flash(500,500); 
			// ledState = true;
			// toggleLED();
			// delay(100); 
			// toggleLED();
			// delay(100); 
			// toggleLED();
			// delay(100); 
			// toggleLED();
			// delay(600); 
		}
	}
}

void interrupt(){
	Serial.print("interrupt"); 
	if(interruptedAt + 1000L < millis() ){
		Serial.println(" yes");
		interrupted = !interrupted; 
		interruptedAt = millis(); 
	}
	else {
		Serial.println(" bounced");
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
}
