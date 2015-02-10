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

printDebug("Main thread is initializing...")


// Threads declaration

pthread readerThread, writerThread, pilotThread, dataLoggerThread, autopilotThread, imuThread;



// IMU initialization : to place in an IMU.c source

RTIMU *imu;
RTIMU_DATA imuData;


RTIMUSettings *settings = new RTIMUSettings("wewillfly");
imu = RTIMU::createIMU(settings);
imu->IMUInit();


// Global variables definition : (normally defined in specific component handler init)


// Main handler init :

handler_t* mainITMHandler = initializeHandler();
if (mainITMHandler == NULL)
{
    printDebug("Main handler init error");
    return 0;
}



// Thread handler filling in :

//  Autopilot :


handler_t* autopilotITMHandler() = initializeHandler();
if (autopilotITMHandler == NULL)
{
    printDebug("Autopilot handler init error");
    return 0;
}

bidirectionalHandler_t autopilotBidirectionalHandler;
autopilotBidirectionalHandler.mainITMHandler = &mainITMHandler;
autopilotBidirectionalHandler.componentITMHandler = &autopilotITMHandler;


//  Pilot

handler_t* pilotITMHandler() = initializeHandler();
if (pilotITMHandler == NULL)
{
    printDebug("Pilot handler init error");
    return 0;
}

bidirectionalHandler_t pilotBidirectionalHandler;
pilotBidirectionalHandler.mainITMHandler = &mainITMHandler;
pilotBidirectionalHandler.componentITMHandler = &pilotITMHandler;

// Data logger :

handler_t* dataLoggerITMHandler() = initializeHandler();
if (dataLoggerITMHandler == NULL)
{
    printDebug("Data Logger handler init error");
    return 0;
}


bidirectionalHandler_t dataLoggerBidirectionalHandler;
dataLoggerBidirectionalHandler.mainITMHandler = &mainITMHandler;
dataLoggerBidirectionalHandler.componentITMHandler = &dataLoggerITMHandler;



// Test initialization :


// End of test initialization


printDebug("Launching components threads...")


pthread_create(&readerThread, NULL, readerHandler, (void*)mainITMHandler);
pthread_create(&writerThread, NULL, writerHandler, (void*)mainITMHandler);
pthread_create(&pilotThread, NULL, pilotHandler, (void*)pilotBidirectionalHandler);
pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, (void*)dataLoggerBidirectionalHandler);
pthread_create(&autopilotThread, NULL, autopilotHandler, (void*)autopilotBidirectionalHandler);
pthread_create(&imuThread, NULL, imuHandler, (void*)autopilotBidirectionalHandler);

// Message handling :

while(1)
{
    message_t* currentMessage;
    currentMessage = retrieveMessage(mainITMHandler);


    // TODO handling message :

    /* Handling message documentation :

    autopilot_init
    autopilot_end_init
    autopilot_objective_file_not_found



    */


    //
}
//



return 0;
}
