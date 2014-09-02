    void setupLSM303() 
  {  
    //Serial.println("Starting up LSM303...");
    compass.init();
    compass.enableDefault();
    delay(500); //wait for the sensor to be ready WAS 1500
    compass.m_min.x = -715; compass.m_min.y = -531; compass.m_min.z = -654;
    compass.m_max.x = +346; compass.m_max.y = +519; compass.m_max.z = +389;
    
  }
  
  
  
  #define CTRL_REG1 0x20
  #define CTRL_REG2 0x21
  #define CTRL_REG3 0x22
  #define CTRL_REG4 0x23
  #define CTRL_REG5 0x24
  
  
int setupL3G4200D(int scale){
  
    // Enable x, y, z and turn off power down:
    // CTRL_REG1 --- DR1 DR0 BW1 BW0 PD Zen Yen Xen
    // 1110 = 800 data rate. LPF2 = 50Hz
    writeRegister(L3G4200D_Address, CTRL_REG1, 0b11101111);
  
    // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
    writeRegister(L3G4200D_Address, CTRL_REG2, 0b00100011);
  
    // Configure CTRL_REG3 to generate data ready interrupt on INT2
    // No interrupts used on INT1, if you'd like to configure INT1
    // or INT2 otherwise, consult the datasheet:
    writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);
  
    // CTRL_REG4 controls the full-scale range, among other things:
  
    if(scale == 250){
      writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
    }else if(scale == 500){
      writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
    }else{
      writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
    }
  
    // CTRL_REG5 controls high-pass filtering of outputs, use it
    // if you'd like:
    writeRegister(L3G4200D_Address, CTRL_REG5, 0b01000010); // 000X0000 HPF enable // 000000x0 LPF2 enable
    delay(500); //wait for the sensor to be ready was 1500
   
  
  }
  
  void writeRegister(int deviceAddress, byte address, byte val) {
      Wire.beginTransmission(deviceAddress); // start transmission to device 
      Wire.write(address);       // send register address
      Wire.write(val);         // send value to write
      Wire.endTransmission();     // end transmission
  }
  
  int readRegister(int deviceAddress, byte address){
  
      int v;
      Wire.beginTransmission(deviceAddress);
      Wire.write(address); // register to read
      Wire.endTransmission();
  
      Wire.requestFrom(deviceAddress, 1); // read a byte
  
      while(!Wire.available()) {
          // waiting
      }
  
      v = Wire.read();
      return v;
  }
  
void zeroSensors()
{
  //Serial.println(" ");
  //Serial.println("Calibrating sensors...");

  for ( int i = 0 ; i < ZEROLOOPS ; i++ )
  {
    byte gyroxMSB = readRegister(L3G4200D_Address, 0x29);
    byte gyroxLSB = readRegister(L3G4200D_Address, 0x28);
    gyro.zero[ROLL] =+ ((gyroxMSB << 8) | gyroxLSB);
  
    byte gyroyMSB = readRegister(L3G4200D_Address, 0x2B);
    byte gyroyLSB = readRegister(L3G4200D_Address, 0x2A);
    gyro.zero[PITCH] =+ ((gyroyMSB << 8) | gyroyLSB);
  
    byte gyrozMSB = readRegister(L3G4200D_Address, 0x2D);
    byte gyrozLSB = readRegister(L3G4200D_Address, 0x2C);
    gyro.zero[YAW] =+ ((gyrozMSB << 8) | gyrozLSB);
    
    //gyro.zero[ROLL]  += gyro.raw[ROLL];
    //gyro.zero[PITCH] += gyro.raw[PITCH];
    //gyro.zero[YAW]   += gyro.raw[YAW];
  }
  
  gyro.zero[ROLL]  /= ZEROLOOPS;
  gyro.zero[PITCH] /= ZEROLOOPS;
  gyro.zero[YAW]   /= ZEROLOOPS;

  //Serial.print("Gyro X Zero Value : ");
  //Serial.println(gyro.zero[ROLL]);
  //Serial.print("Gyro Z Zero Value : ");
  //Serial.println(gyro.zero[PITCH]);
  //Serial.println();

  for ( int i = 0 ; i < ZEROLOOPS ; i++ )
  {
    compass.read();
    acc.zero[ROLL]  += (float)compass.a.x ;
    acc.zero[PITCH] += (float)compass.a.y ;
  }

  acc.zero[ROLL]  *= 1000.0;
  acc.zero[PITCH] *= 1000.0;
  acc.zero[ROLL]  /= ZEROLOOPS;
  acc.zero[PITCH] /= ZEROLOOPS;


  //Serial.print("Acc X  Zero Value : ");
  //Serial.println(acc.zero[ROLL]);
  //Serial.print("Acc Y Zero Value : ");
  //Serial.println(acc.zero[PITCH]);
  //Serial.println();

}

  
  
  
  
  /*
  
  void setupLSM303() 
  {  
    compass.init();
    compass.enableDefault();
    delay(500); //wait for the sensor to be ready WAS 1500
    compass.m_min.x = -715; compass.m_min.y = -531; compass.m_min.z = -654;
    compass.m_max.x = +346; compass.m_max.y = +519; compass.m_max.z = +389;
    heading = compass.heading((LSM303::vector){0,-1,0});
    initHeading = heading;
  }
  
  #define CTRL_REG1 0x20 
  #define CTRL_REG2 0x21 // Enables sensor and turns off 'power down'
  #define CTRL_REG3 0x22 // generates data ready interrupts - to be optimized
  #define CTRL_REG4 0x23 // scale Configuration
  #define CTRL_REG5 0x24 // High pass configuration
  
  int setupL3G4200D(int scale){
    //From  Jim Lindblom of Sparkfun's code
  
    // Enable x, y, z and turn off power down:
    writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);
  
    // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
    writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);
  
    // Configure CTRL_REG3 to generate data ready interrupt on INT2
    // No interrupts used on INT1, if you'd like to configure INT1
    // or INT2 otherwise, consult the datasheet:
    writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);
  
    // CTRL_REG4 controls the full-scale range, among other things:
  
    if(scale == 250){
      writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
    }else if(scale == 500){
      writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
    }else{
      writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000); // SETS 2000 - read datasheet to see what else desirable
    }
  
    // CTRL_REG5 controls high-pass filtering of outputs, use it
    // if you'd like:
    writeRegister(L3G4200D_Address, CTRL_REG5, 0b0100000);
    delay(700); //wait for the sensor to be ready was 1500
    }
    
  void writeRegister(int deviceAddress, byte address, byte val)
  {  
      Wire.beginTransmission(deviceAddress);      // start transmission 
      Wire.write(address);                       // send register address
      Wire.write(val);                           // send value to write
      Wire.endTransmission();                    // end transmission
  }
  
  int readRegister(int deviceAddress, byte address)
  {  
      int v;
      Wire.beginTransmission(deviceAddress);
      Wire.write(address); // register to read
      Wire.endTransmission();
  
      Wire.requestFrom(deviceAddress, 1); // read a byte
  
      while(!Wire.available()) {
          // waiting
      }
  
      v = Wire.read();
      return v;
  }
  
  void zeroSensors()
  {  
    // Zero sensors - check viability - Could avoid uncessary float arithmetic - likely to be same values each time
    // could define integer value instead
    
    for ( int i = 0 ; i < ZEROLOOPS ; i++ )
    {
      byte gyroxMSB = readRegister(L3G4200D_Address, 0x29);
      byte gyroxLSB = readRegister(L3G4200D_Address, 0x28);
      gyro.zero[ROLL] += ((gyroxMSB << 8) | gyroxLSB);
      
      byte gyroyMSB = readRegister(L3G4200D_Address, 0x2B);
      byte gyroyLSB = readRegister(L3G4200D_Address, 0x2A);
      gyro.zero[PITCH] += ((gyroyMSB << 8) | gyroyLSB);
  
      byte gyrozMSB = readRegister(L3G4200D_Address, 0x2D);
      byte gyrozLSB = readRegister(L3G4200D_Address, 0x2C);
      gyro.zero[YAW] += ((gyrozMSB << 8) | gyrozLSB);
    }
    
    gyro.zero[ROLL]  /= ZEROLOOPS;
    gyro.zero[PITCH] /= ZEROLOOPS;
    gyro.zero[YAW]   /= ZEROLOOPS;
      
    for ( int i = 0 ; i < ZEROLOOPS ; i++ )
    {
      compass.read();
      acc.zero[ROLL]  += (float)compass.a.x ;
      acc.zero[PITCH] += (float)compass.a.y ;
    }
      
    acc.zero[ROLL]  *= ACCGAIN;
    acc.zero[PITCH] *= ACCGAIN;
    
    acc.zero[ROLL]  /= ZEROLOOPS;
    acc.zero[PITCH] /= ZEROLOOPS;
      
  }
  
  */
