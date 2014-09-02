void loop()
{
  // Take timings - Reasses which are now necessary
  time.previousMicros = time.currentMicros;
  time.currentMicros  = micros();
  time.previous       = time.current;
  time.current        = millis();

  // DT to be used by sensors
  DT = ( time.currentMicros - time.previousMicros );
  //Serial.println(int(( millis() * 1000.0) / DTcount));
  //Serial.println(int(DT));
  if( processing ) {
    if( auxcount > 10 ) {
      DTs += DT;
      DTcount++; 
      DTavg = DTs / DTcount;  
      DTmax = max(DTmax,DT);
       
    }
    else {
      auxcount++;
    }
  }

  if ( checkStartup ) { 
    Serial.println("Pre loop");
  }

  if ( bSOS && !bInt) {
    if ( checkStartup ) { 
      Serial.println("In Loop");
    }

    // LED 1Hz - WTF y this not correct???
    if ( time.current > ( 1000 + time.hertz ) )
    {
      DTmax = 0.0;
      
      LEDToggle();

      if( processing ) {
        totNodes();
      }

      time.hertz = time.current;
    }

    // Sensors each loop
    
    nodeStart = micros();
    sensors();
    node[0] += micros() - nodeStart;
      
    if ( checkStartup ) { 
      Serial.println("Sensors complete");
    }

    // Control ~50 Hz
    //if ( time.current > ( 14 + time.ctrl ) )
    //{
      nodeStart = micros();
      control();
      node[1] += micros() - nodeStart;
      
      if ( checkStartup ) { 
        Serial.println("Control complete");
      }
      
      nodeStart = micros();
      motors();
      node[2] += micros() - nodeStart;
      
      if ( checkStartup ) { 
        Serial.println("motors complete");
      }

    //  time.ctrl = time.current;
    //}

    // RX ~30 Hz

    if ( time.current > ( 102 + time.rx ))
    {
      if ( checkStartup ) { 
        Serial.println("RX start");
      }
      
      nodeStart = micros();
      RXalt();
      node[4] += micros() - nodeStart;
      
      if ( checkStartup ) { 
        Serial.println("RX complete");
      }

      time.rx = time.current;  
    }

    // Serial Print ~8Hz

    if ( ( time.current > ( 124 + time.serial ) ) && processing  )
    {
      nodeStart = micros();
      serialprint();
      
      node[3] += micros() - nodeStart;
      //node[3] += nodeEnd - nodeStart;

      time.serial = time.current;
    }
    if ( checkStartup ) { 
      Serial.println("Loop complete");
    }
  }
  else
  {
    if ( bInt ) {
      interruptRun();
    }
    else {
      SOS_display();
    }
  }
}

