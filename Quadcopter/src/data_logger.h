#ifndef DATA_LOGGER
#define DATA_LOGGER

#include "shared_librairies.h"
#include "data_logger_global_variables.h"

#include "imu_globals_variables.h"
#include "reader_global_variables.h"
#include "autopilot_global_variables.h"
//#include <mavlink/v1.0/common/mavlink.h>

// Structures :



// Some software defines :

#define LOG_IMU 1
#define IMU_LOGGING_PERIOD 2

#define LOG_AUTOPILOT 1
#define AUTOPILOT_LOGGING_PERIOD 1

// TODO : continue



// Functions prototypes

void* dataLoggerHandler(void * arg);
int sendData();
int receiveData();


#endif
