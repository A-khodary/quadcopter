
#include "shared_librairies.h"
#include "pilot_global_variables.h"
#include "reader_global_variables.h"

#include <pca9685.h>
#include <wiringPi.h>

#ifndef PILOT
#define PILOT


void writeCommands();
void* pilotHandler(void* arg);
int calcTicks(float impulseMs, int hertz);

#endif
