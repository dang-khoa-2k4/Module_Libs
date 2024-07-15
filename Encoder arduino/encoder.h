#ifndef encoder_h
#define encoder_h
#include <Arduino.h>
#include <avr/interrupt.h>

#define res 20
#define radius 20 // max 26mm
#define bitRead_ PORTD&
#define bitWrite_ PORTD|=
#define TIMER1_CYCLE 65536

byte motorL = 2, motorR = 3;
volatile int cur = 0, theta = 0; //microseconds
volatile float speedL = 0, speedR = 0; //m per second

class encoder {
public: 
    void setupEncoder();
    void ExtendInterrupt();
    void PCInterrupt();
    void HalfTimer();
    float getSpeedLeft();
    float getSpeedRight();
private:
    volatile float speedL = 0, speedR = 0; //m per second
}


#endif