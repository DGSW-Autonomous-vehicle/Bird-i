#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>

#define Motor1_Pin 25
#define Motor2_Pin 24

using namespace std;

class UKC_GPIO{

private:

public:
    void GPIO_init();
    void Motor1_on();
    void Motor1_off();
    void Motor2_on();
    void Motor2_off();
};

void UKC_GPIO::GPIO_init(){
    if(wiringPiSetup() == -1){
        cout << "WirginPi Setup Error" << endl;   
        return;
    }

    pinMode(Motor1_Pin,OUTPUT);
    pinMode(Motor2_Pin,OUTPUT);
}

void UKC_GPIO::Motor1_on(){
    digitalWrite(Motor1_Pin,HIGH);
}

void UKC_GPIO::Motor1_off(){
    digitalWrite(Motor1_Pin,LOW);
}


void UKC_GPIO::Motor2_on(){
    digitalWrite(Motor2_Pin,HIGH);
}

void UKC_GPIO::Motor2_off(){
    digitalWrite(Motor2_Pin,LOW);
}


