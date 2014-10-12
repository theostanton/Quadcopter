#ifndef RX_h
#define RX_h

#include "Arduino.h"
#include "Structs.h"

class RX {

    public:
        RX(); 
        boolean updateRX(int *desired); 
        void send();
        float getAUX1();
        float getAUX2();
        void toggleSetKP();
    private:
        boolean update4CH(int *desired);
        boolean update6CH(int *desired);
        float map_coeff(int val, float minn, float maxx);

};

#endif

