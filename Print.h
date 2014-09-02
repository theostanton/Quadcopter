void serialprint()
{
  if ( processing )
  {
    // List 0 - 5
    Serial.print(gyro.angle[PITCH]);
    Serial.print(","); 
    Serial.print(acc.angle[PITCH]);
    Serial.print(","); 
    Serial.print(ctrl.angle[PITCH]);
    Serial.print(","); 
    Serial.print(gyro.angle[ROLL]);
    Serial.print(","); 
    Serial.print(acc.angle[ROLL]);
    Serial.print(","); 
    Serial.print(ctrl.angle[ROLL]);
    Serial.print(","); 
    // List 6 - 11
    Serial.print(ctrl.error[PITCH]);
    Serial.print(","); 
    Serial.print(ctrl.error[ROLL]);
    Serial.print(","); 
    Serial.print(motor.f_correction[A]);
    Serial.print(","); 
    Serial.print(motor.f_correction[B]);
    Serial.print(","); 
    Serial.print(motor.f_correction[C]);
    Serial.print(","); 
    Serial.print(motor.f_correction[D]);
    Serial.print(",");
    // List 12 - 23
    Serial.print(20 * proc.p[A]);
    Serial.print(",");
    Serial.print(20 * proc.i[A]);
    Serial.print(",");
    Serial.print(20 * proc.d[A]);
    Serial.print(",");
    Serial.print(20 * proc.p[B]);
    Serial.print(",");
    Serial.print(20 * proc.i[B]);
    Serial.print(",");
    Serial.print(20 * proc.d[B]);
    Serial.print(",");
    Serial.print(20 * proc.p[C]);
    Serial.print(",");
    Serial.print(20 * proc.i[C]);
    Serial.print(",");
    Serial.print(20 * proc.d[C]);
    Serial.print(",");
    Serial.print(20 * proc.p[D]);
    Serial.print(",");
    Serial.print(20 * proc.i[D]);
    Serial.print(",");
    Serial.print(20 * proc.d[D]);
    Serial.print(",");
    // List 24 - 27
    Serial.print(rx.desired[0]);
    Serial.print(",");
    Serial.print(rx.desired[1]);
    Serial.print(",");
    Serial.print(rx.desired[2]);
    Serial.print(",");
    Serial.print(rx.desired[3]);
    Serial.print(",");
    // list 28 - 33
    Serial.print(nodeTot[0]); // 28 - Sensor
    Serial.print(",");
    Serial.print(nodeTot[1]); // 29 - Ctrl
    Serial.print(",");
    Serial.print(nodeTot[2]); // 30 - Motor
    Serial.print(",");
    Serial.print(nodeTot[3]); // 31 - Serial
    Serial.print(",");
    Serial.print(nodeTot[4]); // 32 - RX
    Serial.print(",");
    Serial.print(DTmax);
    Serial.print(",");
    // list 34 -- 40 
    Serial.print(Heading  );
    Serial.print(",");
    Serial.print(KP);
    Serial.print(",");
    Serial.print(KD);
    Serial.print(",");
    Serial.print(motor.command[A]);
    Serial.print(",");
    Serial.print(motor.command[B]);
    Serial.print(",");
    Serial.print(motor.command[C]);
    Serial.print(",");
    Serial.print(motor.command[D]);
    Serial.print(",");
    // list 41 -- 
    //Serial.print(time.current / 1000.0);
    Serial.print(DTavg);
    Serial.print(",");
    //Serial.print(DTlist[DTlistcountread]);
    //Serial.print(",");
    Serial.println();
    
    
  }
  else
  {
   Serial.println(RXnode[6] - RXnode[5]);
  }
}
