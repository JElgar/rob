// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _MOTORS_H
#define _MOTORS_H

# define L_PWM_PIN 10
# define L_DIR_PIN 16
# define R_PWM_PIN 9
# define R_DIR_PIN 15

# define BASE_SPEED 20

class Motor {
  public:
    Motor(int pwm_pin, int dir_pin) {
      _pwm_pin = pwm_pin;
      _dir_pin = dir_pin;
    }
    double velocity_est = 0;
    
    void initialise() {
      pinMode(_pwm_pin, OUTPUT);
      pinMode(_dir_pin, OUTPUT);
    }
    void setPower(double pwm) {
      // Set motor direction
      if (pwm > 0) {
        digitalWrite(_dir_pin, LOW);
      } else {
        digitalWrite(_dir_pin, HIGH);
      }
      // Set motor speed
      analogWrite(_pwm_pin, min(abs(pwm), 30));
    }
    void updateVelocityEstimate(long count, unsigned long time_delta) {
      long delta_count = count - _last_count;
      _last_count = count;
      velocity_est = -(delta_count / (double)time_delta);
    }

  private:
    int _pwm_pin;
    int _dir_pin;
    long _last_count;
};

// Class to operate the motor(s).
class Motors_c {
  private:
    unsigned long _last_velocity_update;
    unsigned long _velocity_update_interval;
  public:
    Motor right = Motor(R_PWM_PIN, R_DIR_PIN);
    Motor left = Motor(L_PWM_PIN, L_DIR_PIN);
    // Constructor, must exist.
    Motors_c(unsigned long velocity_update_interval) {
      _velocity_update_interval = velocity_update_interval;
    } 

    void initialise() {
      // Set all the motor pins as outputs.
      left.initialise();
      right.initialise();
    }
    
    void update(double error) {
      error = error * 2;
      left.setPower(BASE_SPEED - BASE_SPEED * max(error, 0));
      right.setPower(BASE_SPEED + BASE_SPEED * min(error, 0));
    }

    void updateVelocityEstimates(unsigned long current_time, long count_e0, long count_e1) {
      unsigned long time_delta = current_time - _last_velocity_update;
      if (time_delta < _velocity_update_interval) {
        return;
      }
      _last_velocity_update = current_time;
      
      left.updateVelocityEstimate(count_e1, time_delta);
      right.updateVelocityEstimate(count_e0, time_delta);
    }

    void forward() {
      left.setPower(20);
      right.setPower(20);
    }

    void backward() {
      left.setPower(-20);
      right.setPower(-20);
    }

    void stop() {
      left.setPower(0);
      right.setPower(0);
    }
};



#endif
