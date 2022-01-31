// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _MOTORS_H
#define _MOTORS_H

# define L_PWM_PIN 10
# define L_DIR_PIN 16
# define R_PWM_PIN 9
# define R_DIR_PIN 15

class Motor {
  public:
    Motor(int pwm_pin, int dir_pin) {
      _pwm_pin = pwm_pin;
      _dir_pin = dir_pin;
    }
    void initialise() {
      pinMode(_pwm_pin, OUTPUT);
      pinMode(_dir_pin, OUTPUT);
    }
    void setPower(float pwm) {
      // Set motor direction
      if (pwm > 0) {
        digitalWrite(_dir_pin, LOW);
      } else {
        digitalWrite(_dir_pin, HIGH);
      }
      // Set motor speed
      analogWrite(_pwm_pin, abs(pwm));
    }

  private:
    int _pwm_pin;
    int _dir_pin;
};

// Class to operate the motor(s).
class Motors_c {
  private:
    Motor _right_motor = Motor(R_PWM_PIN, R_DIR_PIN);
    Motor _left_motor = Motor(L_PWM_PIN, L_DIR_PIN);
  public:
    // Constructor, must exist.
    Motors_c() {} 

    void initialise() {
      // Set all the motor pins as outputs.
      _left_motor.initialise();
      _right_motor.initialise();
    }
    
    void forward() {
      setLeftMotorPower(20);
      setRightMotorPower(20);
    }

    void stop() {
      setLeftMotorPower(0);
      setRightMotorPower(0);
    }

    void setLeftMotorPower(float pwm) {
      _right_motor.setPower(pwm);
    }
    
    void setRightMotorPower(float pwm) {
      _left_motor.setPower(pwm);
    }
};



#endif
