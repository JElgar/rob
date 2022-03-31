#include "motors.h"
#include "linesensor.h"
#include "kinematics.h"
#include "pid.h"
#include "encoders.h"

Motors_c motors = Motors_c(50);
LineSensor_c lineSensors = LineSensor_c(100);
Kinematics_c kinematics = Kinematics_c(100);
PID_c pid_left = PID_c(100, 20);
PID_c pid_right = PID_c(100, 20);

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

  kinematics.update(currentTime);
  lineSensors.update(currentTime);

  if (lineSensors.state == READY) {
    pid_left.update(currentTime, 0.5, motors.left.velocity_est);
    pid_right.update(currentTime, 0.5, motors.right.velocity_est);
    //Serial.println("VALS");
    //Serial.println(motors.left.velocity_est, 10);
    Serial.println(motors.right.velocity_est, 10);
    Serial.println(pid_right.feedback_signal, 10);
    //Serial.println(pid_left.feedback_signal, 10);
    //Serial.println(pid_left.i_term, 10);
    
    //motors.update(lineSensors.error);
    motors.left.setPower(pid_left.feedback_signal);
    motors.right.setPower(pid_right.feedback_signal);
    motors.updateVelocityEstimates(currentTime, count_e0, count_e1);
  }
}
