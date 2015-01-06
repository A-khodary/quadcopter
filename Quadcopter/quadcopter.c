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

// IMU initialization : to place in an IMU.c source

RTIMU *imu;
RTIMU_DATA imuData;


RTIMUSettings *settings = new RTIMUSettings("wewillfly");
imu = RTIMU::createIMU(settings);
imu->IMUInit();


// Global variables definition :

pilotCommandsShared quadcopterPilotCommandsShared;
receivedCommandsShared userCommands;



// Test initialization :

quadcopterPilotCommandsShared.chan1 = 66;
quadcopterPilotCommandsShared.chan2 = 6;
quadcopterPilotCommandsShared.chan3 = 23;
quadcopterPilotCommandsShared.chan4 = 67;
quadcopterPilotCommandsShared.chan5 = 12;
quadcopterPilotCommandsShared.chan6 = 89;
quadcopterPilotCommandsShared.chan7 = 3;
quadcopterPilotCommandsShared.chan8 = 35;

pthread readerThread, writerThread;


pthread_create (&readerThread, NULL, &readCommands(),NULL);





return 0;
}
