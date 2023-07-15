#include "PwrMgt.h"

   PwrMgt::PwrMgt(int standby_pin){
        _stby_pin = standby_pin;
    }
   void PwrMgt:: powerdown(){
        digitalWrite(_stby_pin, LOW);
    }

    void PwrMgt:: wakeup(){
        digitalWrite(_stby_pin, HIGH);
    }
