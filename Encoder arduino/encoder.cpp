#include "encoder.h"

void encoder::setupEncoder()
{
    Serial.begin(9600);
    //pinMode(motorL, INPUT);
    //pinMode(motorR, INPUT);
    
    PORTD |= (1 << PD2) | (1 <<PD3);


    cli();
    ExtendInterrupt();
    HalfTimer();
    sei();
}

void encoder::ExtendInterrupt()
{
    EICRA = 0;
    EIMSK = 0;
    // interrupts when rising edge
    EICRA |= (1 << ISC10) | (1 << ISC11); // pin 3
    EICRA |= (1 << ISC00) | (1 << ISC01); // pin 2
    EIMSK |= (1 << INT1) | (1 << INT0);
}

void encoder::PCInterrupt()
{
    PCICR = 0;
    PCMSK0 = 0;
    PCMSK0 |= (1 << PCINT20); // pin 4
    PCICR |= (1 << PCIE2);
}

void encoder::HalfTimer()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    TCCR1B |= (1 << CS11); // 0.5us clock
}

ISR(INT0_vect)
{
    theta = (TCNT1 - cur + TIMER1_CYCLE) % TIMER1_CYCLE;
    speedL = radius * 1e3 * ((3.141592 * 2.0 / res) / float(theta * 0.5));
    cur = TCNT1;
}

ISR(INT1_vect)
{
    theta = (TCNT1 - cur + TIMER1_CYCLE) % TIMER1_CYCLE;
    speedR = radius * 1e3 * ((3.141592 * 2.0 / res) / float(theta * 0.5));
    cur = TCNT1;
}

float encoder::getSpeedLeft()
{
    return speedL;
}
float encoder::getSpeedRight()
{
    return speedR;
}
