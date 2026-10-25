#ifndef _MOTORS_
#define _MOTORS_

#include <Arduino.h>
#include "constants.h"


void initMotors();

void setMotors(uint8_t throttle, float pitch_control, float roll_control, float yaw_control);

void killMotors();

#endif
