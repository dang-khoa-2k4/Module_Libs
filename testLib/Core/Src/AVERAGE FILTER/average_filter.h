#ifndef _AVERAGE_FILTER_H_
#define _AVERAGE_FILTER_H_

 #include "main.h"
#define AVERAGE_LENGTH 10
#define map(a, b, c, d, e) (((a - b)*1.0/(c - b)) * (e - d) + d)

typedef struct
{
    int16_t buffer[AVERAGE_LENGTH];
    int32_t sum;
    float out;
    uint16_t count; 
} average_filter;

/**
  * @brief  Reset all paramater in average filter structure
  * @param  *filter is pointer to the average filter structure
*/
void reset_buffer(average_filter *filter);

/**
  * @brief  update all paramater in average filter structure
  * @param  *filter is pointer to the average filter structure
  * @param  input which you want to noise filtering
*/
void apply_filter(average_filter *filter, int16_t input);

/**
  * @brief  limit value in the range
  * @param  *value is pointer to the value which you want to constrain
  * @param  min_value and max_value is the range you want to limit
*/
void constrain(int32_t *value, int32_t min_value, int32_t max_value);

#endif
