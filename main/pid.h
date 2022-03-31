// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _PID_H
#define _PID_H

// Class to contain generic PID algorithm.
class PID_c {
  private:
    long last_count_e0 = 0;
    long last_count_e1 = 0;

    unsigned long last_update = 0;
    unsigned long update_interval;

    double gain;
    double ki = 0.001;
    
  public:
    double velocity_e0;
    double velocity_e1;
    double feedback_signal = 0;
    double p_term = 0;
    double i_term = 0;
    
    // Constructor, must exist.
    PID_c(double gain, unsigned long update_interval) {
      this->gain = gain;
      this->update_interval = update_interval;
    }

//    double low_pass_filter() {
//      // An initialised valued
//      float lpf = 0;
//      float alpha = 0.2; // value between 0:1
//                         // how much weight to new readings?
//      
//      // Update low pass filter
//      lpf = ( lpf * (1 - alpha ) ) + ( new_reading * alpha );
//    }


    void update(unsigned long current_time, float demand, float measurement) {
      // Only update on demand
      unsigned long time_delta = current_time - last_update;
      if (time_delta < update_interval) {
        return;
      }
      last_update = current_time;

      float et = demand - measurement;
      p_term = et * gain;
      i_term += et * time_delta * ki;

      feedback_signal = p_term;
    }
    

};



#endif
