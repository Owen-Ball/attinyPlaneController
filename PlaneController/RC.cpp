#include "RC.h"


float elev_a = 2*ELEV_ANGLE/(RC_MAX-RC_MIN);
float elev_b = -2*RC_MIN*ELEV_ANGLE/(RC_MAX-RC_MIN) - ELEV_ANGLE;

float ail_a = 2*AIL_ANGLE/(RC_MAX-RC_MIN);
float ail_b = -2*RC_MIN*AIL_ANGLE/(RC_MAX-RC_MIN) - AIL_ANGLE;

float rud_a = 2*RUD_RATE/(RC_MAX-RC_MIN);
float rud_b = -2*RC_MIN*RUD_RATE/(RC_MAX-RC_MIN) - RUD_RATE;

int packet_count;
int prev_count;

static long prev_time;

uint8_t inData[16];
uint8_t inByte;

bool RC_connected;
HardwareSerial *rc_serial;


uint16_t limitChannel(uint16_t val){
  if (val < RC_MIN) return RC_MIN;
  if (val > RC_MAX) return RC_MAX;
  return val;
}


void initRC(HardwareSerial &port){
  rc_serial = &port;
  prev_count = 0;
  RC_connected = false;
  for (uint8_t i = 0; i <= 15; i++){
    inData[i] = 0;
  }
}

void runRC(long t){
  packet_count = rc_serial->available();
  if (prev_count == packet_count) {
     for (int i = 0; i < (packet_count % 16); i++) rc_serial->read();
  }
  prev_count = packet_count;
  while (rc_serial->available() >= 16){
    for (uint8_t i = 0; i <= 15; i++){
      inByte = rc_serial->read();
      inData[i] = inByte;
    }
    prev_time = t;
  }
  RC_connected = (t-prev_time) < 50000;

  /*
  for (int i = 0; i <= 15; i++){
    rc_serial->print(inData[i]);
    rc_serial->print("\t");
  }
  rc_serial->print(RC_connected);
  rc_serial->print("\t");
  rc_serial->println();
  */
}

float getElevator(){
  uint16_t val = (((uint16_t)inData[6]-8)<<8) + (uint16_t)inData[7];
  val = limitChannel(val);
  return -elev_a*val - elev_b;
}

float getAileron(){
  uint16_t val = (((uint16_t)inData[2]-4)<<8) + (uint16_t)inData[3];
  val = limitChannel(val);
  return ail_a*val + ail_b;
}

float getRudder(){
  uint16_t val = (((uint16_t)inData[10]-12)<<8) + (uint16_t)inData[11];
  val = limitChannel(val);
  return rud_a*val + rud_b;
}

uint8_t getThrottle(){
  uint16_t val = ((uint16_t)inData[14]<<8) + (uint16_t)inData[15];
  val = limitChannel(val);
  val = (val-RC_MIN)*(256 - MIN_THROTTLE)/(RC_MAX-RC_MIN) + MIN_THROTTLE;
  return (val > 255) ? 255 : val;
}

bool getAux(){
  return inData[4]==20;
}

bool getGearSwitch(){
  return inData[8]==19;
}

bool getRCStatus(){
  return RC_connected;
}
