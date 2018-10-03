#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>

#define Motor_Pin 25

using namespace std;

class UKC_GPIO{

private:

public:
    void GPIO_init();
    void Motor_on();
    void Motor_off();
};

void UKC_GPIO::GPIO_init(){
    if(wiringPiSetup() == -1){
        cout << "WirginPi Setup Error" << endl;   
        return;
    }

    pinMode(Motor_Pin,OUTPUT);
}

void UKC_GPIO::Motor_on(){
    digitalWrite(Motor_Pin,HIGH);
}

void UKC_GPIO::Motor_off(){
    digitalWrite(Motor_Pin,LOW);
}


