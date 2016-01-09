#include <Servo.h>

Servo servo;
int servoPosition = 90;

int incomingByte = 0;   // for incoming serial data

void setup()
{
  Serial.begin(9600); // // opens serial port, sets data rate to 9600 bps
  
  servo.attach(5); // attaches the servo on pin 5 to the servo object
  servo.write(servoPosition); // set the servo at the mid position
}

void loop()
{
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    
    switch(incomingByte)
    {
      // Rotate camera left
      case 'l':
      servoPosition+=5;
      
      if (servoPosition > 180)
      {
        servoPosition = 180;
      }

      break;
      
      // Rotate camera right
      case 'r':
      servoPosition-=5;
      
      if (servoPosition < 0)
      {
        servoPosition = 0;
      }

      break;
      
      // Center camera
      case 'c':
      servoPosition = 90;
      
      break;
    }
    
    servo.write(servoPosition);
  }
}