#include <Arduino.h>
#include "PID.h"

static float loop_time;
float inv_loop_time;

void updatePIDLoopTime(long t){
  loop_time = (float)t;
  inv_loop_time = 1.0/loop_time;
}

PID::PID(float KP, float KI, float KD){
  this->KP = KP;
  this->KI = KI/1000000.0;
  this->KD = KD*1000000.0;
  this->cum_error = 0.0;
  this->prev_error = 0.0;
}

float PID::updatePID(float sensor_val, float target){
  float error = target - sensor_val;
  this->cum_error += loop_time*error;
  rate_error = (error - this->prev_error)*inv_loop_time; 
  this->prev_error = error;
  return (this->KP)*error + (this->KI)*(this->cum_error) + (this->KD)*rate_error;
}

void PID::resetIntegral(){
  this->cum_error = 0;
}
