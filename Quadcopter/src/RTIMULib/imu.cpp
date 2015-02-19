#include "imu.h"

void imuHandler(void* arg)
 {
     handler_t* mainITMHandler;
     mainITMHandler = (handler_t*)arg;

    // Notifying main thread of init :

    message_t currentMessage;
    strcpy(currentMessage.message, "main_imu_init");
    currentMessage.priority = 20;
    sendMessage(mainITMHandler, currentMessage);

    // TODO Init IMU

    RTIMU *imu;
    RTIMU_DATA imuData;

    RTIMUSettings *settings = new RTIMUSettings("wewillfly");
    imu = RTIMU::createIMU(settings);
    imu->IMUInit();
    imuData.fusionPose ;


 }


