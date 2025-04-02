int ir;
void setup()
{
Serial.begin(9600);
pinMode(6,OUTPUT);
pinMode(A0,INPUT);
}
void loop()
{
ir = analogRead(A0);
  if(ir>120) {
digitalWrite(6,HIGH);
  }
  else {
digitalWrite(6,LOW); 
  }
}