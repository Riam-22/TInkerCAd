
int ledPin = 9;      // LED connected to digital pin 9
int val = 0;         // variable to store the read value

void setup() {
  pinMode(ledPin, OUTPUT);  // sets the pin as output
}

void pulse (int pin, double cycle)
{
  double t;
  t=10000*cycle/100;
  digitalWrite(pin, HIGH);
  delayMicroseconds(t);
  digitalWrite(pin, LOW);
  delayMicroseconds(10000-t);
}
void loop() 
{
  for(double i=0;i<250;i++)
    pulse(ledPin, i/2.5);
 
  for(double i=250;i>0;i--)
  { pulse(ledPin, i/2.5); /*Serial.println(i);*/ }
}