/*
  Motor.h - Library for using TB6612FNG Driver.
  Created by Garth J. Martinsen, July 8, 2023.
  Reserved for Release into the public domain when working.
*/
#include "Motor.h"


Motor::Motor(int pinpwm, int pinin1, int pinin2){
    _pwm=pinpwm;
    _in1=pinin1;
    _in2=pinin2;
    pinMode(_pwm, OUTPUT);
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
}

void Motor::stop(){
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
}
void Motor::run(int speed,  int dir, int duration){
    analogWrite(_pwm, speed); //   0 < speed < 255  experiment to see boundaries where motor actually starts. Avoid 255=> 100% DC.
    digitalWrite(_in1, !dir);
    digitalWrite(_in2, dir);
    unsigned long now = millis();
    while(millis() < now + duration);
    stop();
}
