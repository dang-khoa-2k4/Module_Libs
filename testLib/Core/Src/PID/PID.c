#include "PID.h"

void set_PID_gain(PID_instance *PID, float p, float i, float d)
{
    PID->p_gain = p;
    PID->i_gain = i;
    PID->d_gain = d;
    return;
}

void reset_PID_gain(PID_instance *PID)
{
    PID->p_gain = 0.0;
    PID->i_gain = 0.0;
    PID->d_gain = 0.0;
    PID->pre_error = 0;
    PID->isSaturation = false;
    return;
}


void output_PID(PID_instance *PID, int16_t error_input, uint16_t sampling_rate)
{
    float value_I = 0.0;
    if (!PID->isSaturation)
    {
        PID->integral_error += error_input;
        value_I = PID->i_gain * PID->integral_error / sampling_rate;
    }

    
    constrain(&(PID->integral_error), -MAX_INTEGRAL, MAX_INTEGRAL);
    
    PID->output_PID = PID->p_gain * error_input 
                    + value_I
                    + PID->d_gain * (error_input - PID->pre_error) * sampling_rate;

    int16_t raw_value = PID->output_PID;
    constrain(&(PID->output_PID), -MAX_PID_VALUE, MAX_PID_VALUE);
    
    PID->isSaturation = (raw_value != PID->output_PID) && (raw_value * error_input > 0); //check saturation and same sign
    PID->pre_error = error_input;

    return;
}
