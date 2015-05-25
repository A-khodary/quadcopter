#include "imu.h"


flightStateShared_t flightStateShared;
positionShared_t positionShared;
rawPositionShared_t homeRawPosition;
rawPositionShared_t rawPositionShared;
homePosition_t homePosition;

void* imuHandler(void* arg)
 {

    handler_t* mainITMHandler;
    mainITMHandler = (handler_t*)arg;

    // Initialization of shared variables :
    initialize_mutex(&flightStateShared.readWriteMutex);
    initialize_mutex(&rawPositionShared.readWriteMutex);
    initialize_mutex(&positionShared.readWriteMutex);
    initialize_mutex(&homePosition.readWriteMutex);
    initialize_mutex(&homeRawPosition.readWriteMutex);


    // Notifying main thread of init :
    message_t currentMessage;
    strcpy(currentMessage.message, "main_imu_info_init");
    currentMessage.priority = 20;
    sendMessage(mainITMHandler, currentMessage);

    // Initialization of some handler variables :
    double gpsAltitude=0;
    double altitude=0;

    int sampleCount = 0;
    int sampleRate = 0;
    uint64_t rateTimer;
    uint64_t displayTimer;
    uint64_t now;

    int transitionToUltrasonic = 0;
    int transitionToGPSMS5611 = 0;
    int firstTimeUltra =1;
    int firstTimeGPSMS5611=1;
    int averageCounter = 0;

    int homeMade = 0;

    double latitude;
    double longitude;

    double xComputed;
    double yComputed;

    RTFLOAT MS5611Alt;

    // Waiting for GPS FIX :

    if (DEBUG)
    {
       receivedCommands.latitude = 1;
       receivedCommands.longitude = 1;
       receivedCommands.altitude = 1;
    }
    else
    {
      if (!waitForGPS())
      {
          printDebug("[e] Failed to obtain 3D fix from GPS after several attemps, killing IMU...");
          strcpy(currentMessage.message ,"main_imu_info_initfailed");
          currentMessage.priority = 20;
          sendMessage(mainITMHandler, currentMessage);
          pthread_exit(NULL);
      }

      else printDebug("[i] Got 3D fix from GPS !");
    }

    //  using RTIMULib here allows it to use the .ini file generated by RTIMULibDemo.

    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

    RTIMU *imu = RTIMU::createIMU(settings);
    RTPressure *pressure = RTPressure::createPressure(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {

        printDebug("[e] No IMU found\n");
        strcpy(currentMessage.message ,"main_imu_info_initfailed");
        currentMessage.priority = 20;
        sendMessage(mainITMHandler, currentMessage);

        pthread_exit(NULL);

    }


    //  This is an opportunity to manually override any settings before the call IMUInit

    //  set up IMU

    imu->IMUInit();

    // this is a convenient place to change fusion parameters

    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);

    //  set up pressure sensor

    if (pressure != NULL)
        pressure->pressureInit();

    //  set up for rate timer

    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();

    double air_MS5611_trust = AIR_MS5611_TRUST;
    double air_gps_trust = AIR_GPS_TRUST;

    // Ultrasonic init :
    unsigned int ultrasonicAltitude=0;
    unsigned int ultrasonicAltitudeLastValue=0;

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
        //  poll at the rate recommended by the IMU

        usleep(imu->IMUGetPollInterval() * 1000);

        while (imu->IMURead()) {

 	    RTIMU_DATA imuData = imu->getIMUData();

            //  add the pressure data to the structure
            if (pressure != NULL)
            {
                pressure->pressureRead(imuData);

                if (!homeMade)
                {
                    // Making home position :
                    MS5611Alt = RTMath::convertPressureToHeight(imuData.pressure);
                    makeHome(MS5611Alt);
                    homeMade = 1;
                }
            }
            else MS5611Alt = 0;



            if(DISPLAY)
	    {

	    sampleCount++;

            now = RTMath::currentUSecsSinceEpoch();

            //  display 10 time per second
            if ((now - displayTimer) > 100000) {
                printf("Sample rate %d: %s\n", sampleRate, RTMath::displayDegrees("", imuData.fusionPose));
                fflush(stdout);

                if (pressure != NULL) {
                    printf("Pressure: %4.1f, height above sea level: %4.1f, temperature: %4.1f\n",
                           imuData.pressure, RTMath::convertPressureToHeight(imuData.pressure), imuData.temperature);
                    fflush(stdout);
                }

                fflush(stdout);
                displayTimer = now;
            }

            //  update rate every second
            if ((now - rateTimer) > 1000000) {
                sampleRate = sampleCount;
                sampleCount = 0;
                rateTimer = now;
            }
	    }


        // Altitude calculation Area :
        pthread_mutex_lock(&receivedCommands.readWriteMutex);

	if (DEBUG)
	{
		 ultrasonicAltitude = 10;
		 gpsAltitude = 10;
	}
	else
	{
        	ultrasonicAltitude = receivedCommands.ultrasonicTelemeter;
        	gpsAltitude = receivedCommands.altitude;
	}

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);



        if (ultrasonicAltitude == -1) // We are far from ground
        {
            if ((!transitionToGPSMS5611) && firstTimeUltra)
            {
                transitionToGPSMS5611 = 1;
                transitionToUltrasonic = 0;
                firstTimeUltra = 0;
                firstTimeGPSMS5611 = 1;
            }

            if ( abs(MS5611Alt - gpsAltitude) / MS5611Alt >= MAX_ALT_DIFF )
            {
                printf("[e] Error in IMU : to high divergence between gps altitude and bmp altitude");
                // Switching altitude to pressure-only :
                air_MS5611_trust=100;
                air_gps_trust=0;
            }

            if (transitionToGPSMS5611)
            {
                altitude = (averageCounter*((air_MS5611_trust*MS5611Alt + air_gps_trust*gpsAltitude) / 100) + (NUMBER_OF_AVERAGE-averageCounter)*ultrasonicAltitudeLastValue);
                if (averageCounter <= NUMBER_OF_AVERAGE) averageCounter++;
                else transitionToGPSMS5611 = 0;
            }

            else altitude = (air_MS5611_trust*MS5611Alt + air_gps_trust*gpsAltitude) / 100;
        }

        else // We are close to ground
        {
            if ((!transitionToUltrasonic) && firstTimeGPSMS5611)
            {
                transitionToUltrasonic = 1;
                transitionToGPSMS5611 = 0;
                firstTimeGPSMS5611 = 0;
                firstTimeUltra = 1;
            }

            if (transitionToUltrasonic)
            {
                altitude = ((NUMBER_OF_AVERAGE - averageCounter)*((air_MS5611_trust*MS5611Alt + air_gps_trust*gpsAltitude) / 100) + (averageCounter)*ultrasonicAltitude);
                if (averageCounter <= NUMBER_OF_AVERAGE) averageCounter++;
                else transitionToUltrasonic = 0;
            }

        }



        // Attitude calculation Area :

        RTVector3 *RollPitchYaw = new RTVector3();

        // Position calculation area :

        pthread_mutex_lock(&receivedCommands.readWriteMutex);

        latitude = receivedCommands.latitude;
        longitude = receivedCommands.longitude;

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);

        convertPlanarToHome(&xComputed, &yComputed, latitude, longitude);

        if (DISPLAY_GPS)
        {
            printf("Got new planar position. x : %f, y : %f\n", xComputed, yComputed);
        }

        // Locking the global variables Mutexes :

        pthread_mutex_lock(&flightStateShared.readWriteMutex);
        pthread_mutex_lock(&rawPositionShared.readWriteMutex);
        pthread_mutex_lock(&positionShared.readWriteMutex);

        // Sending the data to the global variables

        imuData.fusionPose.accelToEuler(*RollPitchYaw);

        flightStateShared.roll = RollPitchYaw->x();
        flightStateShared.pitch = RollPitchYaw->y();
        flightStateShared.yaw = RollPitchYaw->z();

        rawPositionShared.altitude = altitude;
        rawPositionShared.longitude = receivedCommands.longitude;
        rawPositionShared.latitude = receivedCommands.latitude;

        positionShared.x = xComputed;
        positionShared.y = yComputed;
        positionShared.z = altitude;
        positionShared.distanceFromGround = altitude - homePosition.z;//assuming the ground is flat

        pthread_mutex_unlock(&flightStateShared.readWriteMutex);
        pthread_mutex_unlock(&rawPositionShared.readWriteMutex);
        pthread_mutex_unlock(&positionShared.readWriteMutex);


        }

    }

 }

 int waitForGPS()
 {
    int waitingGPS = 1;
    int attemps = 0;

    while(waitingGPS)
    {
        pthread_mutex_lock(&receivedCommands.readWriteMutex);

        if(receivedCommands.gpsStatus == FIX) waitingGPS = 0;
        else
        {
            attemps ++;
            if (attemps >= GPS_MAX_ATTEMPS) return 0;
        }

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);
        sleep(1);
    }
    return 1;

 }


 void makeHome(int MS5611Alt)
 {
     printDebug("[i] Setting current position as home");

     pthread_mutex_lock(&homePosition.readWriteMutex);
     pthread_mutex_lock(&receivedCommands.readWriteMutex);

     homePosition.latitude = receivedCommands.latitude;
     homePosition.longitude = receivedCommands.longitude;
     homePosition.z = (GROUND_MS5611_TRUST*MS5611Alt + GROUND_GPS_TRUST*receivedCommands.altitude) / 100;

     if (DISPLAY_GPS)
     {
        printf("[i] New home. latitude : %f, longitude : %f, altitude :%f\n", homePosition.latitude, homePosition.longitude, homePosition.z);
        fflush(stdout);
     }

     convertPlanar(&homePosition.x, &homePosition.y, homePosition.latitude, homePosition.longitude);

     pthread_mutex_unlock(&homePosition.readWriteMutex);
     pthread_mutex_unlock(&receivedCommands.readWriteMutex);
 }

