#ifndef UF0_GYRO_h
#define UF0_GYRO_h

// Preliminary
  #include <BNO055_support.h>
  // Struct for BNO055 info
    struct bno055_t myBNO;
  // Struct for Euler data
    struct bno055_euler myEulerData;
  // Struct for Accel data
    struct bno055_linear_accel myAccelData;
    float yaw, pitch, roll;
  // Heading deviation from Svalbard
    double initHeading;
  // Acceleration MATH for drum hit
    double oldAccelZ{0}, newAccelZ{0}, highestAccelZ{0}, highestDiff{0}, accelDiff{0};
  // For drum hit
    int hit{-1};
    int drumHitDebounce{100} /*(ms)*/;
    int drumHitThreshold{650};
  // Timers
    unsigned long GYRO_t0 = millis(), GYRO_t1 = millis();
//

void resetHeading(){
  initHeading = myEulerData.h / 16.00;
}

double getRelativeHeading(double globalHeading, double headingDrift){
  // Shift initial heading by 'headingDrift' degrees
    double localHeading = (globalHeading - headingDrift);
  // Make cyclical
    if (localHeading >= 360) localHeading = localHeading - 360;
    if (localHeading < 0) localHeading = localHeading + 360;
  // Map [0, 360) -> [-180, 180)
    if (localHeading > 180) localHeading = localHeading - 360;
    return localHeading;
}

class UF0_GYRO{
  private:
  
    void setup(){
      DBG("BEFORE");
    // Assigning the structure to hold information about the device
      BNO_Init(&myBNO);
      DBG("AFTER");
    // Configuration to NDoF mode
      bno055_set_operation_mode(OPERATION_MODE_NDOF);
      DBG("Place towards your front & gimme sec...");
    // A moment for gyro to wake up
      delay(1000);
      DBG("Good!");
    // Update Euler data into the structure
      bno055_read_euler_hrp(&myEulerData);
    // Get heading deviation
      resetHeading();
      DBG("Init Heading:", initHeading);
    // To see heading value
      delay(1000);
    }

  public:

      UF0_GYRO(){}

      UF0_GYRO(bool cmon){
        setup();
      }

      void loop(){
      //Update Euler data into the structure
        bno055_read_euler_hrp(&myEulerData);
      // Our North might not be the same as our planet's pole
        yaw = getRelativeHeading((myEulerData.h / 16.00), initHeading);
        pitch = myEulerData.r / 16.00;
        roll = -myEulerData.p / 16.00;
      //Update Accelerometer data into the structure
        bno055_read_linear_accel_xyz(&myAccelData);
        newAccelZ = myAccelData.z;
      }
};

// Drum hit gesture
int drumHit(){
  if ((millis() - GYRO_t1) > drumHitDebounce){
    accelDiff = newAccelZ - oldAccelZ;
    oldAccelZ = newAccelZ;
    if (accelDiff > drumHitThreshold){
    // If hit, return velocity!
      return map(accelDiff - drumHitThreshold, 0, 2000, 0, 127);
    }
    else{
      return -1;
    }
    GYRO_t1 = millis();
  }

  // // Leave for drum hit optimization
  // if (newAccelZ > highestAccelZ)
  //   highestAccelZ = newAccelZ;
  // if (newAccelZ - oldAccelZ > highestDiff)
  //   highestDiff = newAccelZ - oldAccelZ;  
  // if ((millis() - GYRO_t0) > 1000){
  //     DBG("");
  //     DBG("New:", newAccelZ);
  //     DBG("Old:", oldAccelZ);
  //     DBG("New - old:", newAccelZ - oldAccelZ);
  //     DBG("High:", highestAccelZ);
  //     DBG("Highest diff:", highestDiff);
  //     GYRO_t0 = millis();
  // }
  return -1;
}

#endif