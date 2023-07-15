#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor{
   public:
        Motor(int pwm, int in1, int in);
        void run(int speed,  int dir, int duration) ;
        void stop();
   private:
        int _pwm;
        int _in1;
        int _in2;
};
#endif