#include "shared_librairies.h"
#include "pilot_global_variables.h"

#include "reader_global_variables.h"
#include "linux/i2c-dev.h"
#include "linux/i2c.h"

int writeCommands();
void pilotHandler();
int initPca9685();
