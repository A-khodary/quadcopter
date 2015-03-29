#include "imu.h"

flightStateShared_t flightStateShared;
positionShared_t positionShared;
rawPositionShared_t homeRawPosition;
rawPositionShared_t rawPositionShared;
homePosition_t homePosition;

void* imuHandler(void* arg)
 {
    bidirectionnalHandler_t* bidirectionnalHandler;
    bidirectionnalHandler = (bidirectionnalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* imuITMHandler;

    mainITMHandler = bidirectionnalHandler->mainITMHandler;
    imuITMHandler = bidirectionnalHandler->componentITMHandler;

    // Initialization of shared variables :

    initialize_mutex(&flightStateShared.readWriteMutex);
    initialize_mutex(&rawPositionShared.readWriteMutex);
    initialize_mutex(&positionShared.readWriteMutex);

    // Initialisation of handler variables :

    int waitingGPS = 1;

    // Notifying main thread of init :

    message_t currentMessage;
    strcpy(currentMessage.message, "main_imu_info_init");
    currentMessage.priority = 20;
    sendMessage(mainITMHandler, currentMessage);

    // Init IMU

    // Waiting for GPS FIX :

    while(waitingGPS)
    {
        pthread_mutex_lock(&receivedCommands.readWriteMutex);

        if(receivedCommands.gpsStatus == FIX) waitingGPS = 0;
        sleep(1);

        pthread_mutex_lock(&receivedCommands.readWriteMutex);
    }


    RTIMU_DATA imuData;
    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");
    RTIMU *imu = RTIMU::createIMU(settings);
    imu->IMUInit();

    // GPS init TODO : use UBLOX format


    // BMP085 init

    bmp085_i2c_Begin(); // TODO : test bmp init
    unsigned int pressure, temperature;
    unsigned int up, ut;
    unsigned int bmpAltitude;

    // Ultrasonic init :

    unsigned int ultrasonicAltitude;
    // For now, turning ultrasonic ON on startup :

    strcpy(currentMessage.message ,"reader_imu_order_ultrasonicon");
    currentMessage.priority = 20;
    sendMessage(mainITMHandler, currentMessage);



    // Notify main thread of end of init :

    strcpy(currentMessage.message ,"main_imu_info_endofinit");
    currentMessage.priority = 20;
    sendMessage(mainITMHandler, currentMessage);




    // Entering handler loop Area :

    while(1)
    {
        // Sensor updating Area :

        // Updating IMU :

        imu->IMURead();
        imuData = imu->getIMUData();

        // Updating BMP :

        ut = bmp085_ReadUT();
        up = bmp085_ReadUP();

        pressure = bmp085_GetPressure(up);
        temperature = bmp085_GetTemperature(ut);

        // Calculating bmp altitude :

        bmpAltitude = 44330*(1-pow(pow((pressure/101325),(1/5255)),1000));


        // Altitude calculation Area :

        pthread_mutex_lock(&receivedCommands.readWriteMutex);
        ultrasonicAltitude = receivedCommands.ultrasonicTelemeter;

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);

        // TODO : calculate altitude



        // Position calculation Area :





        // Locking the relative Mutex :

        pthread_mutex_lock(&flightStateShared.readWriteMutex);

        // Sending the altitude data to the global variables

        flightStateShared.roll = imuData.fusionPose.x()* RTMATH_RAD_TO_DEGREE ;
        flightStateShared.pitch = imuData.fusionPose.y()* RTMATH_RAD_TO_DEGREE;
        flightStateShared.yaw = imuData.fusionPose.z()* RTMATH_RAD_TO_DEGREE;

        // Sending the altitude data to global variables :

        //TODO


        // Sending the position data to global variables :

        // TDODO

    }

 }


