#include "motors.h"
#include "linesensor.h"
#include "kinematics.h"
#include "pid.h"
#include "encoders.h"

Motors_c motors = Motors_c();
LineSensor_c lineSensors = LineSensor_c(100);

// put your setup code here, to run once:
void setup() {

  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");

  // Set buttons to inputs
  pinMode(30, INPUT);
  pinMode(14, INPUT);
  pinMode(17, INPUT);

  setupEncoder0();
  setupEncoder1(); 
  
  // Setup line sensors
  lineSensors.initialise();
  motors.initialise();
}


// put your main code here, to run repeatedly:
void loop() {
  unsigned long currentTime = millis();

  lineSensors.update(currentTime);

  if (lineSensors.state == READY) {
    motors.update(lineSensors.error);
  }
}
