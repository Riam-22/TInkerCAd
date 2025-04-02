// C++ code
//
void setup()
{
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{
  analogWrite(11, 255);
  analogWrite(9, 0);
  analogWrite(10, 0);
  delay(10); // Delay a little bit to improve simulation performance
}