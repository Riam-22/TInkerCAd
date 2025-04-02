 /* Arduino dimmer - Dave Gray 
 / Demonstrates simple Arduino code to read a sensor; 
 / map the reading and use the value to adjust an LED 
 / Open serial monitor to see the values being read.
*/

#define LED_PIN 4	// declare LED pin as will not change
long senVal = 0; // somewhere to store the dimmer - sensor reading
int ledVal = 0;  // to hold the mapped value for the LED brightness

void setup()
{
  Serial.begin(9600); // open serial port for feedback
  pinMode(LED_PIN, OUTPUT); // declare the LED pin as an output
}

void loop()
{
  senVal = analogRead(0); // read the analog value
  
  Serial.print(senVal); // print the sensor value
  
  ledVal = map(senVal, 0, 1023, 0, 255); // use the map function to convert the 0-1023 analog value to a 0-255 value we can use for PWM
  
  Serial.print(","); // print a comma to separate values
  Serial.println(ledVal); // print the mapped value and line end
  
  analogWrite(LED_PIN, ledVal); // adjust the LED using the 0-255 mapped sensor value
  
  delay(100);  
}