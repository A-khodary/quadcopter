#include <wiringPiI2C.h>

void pca9685_init(int address);
void pca9685_send_all(int address);
void pca9685_send(int address, long value, int led);
void pca9685_motor_power(int address, int percent, int led);

