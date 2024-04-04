#ifndef UF0_FSM_h
#define UF0_FSM_h

// Preliminary
  // Enterprise
    #include "UF0_MUSIC/UF0_MIDI.h"
    #include "UF0_HARDWARE/UF0_GYRO.h"
    #include "UF0_MUSIC/UF0_PRESET_SYSTEM.h"
  // I2C
    #include <Wire.h>
  // I2C Pins for Lolin32 Lite
    #define I2C_SCL 22
    #define I2C_SDA 19
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

class controllerMode{

};

class noMode : controllerMode{

};

class chordMode : controllerMode{
  /*
  
  
  */
};

// void CHECKKKK(){
// //Check if any MOD buttons are pressed
//   switch(btnMode){
//   // Trigger do:
//     //////////////// N O    M O D E //////////////
//     case NO_MODE:
//     // Hold T1:
    
//       // Turn off previously played note, if any
//         midii->sendMIDINoteOff(MIDINoteOn, 100, 5);
//       // Update root MIDI Note On
//         MIDINoteOn = rootNote;
//       // Play root note
//         midii->sendMIDINoteOn(MIDINoteOn, 100, 5);
//     // Let T1:
    
//       // Turn off previously played note, if any
//         midii->sendMIDINoteOff(MIDINoteOn, 100, 5);
//     // Hold T2:
    
//         midii->enableControl();
//     // Let T2:
    
//         midii->disableControl();
//     // Hold TF:
    
//         // open function
//     // Let TF:
    
//         // close function
//       break;
//     //////////////// S O L O   M O D E ///////////////
//     case SOLO_MODE:
    
//         midii->toggleTransmission(&pitchTransmit);
        
//         midii->toggleTransmission(&rollTransmit);
        
//         midii->toggleTransmission(&yawTransmit);
//       break;
//     /////////////// C H O R D   M O D E /////////////////
//     case CHORD_MODE:
//     // Hold T1:
    
//       // Turn off previously played notes, if any
//         midii->sendChordOff(MIDIChordOn, 100, 5);
//       // Update target MIDI Note On
//         // MIDIChordOn[i] = rootNote + rootChord[i];
//       // Play target note
//         midii->sendChordOn(MIDIChordOn, 100, 5);
//     // Let T1:
    
//       // Turn off previously played notes, if any
//         midii->sendChordOff(MIDIChordOn, 100, 5);
//     default:
//       DBG("WTF");
//   }
// }

/*
noMode is child class as also
1Mode
2Mode
3Mode
4Mode
5Mode
6Mode
7Mode



*/

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