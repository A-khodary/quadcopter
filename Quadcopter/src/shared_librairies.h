#ifndef SHARED
#define SHARED

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <pthread.h>
#include <wiringPiI2C.h>
#include <unistd.h> // needed for the read function in order to read all arduino bytes at once
#include <signal.h> // needed for the timers in autopilot
#include <time.h>


#include "inter-thread-messages.h"
#include "proj/proj_api.h"
#include "imu_globals_variables.h"
#include "global_functions.h"


#endif
