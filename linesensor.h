// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_LEFT_PIN A0
#define LS_CENTRE_PIN A2
#define LS_RIGHT_PIN A3
#define LS_IR_PIN 11

class LineSensor {
  public:
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
    uint8_t state() {
      return digitalRead(_pin);
    }
  private:
    int _pin;
    int _value;
};

// Class to operate the linesensor(s).
class LineSensor_c {
  private:
    LineSensor _left_ls = LineSensor(LS_LEFT_PIN);
    LineSensor _centre_ls = LineSensor(LS_CENTRE_PIN);
    LineSensor _right_ls = LineSensor(LS_RIGHT_PIN);
  public:
    // Constructor, must exist.
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
      unsigned long current_time;

      // Charge capacitor by turning on IR emitter
      _centre_ls.setEmit();
      delayMicroseconds(10);
      _centre_ls.setRead();

      unsigned long start_time = micros();
      while (_centre_ls.state() == HIGH) {}
      unsigned long end_time = micros();

      // Calculate elapsed time
      unsigned long elapsed_time = end_time - start_time;

      // Print output.
      Serial.print("Centre line sensor: " );
      Serial.print( elapsed_time );
      Serial.print("\n");

      delay(100);
    }
};



#endif
