#include "shared_librairies.h"
#include "autopilot_global_variables.h"
#include "imu_globals_variables.h"
#include "pilot_global_variables.h"
#include "PID.h"


#ifndef AUTOPILOT_H
#define AUTOPILOT_H



// Some component defines :

#define AUTOPILOT_REFRESHING_PERIOD 100 //us
#define MESSAGE_CHECKING_LIMIT 200000 // in loop iterations : will be implemented as a timer in the future

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
#define LANDTAKEOFFYAWPI 1
#define LANDTAKEOFFYAWPD 1

// Some speed definitions :

#define MAXSPEED 3
#define MAXSPEEDXY 2

// Some commands restrictions

#define MAXYAW 80
#define MAXGAZ 70
#define MAXELEV 70
#define MAXROLL 70


// Objective text file list path :

#define OBJECTIVES_PATH "objectives"

// Structures declarations :

typedef struct autopilotObjectiveFifo_t
{

    int currentObjectivePriority;
    int numberOfObjectivesPending;
    autopilotObjective_t* firstObjective;


}autopilotObjectiveFifo_t;


typedef struct oneWayServoControl_t
{
    char type[32];
    double consign;
    float kp;
    float ki;
    float kd;

    PID pid;

}oneWayServoControl_t;

typedef struct servoControl_t
{
    int oneWayNumber;
    oneWayServoControl_t** ServoControlData;

}servoControl_t;




void * autopilotHandler(void * arg);

// Fifo managing functions prototypes :



int insertObjective(autopilotObjective_t* objective, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int removeSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int removeSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int flushFifoObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
int removeCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo);
void freeAutopilotObjective (autopilotObjective_t* autopilotObjective);


// Servo controlling prototypes :

servoControl_t* buildServoControl(autopilotObjective_t autopilotObjective);
void freeServoControl(servoControl_t*);
void makeAsserv(servoControl_t* currentServoControl, autopilotObjective_t relativeObjective);

// Objective calculations prototypes :

int initCalculation(autopilotObjective_t* autopilotObjective);
int updateCalculation(autopilotObjective_t* autopilotObjective);


#endif
