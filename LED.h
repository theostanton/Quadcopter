void LEDToggle()
{     
   if ( LEDtoggle )
   {
     digitalWrite(13, HIGH); 
     LEDtoggle = false;
   }
   else   
   {
     digitalWrite(13, LOW); 
     LEDtoggle = true;
   }
}

void ten_dots()
{
  for( int l = 0 ; l < 10 ; l++)
      {
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
      }
}

void two_dots()
{
  for( int l = 0 ; l < 2 ; l++)
      {
        delay(500);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
      }
}

void four_dots()
{
  for( int l = 0 ; l < 2 ; l++)
      {
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
      }
}

void seven_dashes()
{
  for( int l = 0 ; l < 7 ; l++)
      {
        delay(500);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
      }
}


void SOS()
{
  bSOS = false;
}

void SOS_display()
{
        digitalWrite(13, HIGH); 
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW); 
        delay(200);
        
        digitalWrite(13, HIGH); 
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
        delay(200);
        digitalWrite(13, LOW); 
        delay(200);
        bSOS = false;
        
      
}
