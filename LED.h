#ifndef LED_h
#define LED_h

#include "Arduino.h"
class LED {

    public:
    	LED(int pin, String llabel);
    	void tick(); 
    	void toggle(); 
    	void flash(int times); 
    	void flash(int oonFor, int rate); 
    	void flash(int oonFor, int rate, int times); 
    	void set(bool state); 
    	void stop(); 
    private:
    	int times; 
    	String label; 
    	bool halted; 
    	int pin; 
    	int state; 
    	long nextToggle;
    	int onFor; 
    	int offFor; 
    	int acc;
    	int seqLength; 
    	int sequence[]; 


};

#endif