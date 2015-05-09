#include "shared_librairies.h"

#ifndef AUTOPILOT_GLOB
#define AUTOPILOT_GLOB

// Some defines :

// Objective code defines :

#define LAND_TAKEOFF 1
#define GOTO_STANDARD 2
#define GOTO_HOVERING 3
#define POSITION_HOLD 4

// Fifo defines :




// Global variables and structures declarations :


typedef struct autopilotObjective_t
{
    char name[64]; // the objective name that will be displayed
    int priority; // Priority from 0 to 100

    int code; // Objective code : for a complete list, refer to documentation

    // Those elements may not be necessary ( depending from the objective code )

    double destinationX;
    double destinationY;
    double destinationZ;
    double directionBearing;//direction de l'objectif

    double destinationLat;
    double destinationLong;
    double destinationAlt;

    double destinationDistXY; // Distance in m on the X-Y plan
    double maxSpeedXY; // Max speed in m*s-1 on the X-Y plan

    double destinationDist;
    double maxSpeed; // Max speed in m*s-1

    struct autopilotObjective_t* previousObjective; // For FIFO management
    struct autopilotObjective_t* nextObjective; // For FIFO management

}autopilotObjective_t;





typedef struct autopilotSharedState
{

    pthread_mutex_t readWrite;

    int landed; // 1 : landed 0 : in flight
    int crashed; // 1 : crashed 0 : not crashed

    int engaged; // 1 : engaged 0: disengaged

    int stressLevel; // symbolize the autopilot reactions to its environnement : four levels :
    // 1 : normal stress
    // 2 : something is not normal
    // 3 : things are not going well : landing asap
    // 4 : we're in shit : emergency landing

    int currentObjectivePriority;



}autopilotSharedState_t;


// Global variables declarations


extern float landTakeOffCoeff[4][3]; // PID coefficients for Land/Takeoff
extern float gotoStandardCoeff[3][3]; // PID coefficients for goto_standard
//extern float gotoHoverCoeff[4][3]; // PID coefficients for goto_hovering
extern float gotoHoverCoeff[3][3]; // PID coefficients for goto_hovering
extern float positionHoldCoeff[3][3]; // PID coefficients for position_hold


extern autopilotSharedState_t autopilotSharedState;

#endif


