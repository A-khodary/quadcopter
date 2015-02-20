//#include "imu.h"

/*
void imuHandler(void* arg)
 {

     handler_t* mainITMHandler;
     mainITMHandler = (handler_t*)arg;

    // Notifying main thread of init :

    message_t currentMessage;
    strcpy(currentMessage.message, "main_imu_info_imuinitialized");
    currentMessage.priority = 20;
    sendMessage(mainITMHandler, currentMessage);

    // Init IMU

    RTIMUSettings *settings = new RTIMUSettings("wewillfly");
    imu = RTIMU::createIMU(settings);
    imu->IMUInit();

    // GPS init

    initializeDataGPS();

    // BMP085 init

    //initializeBMP085();

    // Send the data to the global variables

    flightStateShared.roll = imuData.fusionPose.x()* RTMATH_RAD_TO_DEGREE ;
    flightStateShared pitch= imuData.fusionPose.y()* RTMATH_RAD_TO_DEGREE;
    flightStateShared.yaw = imuData.fusionPose.z()* RTMATH_RAD_TO_DEGREE;

    // To check

    double imuDataAltitude = 44330*(1-pow(pow((imuData.pressure/101325),(1/5255)),1000));

    rawPositionShared.altitude = dataGPS.altitude;
    rawPositionShared.longitude = dataGPS.longitude;
    rawPositionShared.altitude = (dataGPS.altitude*0.5+imuDataAltitude*0.5);

    /*

    // Directional vector

    positionShared.x = cos(flightStateShared.roll)*cos(flightStateShared.pitch);
    positionShared.y = sin(flightStateShared.roll)*cos(flightStateShared.pitch);
    positionShared.z = sin(flightStateShared.pitch);



    message_t message1,message2;
    strcpy(message1.message,"reader_imu_order_ultrasonicon");
    strcpy(message2.message,"reader_imu_order_ultrasonicoff");

    if (rawPositionShared.altitude < 4)
    {
        sendMessage(mainITMHandler,message1);
        rawPositionShared.altitude = (dataGPS.altitude*0.1+imuDataAltitude*0.1+(double)receivedCommands.ultrasonicTelemeter*100*0.8);

    }
     if (rawPositionShared.altitude < 0.5)
    {
        sendMessage(mainITMHandler,message2);
        rawPositionShared.altitude = (dataGPS.altitude*0.1+imuDataAltitude*0.1+(double)receivedCommands.ultrasonicTelemeter*100*0.8);

    }


 }

*/

