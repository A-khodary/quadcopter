/*
###############################################
#                                                                               #
#   Quadcopter V0.0                                                   #
#   Vivien Billaud and Nicolas de Maubeuge                  #
#   Ensea 2014                                                          #
#   All rights reserved                                                 #
#                                                                              #
##############################################


*/


#include "shared_librairies.h"


//          ###     Components includes     ###

#include "autopilot.h"
#include "data_logger.h"
#include "RTIMULib.h"
#include "osd.h"
#include "pilot.h"
#include "reader_global_functions.h"

//          ###     Shared functions includes      ###

#include "global_functions.h"
#include "autopilot_global_functions.h"
#include "data_logger_global_functions.h"


//          ###     Shared variables includes      ###

#include "reader_global_variables.h"
#include 'pilot_global_variables.h"


int main()
{

// Threads declaration

pthread readerThread, writerThread, pilotThread, dataLoggerThread, autopilotThread, imuThread;



// IMU initialization : to place in an IMU.c source

RTIMU *imu;
RTIMU_DATA imuData;


RTIMUSettings *settings = new RTIMUSettings("wewillfly");
imu = RTIMU::createIMU(settings);
imu->IMUInit();


// Global variables definition : (normally defined in specific component handler init)


// Thread handler filling in :


// Test initialization :


pthread_create(&readerThread, NULL, readerHandler, NULL);
pthread_create(&writerThread, NULL, writerHandler, NULL);
pthread_create(&pilotThread, NULL, pilotHandler, NULL);
pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, NULL);
pthread_create(&autopilotThread, NULL, autopilotHandler, NULL);
pthread_create(&imuThread, NULL, imuHandler, NULL);


//



return 0;
}
