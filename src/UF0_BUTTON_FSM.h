#include <FSM.h>

// State machine variables
#define TOGGLE_BUTTON 1
#define HOLD_BUTTON 2

void on_button_on_enter(){

}

void on_button_on_exit(){
  
}

void on_button_hold_on_enter(){

}

void on_button_hold_on_exit(){
  
}

void off_button_on_enter(){
  
}

void off_button_on_exit(){
  
}

void off_button_hold_on_enter(){
  
}

void off_button_hold_on_exit(){
  
}

State state_button_on(on_button_on_enter, NULL, &on_button_on_exit);
State state_button_on_hold(on_button_hold_on_enter, NULL, &on_button_hold_on_exit);
State state_button_off(on_button_off_enter, NULL, &on_button_off_exit);
State state_button_off_hold(on_button_hold_off_enter, NULL, &on_button_hold_off_exit);
FSM UF0_HARDWARE_FSM(&state_button_off);

void setup_FSM(){

  //////////// T O G G L E /////////////

  UF0_HARDWARE_FSM.add_transition(&state_button_on,
                     &state_button_on_hold,
                     TOGGLE_BUTTON,
                     NULL);

  UF0_HARDWARE_FSM.add_transition(&state_button_on_hold,
                     &state_button_off,
                     TOGGLE_BUTTON,
                     NULL);

  UF0_HARDWARE_FSM.add_transition(&state_button_off,
                     &state_button_off_hold,
                     TOGGLE_BUTTON,
                     NULL);

  UF0_HARDWARE_FSM.add_transition(&state_button_off_hold,
                     &state_button_on,
                     TOGGLE_BUTTON,
                     NULL);

  ///////////// H O L D /////////////

  UF0_HARDWARE_FSM.add_transition(&state_holdButton_on,
                     &state_holdButton_off,
                     HOLD_BUTTON,
                     NULL);

  UF0_HARDWARE_FSM.add_transition(&state_holdButton_off,
                     &state_holdButton_on,
                     HOLD_BUTTON,
                     NULL);
}

void trigggger(){
  bool old_state=0;
  bool new_state=0;
  if (old_state!=new_state){
    UF0_HARDWARE_FSM.trigger(TOGGLE_BUTTON);
    UF0_HARDWARE_FSM.trigger(HOLD_BUTTON);
  }
}