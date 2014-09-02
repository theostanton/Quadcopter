void interrupt() {
  bInt = true;
  motorA.write(MOTOR_MAX);
  motorB.write(MOTOR_MAX);     
  motorC.write(MOTOR_MAX); 
  motorD.write(MOTOR_MAX);
  //bIntToggle = !bIntToggle;
}

void interruptRun() {
  motorA.write(MOTOR_MAX);
  motorB.write(MOTOR_MAX);     
  motorC.write(MOTOR_MAX); 
  motorD.write(MOTOR_MAX);
}


/*
void interruptRun() {
  rx.rawvalue[4] = pulseIn(4, HIGH, 20000);
  if ( rx.rawvalue[4] > 1500 )
    {
      motorA.write(MOTOR_MAX);
      motorB.write(MOTOR_MAX);     
      motorC.write(MOTOR_MAX); 
      motorD.write(MOTOR_MAX);
      digitalWrite(13, LOW);
      delay(200);
      digitalWrite(13, HIGH);
    }
    else if ( rx.rawvalue[4] < 1500 && rx.rawvalue[4] > 900 )
    {
      motorA.write(MOTOR_MIN);
      motorB.write(MOTOR_MIN);     
      motorC.write(MOTOR_MIN); 
      motorD.write(MOTOR_MIN);
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
      delay(200);
      digitalWrite(13, LOW);
      bInt = false;
    }
}
      

/*
void interruptRIS() {
  interruptRISrun();
}

void interruptFAL() {
  interruptFALrun();
}

void interruptRUN()
{
  delay(500);
  digitalWrite(13, LOW); 
  delay(200);
  digitalWrite(13, HIGH); 
}

void interruptFALrun() {
        delay(1000);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
        delay(1000);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
        delay(1000);
        digitalWrite(13, LOW); 
        delay(200);
        digitalWrite(13, HIGH); 
}

void interruptRISrun() {
        delay(200);
        digitalWrite(13, LOW); 
        delay(1000);
        digitalWrite(13, HIGH); 
        delay(200);
        digitalWrite(13, LOW); 
        delay(1000);
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW); 
        delay(1000);
        digitalWrite(13, HIGH);
}
*/
