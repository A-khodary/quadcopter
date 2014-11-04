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
#include "reader.h"

//          ###     Shared ressources includes      ###

#include "global_functions.h"


int main()
{


RTIMU *imu;
RTIMU_DATA imuData;


RTIMUSettings *settings = new RTIMUSettings("wewillfly");
imu = RTIMU::createIMU(settings);
imu->IMUInit();

// Initialization section :







return 0;
}
