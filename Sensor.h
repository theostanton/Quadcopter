void sensors()
{
  DTsec = DT / 1000000.0;
  // Take relative heading reading
   
      
  heading = compass.heading((LSM303::vector){ 0,-1,0 });
  
  if ( time.current < 5000 )
  {
    initHeading = heading; 
    firstHeading = false; 
  }
  
  Heading = heading - initHeading;
  
   // Accelerometer
   // ALOT of float arithmetic for atan2 calc... can this be reduced??
   // documented here http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
   
  //sqrt ( 1000 * A + 1000 * B )
  //   double 	atan2 (double __y, double __x)       ---- from arduino math docs
  // The atan2() function computes the principal value of the arc tangent of __y / __x, using the 
  // signs of both arguments to determine the quadrant of the return value. The returned value is in the range [-pi, +pi] radians.

  compass.read();
  acc.angle[ROLL]   = atan2( (float)compass.a.x * ACCGAIN , sqrt( ACCGAINSQ * ( sq( (float)compass.a.y ) + sq( (float)compass.a.z ) ) ) ) ; // CURRENTLY WITHOUT ZEROING
  acc.angle[PITCH]  = atan2( (float)compass.a.y * ACCGAIN , sqrt( ACCGAINSQ * ( sq( (float)compass.a.z ) + sq( (float)compass.a.x ) ) ) ) ;
  
  acc.angle[ROLL]   *= RAD_TO_DEG ;
  acc.angle[PITCH]  *= RAD_TO_DEG ;
  
  //ACC LPF
  acc.angle[ROLL]  += acc.prev[ROLL];
  acc.angle[PITCH] += acc.prev[PITCH];
  
  acc.angle[ROLL]  /= 2.0;
  acc.angle[PITCH] /= 2.0;
  
  acc.prev[ROLL]  = acc.angle[ROLL];
  acc.prev[PITCH] = acc.angle[PITCH];
  
  
  // Gyro 
  byte gyroxMSB   = readRegister(L3G4200D_Address, 0x29);
  byte gyroxLSB   = readRegister(L3G4200D_Address, 0x28);
  gyro.raw[PITCH] = ((gyroxMSB << 8) | gyroxLSB);

  byte gyroyMSB  = readRegister(L3G4200D_Address, 0x2B);
  byte gyroyLSB  = readRegister(L3G4200D_Address, 0x2A);
  gyro.raw[ROLL] = ((gyroyMSB << 8) | gyroyLSB);

  gyro.raw[PITCH]  -= gyro.zero[PITCH]; // check viability
  gyro.rate[PITCH]  = gyro.raw[PITCH] / GYROGAIN; // HANG ON SHOULDNT ThiS BE MULTIPLIED BY GAINNNN??
  gyro.angle[PITCH] = ctrl.angle[PITCH] + gyro.rate[PITCH] * DTsec; // DT in usec    add [ rate * seconds ] = [ degrees * sec-1 * sec -> degrees ]
  // still quite a lot of drift/delay - to be optimized. 
  
  gyro.raw[ROLL]  -= gyro.zero[ROLL];
  gyro.rate[ROLL]  = gyro.raw[ROLL] / GYROGAIN;
  gyro.angle[ROLL] = ctrl.angle[ROLL] + gyro.rate[PITCH] * DTsec;

  // Error
  ctrl.angle[PITCH]  = gyro.angle[PITCH]  * RATIO + acc.angle[PITCH]  * ( 1.0 - RATIO );
  ctrl.angle[ROLL] = gyro.angle[ROLL] * RATIO + acc.angle[ROLL] * ( 1.0 - RATIO );

  ctrl.error[PITCH]  = ctrl.angle[PITCH] - rx.desired[rxPITCH];
  ctrl.error_integral[PITCH] += ctrl.error[PITCH] * DTsec;

  ctrl.error[ROLL] = ctrl.angle[ROLL] - rx.desired[rxROLL];
  ctrl.error_integral[ROLL] += ctrl.error[ROLL] * DTsec;    
  
  //Serial.println( gyro.angle[PITCH] );

}
  
