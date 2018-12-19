#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>

#define Motor1_Pin 9

using namespace std;

class UKC_GPIO{

private:

public:
    void GPIO_init();
    void Motor1_on();
    void Motor1_off();
};

void UKC_GPIO::GPIO_init(){
    if(wiringPiSetup() == -1){
        cout << "WirginPi Setup Error" << endl;   
        return;
    }
    cout << "WirginPi Setup" << endl;

    pinMode(Motor1_Pin,OUTPUT);
}

void UKC_GPIO::Motor1_on(){
    digitalWrite(Motor1_Pin,HIGH);
}

void UKC_GPIO::Motor1_off(){
    digitalWrite(Motor1_Pin,LOW);
}
