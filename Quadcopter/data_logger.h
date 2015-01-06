#include "shared_librairies.h"
#include "data_logger_global_variables.h"

#include "imu_globals_variables.h"
#include "reader_global_variables.h"
#include "autopilot_global_variables.h"
#include <mavlink/v1.0/common/mavlink.h>

// Structures :



// Some software defines :

LOG_IMU 1
IMU_LOGGING_PERIOD 2

LOG_AUTOPILOT 1
AUTOPILOT_LOGGING_PERIOD 1

// TODO : continue

// Functions prototypes

void dataLoggerHandler();
int sendData();
int receiveData();


