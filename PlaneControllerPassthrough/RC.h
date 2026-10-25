#ifndef _RC_
#define _RC_

#include <Arduino.h>
#include "constants.h"

void initRC(HardwareSerial &port);
void runRC(long t);

float reverseChannel(float val);

float getElevator();
float getAileron();
float getRudder();
float getThrottle();
bool getAux();
bool getGearSwitch();


bool getRCStatus();

#endif
