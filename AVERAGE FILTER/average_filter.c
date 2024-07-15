#include "average_filter.h"

void reset_buffer(average_filter *filter)
{
    filter->sum = 0;
    filter->out = 0.0;
    filter->count = 0;
    
    for (int i = 0; i < AVERAGE_LENGTH; i++)
    {
        filter->buffer[i] = 0;
    }
    return;
}

void apply_filter(average_filter *filter, int16_t input)
{
    filter->sum -= filter->buffer[filter->count];
    filter->buffer[filter->count] = input;
    filter->sum += filter->buffer[filter->count];
    
    filter->count = (filter->count + 1) % AVERAGE_LENGTH;

    filter->out = (filter->sum * 1.0f) / AVERAGE_LENGTH;
}

void constrain(int32_t *value, int32_t min_value, int32_t max_value)
{
    if (*value > max_value)
    {
        *value = max_value;
    }
    if (*value < min_value)
    {
        *value = min_value;
    }
}
