#include "FSM.h"

#include "RC.h"
#include "Buzzer.h"

MODE SYSTEM_MODE;
SWITCH_MODE SYSTEM_SWITCH_MODE;

long prev_gear = true;
long prev_flap = true;

long gear_time = 0;
long flap_time = 0;

//Needs initialization to compile, but will be overwritten
Buzzer* FSMbuzzer;

void initFSM(Buzzer &buz) {
  SYSTEM_MODE = FAILSAFE;
  SYSTEM_SWITCH_MODE = RATE_MANUAL;
  FSMbuzzer = &buz;
}

void runFSM(long t) {
  //bool gear = getGearSwitch();
  //bool flap = getAux();
  //bool toggle_switch_mode = checkSimultaneousPress(t, gear, flap);

  //Serial1.println(toggle_switch_mode);
  updateState(false, getRCStatus(), false);
}


void updateState(bool toggle_switch_mode, bool RC_status, bool gear_status) {
  MODE prev_mode = SYSTEM_MODE;
  if (!RC_status) {
    SYSTEM_MODE = FAILSAFE;
    FSMbuzzer->setBuzzer(1, 500, 750);
    return;
  } else {
    SYSTEM_MODE = RATE_MODE;
  }
  /*
  if (toggle_switch_mode){
    if (SYSTEM_SWITCH_MODE == RATE_MANUAL) {
      SYSTEM_SWITCH_MODE = ANGLE_MANUAL;
    } else {
      SYSTEM_SWITCH_MODE = RATE_MANUAL;
    }
  }
  if (gear_status){
    if (SYSTEM_SWITCH_MODE == RATE_MANUAL) {
      SYSTEM_MODE = RATE_MODE;
    } else {
      SYSTEM_MODE = ANGLE_MODE;
    }
  } else {
    SYSTEM_MODE = MANUAL_MODE;
  }

  if (prev_mode != SYSTEM_MODE){
    switch (SYSTEM_MODE) {
      case FAILSAFE:
        break;
      case MANUAL_MODE:
        FSMbuzzer->setBuzzer(1, 40, 150);
        break;
      case RATE_MODE:
        FSMbuzzer->setBuzzer(2, 40, 150);
        break;
      case ANGLE_MODE:
        FSMbuzzer->setBuzzer(3, 40, 150);
        break;
    }
  }
  */
}

bool checkSimultaneousPress(long t, bool gear, bool flap){
  bool gear_pressed = gear && !prev_gear;
  prev_gear = gear;
  if (gear_pressed) {
    gear_time = t;
    if (gear_time-flap_time < SWITCH_TIME) return true;
  }
  bool flap_pressed = flap && !prev_flap;
  prev_flap = flap;
  if (flap_pressed) {
    flap_time = t;
    if (flap_time-gear_time < SWITCH_TIME) return true;
  }

  return false;
}
