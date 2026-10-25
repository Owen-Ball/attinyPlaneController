#ifndef _CONSTANTS_
#define _CONSTANTS_

enum MODE {FAILSAFE, RATE_MODE, ANGLE_MODE, MANUAL_MODE};
enum SWITCH_MODE {ANGLE_MANUAL, RATE_MANUAL};

#define ELEV_ANGLE  25.0
#define AIL_ANGLE   25.0
#define RUD_RATE    300.0
#define MIN_THROTTLE  20 //out of 255

#define SERVO_MIN    1000
#define SERVO_MAX    2000

#define RC_MIN  153.0
#define RC_MAX  870.0

#define SSPIN PIN_PC3
#define BUZ_PIN PIN_PA4

#define MOTOR1 PIN_PA6
#define MOTOR2 PIN_PA5
#define MOTOR3 PIN_PA3
#define MOTOR4 PIN_PB0

#define SWITCH_TIME 200000

#define IMU_ADDR  0x68

#define AX_MAX 8280
#define AX_MIN -8140

#endif
