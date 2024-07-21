// Preliminary
  #include <Arduino.h>
  // Enterprise
    #define UF0_DBG Serial
    #include <UF0_OS/UF0_DBG.h>
    #include <UF0_MUSIC/UF0_PRESET_SYSTEM.h>
    #include <UF0_OS/UF0_FSM.h>
    #include <UF0_HARDWARE/UF0_GYRO.h>
    #include <UF0_NETWORK/UF0_NETWORK.h>
    #include <UF0_NETWORK/UF0_WEBSITE_API.h>
    #include <UF0_MUSIC/UF0_MIDI.h>
    #include <UF0_OS/UF0_BLACKMAGIC.h>
    #include <UF0_HARDWARE/UF0_HARDWARE.h>
    #include <UF0_NETWORK/UF0_WEBSITE_API.h>

    
  // Instances
    UF0_PRESET_HANDLER presetHandler;
    UF0_BUTTON buttonDriver;
    UF0_TRIGGER triggerDriver;
    UF0_JOYSTICK joystickDriver;
    UF0_LED_handler ledHandler;
    UF0_POWER powerHandler;
    UF0_GYRO_2 gyroDriver_2;
    // UF0_GYRO gyroDriver;
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
  // DBG(F("===================== BOOT PRESETS ==================="));
  // UF0_PRESET_HANDLER presetHandler(true);
  DBG(F("===================== BOOT BUTTONS ==================="));
  UF0_BUTTON buttonDriver(&midiDriver);
  DBG(F("===================== BOOT TRIGGERS ==================="));
  UF0_TRIGGER triggerDriver(true);
  DBG(F("===================== BOOT JOYSTICK ==================="));
  UF0_JOYSTICK joystickDriver(true);
  DBG(F("===================== BOOT LED ==================="));
  UF0_LED_handler ledHandler(true);
  DBG(F("===================== BOOT POWER ==================="));
  UF0_POWER powerHandler(true);
  DBG(F("===================== BOOT WEBSERVER ==================="));
  DBG("Batter %:", powerHandler.get_battery_percent());
  UF0_WEBSERVER serverDriver(&powerHandler, &midiDriver, &blackmagic);
  DBG(F("===================== BOOT WIFI ==================="));
  UF0_WiFi WiFiDriver(true);
  DBG(F("===================== BOOT ICM GYRO ==================="));
  UF0_GYRO_2 gyroDriver_2(true);
  // DBG(F("===================== BOOT GYRO ==================="));
  // UF0_GYRO gyroDriver(true);
  DBG(F("===================== BOOT MIDI ==================="));
  UF0_MIDI midiDriver(true);
  DBG(F("===================== BOOT FSM ==================="));
  UF0_FSM fsmDriver(true);
  DBG(F("===================== BOOT BM ==================="));
  UF0_BLACKMAGIC blackmagic(true);
  DBG(F("===================== BOOT COMPLETE ==================="));
  ledHandler.celebrate();
  // WebsiteAPI api;
  // api.setupWebSite();
  // DBG(api.readHTML());
}

void loop(){
  // presetHandler.loop();
  buttonDriver.loop();
  triggerDriver.loop();
  joystickDriver.loop();
  ledHandler.loop();
  midiDriver.loop();
  // this is ICM
  gyroDriver_2.loop();
  // this is BNO
  // gyroDriver.loop();
  fsmDriver.loop();
  blackmagic.loop();
}