void RX4CH() {

    while( digitalRead( rx.PIN[0] ) == LOW ){
    }
    RXnode[0] = micros();
    //Serial.println(RXnode[0]);

    while( digitalRead( rx.PIN[2] ) == LOW ){
    }
    RXnode[1] = micros();
    //Serial.println(RXnode[1]);

    while( digitalRead( rx.PIN[1] ) == LOW ){
    }
    RXnode[2] = micros();
    //Serial.println(RXnode[2]);

    while( digitalRead( rx.PIN[3] ) == LOW ){
    }
    RXnode[3] = micros();
    //Serial.println(RXnode[3]);

    while( digitalRead( rx.PIN[3] ) == HIGH ){
    }
    RXnode[4] = micros();
    //Serial.println(RXnode[4]);
   
    rx.rawvalue[0] = RXnode[1] - RXnode[0];
    rx.rawvalue[2] = RXnode[2] - RXnode[1];
    rx.rawvalue[1] = RXnode[3] - RXnode[2];
    rx.rawvalue[3] = RXnode[4] - RXnode[3];
   

 
    for ( int l = 0; l < 4; l++) {
    if ( rx.rawvalue[l] > 1000 && rx.rawvalue[l] < 2000 ) {
      rx.desired[l]  = map(rx.rawvalue[l], rx.minimum[l], rx.maximum[l], rx.minDesired[l], rx.maxDesired[l]);
    }
    else if( rx.rawvalue[1] < 1000 ) {
      rx.desired[1] = MOTOR_MIN;
    }
  }
  }


void RX6CH() {

  while( digitalRead( rx.PIN[0] ) == LOW ){
  }
  RXnode[0] = micros();
  //Serial.println(RXnode[0]);

  while( digitalRead( rx.PIN[2] ) == LOW ){
  }
  RXnode[1] = micros();
  //Serial.println(RXnode[1]);

  while( digitalRead( rx.PIN[1] ) == LOW ){
  }
  RXnode[2] = micros();
  //Serial.println(RXnode[2]);

  while( digitalRead( rx.PIN[3] ) == LOW ){
  }
  RXnode[3] = micros();
  //Serial.println(RXnode[3]);

  while( digitalRead( rx.PIN[4] ) == LOW ){
  }
  RXnode[4] = micros();
  //Serial.println(RXnode[4]);

  while( digitalRead( rx.PIN[5] ) == LOW ){
  }
  RXnode[5] = micros();
  //Serial.println(RXnode[5]);

  while( digitalRead( rx.PIN[5] ) == HIGH ){
  }
  RXnode[6] = micros();
  //Serial.println(RXnode[6]);

  rx.rawvalue[0] = RXnode[1] - RXnode[0];
  rx.rawvalue[2] = RXnode[2] - RXnode[1];
  rx.rawvalue[1] = RXnode[3] - RXnode[2];
  rx.rawvalue[3] = RXnode[4] - RXnode[3];
  rx.rawvalue[4] = RXnode[5] - RXnode[4];
  rx.rawvalue[5] = RXnode[6] - RXnode[5];



  for ( int l = 0; l < 4; l++) {
    if ( rx.rawvalue[l] > 1000 && rx.rawvalue[l] < 2000 ) {
      rx.desired[l]  = map(rx.rawvalue[l], rx.minimum[l], rx.maximum[l], rx.minDesired[l], rx.maxDesired[l]);
    }
    else if( rx.rawvalue[1] < 1000 ) {
      rx.desired[1] = MOTOR_MIN;
    }
  }

  if ( rx.rawvalue[4] > 0 )
  {
    KP = map(rx.rawvalue[4], 1000.0, 2000.0, 0.0, 200.0);
    KP /= 1000.0;
  }

  if ( rx.rawvalue[5] > 0 )
  {
    KD = map(rx.rawvalue[5], 1000.0, 2000.0, 0.0, 250.0);
    KD /= -1000.0;
  }
}




void RX() { 

  if( n < 3 )
  {
    n++;
  }
  else
  {
    n = 0;
  }


  //for( n = 0 ; n < 4 ; n++ )
  //{

  // Measure Pulse width
  rx.rawvalue[n] = pulseIn(rx.PIN[n], HIGH, 20000);

  // Map reading to desired
  if ( rx.rawvalue[n] > 0 )
  {
    rx.desired[n]  = map(rx.rawvalue[n], rx.minimum[n], rx.maximum[n], rx.minDesired[n], rx.maxDesired[n]);
  }
  //Serial.print(n);
  //Serial.print(" ");
  //Serial.println(rx.desired[n]);
  //}

  // Every 10 RX calls, check 1 of the AUXILARIES 
  if( AUXACTIVE )
  {  
    if( auxcount < 10 )
    {
      auxcount++;
    }
    else
    {
      auxcount = 0;

      if( auxtoggle )
      {
        rx.rawvalue[4] = pulseIn(rx.PIN[4], HIGH, 20000);
        if ( rx.rawvalue[4] > 0 )
        {
          KP = map(rx.rawvalue[4], 1000.0, 2000.0, 0.0, 200.0);
          KP /= 1000.0;
        }
        auxtoggle = false;
      }
      else
      {
        rx.rawvalue[5] = pulseIn(rx.PIN[5], HIGH, 20000);
        if ( rx.rawvalue[5] > 0 )
        {
          KD = map(rx.rawvalue[5], 1000.0, 2000.0, 0.0, 150.0);
          KD /= -1000.0;
        }
        auxtoggle = true;
      }
    }
  }
}

void RXalt() { // Receive 6CH every 20 RX runs otherwise 4CH

  if( auxcount < 20 ) {
    auxcount++;
    RX4CH();
  }
  else {
    auxcount = 0;
    RX6CH();
    }
  }




    /*
    rx.rawvalue[0] = ( rx.rawvalue[0] + RXnode[1] - RXnode[0] ) / 2;
    rx.rawvalue[2] = ( rx.rawvalue[2] + RXnode[2] - RXnode[1] ) / 2;
    rx.rawvalue[1] = ( rx.rawvalue[1] + RXnode[3] - RXnode[2] ) / 2;
    rx.rawvalue[3] = ( rx.rawvalue[3] + RXnode[4] - RXnode[3] ) / 2;

    
   Serial.println(rx.rawvalue[0]);
     Serial.println(rx.rawvalue[1]);
     Serial.println(rx.rawvalue[2]);
     Serial.println(rx.rawvalue[3]);
     Serial.println(rx.rawvalue[4]);
     Serial.println(rx.rawvalue[5]);
     Serial.println(rx.rawvalue[6]);
     Serial.println();
     */
