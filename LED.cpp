#include "Arduino.h"
#include "LED.h"

LED::LED(int pinn, String llabel){
	pin = pinn; 
	label = llabel; 
	pinMode(pin,OUTPUT); 
	set(false); 
	halted = true; 
}

void LED::flash(int oonFor, int ooffFor){
	onFor = oonFor;
	offFor = ooffFor;
	halted = false; 
}

void LED::stop(){
	halted = true; 
	set(false); 
}

void LED::tick(){
	Serial.println("Tick " + label); 
	if( halted ) return; 
	if( nextToggle < millis() ){
		toggle();
	}
}

void LED::set(bool sstate){
	Serial.println("set");
	digitalWrite(pin,state); 
	state = sstate; 
	// nextToggle = millis();
	nextToggle += state ? onFor : offFor;  
}

void LED::toggle(){
	Serial.println("Toggle"); 
	set( !state );
}
