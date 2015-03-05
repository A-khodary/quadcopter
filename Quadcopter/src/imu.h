#ifndef IMU
#define IMU

#include "shared_librairies.h"

#include "imu_globals_variables.h"
#include "reader_global_variables.h"

#include "RTIMULib/NEO-6M.h" //TODO with UBLOX parsing
#include "RTIMULib/BMP085.h"
#include "RTIMULib/RTIMULib.h"

//Handler prototype :
void imuHandler(void* arg);

#endif


