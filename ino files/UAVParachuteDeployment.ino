/****************************************************
  Pinin/out Test for ATtiny85

  modified date: See date of latest commmit on GitHub
  by John Cook, Allison MacDonald
*****************************************************/

#include <avr/wdt.h>

/* Pin 3 set to send parachute deployment signal */
// CHECK THESE DUMMY VALUES
int DEPLOYPIN  = 1;
int VOLTAGEPIN = 3;
int ACCELPIN   = 6;
boolean PASS   = true;
boolean FAIL   = false;
boolean STATUS = false;
boolean v      = true; // for voltage check inside loop
int iterations = 0;  // for crash reporting

// the setup function runs once when you power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
  pinMode(6, INPUT);

  // create watchdog timer object
  wdt_enable(WDTO_1S);
}

// runs continuosly
void loop() {
  wdt_reset();  // reset watchdog timer
  
  v = checkVoltage(); // do this here to avoid calling it twice
  
  if (!v || checkAcceleration()) {
    sendDeployCommand(v);
    // break somehow, so it doesn't continue to fire solenoid after failure?
  }
}

// sends HIGH to deploypin in order to activate solenoid to open parachute
void sendDeployCommand(boolean statusIn) {
  digitalWrite(DEPLOYPIN, HIGH);  // pop chute
  if (statusIn) shutdown();  // shut down the autopilot and motors if failure mode switches flag
}

// sends a command to Ardupilot to shutdown
void shutdown() {
  // trip hardware disconnect for power here
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
