void motors()
{
  for (int i = 0 ; i < 4 ; i++ )
  {
    motor.command[i] = int( motor.f_correction[i] + rx.desired[rxTHROTTLE]  ) ; // motor command = throttle command + correction 
    
    //limited between max/min motor commands
    
    if( motor.command[i] > MOTOR_MAX )
    {
      motor.command[i] = MOTOR_MAX;
    }
    
    if( motor.command[i] < MOTOR_MIN )
    {
      motor.command[i] = MOTOR_MIN;
    }   
    
  }
  
  
  motorA.write(motor.command[A]);
  motorB.write(motor.command[B]);     
  motorC.write(motor.command[C]); 
  motorD.write(motor.command[D]);
  
  /*
  MANUALspd = map(analogRead(A0), 0, 1023, MOTOR_MIN, MOTOR_MAX);
  motorA.write(MANUALspd);
  motorB.write(MANUALspd);     
  motorC.write(MANUALspd); 
  motorD.write(MANUALspd);
  */
  
}

