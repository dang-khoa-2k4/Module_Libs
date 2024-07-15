#include "encoder.h"

void TimerInitENC(TIM_HandleTypeDef *h_time, uint32_t Channel)
{
    // Enable Output Compare Mode
    HAL_TIM_OC_Start_IT(h_time, Channel);
}

void updateDiffPulse(Encoder *enc, int32_t *diffPulse)
{
    uint16_t cur_counter = enc->htim->Instance->CNT;
    if (cur_counter > enc->pre_counter)
    {
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(enc->htim))
        {
            *diffPulse = -((enc->pre_counter - cur_counter + MAX_COUNTER) % MAX_COUNTER);
        }
        else
        {
            *diffPulse = cur_counter - enc->pre_counter;
        }
    }
    else
    {
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(enc->htim))
        {
            *diffPulse = -(enc->pre_counter - cur_counter);
        }
        else
        {
            *diffPulse = (cur_counter - enc->pre_counter + MAX_COUNTER) % MAX_COUNTER;
        }
    }
    return;
}
// HAL_TIM_OC_DelayElapsedCallback bỏ vào
/*
 * If the first three bits of SMCR register are set to 0x3
 * then the timer is set in X4 mode (TIM_ENCODERMODE_TI12)
 * and we need to divide the pulses counter by two, because
 * they include the pulses for both the channels
 */
void updateEncoder(Encoder *enc, bool mode4X)
{
    int32_t diffPulse = 0;
    updateDiffPulse(enc, &diffPulse);
    enc->_RPM = (((diffPulse * 1.0) / PULSE_PER_REVOLUTION) * 60 * 1000) / TIME_SAMPLING;

    // if ((TIM3->SMCR & 0x3) == 0x3 && (TIM4->SMCR & 0x3) == 0x3)
    if (mode4X)
    {
        enc->_RPM /= 2;
    }
    enc->_PWM = map(enc->_RPM, 0, MAX_RPM, 0, MAX_PID_VALUE);
    enc->pre_counter = enc->htim->Instance->CNT;
    if (enc->_RPM < 0) enc->Direction = BACKWARD;
    else if (enc->_RPM > 0) enc->Direction = FORWARD;
    else enc->Direction = STOP;
}

void Encoder_Init(Encoder *p1, TIM_HandleTypeDef *h_time)
{
    p1->_RPM = 0;
    p1->_PWM = 0;
    p1->pre_counter = 0;
    p1->htim = h_time;
    if (HAL_TIM_Encoder_Start(p1->htim, TIM_CHANNEL_ALL) != HAL_OK)
        Error_Handler(); // write in main.c, maybe turn some led on?
}
