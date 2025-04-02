#include <Servo.h>

Servo myservo; // Object [myservo] of Servo Library is created
               // OOPs Concept

void setup()
{
  myservo.attach(9);
}

void loop()
{
  myservo.write(45);
  delay(1000); 
  myservo.write(90);
  delay(1000);
  myservo.write(135);
  delay(1000);
  myservo.write(180); // Servo Motor Can Rotate maximum 180 degree.
  delay(1000);
}