 void control()
{
  // NEEDS YAW COMMANDS - Diagram correct polarities of errors

  // PID Calculations
  // If motor is physically too high -> correction is negative
  // A = +P + +R -> CW  -Yaw
  // B = +P + -R -> CCW +Yaw
  // C = -P + -R -> CW  -Yaw 
  // D = -P + +R -> CCW +Yaw

  //Processing
  proc.p[A] = ( + ctrl.error[PITCH] + ctrl.error[ROLL] ) * KP ;
  proc.i[A] = ( + ctrl.error_integral[PITCH] - ctrl.error_integral[ROLL] ) * KI ;
  proc.d[A] = ( + gyro.rate[PITCH] - gyro.rate[ROLL] ) * KD ;
  motor.f_correction[A] = proc.p[A] + proc.i[A] + proc.d[A] - rx.desired[rxYAW];
  
  // A correction = - pitch correction + roll correction
  //motor.f_correction[A]  = ( - ctrl.error[PITCH] - ctrl.error[ROLL] ) * KP ;
  //motor.f_correction[A] += ( - ctrl.error_integral[PITCH] - ctrl.error_integral[ROLL] ) * KI ;
  //motor.f_correction[A] -= ( - gyro.rate[PITCH] - gyro.rate[ROLL] ) * KD ;

  // B correction = - pitch correction - roll correction
  proc.p[B]  = ( + ctrl.error[PITCH] - ctrl.error[ROLL] ) * KP ;
  proc.i[B]  = ( + ctrl.error_integral[PITCH] + ctrl.error_integral[ROLL] ) * KI ;
  proc.d[B]  =  + gyro.rate[PITCH]*KD + gyro.rate[ROLL]* KD ;
  motor.f_correction[B] = proc.p[B] + proc.i[B] + proc.d[B] + rx.desired[rxYAW];

  // C correction = + pitch correction - roll correction
  proc.p[C]  = ( - ctrl.error[PITCH] - ctrl.error[ROLL] ) * KP ;
  proc.i[C]  = ( - ctrl.error_integral[PITCH] + ctrl.error_integral[ROLL] ) * KI ;
  proc.d[C]  =  - gyro.rate[PITCH] * KD + gyro.rate[ROLL] * KD ;
  motor.f_correction[C] = proc.p[C] + proc.i[C] + proc.d[C] - rx.desired[rxYAW];


  // D correction = + pitch correction + roll correction
  proc.p[D]  = ( - ctrl.error[PITCH] +  ctrl.error[ROLL] ) * KP ;
  proc.i[D]  = ( - ctrl.error_integral[PITCH] - ctrl.error_integral[ROLL] ) * KI ;
  proc.d[D]  = - gyro.rate[PITCH]*KD - gyro.rate[ROLL]* KD ;
  motor.f_correction[D] = proc.p[D] + proc.i[D] + proc.d[D] + rx.desired[rxYAW];
  
}
