#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin){
  pinMode(pin, OUTPUT);
  this->pin = pin;
  this->active = false;
  this->high_time = 0;
  this->low_time = 0;
  this->pulse = 0;
  this->total_pulses = 0; 
}

void Buzzer::setBuzzer(uint8_t total_pulses, long high_time, long low_time){
  if (this->active) return;
  this->total_pulses = total_pulses;
  this->active = true;
  this->high_time = high_time*1000;
  this->low_time = low_time*1000;
  this->pulse = 0;
  this->start_time = micros();
  this->buzzer_state = false;
}

void Buzzer::runBuzzer(long t){
  bool tempvar = (t - this->start_time) % (this->low_time + this->high_time) < this->high_time;
  if (tempvar == this->buzzer_state  || !this->active) return;

  this->buzzer_state = tempvar;
  if (tempvar) {
    if (this->pulse == this->total_pulses && this->total_pulses != 0) {
      this->active = false;
      digitalWrite(this->pin, LOW);
    } else {
      digitalWrite(this->pin, HIGH);
    }
    if (this->pulse != 255) this->pulse++;
  } else {
    digitalWrite(this->pin, LOW);
  }
}

void Buzzer::stopBuzzer(){
  digitalWrite(this->pin, LOW);
  this->active = false;
  this->buzzer_state = false;
}
