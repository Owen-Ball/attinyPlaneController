#ifndef _PID_
#define _PID_


void updatePIDLoopTime(long t);

class PID {
  public:
    PID(float KP, float KI, float KD);
    float updatePID(float sensor_val, float target);
    void resetIntegral();
                 
  private:
    float KP;
    float KI;
    float KD;
    float target;  
    float prev_error;  
    float cum_error;
    float rate_error;               
};


#endif
