#include "RTIMULib.h"

RTIMU *imu;
RTIMU_DATA imuData;


RTIMUSettings *settings = new RTIMUSettings("wewillfly");
imu = RTIMU::createIMU(settings);
imu->IMUInit();
