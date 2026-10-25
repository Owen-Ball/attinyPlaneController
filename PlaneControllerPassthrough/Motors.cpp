#include "Motors.h"



void initMotors(){
  TCA0.SPLIT.LPER   = 254;
  TCA0.SPLIT.HPER   = 254;
  TCA0.SPLIT.CTRLA  = TCA_SPLIT_CLKSEL_DIV4_gc | TCA_SPLIT_ENABLE_bm;
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(MOTOR3, OUTPUT);
  pinMode(MOTOR4, OUTPUT);
}


void setMotors(uint8_t throttle, float pitch_control, float roll_control, float yaw_control){

  int m1 = throttle;
  int m2 = throttle;
  int m3 = throttle;
  int m4 = throttle;

  m1 += (int)(-roll_control + pitch_control + yaw_control);
  m2 += (int)(-roll_control - pitch_control - yaw_control);
  m3 += (int)(roll_control + pitch_control - yaw_control);
  m4 += (int)(roll_control - pitch_control + yaw_control);

  int max_motor = max(max(m1, m2), max(m3, m4));
  int min_motor = min(min(m1, m2), min(m3, m4));
  int down_shift = max(max_motor - 255, 0);
  int up_shift = max(MIN_THROTTLE - min_motor + down_shift, 0);
  int shift = up_shift - down_shift;
  m1 += shift;
  m2 += shift;
  m3 += shift;
  m4 += shift;
  if (m1 > 255) m1 = 255;
  if (m2 > 255) m2 = 255;
  if (m3 > 255) m3 = 255;
  if (m4 > 255) m4 = 255;

  analogWrite(MOTOR1, (uint8_t)m1);
  analogWrite(MOTOR2, (uint8_t)m2);
  analogWrite(MOTOR3, (uint8_t)m3);
  analogWrite(MOTOR4, (uint8_t)m4);
    
}

void killMotors(){
  analogWrite(MOTOR1, 0);
  analogWrite(MOTOR2, 0);
  analogWrite(MOTOR3, 0);
  analogWrite(MOTOR4, 0);
}
