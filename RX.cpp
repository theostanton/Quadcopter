#include "Arduino.h"
#include "RX.h"
#include "Structs.h"

// #define ROLL_PIN 11
// #define THROTTLE_PIN 4
// #define PITCH_PIN 12 
// #define YAW_PIN 6
// #define AUX1_PIN 7
// #define AUX2_PIN 8

#define ROLL_PIN 12
#define THROTTLE_PIN 8
#define PITCH_PIN 7 
#define YAW_PIN 6
#define AUX1_PIN 11
#define AUX2_PIN 4


volatile boolean setKP = false; 
float fAUX1 = 0.05f;
float fAUX2 = 0.05f;

int RXtemp = 0;
int RXnodeStart = 0;
double RXnode[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int rawvalue[6]  = { 0, 0, 0, 0, 0, 0 }; 
int auxcount = 0; 

const int minimum[6] = { 1060, 1060, 1060, 1060, 1060, 1060};
const int maximum[6] = { 1880, 1880, 1880, 1880, 1880, 1880};
const int minDesired[4] = { 45, 0, -45, 20 };
const int maxDesired[4] = { -45, MOTOR_MAX, 45, -20 };
const int PIN[6] = { ROLL_PIN, THROTTLE_PIN, PITCH_PIN, YAW_PIN, AUX1_PIN, AUX2_PIN};

float rxdesired[6] = { 0,0,0,0,0,0 };

RX::RX(){
    pinMode(ROLL_PIN,INPUT); 
    pinMode(THROTTLE_PIN,INPUT); 
    pinMode(PITCH_PIN,INPUT); 
    pinMode(YAW_PIN,INPUT); 
    pinMode(AUX1_PIN,INPUT); 
    pinMode(AUX2_PIN,INPUT); 

}

float RX::getAUX1(){
    return fAUX1;
}

float RX::getAUX2(){
    return fAUX2;
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
            Serial.println("RXTO");
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


    bool missed = false; 

    // if CH1 already high then wait WHOLE sequence.   
    // TODO: sync!
    if( digitalRead( PIN[0] ) == HIGH ){
        Serial.println("HI"); 
        return true; 
    }

    long strt = micros(); 
    while( digitalRead( PIN[0] ) == LOW ){
        if( micros() - strt > 100000L ){
            //Serial.println("RX Timeout PIN0");
            return false; 
        }
    }
    // long now = micros(); 
    // Serial.print(now - RXnode[0]);
    // Serial.print(" ");
    // Serial.println(strt - now); 
    // RXnode[0] = now;
    RXnode[0] = micros();
    
    while( digitalRead( PIN[2] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RXTO2");
            return false; 
        }
    }
    RXnode[1] = micros();
    //Serial.println(RXnode[1]);
    while( digitalRead( PIN[1] ) == LOW ){
        if( micros() - strt > 1000000L ){
            Serial.println("RXTO1");
            return false; 
        }
    }
    RXnode[2] = micros();
    //Serial.println(RXnode[2]);



    while( digitalRead( PIN[3] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RXTO3");
            return false; 
        }
    }
    RXnode[3] = micros();
    //Serial.println(RXnode[3]);

    while( digitalRead( PIN[4] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RXTO4");
            return false; 
        }
    }
    RXnode[4] = micros();

    while( digitalRead( PIN[5] ) == LOW ){
        if( micros() - strt > 100000L ){
            Serial.println("RXTO5");
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
            // Serial.print(rawvalue[i]);
            // Serial.print(" "); 
            // if(i==4){
            //     rawvalue[i] -= 1500;
            // }
        }
    }
    // Serial.println(); 
   
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

    desired[ROLL] = abs(rxdesired[rxROLL]) > 2 ? rxdesired[rxROLL] : 0;
    desired[PITCH] = abs(rxdesired[rxPITCH]) > 2 ? rxdesired[rxPITCH] : 0;
    desired[YAW] = abs(rxdesired[rxYAW]) > 2 ? rxdesired[rxYAW] : 0;
    desired[THROTTLE] = rxdesired[rxTHROTTLE];
    // desired[PITCH] = rxdesired[rxPITCH];
    // desired[YAW] = rxdesired[rxYAW];

    // for(int i=0; i<4; i++){
    //     Serial.print(" ");
    //     Serial.print(rxdesired[i]); 
    // }
    // for(int i=0; i<4; i++){
    //     Serial.print(" ");
    //     Serial.print(desired[i]); 
    // }
    // Serial.println(); 

    // Serial.print(desired[THROTTLE]); 
    // Serial.print(" ");
    // Serial.print(desired[PITCH]); 
    // Serial.print(" ");
    // Serial.print(desired[ROLL]); 
    // Serial.print(" ");
    // Serial.println(desired[YAW]); 

    // KP = float(rawvalue[4] - 800 )  / 5000.0f; 
    // KD = float(rawvalue[5] - 800 )  / 5000.0f; 

    fAUX1 = map_coeff( rawvalue[4], 0.3f, 0.0f );
    fAUX2 = map_coeff( rawvalue[5], 0.0f, 0.2f );

    // Serial.print( KP );
    //Serial.println(); 

    return true; 
}

void RX::toggleSetKP(){
    setKP = !setKP; 
}

float RX::map_coeff(int val, float minn, float maxx){
    float v = (float)val;
    return  ( v - 1000.0f ) * ( maxx -minn ) / ( 1000.0f ) + minn; 
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

