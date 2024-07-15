#include "UltraSonic.h"


ISR(TIMER1_OVF_vect) 
{
  TCNT1 = 0;
}


ISR (PCINT0_vect)
{
// digitalRead(Echo_Head);
   pin_head = (bool)(PINB & (1 << PD1));
// digitalRead(Echo_Left);
   pin_left = (bool)(PINB & (1 << PD2));
// digitalRead(Echo_right);
   pin_right = (bool)(PINB & (1 << PD3));
 
   if (pin_head != head_high)
   {
      if (head_high) 
      {
        static byte i = 0;
        distance_head_SAMPLE[10] -= distance_head_SAMPLE[i];
        distance_head_SAMPLE[i] = (TCNT1 - last_time_head + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
        distance_head_SAMPLE[10] += distance_head_SAMPLE[i];
        distance_head = ((SOUND_SPEED * STEP_TIMER1) * (distance_head_SAMPLE[10] / 10)) / 2;
        if (distance_head - last_distance_head > 1000)
        {
          distance_head = last_distance_head;
        }
        else
        {
          	last_distance_head = distance_head;
        }
        // increase i and limit i < 10
        i = (i + 1) % 10;
        flag_head = true;
      }
      else
      {
        last_time_head = TCNT1;
      }
      head_high = !head_high;
   }

   if (pin_left != left_high)
   {
      if (left_high) 
      {
        static byte i = 0;
        distance_left_SAMPLE[10] -= distance_left_SAMPLE[i];
        distance_left_SAMPLE[i] = (TCNT1 - last_time_left + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
        distance_left_SAMPLE[10] += distance_left_SAMPLE[i];
        distance_left = ((SOUND_SPEED * STEP_TIMER1) * (distance_left_SAMPLE[10] / 10)) / 2;
        // increase i and limit i < 10
        i = (i + 1) % 10;
        flag_left = true;
      }
      else
      {
        last_time_left = TCNT1;
      }
      left_high = !left_high;
   }

   if (pin_right != right_high)
   {
      if (right_high) 
      {
        static byte i = 0;
        distance_right_SAMPLE[10] -= distance_right_SAMPLE[i];
        distance_right_SAMPLE[i] = (TCNT1 - last_time_right + TIMER1_STEP_CYCLE) % TIMER1_STEP_CYCLE;
        distance_right_SAMPLE[10] += distance_right_SAMPLE[i];
        distance_right = ((SOUND_SPEED * STEP_TIMER1) * (distance_right_SAMPLE[10] / 10)) / 2;
        // increase i and limit i < 10
        i = (i + 1) % 10;
        flag_right = true;
      }
      else
      {
        last_time_right = TCNT1;
      }
      right_high = !right_high;
   }  

   if (flag_head && flag_left && flag_right)
   {
      // set high trig pin
      flag_head = flag_left = flag_right = false;
      PORTB |= (1 << PD0);
   }
}

void UltraSonic::setupUltraSonic()
{
  Serial.begin(9600);

  DDRB |= (1 << DDB0); //set trig pin is output
  PORTB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3);  //enable the interal pull up Pin 9, 10, 11
  
  cli();

  //reset
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;

  //timer 1
  TCCR1B |= (1 << CS11) | (1 << CS10); //prescale = 64 => 1 step = 64 / 16MHz = 4e-6 s;
  TCNT1 = 0;
  TIMSK1 = (1 << TOIE1);

//  reset
  PCICR = 0;
  PCMSK0 = 0;

//  Pin change interrupt pin 9, 10, 11
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
  
  
  //     
  //Enable global interrupt (optional in Arduino because Global Interrupt is enabled by default)
  sei();
	
  //PORTB &= ~(1 << PD0) ;
  distance_head_SAMPLE[10] = 100;
  distance_left_SAMPLE[10] = 100;
  distance_right_SAMPLE[10] = 100;
  
}

double UltraSonic::getDistance_head() 
{
    return distance_head;
}
double UltraSonic::getDistance_left() 
{
    return distance_left;
}
double UltraSonic::getDistance_right() 
{
    return distance_right;
}


