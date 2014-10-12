#include "Arduino.h"
#include "Timer.h"

#define MS "ms "
#define US "us "

Timer::Timer(char *labell, int idd){
	label = labell; 
	id = idd; 
	last = 0; 
	count = 0;
	n = 100; 
	start = 0L;
	printVals = false;  
	first = micros(); 
	ratio = 0.0f; 

	min = 100000L; 
	max = 0L; 

	// Serial.print("Created timer ");
	// Serial.println(label); 
}

void Timer::tick(){
	start = micros(); 
}

bool Timer::ticketytock(){
	bool done = tock();
	tick(); 
	return done; 
}

void Timer::setPrint(bool state){
	printVals = state; 
}

void Timer::calc(){
	if(count == 0){
		return; 
	}
	float gap = (float)micros() - first; 
	ratio = (float)tot / gap; 
	first = micros(); 

	last = tot / (long)count;

	if( last > max ) max = last;
	if( last < min ) min = last; 

	//Serial.println(last); 
	count = 0; 
	tot = 0L;
}

void Timer::print(){


	Serial.print(label);
	Serial.print(" "); 

	if( last > 5000L ){
		Serial.print(last/1000L);
		Serial.print(MS);
		Serial.print(min/1000L);
		Serial.print(MS);
		Serial.print(max/1000L);
		Serial.print(MS);
	}
	else{
		Serial.print( last);
		Serial.print(US); 
		Serial.print(min);
		Serial.print(US); 
		Serial.print(max);
		Serial.print(US); 
	}
	Serial.print(" ");
	Serial.println(ratio); 

	max = 0L;
	min = 10000000L; 
	// Serial.println(); 

}

void Timer::setN(int nn){
	n = nn;
}

bool Timer::tock(){

	if(count<n){
		tot += micros() - start; 
		count++;
		return false; 
	}

	calc(); 

	if(printVals) print();  

	return true;
}