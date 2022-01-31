// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_LEFT_PIN A0
#define LS_CENTRE_PIN A2
#define LS_RIGHT_PIN A3
#define LS_IR_PIN 11

#define LINE_THRESHOLD 1500 // Sensor time to be deemed as on line
#define TIMEOUT_THRESHOLD 5000 // Max time update can take

enum SensorState {
  UPDATED,
  UPDATING,
};

class LineSensor {
  public:
    SensorState state = UPDATED;
  
    LineSensor(int pin) {
      _pin = pin;
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
      return _value > LINE_THRESHOLD;
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
        _value = elapsed_time;

        // Set sensor to updated
        state = UPDATED;
      }
    }
    unsigned long _value;
  private:
    int _pin;
    
};

// Class to operate the linesensor(s).
class LineSensor_c {
  private:

  public:
    LineSensor _left_ls = LineSensor(LS_LEFT_PIN);
    LineSensor _centre_ls = LineSensor(LS_CENTRE_PIN);
    LineSensor _right_ls = LineSensor(LS_RIGHT_PIN);
    LineSensor_c() {}

    void initialise() {
      // Init the sensors.
      _left_ls.initialise();
      _centre_ls.initialise();
      _right_ls.initialise();

      // Turn on IR
      pinMode(LS_IR_PIN, OUTPUT);
      digitalWrite(LS_IR_PIN, HIGH);
    }

    void updateValues() {
      // Mark the sensors as updating
      _centre_ls.state = UPDATING;
      _right_ls.state = UPDATING;
      _left_ls.state = UPDATING;

      // Charge capacitor by turning on IR emitter
      _centre_ls.setEmit();
      _right_ls.setEmit();
      _left_ls.setEmit();
      delayMicroseconds(10);

      // Stop charging
      _centre_ls.setRead();
      _right_ls.setRead();
      _left_ls.setRead();
       
      unsigned long start_time = micros();

      // Wait till all sensors have finished updating
      while (
        _centre_ls.state == UPDATING or
        _right_ls.state == UPDATING or
        _left_ls.state == UPDATING
      ) {
        _centre_ls.checkUpdate(start_time);
        _right_ls.checkUpdate(start_time);
        _left_ls.checkUpdate(start_time); 
      }
    }
};



#endif
