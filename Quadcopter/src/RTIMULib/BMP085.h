#ifndef BMP_H
#define BMP_H

//#include "../shared_librairies.h"


#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
//#include <smbus.h>

#define BMP085_I2C_ADDRESS 0x77

const unsigned char BMP085_OVERSAMPLING_SETTING = 3;

int bmp085_i2c_Begin();
__s32 bmp085_i2c_Read_Int(int fd, __u8 address); //????
void bmp085_i2c_Write_Byte(int fd, __u8 address, __u8 value);
void bmp085_i2c_Read_Block(int fd, __u8 address, __u8 length, __u8 *values);
unsigned int bmp085_ReadUT();
unsigned int bmp085_ReadUP();
unsigned int bmp085_GetPressure(unsigned int up);
unsigned int bmp085_GetTemperature(unsigned int ut);

#endif
