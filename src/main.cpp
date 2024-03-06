/* ````~~~~~~_____%%%%%%   Hi!    %%%%%%_____~~~~~~````

  What works:
    Buttons
      Shift/Fn key (solo key)
      Toggle transmission
    Triggerz
      send MIDI note
      send MIDI chord
    Joystick*
      Click (unasigned)
    LED*
    Gyro
    Wi-Fi
    Web Server
    MIDI over Wi-Fi
    Preset Save/Load
    Virtual Grid
    Drum Hit

  What does'nt:
    * joystick XY (broken hardware)
    * broken hardware

  TO - DOs:
    v0.5:
      On / Off the webServer when connecting to LAN

      inheritances in preset_system.h

      try pairing!

      calculate battery percentage?

      try pico wifi HID -> try library for esp wifi HID?
      solder + test joystick mouse with pico

      Send auto-connect to rtpMIDI? (check other example with ws500, mail Thobias?)

    LED:
      Ambient light presets
      When saving preset blink LED & press continue
    HW:
      Fix an (ideally) WiFi Joystick mouse
    Network:
      design controlPage HTML
      make API for HTML page
      redirect from 192.168.4.1/ -> 192.168.1.158/jamm~! (in one button -> Connect!)
    MISC:
      invent new notes! (name, freq/pitch)
      abstract & polymorph & inheritance? (UF0_MIDI extends APPLEMIDI?)
    XRTS:
      xtaskcreate -> loopbutton core 1 (task ex.)
      task no affinity
      semaphorez give/take
    Shell + stuff:
      TRANSLUCID MATERIAL FOR AMBIENT LIGHTNING
      Controller Stand
*/

// Preliminary
  #include <Arduino.h>
  // Enterprise
    #define UF0_DBG Serial
    #include <UF0_DBG.h>
    #include "UF0_MUSIC_THEORY.h"
    #include "UF0_PRESET_SYSTEM.h"
    #include "UF0_FSM.h"
    #include "UF0_GYRO.h"
    #include "UF0_NETWORK\UF0_NETWORK.h"
    #include "UF0_MIDI.h"
    #include "UF0_BLACKMAGIC.h"
    #include "UF0_HARDWARE.h"

    
  // Instances
    UF0_BUTTON buttonDriver;
    UF0_TRIGGER triggerDriver;
    UF0_JOYSTICK joystickDriver;
    UF0_LED ledDriver;
    UF0_GYRO gyroDriver;
    UF0_WiFi WiFiDriver;
    UF0_WEBSERVER serverDriver;
    UF0_MIDI midiDriver;
    UF0_FSM fsmDriver;
    UF0_BLACKMAGIC blackmagic;
//

// unsigned long OS_timer{0};
// int OS_clock{10};

void setup(){
  DBG_SETUP(115200);
  delay(1000);
  DBG(F("===================== Das Booting ==================="));
  init();
  DBG(F("===================== BOOT BUTTONS ==================="));
  UF0_BUTTON buttonDriver(&midiDriver);
  DBG(F("===================== BOOT TRIGGERS ==================="));
  UF0_TRIGGER triggerDriver(true);
  DBG(F("===================== BOOT JOYSTICK ==================="));
  UF0_JOYSTICK joystickDriver(true);
  DBG(F("===================== BOOT LED ==================="));
  UF0_LED ledDriver;
  DBG(F("===================== BOOT WEBSERVER ==================="));
  UF0_WEBSERVER serverDriver(&midiDriver, &blackmagic);
  DBG(F("===================== BOOT WIFI ==================="));
  UF0_WiFi WiFiDriver(true);
  DBG(F("===================== BOOT GYRO ==================="));
  UF0_GYRO gyroDriver(true);
  DBG(F("===================== BOOT MIDI ==================="));
  UF0_MIDI midiDriver(&gyroDriver);
  DBG(F("===================== BOOT FSM ==================="));
  UF0_FSM fsmDriver(true);
  DBG(F("===================== BOOT BM ==================="));
  UF0_BLACKMAGIC blackmagic(true);
  DBG(F("===================== BOOT COMPLETE ==================="));
  ledDriver.celebrate();
}

void loop(){
  // if ((millis() - OS_timer) > OS_clock){
    buttonDriver.loop();
    triggerDriver.loop();
    joystickDriver.loop();
    ledDriver.loop();
    midiDriver.loop();
    gyroDriver.loop();
    fsmDriver.loop();
    blackmagic.loop();
    // OS_timer = millis();
  // }
}