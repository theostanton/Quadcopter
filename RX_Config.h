// To be ammended - midpoint rather than min

void configRX()
{  
      for(int i=0; i < RXZEROLOOPS; i++)
      {   
        for(int n=0; n < 4; n++) // ZERO PITCH ROLL THROTTLE AND YAW
        {
          rx.rawvalue[n] += pulseIn(rx.PIN[n], HIGH, 20000); /// Accumuate zero readings
          delay(10);
        }
      }
      
      for(int n=0; n < 4; n++) // ZERO PITCH ROLL THROTTLE AND YAW
      {
        rx.minimum[n] = rx.rawvalue[n] / RXZEROLOOPS;
      }
      
}
