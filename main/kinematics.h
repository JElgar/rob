// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _KINEMATICS_H
#define _KINEMATICS_H

#include "encoders.h"

#define CPR 358.3
#define WHEEL_RADIUS 16
#define WHEEL_SEPERATION 80

double mod_360(double value) {
  if (value > 360) {
    return value - 360;
  }
  if (value < 0) {
    return value + 360;
  }
  return value;
}

class MotorKinematics {
  private:
    long lastCount = 0;

    void setVelocity(long count, unsigned long timeDelta) {
      velocity = (double)(count - lastCount) * ((double)360 / (double)CPR);
      // velocity /= timeDelta;
    }

    void setDistance() {
      distance = velocity * WHEEL_RADIUS;
    }

  public:
    double velocity = 0;
    double distance = 0;
    
    void update(long count, unsigned long timeDelta) {
      setVelocity(count, timeDelta);
      setDistance();
      lastCount = count;
    }
};

// Class to track robot position.
class Kinematics_c {

  private:
    unsigned long lastUpdate = 0;
    MotorKinematics mk0 = MotorKinematics();
    MotorKinematics mk1 = MotorKinematics();
    unsigned long updateInterval = 0;
  
  public:
    double x = 0;
    double y = 0;
    double r = 0;
  
    // Constructor, must exist.
    Kinematics_c(unsigned long updateInterval) {
      this->updateInterval = updateInterval;
    } 

    // Use this function to update
    // your kinematics
    void update(unsigned long currentTime) {

      unsigned long timeDelta = currentTime - lastUpdate;
      if (timeDelta < updateInterval) {
        return;
      }

      mk0.update(count_e0, timeDelta);
      mk1.update(count_e1, timeDelta);

      double xr = (mk0.distance + mk1.distance) / (double)2;
      double rr = (mk0.distance - mk1.distance) / (double)(WHEEL_SEPERATION);

      x = x + xr*cos(radians(r));
      y = y + xr*sin(radians(r));
      r = mod_360(r + rr);

      lastUpdate = currentTime;
    }

};

#endif
