//#use I2C(master, slow, i2c1, force_hw)
#include "pca9685_reg.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

void pca9685_init(int address);
void pca9685_send_all(int address);
void pca9685_send(int address, long value, int led);
void pca9685_brightness(int address, int percent, int led);
void PCA9685AllLedOff(int address);
