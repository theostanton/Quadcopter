#include "Arduino.h"
#include "Sensors.h"

#include <Wire.h>
#include <LSM303.h>

LSM303 compass; 

#define RATIO 0.9 

#define GYROGAIN 14.735
#define RAD_TO_DEG 57.30

#define ACCGAIN 1100.0 // Accelerometer gain, square of gain calced in setup
#define ACCGAINSQ 1210000.0   

#define L3G4200D_Address 107

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24


Sensors::Sensors(){
    Wire.begin();

    a = (Accel){ 
    	{ 0.0, 0.0, 0.0 }, 
    	{ 0.0, 0.0, 0.0 },
    	{ 0.0, 0.0, 0.0 },
    	{ 0.0, 0.0, 0.0 }  
    };

    g = (Gyro){ 
    	{ 0.0, 0.0, 0.0 }, 
    	{ 0.0, 0.0, 0.0 },
    	{ 0.0, 0.0, 0.0 },
    	{ 0.0, 0.0, 0.0 }  
    };

    c = (Ctrl){ 
    	{ 0.0f, 0.0f, 0.0f }, 
      { 0.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f },
    	{ 0.0f, 0.0f, 0.0f }
    };
}

void Sensors::init(){
  Serial.println("Init sensors");
	Serial.println("setupAccel"); 
	setupAccel(); 
	Serial.println("setupGyor"); 
	setupGyro(); 
	Serial.println("Done"); 
}

void Sensors::read(float DT, int *desired){

  	float DTsec = DT / 1000000.0f;

	// Accel 

	compass.read();

	a.angle[ROLL]   = atan2( (float)compass.a.x * ACCGAIN , sqrt( ACCGAINSQ * ( sq( (float)compass.a.y ) + sq( (float)compass.a.z ) ) ) ) ; 
  a.angle[PITCH]  = atan2( (float)compass.a.y * ACCGAIN , sqrt( ACCGAINSQ * ( sq( (float)compass.a.z ) + sq( (float)compass.a.x ) ) ) ) ;

  a.angle[ROLL]   *= RAD_TO_DEG ;
  a.angle[PITCH]  *= RAD_TO_DEG ;
  a.angle[YAW]    *= RAD_TO_DEG ;
  
  	//ACC LPF
	a.angle[ROLL]  += a.prev[ROLL];
	a.angle[PITCH] += a.prev[PITCH];

	a.angle[ROLL]  /= 2.0;
	a.angle[PITCH] /= 2.0;

	a.prev[ROLL]  = a.angle[ROLL];
	a.prev[PITCH] = a.angle[PITCH];

  	// Gyro 

  byte gyroxMSB   = readRegister(L3G4200D_Address, 0x29);
	byte gyroxLSB   = readRegister(L3G4200D_Address, 0x28);
	g.raw[PITCH] = ((gyroxMSB << 8) | gyroxLSB);

	byte gyroyMSB  = readRegister(L3G4200D_Address, 0x2B);
	byte gyroyLSB  = readRegister(L3G4200D_Address, 0x2A);
	g.raw[ROLL] = ((gyroyMSB << 8) | gyroyLSB);

	//gyro.raw[PITCH]  -= gyro.zero[PITCH]; // check viability
	g.rate[PITCH]  = g.raw[PITCH] / GYROGAIN; // HANG ON SHOULDNT ThiS BE MULTIPLIED BY GAINNNN??
	g.angle[PITCH] = c.angle[PITCH] + g.rate[PITCH] * DTsec; // DT in usec    add [ rate * seconds ] = [ degrees * sec-1 * sec -> degrees ]
	// still quite a lot of drift/delay - to be optimized. 
	
	//gyro.raw[ROLL]  -= gyro.zero[ROLL];
	g.rate[ROLL]  = g.raw[ROLL] / GYROGAIN;
	g.angle[ROLL] = c.angle[ROLL] + g.rate[PITCH] * DTsec;


	// Error
	c.angle[PITCH]  = g.angle[PITCH]  * RATIO + a.angle[PITCH]  * ( 1.0 - RATIO );
	c.angle[ROLL] = g.angle[ROLL] * RATIO + a.angle[ROLL] * ( 1.0 - RATIO );

	c.error[PITCH] = (float)desired[PITCH] - c.angle[PITCH];
	c.error_integral[PITCH] += c.error[PITCH] * DTsec;

	c.error[ROLL] = (float)desired[ROLL] - c.angle[ROLL];
	c.error_integral[ROLL] += c.error[ROLL] * DTsec;  

  c.rate[ROLL] = g.rate[ROLL];  
  c.rate[PITCH] = g.rate[PITCH];  
  c.rate[YAW] = g.rate[YAW];  
}

void Sensors::print(){
	Serial.print( a.angle[ROLL] );
	Serial.print(" ");
	Serial.print( a.angle[PITCH] );
  Serial.print(" ");
  Serial.print( a.angle[YAW] );
	Serial.print(" "); 
	Serial.print( g.angle[ROLL] );
	Serial.print(" ");
	Serial.print( g.angle[PITCH] );
	Serial.print(" "); 
	Serial.print( c.angle[ROLL] );
	Serial.print(" ");
	Serial.print( c.angle[PITCH] );
	Serial.println(); 
}

void Sensors::setupAccel(){
    compass.init();
    compass.enableDefault();
    delay(500); //wait for the sensor to be ready WAS 1500
    compass.m_min.x = -715; compass.m_min.y = -531; compass.m_min.z = -654;
    compass.m_max.x = +346; compass.m_max.y = +519; compass.m_max.z = +389;
}

void Sensors::setupGyro(){

	int scale = 2000; 

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

void Sensors::writeRegister(int deviceAddress, byte address, byte val) {
      Wire.beginTransmission(deviceAddress); // start transmission to device 
      Wire.write(address);       // send register address
      Wire.write(val);         // send value to write
      Wire.endTransmission();     // end transmission
}

int Sensors::readRegister(int deviceAddress, byte address){
  
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

