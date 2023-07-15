#include "Motor.h"
#include "PwrMgt.h"
int CW = 0;
int CCW = 1;

Motor motorA(3,2,4);
Motor motorB(5,6,7);
PwrMgt power(8);

void setup() {
 Serial.begin(115200);
 while(!Serial);
 Serial.println("---------Starting-----");
}

void loop() {
  Serial.println("Waking up motors..." );
  power.wakeup();
  Serial.println("Starting motors: A(3,2,4) and B(5,6,7)" );
  motorA.run(130, CW, 1200); //(pwm,dir, dur)
  motorB.run(200, CCW, 1200);
  Serial.println("Putting motors to sleep ..." );
  power.powerdown();
}
