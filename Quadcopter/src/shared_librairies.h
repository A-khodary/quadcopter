#ifndef SHARED
#define SHARED

#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <wiringPiI2C.h>
#include <unistd.h> // needed for the read function in order to read all arduino bytes at once


#include "inter-thread-messages.h"

#endif
