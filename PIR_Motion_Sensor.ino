// Constants
#define ledCount (sizeof(leds)/sizeof(uint8_t))
#define DUTY 50
const byte led3 = 3, led1 = 1, led0 = 0;
uint8_t leds[] = {led3, led1, led0};

// Variables
uint32_t prevTime = 0;
uint8_t pwm = 0, index = 0;

void setup() {

  for(uint8_t i = 0; i < ledCount; i++){
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], pwm);
        delay(10);
    }
    delay(500);
}

void loop() {

    if(millis() - prevTime >= DUTY){
        prevTime = millis();
                uint16_t val = analogRead(A2);
                pwm = map(val, 0, 1024, 0, 254);
                    for(uint8_t i = 0; i < ledCount; i++){
                        analogWrite(leds[i], pwm);
                        delay(10);
                    }
            }    
}