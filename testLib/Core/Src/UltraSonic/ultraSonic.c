#include "ultraSonic.h"

void ultraSonic_Init(ultraSonic *sensor, TIM_HandleTypeDef *htim, average_filter *filter, GPIO_TypeDef *TrigPort, uint16_t TrigPin)
{
    sensor->htim = htim;
    sensor->pre_time = 0;
    sensor->distance = 0;
    sensor->TrigPort = TrigPort;
    sensor->TrigPin = TrigPin;
    sensor->echoHigh = false;
    sensor->filter = filter;
    HAL_GPIO_WritePin(sensor->TrigPort, sensor->TrigPin, 0);
    reset_buffer(sensor->filter);
}

// throw into void HAL_TIM_IC_CaptureCallback (TIM_HandleTypeDef* htim)
void updateDistance(ultraSonic *sensor, uint32_t Channel)
{
    if (sensor->echoHigh == false)
    {
        sensor->pre_time = HAL_TIM_ReadCapturedValue(sensor->htim, Channel);
        __HAL_TIM_SET_CAPTUREPOLARITY(sensor->htim, Channel, TIM_INPUTCHANNELPOLARITY_FALLING);
        sensor->echoHigh = true;
    }
    else
    {
        // uint32_t diffTime = (sensor->htim->Instance->CCR1 - sensor->pre_time + TIMER_STEP_CYCLE) % TIMER_STEP_CYCLE;
        uint32_t diffTime = (HAL_TIM_ReadCapturedValue(sensor->htim, Channel) - sensor->pre_time + TIMER_STEP_CYCLE) % TIMER_STEP_CYCLE;

        apply_filter(sensor->filter, diffTime);
        sensor->distance = ((SOUND_SPEED * STEP_TIMER) * (sensor->filter->out)) / 2.0;
        sensor->echoHigh = false;

        __HAL_TIM_SET_CAPTUREPOLARITY(sensor->htim, Channel, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Stop_IT(sensor->htim, Channel);
    }
    // if (sensor->htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    // {
    //     if (sensor->echoHigh == false)
    //     {
    //         sensor->pre_time = HAL_TIM_ReadCapturedValue(sensor->htim, TIM_CHANNEL_1);
    //         //            __HAL_TIM_SET_CAPTUREPOLARITY(sensor->htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    //         sensor->echoHigh = true;
    //         __HAL_TIM_DISABLE_IT(sensor->htim, TIM_IT_CC1);
    //         __HAL_TIM_ENABLE_IT(sensor->htim, TIM_IT_CC2);
    //     }
    // }
    // else if (sensor->htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    // {
    //     if (sensor->echoHigh == true)
    //     {
    //         //        	uint32_t diffTime = (sensor->htim->Instance->CCR1 - sensor->pre_time + TIMER_STEP_CYCLE) % TIMER_STEP_CYCLE;
    //         uint32_t diffTime = (HAL_TIM_ReadCapturedValue(sensor->htim, TIM_CHANNEL_2) - sensor->pre_time + TIMER_STEP_CYCLE) % TIMER_STEP_CYCLE;

    //         apply_filter(sensor->filter, diffTime);
    //         sensor->distance = ((SOUND_SPEED * STEP_TIMER) * (sensor->filter->out)) / 2.0;
    //         sensor->echoHigh = false;
    //         sensor->htim->Instance->CNT = 0;

    //         //    	            __HAL_TIM_SET_CAPTUREPOLARITY(sensor->htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
    //         __HAL_TIM_DISABLE_IT(sensor->htim, TIM_IT_CC2);
    //     }
    // }
}

// throw into void SysTick_CallBack(void)
// {
//     //for example
//     static time = 0;
//     time++;
//     if (time == 25) //25ms
//     {
//         activeTrigger(&sensor);
//         time = 0;
//     }
// }

void activeTrigger(ultraSonic *sensor)
{
    HAL_GPIO_WritePin(sensor->TrigPort, sensor->TrigPin, 1);
    delay_us(sensor->htim, 10);
    HAL_GPIO_WritePin(sensor->TrigPort, sensor->TrigPin, 0);
//    __HAL_TIM_ENABLE_IT(sensor->htim, TIM_IT_CC1);
    // __HAL_TIM_ENABLE_IT(sensor->htim, TIM_IT_CC2);
}

void delay_us(TIM_HandleTypeDef *htim, uint16_t time)
{
	HAL_TIM_Base_Start(htim);
    htim->Instance->CNT = 0; // __HAL_TIM_SET_COUNTER(&htim, Cnt);
    while (htim->Instance->CNT < time); // while (__HAL_TIM_GET_COUNTER(&htim) < time)
    HAL_TIM_Base_Stop(htim);
}
