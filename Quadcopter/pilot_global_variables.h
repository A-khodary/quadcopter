// Some defines for pilot mode :

#define USER_DIRECT 0
#define AUTOPILOT 1
#define MIXED 2

// Global variables structures declaration

typedef struct pilotCommandsShared
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

} pilotCommandsShared;

typedef struct pilotStateShared // Pilot state for datalogging
{
    int pilotMode;

}


// Global variables definitions

extern pilotCommandsShared quadcopterPilotCommandsShared;

