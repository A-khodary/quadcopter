
#ifndef PILOT_GLOBAL_V
#define PILOT_GLOBAL_V

#include "shared_librairies.h"


// Some defines for pilot mode :

#define USER_DIRECT 0
#define AUTOPILOT 1
#define MIXED 2

#define AUTOPILOT_NORMAL 1
#define AUTOPILOT_EMERGENCY_LANDING 2
#define MANUAL 3
#define CUTOFF 4
#define AUTOPILOT_LANDING 5
#define AUTOPILOT_RTH 6
#define TEST 7


// Global variables structures declaration

typedef struct pilotCommandsShared_t
{
    double chan1; // 0-100%
    double chan2; // 0-100%
    double chan3; // 0-100%
    double chan4; // 0-100%
    double chan5; // 0-100%
    double chan6; // 0-100%
    double chan7; // 0-100%
    double chan8; // 0-100%
    double chan9; // 0-100%

    int refreshingPeriod;

    pthread_mutex_t readWrite;

} pilotCommandsShared_t;

typedef struct pilotStateShared_t // Pilot state for datalogging
{
    int pilotMode;
    pthread_mutex_t readWriteMutex;

}pilotStateShared_t;


// Global variables definitions

extern pilotCommandsShared_t pilotCommandsShared;
extern pilotStateShared_t pilotStateShared;

#endif
