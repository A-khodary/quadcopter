// Some defines :

// Objective code defines :

#define LAND_TAKEOFF 1
#define GOTO_STANDARD 2
#define GOTO_HOVERING 3
#define POSITION_HOLD 4

// Fifo defines :

#define AUTOPILOT_OBJECTIVE_FIFO_SIZE 256


// Global variables and structures declarations :


typedef struct autopilotObjective
{
    char name[64]; // the objective name that will be displayed
    int priority; // Priority from 0 to 100

    int code; // Objective code : for a complete list, refer to documentation

    // Those elements may not be necessary ( depending from the objective code )

    double destinationX;
    double destinationY;
    double destinationZ;
    double directionYAW;//direction de l'objectif

    double max_speed; // Max speed in ms-1



    pthread_mutex_t readWrite;

}autopilotObjective_t;

typedef struct autopilotObjectiveFifo
{
    autopilotObjective* ObjectiveFifo[AUTOPILOT_OBJECTIVE_FIFO_SIZE];

    int isFifoInitialized;
    int currentObjectiveIndex;
    int numberOfObjectivesPending;

    pthread_mutex_t readWrite;

}autopilotObjectiveFifo_t;





typedef struct autopilotSharedState
{

    pthread_mutex_t readWrite;

}autopilotSharedState_t;


// Global variables declarations

extern float landTakeoffXP, landTakeoffXPI, landTakeoffXPD, landTakeoffYP, landTakeoffYPI, landTakeoffYPD, landTakeoffZP, landTakeoffZPI, landTakeoffZPD, landTakeoffYawP, landTakeoffYawPI, landTakeoffYawPD // PID coefficients for Land/Takeoff
extern autopilotObjectiveFifo_t autopilotObjectiveFifo;
extern autopilotSharedState_t autopilotSharedState;


