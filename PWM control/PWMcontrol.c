/*
 * PWMcontrol.c
 *
 *  Created on: Nov 22, 2023
 *      Author: Khoa_Nguyen
 */
#include "PWMcontrol.h"



void Motor_Init(PWMcontrol *PWMcontrol, TIM_HandleTypeDef *htim, uint32_t Channel1, uint32_t Channel2)
{
    PWMcontrol->htim = htim;
    PWMcontrol->Channel1 = Channel1;
    PWMcontrol->Channel2 = Channel2;
    HAL_TIM_PWM_Start(htim, Channel1);
    HAL_TIM_PWM_Start(htim, Channel2);
    __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel1, 0);
    __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel2, 0);
    return;
}

void Servo_Init(PWMcontrol *PWMcontrol, TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t PWM_middle)
{
    PWMcontrol->htim = htim;
    PWMcontrol->Channel1 = Channel;
//	HAL_TIM_PWM_Start(htim, Channel);
    __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel1, PWM_middle);
    return;
}

void set_motor(PWMcontrol *PWMcontrol, int8_t direction, uint16_t PWM)
{
    if (direction == BACKWARD)
    {
        __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel1, PWM);
        __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel2, 0);
    }
    else if (direction == FORWARD)
    {
        __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel1, 0);
        __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel2, PWM);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel1, 0);
        __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel2, 0);
    }
    return;
}

void set_servo(PWMcontrol *PWMcontrol, uint16_t PWM)
{
    if (PWM > SERVO_MAX_PULSE)
    {
        PWM = SERVO_MAX_PULSE;
    }
    else if (PWM < SERVO_MIN_PULSE)
    {
        PWM = SERVO_MIN_PULSE;
    }
    __HAL_TIM_SET_COMPARE(PWMcontrol->htim, PWMcontrol->Channel1, PWM);
    return;
}


