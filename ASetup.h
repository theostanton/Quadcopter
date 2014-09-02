  void setup()
  {
      pinMode(3, OUTPUT);
      pinMode(10, OUTPUT);
      pinMode(9, OUTPUT);
      pinMode(5, OUTPUT);
      
      pinMode(11, INPUT);
      pinMode(4, INPUT);
      pinMode(12, INPUT);
      pinMode(6, INPUT);
      pinMode(7, INPUT);
      pinMode(8, INPUT);    
    
      pinMode(13, OUTPUT);
      digitalWrite(13, HIGH);
        
      if ( serial || processing )
      {
        Serial.begin(115200);
        Serial.flush();
        //Serial.println("Serial active");
      }
      
      
      Wire.begin();
      
      if ( checkStartup ) {
        Serial.println("\n \n Checking Startup Sequence");
      }
      
      attachInterrupt(0, interrupt, FALLING);
      
      serialprint(); // send processing zeros to draw with
           
      motorA.attach(motor.PIN[0]);
      motorB.attach(motor.PIN[1]);
      motorC.attach(motor.PIN[2]);
      motorD.attach(motor.PIN[3]);
          
      motorA.write(1);
      motorB.write(1);
      motorC.write(1);
      motorD.write(1);
      
      //ten_dots();
      
      if ( checkStartup ) {
        Serial.println("Motors done");
      }
      
      delay(100);
        
      // configRX(); INSTEAD OF ZEROING -> USE MID POINT
      
      if ( checkStartup ) {
        Serial.print("LSM303 :");
      }
      
      setupLSM303();   
      
      if ( checkStartup ) {
        Serial.println(" Done");
      }
      
      if ( checkStartup ) {
        Serial.print("L3g4200D :");
      }
      
      setupL3G4200D(2000); // configure L3G4200 - 250, 500 or 2000 deg/sec
      
      if ( checkStartup ) {
        Serial.println(" Done");
      }
      
     
      //zeroSensors();
      
      time.previous = millis();          // Asses which of these are needed
      time.previousMicros = micros();
      time.sensorMicros = micros();
      
      // Init heading
      heading = compass.heading((LSM303::vector){0,-1,0});
      initHeading = heading;
           
      if ( checkStartup ) {
        Serial.println("Startup Complete");
      }
      
      digitalWrite(13, LOW); 
  }
