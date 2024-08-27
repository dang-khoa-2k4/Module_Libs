/*
 * hmc5883l.c
 *
 *  Created on: Aug 6, 2024
 *      Author: Nguyen Tran Dang Khoa
 */
#include "hmc5883l.h"

float digital_resolution[8] = {0.73, 0.92, 1.22, 1.52, 2.27, 2.56, 3.03, 4.35};

// Write byte to register
static bool HMC5883L_writeRegister8(uint8_t reg, uint8_t value)
{
    if (HAL_I2C_Mem_Write(HMC5883L_I2C, HMC5883L_DEFAULT_ADDRESS, reg, 1, &value, 1, I2C_TIMEOUT)
            == HAL_OK)
        return true;
    return false;
}

// Read byte from register
static bool HMC5883L_readRegister8(uint8_t reg, uint8_t * value)
{
    if (HAL_I2C_Mem_Read(HMC5883L_I2C, HMC5883L_DEFAULT_ADDRESS, reg, 1, value, 1, I2C_TIMEOUT)
            == HAL_OK)
        return true;
    return false;
}

// Read word from register
static bool HMC5883L_readRegister16(uint8_t reg, int16_t * value)
{
    uint8_t raw[2];
    if (HAL_I2C_Mem_Read(HMC5883L_I2C, HMC5883L_DEFAULT_ADDRESS, reg, 1, raw, 2, I2C_TIMEOUT)
            == HAL_OK)
    {
        *value =  ((int16_t) raw[0] << 8 | raw[1]);
        return true;
    }
    return false;
}

bool config_HMC5883L(HMC5883L_t *hmc)
{
    // set up offset
    hmc->x_offset = 0;
    hmc->y_offset = 0;
    // Set up configuration register
	uint8_t reg_A;
	HMC5883L_readRegister8(CONFIG_A, &reg_A);
	// Config sampling_rate
	reg_A = (reg_A & 0x1F) | (hmc->config.sampling_rate << 5);
	// Config data output rate
	reg_A = (reg_A & 0xE3) | (hmc->config.data_rate << 2);

	uint8_t reg_B;
	HMC5883L_readRegister8(CONFIG_B, &reg_B);
	// Config gain
	reg_B = (reg_B & 0x1F) | (hmc->config.gain << 5);

	uint8_t reg_mode;
	HMC5883L_readRegister8(MODE, &reg_mode);
	// Config mode
	reg_mode = (reg_mode & 0xFC) | (hmc->config.mode);

    if (HMC5883L_writeRegister8(CONFIG_A, reg_A)
     && HMC5883L_writeRegister8(CONFIG_B, reg_B)
     && HMC5883L_writeRegister8(MODE	, reg_mode))
        return true;
    return false;
}

bool config_HMC5883L_Drone(HMC5883L_t *hmc)
{  
    hmc->config.sampling_rate = SAMPLING_1;
    hmc->config.data_rate = DATA_RATE_15;
    hmc->config.gain = GAIN_1090;
    hmc->config.mode = CONTINUOUS;
    return config_HMC5883L(hmc);
}

bool get_HMC5883L_data(HMC5883L_t *hmc)
{
    int16_t data[3];
    if (HMC5883L_readRegister16(DATA_OUT_X_MSB, &data[0])
     && HMC5883L_readRegister16(DATA_OUT_Y_MSB, &data[2])
     && HMC5883L_readRegister16(DATA_OUT_Z_MSB, &data[1]))
    {   
        hmc->X = ((float) data[0] - hmc->x_offset) * digital_resolution[hmc->config.gain];
        hmc->Y = ((float) data[2] - hmc->y_offset) * digital_resolution[hmc->config.gain];
        hmc->Z = (float) data[1] * digital_resolution[hmc->config.gain];
        return true;
    }
    return false; 
}

void get_offset(HMC5883L_t * hmc, float x_offset, float y_offset)
{
    hmc->x_offset = x_offset;
    hmc->y_offset = y_offset;
}

float get_heading(HMC5883L_t *hmc)
{
    float heading = 0;
	if (get_HMC5883L_data(hmc))
    {
		heading = atan2(hmc->Y, hmc->X) * RAD_TO_DEG + DECLINATION_ANGLE;
        if (heading < 0) heading += 360;
        else if (heading >= 360) heading -= 360;
        return heading;
    }
	else return -1;
}
