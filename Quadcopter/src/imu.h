#ifndef IMU_h
#define IMU_h

#include "shared_librairies.h"

#include "imu_globals_variables.h"
#include "reader_global_variables.h"

#include "RTIMULib/RTIMULib.h"

#define GPS_MAX_ATTEMPS 35

#define GROUND_MS5611_TRUST 80
#define GROUND_GPS_TRUST 20

#define AIR_MS5611_TRUST 90
#define AIR_GPS_TRUST 10
#define MAX_ALT_DIFF 0.30

#define NUMBER_OF_AVERAGE 200

#define DISPLAY 0 //1 to display accel data on console
#define DEBUG 0 // 1 to test and debug IMU

#define DISPLAY_GPS 1

//Handler prototype :
void* imuHandler(void* arg);

// Other function prototype :
int waitForGPS();
void makeHome(int MS5611Alt);
//double calculateBmpAlt();

#endif


