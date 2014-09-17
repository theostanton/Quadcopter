#include "Arduino.h"
#include "RX.h"
#include "Structs.h"

#define ROLL_PIN 11
#define THROTTLE_PIN 4
#define PITCH_PIN 12 
#define YAW_PIN 6
#define AUX1_PIN 7
#define AUX2_PIN 8

int RXtemp = 0;
int RXnodeStart = 0;
double RXnode[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rawvalue[6]  = { 0, 0, 0, 0, 0, 0 }; 
int auxcount = 0; 

const int minimum[6] = { 1000, 1045, 1000, 1040, 1000, 1000};
const int maximum[6] = { 1900, 1835, 1900, 1800, 1900, 1900};
const int minDesired[4] = { 45, MOTOR_MIN, -45, 10 };
const int maxDesired[4] = { -45, MOTOR_MAX, 45, -10 };
const int PIN[6] = { ROLL_PIN, THROTTLE_PIN, PITCH_PIN, YAW_PIN, AUX1_PIN, AUX2_PIN};

int rxdesired[6] = { 0,0,0,0,0,0 };

RX::RX(){
    pinMode(ROLL_PIN,INPUT); 
    pinMode(THROTTLE_PIN,INPUT); 
    pinMode(PITCH_PIN,INPUT); 
    pinMode(YAW_PIN,INPUT); 
    pinMode(AUX1_PIN,INPUT); 
    pinMode(AUX2_PIN,INPUT); 

}

void RX::send(){
    Serial.print("R");
    for (int i = 0; i < 6; i++){
        Serial.print(",");
        Serial.print( int( rawvalue[i] ) );
    }
    Serial.println(); 
}

boolean RX::update4CH( int *desired ) {


    long strt = micros(); 
    while( digitalRead( PIN[0] ) == LOW ){
        if( micros() - strt > 20000L ){
            Serial.println("RX Timeout");
            return false; 
        }
    }
    RXnode[0] = micros();
    //Serial.println(RXnode[0]);

    while( digitalRead( PIN[2] ) == LOW ){
    }
    RXnode[1] = micros();
    //Serial.println(RXnode[1]);

    while( digitalRead( PIN[1] ) == LOW ){
    }
    RXnode[2] = micros();
    //Serial.println(RXnode[2]);



    while( digitalRead( PIN[3] ) == LOW ){
    }
    RXnode[3] = micros();
    //Serial.println(RXnode[3]);

    while( digitalRead( PIN[3] ) == HIGH ){
    }
    RXnode[4] = micros();
    //Serial.println(RXnode[4]);
   
    rawvalue[0] = RXnode[1] - RXnode[0];
    rawvalue[1] = RXnode[2] - RXnode[1];
    rawvalue[2] = RXnode[3] - RXnode[2];
    rawvalue[3] = RXnode[4] - RXnode[3];
   

 
    for ( int l = 0; l < 4; l++) {
      if ( rawvalue[l] > 1000 && rawvalue[l] < 2000 ) {
        rxdesired[l]  = map(rawvalue[l], minimum[l], maximum[l], minDesired[l], maxDesired[l]);
      }
      else if( rawvalue[1] < 1000 ) {
        rxdesired[1] = MOTOR_MIN;
      }
    }

    desired[ROLL] = rxdesired[rxROLL];
    desired[THROTTLE] = rxdesired[rxTHROTTLE];
    desired[PITCH] = rxdesired[rxPITCH];
    desired[YAW] = rxdesired[rxYAW];

    return true; }

boolean RX::update6CH( int *desired ) {


    long strt = micros(); 
    while( digitalRead( PIN[0] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RX Timeout PIN0");
            return false; 
        }
    }
    RXnode[0] = micros();
    //Serial.println(RXnode[0]);
    
    while( digitalRead( PIN[2] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RX Timeout PIN2");
            return false; 
        }
    }
    RXnode[1] = micros();
    //Serial.println(RXnode[1]);
    while( digitalRead( PIN[1] ) == LOW ){
        if( micros() - strt > 1000000.L ){
            Serial.println("RX Timeout PIN1");
            return false; 
        }
    }
    RXnode[2] = micros();
    //Serial.println(RXnode[2]);



    while( digitalRead( PIN[3] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RX Timeout PIN3");
            return false; 
        }
    }
    RXnode[3] = micros();
    //Serial.println(RXnode[3]);

    while( digitalRead( PIN[4] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RX Timeout PIN4");
            return false; 
        }
    }
    RXnode[4] = micros();

    while( digitalRead( PIN[5] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RX Timeout PIN5");
            return false; 
        }
    }
    RXnode[5] = micros();
    //Serial.println(RXnode[1]);

    while( digitalRead( PIN[5] ) == HIGH ){
    }
    RXnode[6] = micros();
    //Serial.println(RXnode[2]);
    //Serial.println(RXnode[4]);


    for(int i=0; i<6; i++){
        if(RXnode[i+1] - RXnode[i] > 950){
            rawvalue[i] = RXnode[i+1] - RXnode[i];
            if(i==4){
                rawvalue[i] -= 1500;
            }
        }
    }
   
    // rawvalue[0] = RXnode[1] - RXnode[0];
    // rawvalue[1] = RXnode[2] - RXnode[1];
    // rawvalue[2] = RXnode[3] - RXnode[2];
    // rawvalue[3] = RXnode[4] - RXnode[3];
    // rawvalue[4] = RXnode[5] - RXnode[4];
    // rawvalue[5] = RXnode[6] - RXnode[5];
   

 
    for ( int l = 0; l < 4; l++) {
      if ( rawvalue[l] > 1000 && rawvalue[l] < 2000 ) {
        rxdesired[l]  = map(rawvalue[l], minimum[l], maximum[l], minDesired[l], maxDesired[l]);
      }
      else if( rawvalue[1] < 1000 ) {
        rxdesired[1] = MOTOR_MIN;
      }
    }

    desired[ROLL] = rxdesired[rxROLL];
    desired[THROTTLE] = rxdesired[rxTHROTTLE];
    desired[PITCH] = rxdesired[rxPITCH];
    desired[YAW] = rxdesired[rxYAW];

    // Serial.print(rawvalue[4]);
    // Serial.print(" ");
    // Serial.println(rawvalue[5]);
    //Serial.println(); 

    return true; 
}

boolean  RX::updateRX( int *desired ) { 

    return update6CH( desired ); 
    
    // if( auxcount < 20 ) {
    //     auxcount++;
    //     return update4CH(); 
    // }
    // else {
    // auxcount = 0;
    // // update6CH();
    // }

    // return true; 
}

