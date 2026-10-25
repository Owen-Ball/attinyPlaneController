#ifndef _IMU_
#define _IMU_

#include <Arduino.h>
#include <Wire.h>
#include "constants.h"
#include "SensorFusion.h"

extern float pitch, roll, pitch_rate, roll_rate, yaw_rate;

void initIMU();
void runIMU(long t);
void calibrateIMU();
void readIMURegisters();

#endif
