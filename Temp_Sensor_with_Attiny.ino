// C++ code
//
int buttonState = 0;

int counter;

void setup()
{
  pinMode(7, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  buttonState = digitalRead(7);
  if (buttonState == 1) {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    delay(1000); // Wait for 1000 millisecond(s)
    for (counter = 0; counter < 3; ++counter) {
      digitalWrite(3, LOW);
      delay(1000); // Wait for 1000 millisecond(s)
      digitalWrite(3, HIGH);
      delay(1000); // Wait for 1000 millisecond(s)
    }
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    delay(5000); // Wait for 5000 millisecond(s)
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  } else {
    digitalWrite(2, HIGH);
    digitalWrite(6, HIGH);
  }
}