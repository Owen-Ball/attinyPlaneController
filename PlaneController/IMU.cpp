#include "IMU.h"

SF fusion;

float uncal_pitch, uncal_roll;
float pitch_offset = 1.75;
float roll_offset = -0.9;
float pitch, roll, pitch_rate, roll_rate, yaw_rate;
int16_t AX_raw,AY_raw,AZ_raw,GX_raw,GY_raw,GZ_raw,tmp;
float AX,AY,AZ,GX,GY,GZ;

void initIMU(){
  Wire.begin();
  Wire.setClock(400000L);
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Wire.beginTransmission(IMU_ADDR);
  Wire.write(0x1A);
  Wire.write(B00000000); 
  Wire.endTransmission(true);
  
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(0x1B);  // Gyro and accel config registers
  Wire.write(B00010000);     // +-1000deg/s mode
  Wire.write(B00001000);     // +-4g mode
  Wire.endTransmission(true);
  
  pitch = 0;
  roll = 0;
  pitch_rate = 0;
  roll_rate = 0;
  yaw_rate = 0;

}

void runIMU(long t){
  readIMURegisters();
  //Serial1.println(((2.0*(AX_raw - AX_MIN))/float((AX_MAX-AX_MIN)))-1.0);

  //yaw_rate = IMU.gyrZ();
  //fusion.MahonyUpdate(IMU.gyrX()*DEG_TO_RAD, IMU.gyrY()*DEG_TO_RAD, IMU.gyrZ()*DEG_TO_RAD, -IMU.accX(), -IMU.accY(), -IMU.accZ(), t/1000000.0);

  //uncal_pitch = fusion.getRoll();
  //uncal_roll = fusion.getPitch();

  pitch = uncal_pitch - pitch_offset;
  roll = uncal_roll - roll_offset;

}

void readIMURegisters(){
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(IMU_ADDR,14,true);
  AX_raw=Wire.read()<<8|Wire.read();
  AY_raw=Wire.read()<<8|Wire.read();
  AZ_raw=Wire.read()<<8|Wire.read(); 
  tmp=Wire.read()<<8|Wire.read(); 
  GX_raw=Wire.read()<<8|Wire.read();  
  GY_raw=Wire.read()<<8|Wire.read(); 
  GZ_raw=Wire.read()<<8|Wire.read(); 
}


/*
void calibrateIMU(){
  IMU.calibrateGyro();
  for (uint16_t i = 0; i < 20; i++) {
    IMU.getAGT();
    fusion.MahonyUpdate(0, 0, 0, -IMU.accX(), -IMU.accY(), -IMU.accZ(), 1);
    delay(1);
  }
  pitch_offset = fusion.getRoll();
  roll_offset = fusion.getPitch();
  
}
*/
