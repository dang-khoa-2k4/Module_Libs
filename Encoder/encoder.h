#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "../PID/PID.h"
#include "../AVERAGE FILTER/average_filter.h"
#include "../PWM control/PWMcontrol.h"

#define NUMS_OF_ENC 2

#define MAX_COUNTER ((1 << 16) - 1)
#define TIME_SAMPLING  50 // ms
// set based on Specification of your motor
#define MAX_RPM 200
#define PULSE_PER_REVOLUTION 1050


typedef struct
{
    volatile double _RPM;
    volatile double _PWM;
    volatile uint32_t pre_counter;
    int8_t Direction;
    TIM_HandleTypeDef *htim;
} Encoder;


void updateDiffPulse(Encoder* en, int32_t *diffPulse);
/**
  * @brief  Reset all paramater in Encoder structure
  * @param  *en is pointer to the encoder structure
*/
void Encoder_Init(Encoder *p1, TIM_HandleTypeDef *h_time);
/**
  * @brief  update all values of encoder
  * @param  htim Timer handle of encoder module
  * @param  *en is pointer to the encoder structure
*/
void TimerInitENC(TIM_HandleTypeDef *h_time, uint32_t Channel);

void updateEncoder(Encoder *enc, bool mode4X);

#endif
