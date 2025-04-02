
const int D0 = 0; 
const int D1 = 1;
const int D2 = 2;
const int D3 = 3;

const int D4 = 4; // NO CHANGE PIN - BUTTON

void One() {
  digitalWrite(D2, HIGH);
}

void Two() {
  digitalWrite(D3, HIGH);
}

void Three() {
  One();
  Two();
}

void Four() {
  Two();
  digitalWrite(D0, HIGH);
}

void Five() {
  One();
  Four();
}

void Six() {
  Four();
  digitalWrite(D1, HIGH);
}
void ledOff() {
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
}

void setup() {
  pinMode(D4, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D0, OUTPUT);
  rollDice(2);
}

void rollDice(int times) {
  for (int i = 0; i < times; i++) {
    One(); delay(100); ledOff();
    Two(); delay(100); ledOff();
    Three(); delay(100); ledOff();
    Four(); delay(100); ledOff();
    Five(); delay(100); ledOff();
    Six(); delay(100); ledOff();
  }
}

int RND() {
  randomSeed(analogRead(D4));
  int seed = 0;
  int digit = 0;
  while (digit > 6 || digit <= 0) {
    seed = (seed * 53) + 21;
    digit = seed % 6;
    seed = random(50) + digit;
    digit += seed;
  }
  return digit;
}

void loop() {
  int btn = 0;
  btn = digitalRead(D4);
  if (btn == HIGH) {
    ledOff();
    int digit = RND();
    rollDice(2);
    if (digit == 1) One();
    if (digit == 2) Two();
    if (digit == 3) Three();
    if (digit == 4) Four();
    if (digit == 5) Five();
    if (digit == 6) Six();
  }
}
