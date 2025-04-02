              // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0; 

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2,INPUT);
  

    // declare sensor as input
 
  Serial.begin(100);
}

void loop()
{
  
  val = digitalRead(2);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  delay(100);
  digitalWrite(12,LOW);
  digitalWrite(11,HIGH);
  delay(100);
  digitalWrite(11,LOW);  // turn LED ON
    
  } else {
    digitalWrite(2, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }}}
  
 
