#ifndef UF0_FSM_h
#define UF0_FSM_h

// Preliminary
  // Enterprise
    #include "UF0_MIDI.h"
    #include "UF0_GYRO.h"
  // I2C
    #include <Wire.h>
  // I2C Pins for Lolin32 Lite
    #define I2C_SCL 19
    #define I2C_SDA 23
  // I2C Pins for Lolin32
  //   #define I2C_SDA 21
  //   #define I2C_SCL 22
  // Disk access
    #include <Preferences.h>
  // Main DBG
    #define monitorActive true
    #define monitorHz 5000000
  // SLEEP stuff
    #define BUTTON_ON_OFF_BITMASK 0x200000000 // 2^BUTTON_ON_OFF in hex (this case BUTTON_ON_OFF = 33)
    #define TIME_FOR_SLEEP 3000
    bool SLEEP_TIMER_ACTIVE = false;
    unsigned long SLEEP_TIMER;
  // Timers
    unsigned long FSM_t0 = millis();
    
    enum class toggleState{
    off1,
    off2,
    off3,
    off4,
    on1,
    on2,
    on3,
    on4
};
//

void init(){
  // DBG_SETUP(115200);
  // Begin I2C connection
    Wire.begin(I2C_SDA, I2C_SCL);
}

void startSleepTimer(){
  SLEEP_TIMER = millis();
  SLEEP_TIMER_ACTIVE = true;
}

void resetSleepTimer(){
  SLEEP_TIMER = -1;
  SLEEP_TIMER_ACTIVE = false;
}

class UF0_FSM{
  private:

    void setup(){
      esp_sleep_enable_ext1_wakeup(BUTTON_ON_OFF_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
    }

  public:

      UF0_FSM(){}

      UF0_FSM(bool cmon){
        setup();
      }

      void loop(){
        if ((millis() - FSM_t0) > monitorHz){
          DBG(F("\nRaw Roll:"), roll, F("\tMIDI Roll:"), rollMIDI);
          DBG(F("Raw Pitch:"), pitch, F("\tMIDI Pitch:"), pitchMIDI);
          DBG(F("Relative Yaw:"), yaw, F("\tMIDI Yaw:"), yawMIDI);
          DBG(F("State Roll, Pitch, Yaw:"), rollTransmit, pitchTransmit, yawTransmit);
          DBG(F("Virtual Target:"), virtualTarget);
          DBG(F("IP:"), WiFi.localIP());
          FSM_t0 = millis();
        }
        
        // DEEP SLEEP
        if (SLEEP_TIMER_ACTIVE){
          if ((millis() - SLEEP_TIMER) > TIME_FOR_SLEEP){
            esp_deep_sleep_start();
          }
        }
      }

};

#endif