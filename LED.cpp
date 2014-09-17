#include "Arduino.h"
#include "LED.h"

LED::LED(int pinn, String llabel){
	pin = pinn; 
	label = llabel; 
	pinMode(pin,OUTPUT); 
	set(false); 
	halted = true; 
}

void LED::flash(int ttimes){
	flash(100,1000,ttimes);
}

void LED::flash(int oonFor, int rate){
	flash(oonFor,rate,1);
}

void LED::flash(int oonFor, int rate,int ttimes){
	onFor = oonFor;
	offFor = rate - oonFor * ttimes - (ttimes - 1) * 100;
	times = ttimes; 
	halted = false; 
	acc = 0; 
	state = false;
	nextToggle = millis();
	set(false);
}

void LED::stop(){
	halted = true; 
	set(false); 
}

void LED::tick(){
	if( halted ) return; 
	if( nextToggle < millis() ){
		toggle();
	}
}

void LED::set(bool sstate){
	state = sstate; 
	digitalWrite(pin,state); 
	// nextToggle = millis();

	if(times == 1){
		nextToggle += state ? onFor : offFor;  
	}
	else if( acc == times){
		nextToggle += offFor; 
		acc = 0; 
	}
	else {
		if(state){
			nextToggle += onFor;
			acc ++; 
		}
		else { 
			nextToggle += 100; 
		}
	}
}

void LED::toggle(){
	set( !state );
}
