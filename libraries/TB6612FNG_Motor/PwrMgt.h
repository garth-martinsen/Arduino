#ifndef PwrMgt_h
#define PwrMgr_h
#include "Arduino.h"

class PwrMgt{
   public:
        PwrMgt(int stby_pin);
        void powerdown();
        void wakeup();
   private:
        int _stby_pin;
    
};
#endif