#ifndef _FSM_
#define _FSM_

#include "Buzzer.h"
#include "constants.h"

extern MODE SYSTEM_MODE;
extern SWITCH_MODE SYSTEM_SWITCH_MODE;

void initFSM(Buzzer &buz);

void runFSM(long t);

void updateState(bool toggle_switch_mode, bool RC_status, bool gear_status);

bool checkSimultaneousPress(long t, bool gear, bool flap);


#endif // _FSM_
