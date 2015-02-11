#ifndef AUTOPILOT
#define AUTOPILOT

#include "shared_librairies.h"
#include "autopilot_global_variables.h"
#include "autopilot_global_functions.h"
#include "imu_globals_variables.h"

// Some component defines :

#define AUTOPILOT_REFRESHING_PERIOD 100 //us

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

// Objective text file list path :

#define OBJECTIVES_PATH objectives.txt

// Fifo managing functions prototypes :

void trajAsserv(autopilotObjective_t* autopilotObjective);
void * autopilotHandler(void * arg);

int insertObjective(autopilotObjective_t* objective, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int removeSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo)
int removeSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int flushFifoObjective(autopilotOjectiveFifo_t autopilotObjectiveFifo);
int removeCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo);

void updateCalculation(autopilotObjective_t* autopilotObjective);

// Servo controlling prototypes :

servoControl_t buildServoControl(autopilotObjective_t autopilotObjective);
double updateServoControl(oneWayServoControl);



// Structures declarations :

typedef struct autopilotObjectiveFifo_t
{
    autopilotObjective* ObjectiveFifo[AUTOPILOT_OBJECTIVE_FIFO_SIZE];

    int currentObjectivePriority;
    int numberOfObjectivesPending;
    autopilotObjective_t firstObjective;

}autopilotObjectiveFifo_t;


typedef struct oneWayServoControl
{
    char type[32];
    double consign;
    float kp;
    float ki;
    float kd;

}oneWayServoControl;

typedef struct servoControl_t
{
    int oneWayNumber;
    oneWayServoControl* ServoControlData;

}servoControl_t;


#endif