/*
*******************************************************
	Smart Car Parking System Using Arduino Assignment,
    
*******************************************************

1: An LCD show the number of available parking spots.

2: If the ultra-sonic sensor measures a car within 1.5 meter, the entry gate opens for 4sec, and closes.

3: The exit system implemented with an Interrupt. 
4: Once the PIR senses a movement - the exit gate opens for 4sec, and closes.

5: For each gate that opens, the number of available spots is updated accordingly. 
6: The Neo-Pixel is a taken/free spots visualiztion, 
7: and change its lights according to the available Spots.

8: In addition to that, the LED changes form red to green, to notify that the gate is open.
When either gate is closed, the LED lights is red.
**************************************************************************************************
*/

// include the library code:
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <Servo.h>

//pins:
#define pingPin  13 
#define IRPin    4 
#define servoIn  5
#define servoOut 6
#define PIRPin 	 3 
//RGB LED:
#define redLed   0  
#define greenLed 1
//neo LED:
#define neoPin   2
#define neoCount 12
//servo constants:
#define barLow   90
#define barHigh  185
#define carDelay 4000 
//constants:
#define capacity 	12
#define minDistance 100
   
// initialize the LCD pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// initialize the servo motor
//Servo servo;
Servo serv_in;
Servo serv_out;
bool en_entry = false;

// initialize the neo LEDs
Adafruit_NeoPixel pixels(neoCount, neoPin, NEO_GRB + NEO_KHZ800);

long pingDistance;
int availableSpots = 4;
int takenSpots = capacity - availableSpots;
const int entryCode = 255; 

// setup:
void setup() {
  
//servo set up
  	serv_out.attach(servoOut);
  	serv_out.write (barLow);
//delay(750);
    serv_in.attach(servoIn);
  	serv_in.write (barLow);
  
  	
	lcd.begin(16, 2);
	lcd.print("   Smart Car  ");
    lcd.setCursor(0,1);
  	lcd.print("Parking System ");
	delay(5000);
	lcd.clear();
	 	
// neo leds setup
// 255 = 100% brightness
	pixels.begin();
	pixels.setBrightness(255); 
  
//lights takenSpots in red
	for(int i = 0; i < takenSpots; i++){
		pixels.setPixelColor (i, 255, 0, 0);
	}
//lights availableSpots in green
	for(int i=capacity-availableSpots; i<capacity; i++){
		pixels.setPixelColor (i, 0, 255, 0);
	}
	pixels.show();
  
  
// pin mode:
	pinMode(redLed, OUTPUT);
	digitalWrite(redLed, HIGH);
	pinMode(greenLed, OUTPUT);  

//PIR pinMode
	pinMode(PIRPin, INPUT);
	
// Enable interrupts of the PIR, and exit-gate
    attachInterrupt(digitalPinToInterrupt(PIRPin), letCarOut, RISING);
}

// program loop:
void loop() {
	      // measure ping distace fo the entrance to the parking lot 
	pingDistance = getPingDistance();

	          // IN
	         //checks if there are more free parking spots:  
	if (availableSpots >= 1){
		    //checks if the minimun entrance distance condition is met
		if(pingDistance <= minDistance){
			//asks for the password to get in
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Welcome!!");
          	delay(3000);

						en_entry = true;
						openGate();
					   // car enters
						delay(carDelay);
                 		lcd.clear();
                  		en_entry = true;
						closeGate();
						en_entry = false;

		}
		   // minDistance condition for entrance is not met
		  // waitnig for action
		else{
          	lcd.setCursor(11,0);
          	lcd.print("  ");
            //lcd.clear();
			waitForAction();
		}
	}
  
	   //else - no free spots left
	  // LCD prints "NO Free Spots"
	else{ 
		lcd.clear();
		lcd.setCursor(1,0);
		lcd.print("Sorry!");
		lcd.setCursor(0,1);
		lcd.print("No Free Spots");
		delay(50);
	}
}
 
  /*
   opens ans closes the exit-gate.
   if the parking-lot is empty - 
   the exit-gate will NOT open
  */
void letCarOut(){
 // OUT
// opens the gate only if NOT empty
  	if(availableSpots < capacity){
			openGate();
            // car exits
			delay(carDelay);
      		lcd.clear();
			closeGate();
    } 
}

 /*
  sends a ping
  get the time it takes for the ping to read
  returns the distance in cm
 */
long getPingDistance(){
	// send a ping of 5uSec
	pinMode(pingPin, OUTPUT);
	digitalWrite(pingPin, LOW);
	delayMicroseconds(5);
	digitalWrite(pingPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pingPin, LOW);
  
	//reads the time
	pinMode(pingPin, INPUT);
  
	// returns distance in cm
	return pulseIn(pingPin, HIGH) / 29 / 2 ;
}


/*
openGate():
checks the en_entry state (true/false)
and performs a series of different actions 
according to the 'en_entry' state:

opens the gate
update the available Spots 
print a message on the LCD
and changes the RGB from red to green
*/
void openGate(){	

//let car in
// open the entry gate
	if(en_entry){ 
      	en_entry = false;
		serv_in.write(barHigh);
		availableSpots--;
		pixels.setPixelColor(capacity-availableSpots-1, 255, 0, 0);
		pixels.show();
	 // lcd's welcome message
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("May Enter Now");
	}
        // let car out:
	   // open the exit gate
	else			
	{
		serv_out.write(barHigh);
		availableSpots++;
        pixels.setPixelColor(capacity-availableSpots, 0, 255, 0);
        pixels.show();
	    // lcd's good-bye message
		lcd.clear();
		lcd.setCursor(0, 1);
		lcd.print("Good Bye...");
	}
	//change  RGB-Led from red to green
	digitalWrite(greenLed, HIGH);
	digitalWrite(redLed, LOW);  
}

/*
closes the gate according to
the 'en_entry' state
and changes the RGB from green to red
*/
void closeGate(){
	
 // car got in:
 // close the entry gate
	if(en_entry){ 
		serv_in.write(barLow);
	}
	                                  
  // car got out:
  // close the exit gate
	else			
	{
		serv_out.write(barLow);
	}
//change  RGB-Led from green to red
	digitalWrite(greenLed, LOW);
	digitalWrite(redLed, HIGH);
}

 /*
  if on one comes in or out:
  LCD shows availableSpots
 */
void waitForAction(){
	lcd.setCursor(0, 0);
	lcd.print("Free Spots:");
	lcd.setCursor(0,1);
	lcd.setCursor(7, 1);
	lcd.print(availableSpots);
	delay(30);
}