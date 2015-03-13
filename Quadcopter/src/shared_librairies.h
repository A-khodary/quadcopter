#ifndef SHARED
#define SHARED

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>

#include <pthread.h>
#include <unistd.h> // needed for the read function in order to read all arduino bytes at once
#include <signal.h> // needed for the timers in autopilot (not yet implemented)
#include <time.h>
#include <sys/time.h>
#include <proj_api.h>


#include "inter-thread-messages.h"
#include "global_functions.h"


#endif
