#ifndef UltraSonic_h
#define UltraSonic_h
#include <avr/interrupt.h>
#include <Arduino.h>

#define Trig_pin 8
#define Echo_Head 9
#define Echo_Left 10
#define Echo_Right 11

// define const variable
#define SOUND_SPEED 34320
#define STEP_TIMER1 4e-6
#define TIMER1_STEP_CYCLE (1 << 16)-1
 

// some variable in Interrupt function
volatile double distance_head, distance_right, distance_left;
volatile unsigned long last_time_head = 0, last_time_right = 0, last_time_left = 0;
volatile unsigned long last_distance_head = 0, last_distance_left = 0, last_distance_right = 0;
volatile bool head_high = false, left_high = false, right_high = false;
volatile bool flag_head = false, flag_right = false, flag_left = false;
volatile bool pin_head, pin_left, pin_right;
volatile double distance_head_SAMPLE[11], distance_left_SAMPLE[11], distance_right_SAMPLE[11];

class UltraSonic {
public:
    void setupUltraSonic();
    double getDistance_head();
    double getDistance_right();
    double getDistance_left();
    
private:
    volatile double distance_head, distance_right, distance_left;
};

#endif