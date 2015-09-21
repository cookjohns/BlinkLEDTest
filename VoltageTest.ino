/****************************************************
  Voltage read/signal Test for ATtiny85

  modified date: See date of latest commmit on GitHub
  by John Cook, Allison MacDonald
*****************************************************/

/* Set up pins */
int INPUTPIN     = 3;
int OUTPUTLOW    = 1;
int OUTPUTHIGH   = 2;

// executes once on power up
void setup() {
  pinMode(3, INPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
}

// executes continuously
void loop() {
  int voltage = analogRead(INPUTPIN); // 0-5v over range 0-1023 (.0049v/unit)

  // if voltage is over 3.5v 
  if (voltage > 716) digitalWrite(OUTPUTHIGH, HIGH);

  // if voltage is under 1v
  else if (voltage < 102) digitalWrite(OUTPUTLOW, HIGH);

  // otherwise, clear the lights
  else {
    digitalWrite(OUTPUTHIGH, LOW);
    digitalWrite(OUTPUTLOW,  LOW);
  }
}
