#include "imu.h"

//PIN configuration

//Global variables

//Register map

#define MPU6050_SMPLRT_DIV 0x19
#define MPU6050_CONFIG 0x1a
#define MPU6050_GYRO_CONFIG 0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_FIFO_EN 0x23
#define MPU6050_I2C_MST_CTRL 0x24
#define MPU6050_I2C_SLV0_ADDR 0x25
#define MPU6050_I2C_SLV0_REG 0x26
#define MPU6050_I2C_SLV0_CTRL 0x27
#define MPU6050_I2C_SLV1_ADDR 0x28
#define MPU6050_I2C_SLV1_REG 0x29
#define MPU6050_I2C_SLV1_CTRL 0x2a
#define MPU6050_I2C_SLV4_CTRL 0x34
#define MPU6050_INT_PIN_CFG 0x37
#define MPU6050_INT_ENABLE 0x38
#define MPU6050_INT_STATUS 0x3a
#define MPU6050_ACCEL_XOUT_H 0x3b
#define MPU6050_ACCEL_XOUT_L 0x3c
#define MPU6050_ACCEL_YOUT_H 0x3d
#define MPU6050_ACCEL_YOUT_L 0x3e
#define MPU6050_ACCEL_ZOUT_H 0x3f
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H 0x41
#define MPU6050_TEMP_OUT_L 0x42
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48
#define MPU6050_I2C_SLV1_DO 0x64
#define MPU6050_I2C_MST_DELAY_CTRL 0x67
#define MPU6050_USER_CTRL 0x6a
#define MPU6050_PWR_MGMT_1 0x6b
#define MPU6050_PWR_MGMT_2 0x6c
#define MPU6050_FIFO_COUNT_H 0x72
#define MPU6050_FIFO_R_W 0x74
#define MPU6050_WHO_AM_I 0x75

// sample rate defines (applies to gyros and accels, not mags)
#define MPU6050_SAMPLERATE_MIN 5 // 5 samples per second is the lowest
#define MPU6050_SAMPLERATE_MAX 1000 // 1000 samples per second is the absolute maximum
