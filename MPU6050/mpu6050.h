/*
 * mpu6050.h
 *
 *  Created on: Jun 30, 2024
 *      Author: Nguyen Tran Dang Khoa
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"
#include <math.h>
/* 										User notes  						   */
/* Must to set up interrupt or anything for sampling time equal to your define */
/*******************************************************************************/
/* User Configurations */
extern I2C_HandleTypeDef 				 hi2c1;
#define MPU6050_I2C 					(&hi2c1)
// calibration to reach 1g for accelerometer
#define CALIB_Ax_VALUE 0.01
#define CALIB_Ay_VALUE 0.01
#define CALIB_Az_VALUE 0.01

#define SAMPLING_TIME 	0.004 	// s
#define STD_DEV_MEA		3		// degree per second
#define STD_DEV_EST 	4  		// degree per second
#define E_est SAMPLING_TIME*SAMPLING_TIME*STD_DEV_EST*STD_DEV_EST
/* END User Configurations */

/* MPU6050 typedefs and define */
#define MPU6050_ADDR 0xD0
#define RAD_TO_DEG 57.295779513082320876798154814105
#define PHYSICAL_CONVERT_GYRO 65.5
#define PHYSICAL_CONVERT_ACCEL 4096
#define TIME_REF 2000
#define i2c_timeout 100

typedef struct
{
    int16_t angle;
    // User define initial estimate error
    double estimate_error;
} Kalman_t;

typedef struct
{
	int16_t X_raw;
	int16_t Y_raw;
	int16_t Z_raw;

	double rateRoll;
	double ratePitch;
	double rateYaw;
	double ref_point_X;
	double ref_point_Y;
	double ref_point_Z;

	double Ax;
	double Ay;
	double Az;
	double angleRoll;
	double anglePitch;

	Kalman_t Kalman_X;
	Kalman_t Kalman_Y;
} MPU_6050;
/* END MPU6050 typedefs and define */

/* MPU6050 functions */
uint8_t mpu6050_init(MPU_6050 * mpu);
void get_ref_point(MPU_6050 * mpu);
void concat_raw_data(MPU_6050 * mpu, uint8_t * raw_data);
void mpu6050_update_all(MPU_6050 * mpu);
double Kalman_getAngle(Kalman_t * Kalman, double newAngle, double newRate);
/* END MPU6050 functions */

/* MPU6050 Registers */
#define WHO_AM_I 		0x75
#define CONFIG 			0x1A
#define PWR_MGMT_1 		0x6B
#define SMPLRT_DIV 		0x19
#define ACCEL_CONFIG 	0x1C
#define ACCEL_XOUT_H 	0x3B
#define TEMP_OUT_H 		0x41
#define GYRO_CONFIG 	0x1B
#define GYRO_XOUT_H 	0x43
/* END MPU6050 Registers */

#endif /* INC_MPU6050_H_ */
