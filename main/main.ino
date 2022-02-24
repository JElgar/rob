#include "motors.h"
#include "linesensor.h"
#include "kinematics.h"
#include "pid.h"

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
  
  // Setup line sensors
  lineSensors.initialise();
  motors.initialise();
}


//void bang_bang() {
//   if (line_sensors.centre.isOnLine()) {
//    motors.forward();
//  } else if (line_sensors.right.isOnLine()) {
//    motors.left();
//  } else if (line_sensors.left.isOnLine()) {
//    motors.right();
//  }
//}


// put your main code here, to run repeatedly:
void loop() {
  unsigned long currentTime = millis();

  lineSensors.update(currentTime);

  if (lineSensors.state == READY) {
    if (currentTime % 1000 == 0) {
      Serial.println("Updating motors ");
      Serial.print("Left value: ");
      Serial.println(lineSensors.left.value);
      Serial.print("Right value: ");
      Serial.println(lineSensors.right.value);
      Serial.println(lineSensors.error, 10);
    }

    motors.update(lineSensors.error);
  }
}
