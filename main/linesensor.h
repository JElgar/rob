// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_LEFT_PIN A0
#define LS_CENTRE_PIN A2
#define LS_RIGHT_PIN A3
#define LS_IR_PIN 11

#define LINE_THRESHOLD 1000 // Sensor time to be deemed as on line
#define TIMEOUT_THRESHOLD 5000 // Max time update can take

#define CALIBRATION_STEPS 10
#define CALIBRATION_UPDATE_INTERVAL 1000

bool LOG = false;

enum SensorState {
  UPDATED,
  UPDATING,
};

enum SensorsState {
  WAITING,
  CALIBRATING_LIGHT,
  CALIBRATING_DARK,
  READY,
};

class LineSensor {
  private:
    unsigned long calibration_vals[CALIBRATION_STEPS];
    double lightCalibrationAverage = 0;
    double darkCalibrationAverage = 0;
  public:
    SensorState state = UPDATED;
    unsigned long value;
  
    LineSensor(int pin, String sensor_name) {
      _pin = pin;
      _name = sensor_name;
    }
    void initialise() {
      pinMode(_pin, INPUT);
    }
    void setEmit() {
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, HIGH);
    }
    void setRead() {
      pinMode(_pin, INPUT);
    }
    uint8_t pinState() {
      return digitalRead(_pin);
    }
    bool isOnLine() {
      return value > LINE_THRESHOLD;
    }

    // During an update, check the sensor value. If it is low the capacitor has fully 
    // discharged so the update is completed. Save the time of discharge as the value 
    // and mark the sensor as updated.
    void checkUpdate(unsigned long start_time) {
      if (state != UPDATING) return;

      // Calculate elapsed time
      unsigned long current_time = micros();
      unsigned long elapsed_time = current_time - start_time;
 
      // If the capacitor has finished discharging
      if(pinState() == LOW or elapsed_time > TIMEOUT_THRESHOLD) {
        value = elapsed_time;

        // Set sensor to updated
        state = UPDATED;
      }
    }

    void setCalibrationValue(int calibration_step) {
      Serial.println("Calibration step");
      Serial.println(calibration_step);
      calibration_vals[calibration_step] = value;
    }

    void completeCalibration(SensorsState calibrationState) {
      float calibrationAverage = 0;
      for (int i = 0; i < CALIBRATION_STEPS; i++) {
        calibrationAverage += calibration_vals[i];
      }
      calibrationAverage /= (double)CALIBRATION_STEPS;

      if (calibrationState == CALIBRATING_LIGHT) {
        lightCalibrationAverage = calibrationState;
      } else if (calibrationState == CALIBRATING_DARK) {
        darkCalibrationAverage = calibrationState;
      }

      Serial.println("Hello");
      Serial.println("Sensor " + _name + " calibration average: " + calibrationAverage);
    }

    void log_values() {
      Serial.print("Sensor " + _name + " updated: ");
      Serial.print(isOnLine());
      Serial.print(" and value is ");
      Serial.println(value);
    }

  private:
    String _name;
    int _pin;
    
};

// Class to operate the linesensor(s).
class LineSensor_c {
  private:
    unsigned long _update_interval;
    unsigned long _last_update = 0;
    unsigned long _last_calibration_update = 0;

  public:
    LineSensor left = LineSensor(LS_LEFT_PIN, "left");
    LineSensor centre = LineSensor(LS_CENTRE_PIN, "centre");
    LineSensor right = LineSensor(LS_RIGHT_PIN, "right");
    // SensorsState state = WAITING;
    SensorsState state = READY;
    
    LineSensor_c(unsigned long update_interval) {
      _update_interval = update_interval;
    }

    double error = 0;
    int calibration_step = 0;
    
    void initialise() {
      // Init the sensors.
      left.initialise();
      centre.initialise();
      right.initialise();

      // Turn on IR
      pinMode(LS_IR_PIN, OUTPUT);
      digitalWrite(LS_IR_PIN, HIGH);
    }

    unsigned long sum() {
      return left.value + right.value + centre.value;
    }

    void update_error() {
      double total = (double)sum();
      error = (double)left.value/total - (double)right.value/total;
      Serial.println(error);
    }

    void calibrationUpdate(unsigned long current_time) {
      if (state == READY) {
        return;
      }
      
      if (state == WAITING) {
        if (digitalRead(30) == HIGH) {
          Serial.println("A button pressed");
        }
        if (digitalRead(14) == HIGH) {
          Serial.println("B button pressed");
        }
        if (digitalRead(17) == HIGH) {
          Serial.println("C button pressed");
        }
        return;
      }
      
      if (current_time - _last_calibration_update < CALIBRATION_UPDATE_INTERVAL) {
        return;
      } 
      
      sensorsUpdate();

      // Store curent value in calibration
      centre.setCalibrationValue(calibration_step);
      right.setCalibrationValue(calibration_step);
      left.setCalibrationValue(calibration_step);

      // Increment calibration step
      calibration_step += 1;

      _last_calibration_update = millis();

      if (calibration_step >= CALIBRATION_STEPS) {
        centre.completeCalibration(state);
        right.completeCalibration(state);
        left.completeCalibration(state);
        
        if (state == CALIBRATING_LIGHT) {
          state = CALIBRATING_DARK;
          calibration_step = 0;
        } else {
          state = READY;
        }

      }
    }

    void sensorsUpdate() {  
  
      // Mark the sensors as updatingnsigned long
      centre.state = UPDATING;
      right.state = UPDATING;
      left.state = UPDATING;

      // Charge capacitor by turning on IR emitter
      centre.setEmit();
      right.setEmit();
      left.setEmit();
      delayMicroseconds(10);

      // Stop charging
      centre.setRead();
      right.setRead();
      left.setRead();
       
      unsigned long start_time = micros();

      // Wait till all sensors have finished updating
      while (
        centre.state == UPDATING or
        right.state == UPDATING or
        left.state == UPDATING
      ) {
        centre.checkUpdate(start_time);
        right.checkUpdate(start_time);
        left.checkUpdate(start_time); 
      }

      update_error();
      
      if (LOG) {
        centre.log_values();
        left.log_values();
        right.log_values();
        
        Serial.print("Error value is: ");
        Serial.println(error);
      }
    }

    void update(unsigned long current_time) {
      if (state != READY) {
        calibrationUpdate(current_time);
        return;
      }
      
      if (current_time - _last_update < _update_interval) {
        return;
      } 

      sensorsUpdate();
      
      _last_update = millis();
    }

    bool isOnLine() {
      return centre.isOnLine() || right.isOnLine() || left.isOnLine();
    }
};



#endif
