#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include "shared_librairies.h"
#include "autopilot_global_variables.h"
#include "imu_globals_variables.h"
#include "pilot_global_variables.h"
#include "PID.h"

// Some component defines :

#define AUTOPILOT_REFRESHING_PERIOD 1000000 //us
#define MESSAGE_CHECKING_LIMIT 2 // in loop iterations : will be implemented as a timer in the future

// Some PID coefficient defines : to determine expirimentaly

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

#define GOTOHOVERXP 1
#define GOTOHOVERXPI 1
#define GOTOHOVERXPD 1
#define GOTOHOVERYP 1
#define GOTOHOVERYPI 1
#define GOTOHOVERYPD 1
#define GOTOHOVERYAWP 1
#define GOTOHOVERYAWPI 1
#define GOTOHOVERYAWPD 1
#define GOTOHOVERDISTP 1
#define GOTOHOVERDISTPI 1
#define GOTOHOVERDISTPD 1

#define GOTOSTANDARDZP 1
#define GOTOSTANDARDZPI 1
#define GOTOSTANDARDZPD 1
#define GOTOSTANDARDYAWP 1
#define GOTOSTANDARDYAWPI 1
#define GOTOSTANDARDYAWPD 1
#define GOTOSTANDARDDISTP 1
#define GOTOSTANDARDDISTPI 1
#define GOTOSTANDARDDISTPD 1

#define POSITIONHOLDXP 1
#define POSITIONHOLDXPI 1
#define POSITIONHOLDXPD 1
#define POSITIONHOLDYP 1
#define POSITIONHOLDYPI 1
#define POSITIONHOLDYPD 1
#define POSITIONHOLDZP 1
#define POSITIONHOLDZPI 1
#define POSITIONHOLDZPD 1

// Some speed definitions :

#define MAXSPEED 3
#define MAXSPEEDXY 2

// Some commands restrictions

#define MAXYAW 80
#define MAXGAZ 70
#define MAXELEV 70
#define MAXROLL 70


// Objective text file list path :

#define OBJECTIVES_PATH "objectives.txt"


class oneWayServoControl_t
{
    public:

        oneWayServoControl_t();
        virtual ~oneWayServoControl_t();
        char type[32];
        double consign;
        double kp;
        double ki;
        double kd;
        PID* pid;

    private:
};

class servoControl_t
{
    public:

        servoControl_t();
        servoControl_t(autopilotObjective_t* autopilotObjective);

        virtual ~servoControl_t();

        void makeAsserv(autopilotObjective_t* relativeObjective);

        int oneWayNumber;
        oneWayServoControl_t** ServoControlData;

    private:

};


// Structures declarations :

typedef struct autopilotObjectiveFifo_t
{

    int currentObjectivePriority;
    int numberOfObjectivesPending;
    autopilotObjective_t* firstObjective;


}autopilotObjectiveFifo_t;








void * autopilotHandler(void * arg);

// Fifo managing functions prototypes :

int insertObjective(autopilotObjective_t* objectiveToInsert, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int removeSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int removeSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo);
int flushFifoObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
int removeCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo);
autopilotObjective_t* readSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo);
void freeAutopilotObjective (autopilotObjective_t* autopilotObjective);

// Objective calculations prototypes :

int initCalculation(autopilotObjective_t* autopilotObjective);
int updateCalculation(autopilotObjective_t* autopilotObjective);


#endif
