/*
Capacitance measuring device on Arduino.

This is a redesign of my previous design published on Tinkercad
Please use capacitor values between 150pF and 1mF

Version: 1.0
Author: Emilien Dole
*/

// PRECOMPILATION
// LIBRARIES
#include <stdlib.h>
#include <LiquidCrystal.h>
// ARDUINO PINS
// Inputs pins
#define ACP A0
#define PBUTTON 2
// LCD pins
#define LCRS 9
#define LCEN 8
#define LCD4 7
#define LCD5 6
#define LCD6 5
#define LCD7 4
// Resistors pins
#define R1_PIN 12
#define R2_PIN 11
#define R3_PIN 10
#define RM_PIN 3
// RESISTORS VALUES
// Measurements resistance
#define R1_VALUE 200
#define R2_VALUE 20000
#define R3_VALUE 2000000
// Discharge resistance
#define RM_VALUE 10 
// CONST. VARIABLES
#define TIMEOUT 2000000
#define ANALOG_THRESHOLD 644
#define RECTIFYING_FACTOR 1005

// RESISTOR CLASS
class Resistor {
  //
  public:
    // VARIABLES
    String name; // Resistor name
    size_t pin; // Resistor pin
    double value; // Resistor value
  
    // CONSTRUCTOR
    Resistor(String rname, size_t apin, float rvalue) {
      //
      name = rname; // Resistor name
      pin = apin; // Arduino pin
      value = rvalue; // Resistor value
    }
};

// PROTOTYPES
void proceedMeasure();
double Evaluate(Resistor resistor);
void DischargeCapacitor(size_t rpin, size_t apin);

// GLOBAL STATEMENTS
bool stateProceed = false;

//
byte micro[8] = { // Micro character
  B00000,
  B00000,
  B01001,
  B01001,
  B01001,
  B00111,
  B00001,
};

//
Resistor R1("R1", R1_PIN, R1_VALUE); // Low value resistor
Resistor R2("R2", R2_PIN, R2_VALUE); // Mid value resistor
Resistor R3("R3", R3_PIN, R3_VALUE); // High value resistor
Resistor Rm("Rm", RM_PIN, RM_VALUE); // Discharging resistor (mass resistor)

//
LiquidCrystal lcd(LCRS, LCEN, LCD4, LCD5, LCD6, LCD7);

// FUNCTIONS
void proceedMeasure() {
  if(stateProceed == false)
  	stateProceed = !stateProceed;
}

//
double Evaluate(Resistor resistor) {
  // Set timer variables
  unsigned long st; // Start time
  unsigned long dt; // Time delta
  
  // Return value
  double cp_value = 0;
  
  // Print to console
  Serial.print(resistor.name);
  Serial.println(" Evaluating");
  
  // Print to LCD
  
  
  // Set pin mode to output
  pinMode(resistor.pin, OUTPUT);
  
  // Wait 100ms
  delay(100);
  
  // Set output to high
  digitalWrite(resistor.pin, HIGH);
  
  // Start measurment
  volatile byte enable = true;
  st = micros(); // Set start time
  while(enable) { // Wait 63% of 5V or dt > TIMEOUT
    //
    dt = micros() - st; // Set delta
    
    //
    if(dt >= TIMEOUT) { // Timemout reached
      //
      cp_value = -1;
      enable = false;
    }
    
    //
    else if(analogRead(ACP) >= ANALOG_THRESHOLD) {
      //
      cp_value = RECTIFYING_FACTOR * (dt / resistor.value);
      enable = false;
    }
  }
  
  // Set output to low
  digitalWrite(resistor.pin, LOW);
  
  // Wait 100ms
  delay(100);
  
  // Set pin mode to input
  pinMode(resistor.pin, INPUT);
  
  //
  return cp_value;
}

//
void DischargeCapacitor(size_t rpin, size_t apin) {
  // Set rpin mode to output
  pinMode(rpin, OUTPUT);
  
  // Print to console
  Serial.println("Discharging");
  
  //Wait for the capacitor to discharge
  while(analogRead(apin) > 0) {} 
  
  // Wait 200ms -- Making sure that the capacitor is fully discharged
  delay(200);
  
  // Set rpin mode to input
  pinMode(rpin, INPUT);
  
  // Wait 100ms
  delay(100);
}

// SETUP
void setup() {
  //
  pinMode(ACP, INPUT);
  pinMode(PBUTTON, INPUT_PULLUP);
  
  //
  pinMode(R1.pin, INPUT);
  pinMode(R2.pin, INPUT);
  pinMode(R3.pin, INPUT);
  pinMode(Rm.pin, INPUT);
  
  //
  lcd.begin(16, 2);
  lcd.noCursor();
  lcd.createChar(1, micro);
  
  //
  Serial.begin(9600);
  Serial.println("Ready to measure");
  
  //
  attachInterrupt(digitalPinToInterrupt(PBUTTON), proceedMeasure, FALLING);
}


// LOOP
void loop() {
  //
  if(stateProceed == true) {
    //
    detachInterrupt(digitalPinToInterrupt(PBUTTON));
    
    //
    Serial.println("Start of measurements");
    Serial.println("Please consider not resetting the arduino until the whole process is complete");
    
  	//
    lcd.clear();
    
    //
    DischargeCapacitor(Rm.pin, ACP);
    
    /*----------------*/
    
    //
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.write("R1 Evaluating");
    lcd.setCursor(2, 1);
    lcd.write("[..........]");
    
    //
    double C1 = Evaluate(R1);
    
    //
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.write("Discharging");
    lcd.setCursor(2, 1);
    lcd.write("[**........]");
    
    //
    DischargeCapacitor(Rm.pin, ACP);
    
    /*----------------*/
    
    //
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.write("R2 Evaluating");
    lcd.setCursor(2, 1);
    lcd.write("[****......]");
    
    //
    double C2 = Evaluate(R2);
	
    //
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.write("Discharging");
    lcd.setCursor(2, 1);
    lcd.write("[******....]");
    
    //
    DischargeCapacitor(Rm.pin, ACP);
    
    /*----------------*/
    
    //
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.write("R3 Evaluating");
    lcd.setCursor(2, 1);
    lcd.write("[********..]");
    
    //
    double C3 = Evaluate(R3);

    //
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.write("Discharging");
    lcd.setCursor(2, 1);
    lcd.write("[**********]");
    
    delay(1000);
    
    //
    DischargeCapacitor(Rm.pin, ACP);
    
    /*----------------*/
    
    //
    lcd.clear();
    
    //
    if((C1 >= 100000) && (C1 < 5000000)) {
      Serial.println("C1 result is printed");
      lcd.setCursor(5, 0);
      lcd.print(C1 / 1000000, 1.0);
      lcd.write("mF"); //mF
    }
    
    //    
    else if((C2 >= 10) && (C2 < 100000)) {
      Serial.println("C2 result is printed");
      lcd.setCursor(5, 0);
      lcd.print(C2 / 1000, 2.0);
      lcd.write(1); lcd.write("F"); //uF
    }

    //
    else if((C3 >= 0.15) && (C3 < 10)) {
      Serial.println("C3 result is printed");
      lcd.setCursor(5, 0);
      lcd.print(C3, 2.0);
      lcd.write("nF"); //uF
    }
    
    //
    else {
      Serial.println("An error has occurred");
      lcd.setCursor(5, 0);
      lcd.write("error");
    }
	
    //
    attachInterrupt(digitalPinToInterrupt(PBUTTON), proceedMeasure, FALLING);
    
    //
    stateProceed = !stateProceed;
  }
}