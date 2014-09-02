
#define ROLL_PIN 11
#define THROTTLE_PIN 4
#define PITCH_PIN 12 
#define YAW_PIN 6
#define AUX1_PIN 7
#define AUX2_PIN 8

int RXtemp = 0;
int RXnodeStart = 0;
double RXnode[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
int rawvalue[6]  = { 0, 0, 0, 0, 0, 0 }; 
int auxcount = 0; 

int minimum[6] = { 1000, 1045, 1000, 1040, 1000, 1000};
int maximum[6] = { 1900, 1835, 1900, 1800, 1900, 1900};
int minDesired[4] = { 45, 10, -45, 10 };
int maxDesired[4] = { -45, 179, 45, -10 };
int PIN[6] = { ROLL_PIN, THROTTLE_PIN, PITCH_PIN, YAW_PIN, AUX1_PIN, AUX2_PIN};
int rxdesired[6] = { 0,0,0,0,0,0 };

void update4CH(  ) {

    while( digitalRead( PIN[0] ) == LOW ){
    }
    RXnode[0] = micros();
    Serial.println(RXnode[0]);

    while( digitalRead( PIN[2] ) == LOW ){
    }
    RXnode[1] = micros();
    Serial.println(RXnode[1]);

    while( digitalRead( PIN[1] ) == LOW ){
    }
    RXnode[2] = micros();
    Serial.println(RXnode[2]);

    while( digitalRead( PIN[3] ) == LOW ){
    }
    RXnode[3] = micros();
    Serial.println(RXnode[3]);

    while( digitalRead( PIN[3] ) == HIGH ){
    }
    RXnode[4] = micros();
    Serial.println(RXnode[4]);
   
    rawvalue[0] = RXnode[1] - RXnode[0];
    rawvalue[2] = RXnode[2] - RXnode[1];
    rawvalue[1] = RXnode[3] - RXnode[2];
    rawvalue[3] = RXnode[4] - RXnode[3];
   

 
    for ( int l = 0; l < 4; l++) {
      if ( rawvalue[l] > 1000 && rawvalue[l] < 2000 ) {
        rxdesired[l]  = map(rawvalue[l], minimum[l], maximum[l], minDesired[l], maxDesired[l]);
      }
      else if( rawvalue[1] < 1000 ) {
        rxdesired[1] = MOTOR_MIN;
      }
    }

  
  }


// void update6CH() {

//   while( digitalRead( rx.PIN[0] ) == LOW ){
//   }
//   RXnode[0] = micros();
//   //Serial.println(RXnode[0]);

//   while( digitalRead( rx.PIN[2] ) == LOW ){
//   }
//   RXnode[1] = micros();
//   //Serial.println(RXnode[1]);

//   while( digitalRead( rx.PIN[1] ) == LOW ){
//   }
//   RXnode[2] = micros();
//   //Serial.println(RXnode[2]);

//   while( digitalRead( rx.PIN[3] ) == LOW ){
//   }
//   RXnode[3] = micros();
//   //Serial.println(RXnode[3]);

//   while( digitalRead( rx.PIN[4] ) == LOW ){
//   }
//   RXnode[4] = micros();
//   //Serial.println(RXnode[4]);

//   while( digitalRead( rx.PIN[5] ) == LOW ){
//   }
//   RXnode[5] = micros();
//   //Serial.println(RXnode[5]);

//   while( digitalRead( rx.PIN[5] ) == HIGH ){
//   }
//   RXnode[6] = micros();
//   //Serial.println(RXnode[6]);

//   rx.rawvalue[0] = RXnode[1] - RXnode[0];
//   rx.rawvalue[2] = RXnode[2] - RXnode[1];
//   rx.rawvalue[1] = RXnode[3] - RXnode[2];
//   rx.rawvalue[3] = RXnode[4] - RXnode[3];
//   rx.rawvalue[4] = RXnode[5] - RXnode[4];
//   rx.rawvalue[5] = RXnode[6] - RXnode[5];



//   for ( int l = 0; l < 4; l++) {
//     if ( rx.rawvalue[l] > 1000 && rx.rawvalue[l] < 2000 ) {
//       rx.desired[l]  = map(rx.rawvalue[l], rx.minimum[l], rx.maximum[l], rx.minDesired[l], rx.maxDesired[l]);
//     }
//     else if( rx.rawvalue[1] < 1000 ) {
//       rx.desired[1] = MOTOR_MIN;
//     }
//   }

//   if ( rx.rawvalue[4] > 0 )
//   {
//     KP = map(rx.rawvalue[4], 1000.0, 2000.0, 0.0, 200.0);
//     KP /= 1000.0;
//   }

//   if ( rx.rawvalue[5] > 0 )
//   {
//     KD = map(rx.rawvalue[5], 1000.0, 2000.0, 0.0, 250.0);
//     KD /= -1000.0;
//   }
// }


void updateRX() { // Receive 6CH every 20 RX runs otherwise 4CH

  if( auxcount < 20 ) {
    auxcount++;
    update4CH();
  }
  else {
    auxcount = 0;
    // update6CH();
    }
}
