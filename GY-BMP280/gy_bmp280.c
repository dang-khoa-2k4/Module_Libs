/*
 * gy_bmp280.h
 *
 *  Created on: Jul 25, 2024
 *      Author: Nguyen Tran Dang Khoa
 */

#include "gy_bmp280.h"

void bmp280_setup_Standard(BMP280_setup *new_setup)
{
	new_setup->mode = NORMAL_MODE;
	new_setup->filter = FILTER_OFF;
	new_setup->oversampling_temp = SR;
	new_setup->oversampling_press = SR;
	new_setup->standby = STANDBY_250;
}

void bmp280_setup_Drone(BMP280_setup *new_setup)
{
	new_setup->mode = NORMAL_MODE;
	new_setup->filter = FILTER_16;
	new_setup->oversampling_temp = LP;
	new_setup->oversampling_press = UHR;
	new_setup->standby = STANDBY_0_5;
}

void bmp280_setup_Elevator(BMP280_setup *new_setup)
{
	new_setup->mode = NORMAL_MODE;
	new_setup->filter = FILTER_4;
	new_setup->oversampling_temp = ULP;
	new_setup->oversampling_press = SR;
	new_setup->standby = STANDBY_125;
}

void bmp280_setup_DropDetect(BMP280_setup *new_setup)
{
	new_setup->mode = NORMAL_MODE;
	new_setup->filter = FILTER_OFF;
	new_setup->oversampling_temp = ULP;
	new_setup->oversampling_press = LP;
	new_setup->standby = STANDBY_0_5;
}

static bool read_8bit_register (uint8_t *value, uint8_t addr)
{
	uint16_t tx_buff;
	uint8_t rx_buff;
	tx_buff = (I2C_BMP280_ADDRESS << 1);

	if (HAL_I2C_Mem_Read(BMP280_I2C, tx_buff, addr, 1, rx_buff, 1, i2c_timeout)
			== HAL_OK)
	{
		*value = rx_buff;
		return 1;
	} else
		return 0;
}

static bool read_16bit_register (uint16_t *value, uint8_t addr)
{
	uint16_t tx_buff;
	uint8_t rx_buff[2];
	tx_buff = (I2C_BMP280_ADDRESS << 1);

	if (HAL_I2C_Mem_Read(BMP280_I2C, tx_buff, addr, 1, rx_buff, 2, i2c_timeout)
			== HAL_OK)
	{
		*value = (uint16_t) ((rx_buff[1] << 8) | rx_buff[0]);
		return 1;
	} else
		return 0;
}

static bool write_8bit_register (uint8_t *value, uint8_t addr)
{
	uint16_t tx_buff = (I2C_BMP280_ADDRESS << 1);
	if (HAL_I2C_Mem_Write(BMP280_I2C, tx_buff, addr, 1, value, 1, i2c_timeout)
			== HAL_OK)
		return 1;
	else
		return 0;
}

static bool read_data (uint8_t * value, uint8_t addr, uint8_t len)
{
	uint16_t tx_buff = (I2C_BMP280_ADDRESS << 1);
	if (HAL_I2C_MEM_Read(BMP280_I2C, tx_buff, addr, 1, value, len, i2c_timeout)
			== HAL_OK)
		return 1;
	else
		return 0;
}

static int read_calibration_data(BMP280 * bmp)
{
	if (read_16bit_register(&bmp->dig_T1, 0x88)
	&&  read_16bit_register((uint16_t*)&bmp->dig_T2, 0x8A)
	&&  read_16bit_register((uint16_t*)&bmp->dig_T3, 0x8C)
	&&  read_16bit_register(&bmp->dig_P1, 0x8E)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P2, 0x90)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P3, 0x92)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P4, 0x94)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P5, 0x96)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P6, 0x98)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P7, 0x9A)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P8, 0x9C)
	&&  read_16bit_register((uint16_t*)&bmp->dig_P9, 0x9E))
		return 1;
	else
		return 0;
}

bool bmp280_init(BMP280 * bmp, BMP280_setup *setup)
{
	if (!write_8bit_register(BMP280_RESET_VALUE, RESET)) return false;
	while (1) 
	{
		uint8_t status;
		if (!read_data(&status, STATUS, 1)
			&& (status & 1) == 0)
				break;
	}
	if (!read_calibration_data(bmp)) return false;
	if (setup->mode == FORCE_MODE) setup->mode = SLEEP_MODE;
	uint8_t ctrl = 	(setup->oversampling_temp << 5) | 
				   	(setup->oversampling_press << 2)|
					setup->mode;
	if (!write_8bit_register(&ctrl, CTRL_MEAS)) return false;
	uint8_t config = (setup->standby << 5) | (setup->filter << 2);
	if (!write_8bit_register(&config, CONFIG)) return false;
	return true;
}

bool is_measurement_done()
{
	uint8_t status;
	if (!read_data(&status, STATUS, 1))
		return false;
	return (status & (1 << 3)) == 0;
}

double bmp280_get_temp (BMP280 * bmp)
{
	uint8_t data[3];
	if (!read_data(data, TEMP_MSB, 3))
		return -1;
	int32_t adc_T = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
	double var1, var2, T;
	var1 = (((double) adc_T)/16384.0 - ((double) bmp->dig_T1)/1024.0) * ((double) bmp->dig_T2);
	var2 = ((((double) adc_T)/131072.0  - ((double) bmp->dig_T1)/8192.0) *
	(((double)adc_T)/131072.0 - ((double) bmp->dig_T1)/8192.0)) * ((double) bmp->dig_T3);
	bmp->t_fine = (int32_t)(var1 + var2);
	T = (var1 + var2) / 5120.0;
	return T;
}

double bmp280_get_altitude (BMP280 * bmp)
{
	uint8_t data[3];
	if (!read_data(data, PRESS_MSB, 3))
		return -1;
	int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);

	int64_t var1, var2, p;
	var1 = ((double) bmp->t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double) bmp->dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double) bmp->dig_P6) * 2.0;
	var2 = (var2/4.0)+(((double) bmp->dig_P4) * 65536.0);
	var1 = (((double) bmp->dig_P3) * var1 * var1 / 524288.0 + ((double) bmp->dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double) bmp->dig_P1);
	if (var1 == 0.0)
		return -1; // avoid exception caused by division by zero
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double) bmp->dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double) bmp->dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double) bmp->dig_P7)) / 16.0;
	
	double altitude = 44330 * (1.0 - pow(p / (1013.25 * 100), 0.1903));
	return altitude;
}