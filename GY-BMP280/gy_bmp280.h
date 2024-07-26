/*
 * gy_bmp280.h
 *
 *  Created on: Jul 25, 2024
 *      Author: Nguyen Tran Dang Khoa
 */

#ifndef INC_GY_BMP280_H_
#define INC_GY_BMP280_H_

#include "main.h"
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* User Configurations */
extern I2C_HandleTypeDef 				 hi2c1;
#define BMP280_I2C 						(&hi2c1)

/* End User Configurations */

/* Typedef & Define Part */
#define I2C_BMP280_ADDRESS 	0x76
#define BMP280_CHIP_ID  	0x58
#define i2c_timeout			5000
#define BMP280_RESET_VALUE 	0xB6
typedef enum
{
	SLEEP_MODE  = 0, // Sleep module
	FORCE_MODE  = 1, // Measurement is initiated by user
	NORMAL_MODE = 3  // Continues measurement
}BMP280_MODE;

typedef enum
{
	FILTER_OFF = 0,
	FILTER_2   = 1,
	FILTER_4   = 2,
	FILTER_8   = 3,
	FILTER_16  = 4
} BMP280_FILTER_COEFF;

typedef enum
{
	OFF = 0, // skip
	ULP = 1, // ultra low power 	  	(x1)
	LP  = 2, // low power 			  	(x2)
	SR  = 3, // standard resolution  	(x4)
	HR  = 4, // high resolution		 	(x8)
	UHR = 5  // ultra high resolution	(x16)
}BMP280_oversampling;

typedef enum
{
	STANDBY_0_5 	= 0, // 0.5 ms
	STANDBY_62_5	= 1, // 62.5 ms
	STANDBY_125		= 2, // 125 ms
	STANDBY_250		= 3, // 250 ms
	STANDBY_500		= 4, // 500 ms
	STANDBY_1000	= 5, // 1000 ms
	STANDBY_2000	= 6, // 2000 ms
	STANDBY_4000	= 7  // 4000 ms
}BMP280_standby;

typedef struct
{
	// calibration data
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    // raw data
    int32_t adc_pressure;
    int32_t adc_temp;

	uint32_t t_fine;
}BMP280;

typedef struct
{
	BMP280_MODE mode;
	BMP280_FILTER_COEFF filter;
	BMP280_oversampling oversampling_temp;
	BMP280_oversampling oversampling_press;
	BMP280_standby standby;
}BMP280_setup;
/* End Typedef Part */

/* Main Functions */
/**
  * @brief  set up for standard mode (mode: Normal, filter: off, oversampling (temp, press): x4, x4)
  * @param  new_setup: pointer to BMP280_setup
  * @return Null
*/
void bmp280_setup_Standard(BMP280_setup *new_setup);
/**
  * @brief  set up for Drone (indoor navigation) (mode: Normal, filter: 16, oversampling (temp, press): x2, x16)
  * @param  new_setup: pointer to BMP280_setup
  * @return Null
*/
void bmp280_setup_Drone(BMP280_setup *new_setup);
// some option for user
/**
  * @brief  set up for Elevator (floor change detect) (mode: Normal, filter: 4, oversampling (temp, press): x1, x4)
  * @param  new_setup: pointer to BMP280_setup
  * @return Null
*/
void bmp280_setup_Elevator(BMP280_setup *new_setup);
/**
  * @brief  set up for Drop Detect (mode: Normal, filter: off, oversampling (temp, press): x1, x2)
  * @param  new_setup: pointer to BMP280_setup
  * @return Null
*/
void bmp280_setup_DropDetect(BMP280_setup *new_setup);

/**
  * @brief  Get current status register
  * @param  Null
  * @return Status register
*/
bool bmp280_init(BMP280 * bmp, BMP280_setup *setup);

/**
  * @brief  Check if measurement is done
  * @param  Null
  * @return True if measurement is done, False if not
*/
bool is_measurement_done();

/**
  * @brief  Calculate altitude from pressure
  * @param  raw: pointer to BMP280 structure
  * @return Altitude in meter (return - 1 if exit error)
*/
double bmp280_get_altitude (BMP280 * raw);

/**
  * @brief  Calculate temperature from raw data
  * @param  raw: pointer to BMP280 structure
  * @return Temperature in Celsius (return - 1 if exit error)
*/
double bmp280_get_temp (BMP280 * raw);
/* End Main Function */
/* BMP280 Register */
#define TEMP_XLSB 	0xFC
#define TEMP_LSB  	0xFB
#define TEMP_MSB  	0xFA
#define PRESS_XLSB 	0xF9
#define PRESS_LSB 	0xF8
#define PRESS_MSB 	0xF7
#define CONFIG 		0xF5
#define CTRL_MEAS 	0xF4
#define STATUS 		0xF3
#define RESET 		0xE0
#define ID 			0xD0
/* End BMP280 Register */


#endif /* INC_GY_BMP280_H_ */
