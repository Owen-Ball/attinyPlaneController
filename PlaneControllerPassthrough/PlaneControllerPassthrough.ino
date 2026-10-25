#include <Arduino.h>
#include "PID.h"
#include "Buzzer.h"
#include "RC.h"
#include "IMU.h"
#include "helpers.h"
#include "Motors.h"
#include "FSM.h"
#include "Servo_inverted.h"
//#include <Servo_megaTinyCore.h>

long t;
long loop_time;
long prev_time;

//PID elev(1, .5, .175);
//PID ail(1, .5, .175);
PID elev(.8, .5, .125);
PID ail(.8, .5, .125);
PID rud(2, 0, 0);

Buzzer buzzer(BUZ_PIN);

bool armed = false;

Servo elevator_servo;
Servo aileron_servo;
Servo rudder_servo;
Servo throttle_servo;
Servo aux_servo;
Servo gear_servo;

void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);

  analogReference(INTERNAL2V5);

  initRC(Serial1);
  
  initIMU();
  initFSM(buzzer);

  prev_time = micros();
  
  throttle_servo.attach(PIN_PA6);
  elevator_servo.attach(PIN_PB5);
  aileron_servo.attach(PIN_PA7);
  rudder_servo.attach(PIN_PB4);
  aux_servo.attach(PIN_PC0);
  gear_servo.attach(PIN_PB3);

  delay(3000);

  
}

float limitFloat(float val) {
  if (val > 2000.0) return 2000.0;
  if (val < 1000.0) return 1000.0;
  return val;
}

void loop() {
  t = micros();
  loop_time = t - prev_time;
  prev_time = t;
  runRC(t);
  runFSM(t);
  buzzer.runBuzzer(t);

  runIMU(loop_time);
  //Serial1.print(getThrottle());
  //Serial1.print(" ");
  //Serial1.println(getAileron());
  if (SYSTEM_MODE == FAILSAFE) {
    throttle_servo.writeMicroseconds(1000);
    elevator_servo.writeMicroseconds(1500);
    aileron_servo.writeMicroseconds(1500);
    rudder_servo.writeMicroseconds(1500);
    aux_servo.writeMicroseconds(1500);
    gear_servo.writeMicroseconds(1500);
    
  } else {
    throttle_servo.writeMicroseconds(getThrottle());
    elevator_servo.writeMicroseconds(reverseChannel(getElevator()));
    aileron_servo.writeMicroseconds(limitFloat(reverseChannel(getAileron()) - 300*getAux()));
    rudder_servo.writeMicroseconds(reverseChannel(getRudder()));
    aux_servo.writeMicroseconds(limitFloat(reverseChannel(getAileron()) + 300*getAux()));
    gear_servo.writeMicroseconds(1500);
  }
}
