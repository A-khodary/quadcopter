#include "imu.h"

void imuHandler(void* arg)
 {
     handler_t* mainITMHandler;
     mainITMHandler = (handler_t*)arg;

    // Notifyng main thread of init :

    message_t currentMessage;
    currentMessage.message = "main_imu_init";
    currentMessage.priority = 20;
    sendMessage(mainITMHandler, currentMessage);

    // TODO Init IMU


 }



