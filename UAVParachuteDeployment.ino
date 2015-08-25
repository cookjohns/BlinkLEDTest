/*
  Pinin/out Test for ATtiny85

  modified date: See date of latest commmit on GitHub
  by John Cook, Allison MacDonald
 */

/* Pin 3 set to send parachute deployment signal */
// CHECK THESE DUMMY VALUES
int DEPLOYPIN  = 3;
int VOLTAGEPIN = 5;
int ACCELPIN   = 6;
boolean PASS   = true;
boolean FAIL   = false;
boolean STATUS = false;

// runs continuosly
void loop() {
  // insert watchdog timer  
  
  if (!checkVoltage() || !checkAcceleration()) {  // need specific order here, so as to short circuit appropriately?
    sendDeployCommand(STATUS);
    // break somehow, so it doesn't continue to fire solenoid after failure?
  }
}

// the setup function runs once when you power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// writes a 1 to pin 3 in order to activate solenoid to open parachute
void sendDeployCommand(boolean statusIn) {
  digitalWrite(DEPLOYPIN, HIGH);
  if (statusIn) shutdown();  // shut down the autopilot and motors if failure mode switches flag
}

// sends a command to Ardupilot to shutdown
void shutdown() {
  // do stuff here
}

// returns true if voltage is above 0.3
boolean checkVoltage() {
  int voltage = analogRead(VOLTAGEPIN);
  if (voltage >= 61.44) return PASS;
  return FAIL;
}

// returns true if downward acceleration is too fast (need exact value)
boolean checkAcceleration() {
  int acceleration = analogRead(ACCELPIN);
  if (acceleration >= 500) return FAIL;
  return PASS;
}
