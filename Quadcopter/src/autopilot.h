#ifndef AUTOPILOT
#define AUTOPILOT

#include "shared_librairies.h"
#include "autopilot_global_variables.h"
#include "imu_globals_variables.h"


// Some PID coefficient defines :

#define LANDTAKEOFFXP 1
#define LANDTAKEOFFXPI 1
#define LANDTAKEOFFXPD 1
#define LANDTAKEOFFYP 1
#define LANDTAKEOFFYPI 1
#define LANDTAKEOFFYPD 1
#define LANDTAKEOFFZP 1
#define LANDTAKEOFFZPI 1
#define LANDTAKEOFFZPD 1
#define LANDTAKEOFFYAWP 1
#define LANDTAKEOFFYAMPI 1
#define LANDTAKEOFFYAWPD 1


void trajAsserv(autopilotObjective_t* autopilotObjective);
void * autopilotHandler(void * arg);

#endif
