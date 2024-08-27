/*
 * hmc5883l.h
 *
 *  Created on: Aug 6, 2024
 *      Author: Nguyen Tran Dang Khoa
 */

#ifndef INC_HMC5883L_H_
#define INC_HMC5883L_H_

#include "main.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/* User Configurations */
extern  I2C_HandleTypeDef 				 hi2c1;
#define HMC5883L_I2C 					    (&hi2c1)
/* Get declination tutorial 
Visit the NOAA Website: Go to the NOAA Magnetic Field Calculators (https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml).

Enter Your Location: Input your latitude and longitude coordinates. You can also use the map to pinpoint your location.

Enter the Date: Specify the date for which you want the declination data (usually the current date).

Get the Declination: The tool will calculate and display the declination angle in degrees, indicating whether it is east or west of true North.
*/
#define DECLINATION_ANGLE          0.5
/* END User Configurations */

/* HMC5883L Address */
#define HMC5883L_ADDRESS 0x1E
#define HMC5883L_DEFAULT_ADDRESS (HMC5883L_ADDRESS << 1) // 0x3C
/* END HMC5883L Address */

/* HMC5883L Registers */
#define CONFIG_A 		    0x00
#define CONFIG_B 		    0x01
#define MODE    		    0x02
#define DATA_OUT_X_MSB 		0x03
#define DATA_OUT_X_LSB 	    0x04
#define DATA_OUT_Z_MSB 	    0x05
#define DATA_OUT_Z_LSB		0x06
#define DATA_OUT_Y_MSB 	    0x07
#define DATA_OUT_Y_LSB 	    0x08
#define STATUS 			    0x09
#define ID_A 			    0x0A
#define ID_B 			    0x0B
#define ID_C 			    0x0C
/* END HCM5883L Registers */

/* HMC5883L Configuration Typedef & Define */
#define I2C_TIMEOUT 500 // ms
#define RAD_TO_DEG 57.295779513082320876798154814105

typedef enum
{
    SAMPLING_1 = 0, // default
    SAMPLING_2,
    SAMPLING_4,
    SAMPLING_8
} HMC5883L_Sampling_t;

typedef enum
{
    DATA_RATE_0_75 = 0,
    DATA_RATE_1_5,
    DATA_RATE_3,
    DATA_RATE_7_5,
    DATA_RATE_15, // default
    DATA_RATE_30,
    DATA_RATE_75
} HMC5883L_DataRate_t;

typedef enum
{
    GAIN_1370 = 0,
    GAIN_1090,  // default
    GAIN_820, 
    GAIN_660,
    GAIN_440,
    GAIN_390,
    GAIN_330,
    GAIN_230
} HMC5883L_Gain_t;

typedef enum
{
    CONTINUOUS = 0, // default
    SINGLE,
    IDLE   
} HMC5883L_Mode_t;

typedef struct
{
    HMC5883L_Sampling_t sampling_rate;
    HMC5883L_DataRate_t data_rate;
    HMC5883L_Gain_t gain;
    HMC5883L_Mode_t mode;
} HMC5883L_Config_t;

typedef struct
{
    HMC5883L_Config_t config;
    float X;
    float Y;
    float Z;
    
    float x_offset;
    float y_offset;
} HMC5883L_t;
/* END HMC5883L Configuration Typedef */

/* HMC5883L Functions */
/**
  * @brief  Initialize HMC5883L
  * @param  hmc: pointer to HMC5883L_t struct
  * @retval 0 if success, 1 if failed
*/
bool config_HMC5883L(HMC5883L_t *hmc);

/**
  * @brief  Initialize HMC5883L for Drone
  * @param  hmc: pointer to HMC5883L_t struct
  * @retval 0 if success, 1 if failed
*/
bool config_HMC5883L_Drone(HMC5883L_t *hmc);

/**
  * @brief  Get data from HMC5883L
  * @param  hmc: pointer to HMC5883L_t struct
  * @param  hmc_config: pointer to HMC5883L_Config_t struct
  * @retval 0 if success, 1 if failed
*/
bool get_HMC5883L_data(HMC5883L_t *hmc);

void get_offset(HMC5883L_t *hmc, float x_offset, float y_offset);

/**
  * @brief  Get heading from HMC5883L
  * @param  hmc: pointer to HMC5883L_t struct
  * @retval heading in degree
*/
float get_heading(HMC5883L_t *hmc);
/* END HMC5883L Functions */
#endif /* INC_HMC5883L_H_ */
