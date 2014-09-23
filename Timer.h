#ifndef Timerr_h
#define Timerr_h

#include "Arduino.h"

class Timer {

	public:
		Timer(char *label, int id);
		void tick();
		bool tock(); 
		bool ticketytock(); 
		void setN(int nn);
		void setPrint(bool state); 
		void print(); 
		void calc(); 
		long last; 
		float ratio; 
	private:
		bool printVals; 
		String label; 
		int id; 
		long first; 
		long start; 
		long min;
		long max; 
		long tot;
		int count; 
		int n; 
};

#endif