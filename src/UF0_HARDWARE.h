#ifndef UF0_HARDWARE_h
#define UF0_HARDWARE_h

// Preliminary
  // LED
    #include <Adafruit_NeoPixel.h>
  // Joystick
    #include <ezButton.h>
  // BUTTON pinZ
    #define BUTTON_ON_OFF       15
    // #define BUTTON_VIRTUAL_GRID 15
    #define BUTTON_SOLO         2
    #define BUTTON_YAW_RESET    13
    // #define BUTTON_TOGGLE_MOUSE 15
    // #define BUTTON_SCALE        13
    #define BUTTON_CHORD        2
    // #define BUTTON_ROOT         15
    bool BUTTON_ON_OFF_state, BUTTON_ON_OFF_state_past;
    // bool BUTTON_VIRTUAL_GRID_state, BUTTON_VIRTUAL_GRID_state_past;
    bool BUTTON_SOLO_state, BUTTON_SOLO_state_past;
    bool BUTTON_YAW_RESET_state, BUTTON_YAW_RESET_state_past;
    // bool BUTTON_TOGGLE_MOUSE_state, BUTTON_TOGGLE_MOUSE_state_past;
    // bool BUTTON_SCALE_state, BUTTON_SCALE_state_past;
    bool BUTTON_CHORD_state, BUTTON_CHORD_state_past;
    // bool BUTTON_ROOT_state, BUTTON_ROOT_state_past;
  // BUTTON modeZ
    #define NO_MODE          -1
    #define ON_OFF_MODE       0
    #define VIRTUAL_GRID_MODE 1
    #define SOLO_MODE         2
    #define YAW_RESET_MODE    3
    #define MOUSE_MODE        4
    #define SCALE_MODE        5
    #define CHORD_MODE        6
    #define ROOT_MODE         7
  
  // TRIGGERZ
    #define TRIGGER_A 32  // R
    #define TRIGGER_B 33  // G
    #define TRIGGER_C 25  // B
    bool TRIGGER_A_STATE, TRIGGER_B_STATE, TRIGGER_C_STATE,
         TRIGGER_A_STATE_past, TRIGGER_B_STATE_past, TRIGGER_C_STATE_past;
  
  // JOYSTICK
    #define JOYSTICK_Y  36  // ESP32 pin GPIO36 (ADC0) connected to VRX pin
    #define JOYSTICK_X  39 // ESP32 pin GPIO39 (ADC0) connected to VRY pin
    #define JOYSTICK_CLICK   16 // ESP32 pin GPIO17 connected to SW  pin
    ezButton button(JOYSTICK_CLICK);
    // Joystick values
    int joystickX, joystickY, joystickButton;

  // PHOTONIC DISCHARGE DEVICE aka LED
    #define LED_FPS  20
    #define LED_STRIP  4   // Yellow
    #define NUM_PIXELS 8
    Adafruit_NeoPixel ws2812b(NUM_PIXELS, LED_STRIP, NEO_GRB + NEO_KHZ800);
    bool toggleLED = false;
    int debounceTime = 30;
    #define BATTERY_FULL 1
    #define BATTERY_HALF 2
    #define BATTERY_LOW 3
    #define BATTERY_CRITICAL 4
  // -1 = No Mode, 0 = First Mode, 1 = Y Mode, 2 = Z Mode
    int btnMode = NO_MODE;
//

////////////// B U T T O N S //////////////
class UF0_BUTTON{
  private:
    UF0_MIDI* midii;

    void setup_buttons(){
      pinMode(BUTTON_ON_OFF, INPUT_PULLUP);
      // pinMode(BUTTON_VIRTUAL_GRID, INPUT_PULLUP);
      pinMode(BUTTON_SOLO, INPUT_PULLUP);
      pinMode(BUTTON_YAW_RESET, INPUT_PULLUP);
      // pinMode(BUTTON_TOGGLE_MOUSE, INPUT_PULLUP);
      // pinMode(BUTTON_SCALE, INPUT_PULLUP);
      pinMode(BUTTON_CHORD, INPUT_PULLUP);
      // pinMode(BUTTON_ROOT, INPUT_PULLUP);
    }

  public:

    UF0_BUTTON(){}

    UF0_BUTTON(UF0_MIDI *midiii){
      midii = midiii;
      setup_buttons();
    }
    void loop(){
    // Update btn states
      BUTTON_ON_OFF_state = !digitalRead(BUTTON_ON_OFF);
      // BUTTON_VIRTUAL_GRID_state = !digitalRead(BUTTON_VIRTUAL_GRID);
      BUTTON_SOLO_state = !digitalRead(BUTTON_SOLO);
      BUTTON_YAW_RESET_state = !digitalRead(BUTTON_YAW_RESET);
      // BUTTON_TOGGLE_MOUSE_state = !digitalRead(BUTTON_TOGGLE_MOUSE);
      // BUTTON_SCALE_state = !digitalRead(BUTTON_SCALE);
      BUTTON_CHORD_state = !digitalRead(BUTTON_CHORD);
      // BUTTON_ROOT_state = !digitalRead(BUTTON_ROOT);

    // Button do:
    // Hold BUTTON_ON_OFF:
      if (BUTTON_ON_OFF_state == HIGH && BUTTON_ON_OFF_state_past == LOW){
        startSleepTimer();
      }
    // Let BUTTON_ON_OFF
      if (BUTTON_ON_OFF_state == LOW && BUTTON_ON_OFF_state_past == HIGH){
        resetSleepTimer();
      }
    // Hold BUTTON_SOLO:
      if (BUTTON_SOLO_state == HIGH){
        btnMode = SOLO_MODE;
      }
    // Bang BUTTON_YAW_RESET:
      if (BUTTON_YAW_RESET_state == HIGH && BUTTON_YAW_RESET_state_past == LOW){
        resetHeading();
      }

    // Update past btn states
      BUTTON_ON_OFF_state_past = BUTTON_ON_OFF_state;
      // BUTTON_VIRTUAL_GRID_state_past = BUTTON_VIRTUAL_GRID_state;
      BUTTON_SOLO_state_past = BUTTON_SOLO_state;
      BUTTON_YAW_RESET_state_past = BUTTON_YAW_RESET_state;
      // BUTTON_TOGGLE_MOUSE_state_past = BUTTON_TOGGLE_MOUSE_state;
      // BUTTON_SCALE_state_past = BUTTON_SCALE_state;
      BUTTON_CHORD_state_past = BUTTON_CHORD_state;
      // BUTTON_ROOT_state_past = BUTTON_ROOT_state;
      // BUTTON_Z_state_past = BUTTON_Z_state;
    }
};

///////////// T R I G G E R S /////////////
class UF0_TRIGGER{
  private:
    UF0_MIDI* midii;

    void setup_triggers(){
      pinMode(TRIGGER_A, INPUT_PULLUP);
      pinMode(TRIGGER_B, INPUT_PULLUP);
      pinMode(TRIGGER_C, INPUT_PULLUP);
    }

  public:
    UF0_TRIGGER(){}

    UF0_TRIGGER(bool cmon){
      setup_triggers();
    }

    void loop(){
    // Update trigger states
      TRIGGER_A_STATE = !digitalRead(TRIGGER_A);
      TRIGGER_B_STATE = !digitalRead(TRIGGER_B);
      TRIGGER_C_STATE = !digitalRead(TRIGGER_C);
    //Check if any MOD buttons are pressed
      switch(btnMode){
      // Trigger do:
        //////////////// N O    M O D E //////////////
        case NO_MODE:
        // Hold T1:
          if (TRIGGER_A_STATE == HIGH && TRIGGER_A_STATE_past == LOW){
          // Turn off previously played note, if any
            midii->sendMIDINoteOff(MIDINoteOn, 100, 5);
          // Update root MIDI Note On
            MIDINoteOn = rootNote;
          // Play root note
            midii->sendMIDINoteOn(MIDINoteOn, 100, 5);
          }
        // Let T1:
          if (TRIGGER_A_STATE == LOW && TRIGGER_A_STATE_past == HIGH){
          // Turn off previously played note, if any
            midii->sendMIDINoteOff(MIDINoteOn, 100, 5);
          }
        // Hold T2:
          if (TRIGGER_B_STATE == HIGH && TRIGGER_B_STATE_past == LOW){
            midii->enableControl();
          }
        // Let T2:
          if (TRIGGER_B_STATE == LOW && TRIGGER_B_STATE_past == HIGH){
            midii->disableControl();
          }
        // Hold TF:
          if (TRIGGER_C_STATE == HIGH && TRIGGER_C_STATE_past == LOW){
            // open function
          }
        // Let TF:
          if (TRIGGER_C_STATE == LOW && TRIGGER_C_STATE_past == HIGH){
            // close function
          }
          break;
        //////////////// S O L O   M O D E ///////////////
        case SOLO_MODE:
          if (TRIGGER_A_STATE == HIGH && TRIGGER_A_STATE_past == LOW){
            midii->toggleTransmission(&pitchTransmit);
          }
          if (TRIGGER_B_STATE == HIGH && TRIGGER_B_STATE_past == LOW){
            midii->toggleTransmission(&rollTransmit);
          }
          if (TRIGGER_C_STATE == HIGH && TRIGGER_C_STATE_past == LOW){
            midii->toggleTransmission(&yawTransmit);
          }
          break;
        /////////////// C H O R D   M O D E /////////////////
        case CHORD_MODE:
        // Hold T1:
          if (TRIGGER_A_STATE == HIGH && TRIGGER_A_STATE_past == LOW){
          // Turn off previously played notes, if any
            midii->sendChordOff(MIDIChordOn, 100, 5);
          // Update target MIDI Note On
            // MIDIChordOn[i] = rootNote + rootChord[i];
          // Play target note
            midii->sendChordOn(MIDIChordOn, 100, 5);
          }
        // Let T1:
          if (TRIGGER_A_STATE == LOW && TRIGGER_A_STATE_past == HIGH){
          // Turn off previously played notes, if any
            midii->sendChordOff(MIDIChordOn, 100, 5);
          }
        default:
          DBG("WTF");
      }

      // Update old trigger states
      TRIGGER_A_STATE_past = TRIGGER_A_STATE;
      TRIGGER_B_STATE_past = TRIGGER_B_STATE;
      TRIGGER_C_STATE_past = TRIGGER_C_STATE;
    }
};

////////////// J O Y S T I C K ////////////
class UF0_JOYSTICK{
  private:
    // Timers
    unsigned long JOYSTICK_t0 = millis();
    UF0_MIDI* midii;

    void setup_joystick(){
      button.setDebounceTime(debounceTime);
    }

  public:
    UF0_JOYSTICK(){}

    UF0_JOYSTICK(bool cmon){
      setup_joystick();
    }
    
    void loop(){
      button.loop(); // MUST call the loop() function first
      // read X and Y analog values
      joystickX = analogRead(JOYSTICK_X);
      joystickY = analogRead(JOYSTICK_Y);
      // Read the button value
      joystickButton = button.getState();

      if (button.isPressed()) {
        Serial.println("The button is pressed");
        // TODO do something here
      }

      if (button.isReleased()) {
        Serial.println("The button is released");
        // TODO do something here
      }

      if ((millis() - JOYSTICK_t0) > monitorHz){
        // DBG("x = ", joystickX,", y = ", joystickY, "button = ", joystickButton);
        JOYSTICK_t0 = millis();
      }
    }
};

////////////////// L E D //////////////////
class UF0_LED{
  private:
  UF0_MIDI midii;
  
    void setup_LED(UF0_MIDI midii){
      // this.midii = midii;
      ws2812b.begin();
    }
  // Timers
  unsigned long LED_FPS_timer = millis(), LED_t0 = millis();

  public:
    UF0_LED(){}

    UF0_LED(UF0_MIDI midii){
      setup_LED(midii);
    }

    void loop(){
      ws2812b.clear();
      ws2812b.show();
      if ((millis() - LED_FPS_timer) > LED_FPS){
        ws2812b.clear();

        if (pitchTransmit)
          ws2812b.setPixelColor(4, ws2812b.Color(153, 0, 255));
        if (rollTransmit)
          ws2812b.setPixelColor(3, ws2812b.Color(153, 0, 255));
        if (yawTransmit)
          ws2812b.setPixelColor(2, ws2812b.Color(153, 0, 255));
        if (isConnected)
          // 0 255 0 = GREEN
          ws2812b.setPixelColor(7, ws2812b.Color(0, 255, 0));
        else{
          if ((millis() - LED_t0) > 200){
            ws2812b.setPixelColor(7, ws2812b.Color(0, (toggleLED * 255), 0));
            toggleLED = !toggleLED;
            LED_t0 = millis();
          }
        }
        ws2812b.setBrightness(10);
        ws2812b.show();
      }
    }

    void openTargetLED(int targetLED, int targetLEDColorR, int targetLEDColorG, int targetLEDColorB, int targetLEDBrightness){
      // ws2812b.clear();
      ws2812b.setPixelColor(targetLED, ws2812b.Color(targetLEDColorR, targetLEDColorG, targetLEDColorB));  // it only takes effect if pixels.show() is called
      ws2812b.setBrightness(targetLEDBrightness);
      ws2812b.show();
    }

    void openAllLEDs(){
      ws2812b.clear();
      for (int i=0; i<10; i++){
        ws2812b.setPixelColor(i, ws2812b.Color(153, 0, 255));  // it only takes effect if pixels.show() is called
        ws2812b.setBrightness(10);
      }
      ws2812b.show();
    }

    void closeAllLEDs(){
      ws2812b.clear();
      ws2812b.show();
    }

    void batteryMonitor(){
      // Move to power header file
      int batteryLevel = 0;
      switch(batteryLevel){
        case 61 ... 100:
          batteryLevel = BATTERY_FULL;
          break;
        case 31 ... 60:
          batteryLevel = BATTERY_HALF;
          break;
        case 11 ... 30:
          batteryLevel = BATTERY_LOW;
          break;
        case 0 ... 10:
          batteryLevel = BATTERY_CRITICAL;
          break;
        default:
          batteryLevel = -1;
      }
      switch(batteryLevel){
      // Trigger do:
        case BATTERY_FULL:
          openTargetLED(1, 0, 255, 0, 10);
          break;
        case BATTERY_HALF:
          openTargetLED(1, 0, 122, 122, 10);
          break;
        case BATTERY_LOW:
          openTargetLED(1, 85, 85, 85, 10);
          break;
        case BATTERY_CRITICAL:
          openTargetLED(1, 255, 0, 0, 10);
          break;
        default:
          openTargetLED(1, 0, 0, 0, 10);
      }
    }

    void celebrate(){
      // Yoo!!
      openAllLEDs();
      delay(100);
      closeAllLEDs();
      delay(100);
      openAllLEDs();
      delay(100);
      closeAllLEDs();
      delay(100);
    }
};

#endif