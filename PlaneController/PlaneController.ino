#include <Arduino.h>
#include "PID.h"
#include "Buzzer.h"
#include "RC.h"
#include "IMU.h"
#include "helpers.h"
#include "Motors.h"
#include "FSM.h"

long t;
long loop_time;
long prev_time;

//PID elev(1, .5, .175);
//PID ail(1, .5, .175);
PID elev(.8, .5, .125);
PID ail(.8, .5, .125);
PID rud(2, 0, 0);

Buzzer buzzer(BUZ_PIN);
//Buzzer buzzer(PIN_PA6);

bool armed = false;


void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);

  analogReference(INTERNAL2V5);
  pinMode(BAT_PIN, INPUT);

  initRC(Serial1);
  
  initIMU();
  initFSM(buzzer);

  prev_time = micros();
}

void loop() {
  t = micros();
  loop_time = t - prev_time;
  prev_time = t;
  runRC(t);
  runFSM(t);
  buzzer.runBuzzer(t);

  runIMU(loop_time);
  Serial1.println("hi");
}
