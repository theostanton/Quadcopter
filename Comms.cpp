#include "Arduino.h"
#include "Comms.h"

#define COMMA '.'

Comms::Comms(){
	Serial.println("coms");
}

void Comms::init(){
	Serial.println("init");
}


void Comms::sendPacket(Ctrl ctrl, Accel accel, Gyro gyro, Data data, int * desired){
	//long now = micros(); 
	Serial.print("P,"); 
	// for(int i=0; i<25;i++){
	// 	Serial.print(i);
	// 	Serial.print(","); 
	// }
	// Serial.println(); 

	Serial.print( int( gyro.angle[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( gyro.angle[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( gyro.angle[YAW]  ) );
	Serial.print(COMMA);
	Serial.print( int( accel.angle[ROLL]) );
	Serial.print(COMMA);
	Serial.print( int( accel.angle[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.angle[YAW]  ) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.angle[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.angle[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.error[YAW]  ) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.error[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.error[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.error_integral[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.error_integral[PITCH]) );
	Serial.print(COMMA);
	Serial.print( desired[ROLL]);
	Serial.print(COMMA);
	Serial.print( desired[PITCH]);
	Serial.print(COMMA);
	Serial.print( desired[YAW]);
	Serial.print(COMMA);
	Serial.print( desired[THROTTLE]);
	Serial.print(COMMA);

	char lab[] = {'a','b','c','d'};
	for( int i=0; i<4; i++){
		Serial.print( int(data.p[i]*1000.0f) );
		Serial.print(COMMA);
		Serial.print( int(data.i[i]*1000.0f) );
		Serial.print(COMMA);
		Serial.print( int(data.d[i]*1000.0f) );
		Serial.print(COMMA);
	}

	//long gap = micros() - now; 
	Serial.println(); 

}


void Comms::send(int *desired){
	Serial.print("D,");
	Serial.print( -desired[ROLL]);
	Serial.print(COMMA);
	Serial.print( -desired[PITCH]);
	Serial.print(COMMA);
	Serial.print( -desired[YAW]);
	Serial.println(); 

}


void Comms::send(Ctrl ctrl){
	Serial.print("M,");
	Serial.print( int( ctrl.angle[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.angle[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.angle[YAW]  ) );
	Serial.println();
	Serial.print("E,");
	Serial.print( int( ctrl.error[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.error[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( ctrl.error[YAW]  ) );
	Serial.println();
}

void Comms::send(Accel accel){
	Serial.print("A,");
	Serial.print( int( accel.angle[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( accel.angle[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( accel.angle[YAW]  ) );
	Serial.println();
}

void Comms::send(Gyro gyro){
	Serial.print("G,");
	Serial.print( int( gyro.angle[ROLL] ) );
	Serial.print(COMMA);
	Serial.print( int( gyro.angle[PITCH]) );
	Serial.print(COMMA);
	Serial.print( int( gyro.angle[YAW]  ) );
	Serial.println();
}

// void Comms::send(Data data){
// 	char lab[] = {'a','b','c','d'};
// 	for( int i=0; i<4; i++){
// 		Serial.print(lab[i]);
// 		Serial.print(",");
// 		Serial.print( int(data.p[i]*1000.0f) );
// 		Serial.print(",");
// 		Serial.print( int(data.i[i]*1000.0f) );
// 		Serial.print(",");
// 		Serial.print( int(data.d[i]*1000.0f) );
// 		Serial.print(",");
// 		Serial.print( int(data.))
// 		Serial.println(); 
// 	}
// }



