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
    
  public:
    double velocity;
    
    // Constructor, must exist.
    PID_c() {} 

  

};



#endif
