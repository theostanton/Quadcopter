#ifndef RX_h
#define RX_h

#include "Arduino.h"
#include "Structs.h"

class RX {

    public:
        RX(); 
        boolean updateRX(int *desired); 
    private:
        boolean update4CH(int *desired);
        boolean update6CH(int *desired);

};

#endif

