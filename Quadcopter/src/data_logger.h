#ifndef DATA_LOGGER
#define DATA_LOGGER

#include "shared_librairies.h"
#include "data_logger_global_variables.h"

#include "imu_globals_variables.h"
#include "reader_global_variables.h"
#include "autopilot_global_variables.h"

#include "mavlink/common/mavlink.h"

// Structures :



// Some software defines :

#define LOG_IMU 1
#define IMU_LOGGING_PERIOD 2

#define LOG_AUTOPILOT 1
#define AUTOPILOT_LOGGING_PERIOD 1

// Some mavlink defines:

#define BUFFER_LENGTH 2041

#define GCS_IP "192.168.1.107"
#define GCS_PORT 14550



// Functions prototypes

void* dataLoggerHandler(void * arg);

// Mavlink prototypes :

int initSocket(char targetIp[100], int targetPort, struct sockaddr_in gcAddr);


#endif
