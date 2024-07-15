/*
  ***Warnming***
      Each timer is only used for one ultra sonic sensor
      And channel used for your sensor is always TIM_CHANNEL_1
*/


#ifndef _ULTRA_SONIC_
#define _ULTRA_SONIC_

#include "..\AVERAGE FILTER\average_filter.h"
#include <stdbool.h>

#define SOUND_SPEED 34320 // cm/s
#define STEP_TIMER 1e-6 // prescalers / SysClk = 72 / 72Mhz = 1 us
#define TIMER_STEP_CYCLE (1 << 16)-1 // 65536
#define SAMPLING_TIME 15 // ms

typedef struct
{
    TIM_HandleTypeDef *htim;
    volatile uint16_t pre_time;
    volatile double distance; // in cm
    GPIO_TypeDef *TrigPort;
    uint32_t TrigPin;
    bool echoHigh;
    average_filter *filter;
} ultraSonic;

/**
  * @brief  Init ultra sonic sensor
  * @param  *sensor is pointer to the ultra sonic structure
  * @param  *htim is timer used for edge detect echo pin
  * @param  *TrigPort, TrigPin is port and pin of trig pin
*/
void ultraSonic_Init(ultraSonic *sensor, TIM_HandleTypeDef *htim, average_filter *filter, GPIO_TypeDef *TrigPort, uint16_t TrigPin);

/**
  * @brief  update all paramater in ultra sonic structure
  * @param  *sensor is pointer to the ultra sonic structure
*/
void updateDistance(ultraSonic *sensor, uint32_t Channel);

/**
  * @brief  set trig pin and active Interrupt
  * @param  *sensor is pointer to the ultra sonic structure
*/
void activeTrigger(ultraSonic *sensor);

/**
  * @brief delay is used for set trig pin in us
  * @param  *htim is timer used for edge detect echo pin
  * @param  time is the time you need to delay
**/
void delay_us(TIM_HandleTypeDef *htim, uint16_t time);


#endif
