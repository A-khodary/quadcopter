#ifndef AUTOPILOT
#define AUTOPILOT

#include "shared_librairies.h"
#include "autopilot_global_variables.h"
#include "autopilot_global_functions.h"
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

#define OBJECTIVES_PATH objectives.txt

// Some mode defines :

#define GOTO_STANDARD 1
#define LAND_TAKEOFF 2
#define GOTO_OVERING 3
#define POSITION_HOLD 4


void trajAsserv(autopilotObjective_t* autopilotObjective);
void * autopilotHandler(void * arg);

typedef struct autopilotObjectiveFifo
{
    autopilotObjective* ObjectiveFifo[AUTOPILOT_OBJECTIVE_FIFO_SIZE];

    int currentObjectivePriority;
    int numberOfObjectivesPending;
    autopilotObjective_t firstObjective;

    pthread_mutex_t readWrite;

}autopilotObjectiveFifo_t;


#endif
