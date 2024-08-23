// Preliminary
  #include <Arduino.h>
  // Enterprise
    #define UF0_DBG Serial
    #include <UF0_OS/UF0_DBG.h>
    #include <UF0_OS/UF0_FSM.h>
    #include <UF0_HARDWARE/UF0_GYRO.h>
    #include <UF0_NETWORK/UF0_NETWORK.h>
    #include <UF0_MUSIC/UF0_MIDI.h>
    #include <UF0_OS/UF0_BLACKMAGIC.h>
    #include <UF0_HARDWARE/UF0_HARDWARE.h>

    
  // Instances
    UF0_TRIGGER triggerDriver;
    UF0_POWER powerHandler;
    UF0_GYRO gyroDriver;
    UF0_WiFi WiFiDriver;
    UF0_WEBSERVER serverDriver;
    UF0_MIDI midiDriver;
    UF0_FSM fsmDriver;
    UF0_BLACKMAGIC blackmagic;
//

void setup(){
  DBG_SETUP(115200);
  delay(1000);
  DBG(F("===================== Das Booting ==================="));
  init();
  DBG(F("===================== BOOT TRIGGERS ==================="));
  UF0_TRIGGER triggerDriver(true);
  DBG(F("===================== BOOT WEBSERVER ==================="));
  UF0_WEBSERVER serverDriver(&powerHandler, &midiDriver, &blackmagic);
  DBG(F("===================== BOOT WIFI ==================="));
  UF0_WiFi WiFiDriver(true);
  DBG(F("===================== BOOT GYRO ==================="));
  UF0_GYRO gyroDriver(true);
  DBG(F("===================== BOOT MIDI ==================="));
  UF0_MIDI midiDriver(true);
  DBG(F("===================== BOOT FSM ==================="));
  UF0_FSM fsmDriver(true);
  DBG(F("===================== BOOT BM ==================="));
  UF0_BLACKMAGIC blackmagic(true);
  DBG(F("===================== BOOT COMPLETE ==================="));
}

void loop(){
  triggerDriver.loop();
  gyroDriver.loop();
  midiDriver.loop();
  fsmDriver.loop();
  blackmagic.loop();
}