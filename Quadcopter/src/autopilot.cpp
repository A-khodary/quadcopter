
#include "autopilot.h"
#include <errno.h>
#include "pilot.h"

/*
##############################################
#   Autopilot functionnality                 #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

// Global variables definitions :

float landTakeOffCoeff[4][3]; // PID coefficients for Land/Takeoff
float gotoStandardCoeff[3][3]; // PID coefficients for goto_standard
float gotoHoverCoeff[4][3]; // PID coefficients for goto_hovering
float positionHoldCoeff[3][3]; // PID coefficients for position_hold

autopilotSharedState_t autopilotSharedState;

servoControl_t::servoControl_t()
{

}

servoControl_t::servoControl_t(autopilotObjective_t* autopilotObjective)
{
//ctor

    int i,j;
    servoControl_t();

    if (autopilotObjective == NULL)
    {
        printDebug("The autopilot objective passed to buildServoControl was NULL");
    }

    switch(autopilotObjective->code)
    {

    case LAND_TAKEOFF : //mode  LAND_TAKEOFF

        oneWayNumber = 4;

        for (i=0; i< 4; i++);
        {
            ServoControlData[i] = new oneWayServoControl_t;
        }

        // Basic initialization of commands :
        strcpy(ServoControlData[0]->type, "x");
        strcpy(ServoControlData[1]->type, "y");
        strcpy(ServoControlData[2]->type, "z");
        strcpy(ServoControlData[3]->type, "yaw");

        //locking position mutex in order to get position
        pthread_mutex_lock(&positionShared.readWriteMutex);

        ServoControlData[0]->consign = positionShared.x;
        ServoControlData[1]->consign = positionShared.y;

        //Unlocking mutex now we don't need the position anymore :
        pthread_mutex_unlock(&positionShared.readWriteMutex);

        ServoControlData[2]->consign = autopilotObjective->destinationZ;
        ServoControlData[3]->consign = autopilotObjective->directionBearing;

        for (i=0 ; i < oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
            {
                // Filling-in coefficients :
                if (j==0) ServoControlData[i]->kp = landTakeOffCoeff[i][j];
                if (j==1) ServoControlData[i]->kd = landTakeOffCoeff[i][j];
                if (j==2) ServoControlData[i]->ki = landTakeOffCoeff[i][j];

            }

            // Creating the PID instance :


            ServoControlData[i]->pid = new PID;
            ServoControlData[i]->pid->setConstants(ServoControlData[i]->kp, ServoControlData[i]->ki, ServoControlData[i]->kd);

        }


        break;

    case GOTO_STANDARD: //mode GOTO_STANDARD

        oneWayNumber = 3;

        for (i=0; i< 3; i++);
        {
            ServoControlData[i] = new oneWayServoControl_t;
        }

        // Basic initialization of commands :
        strcpy(ServoControlData[0]->type, "z");
        strcpy(ServoControlData[1]->type, "yaw");
        strcpy(ServoControlData[2]->type, "dist");//none asserv on x or y => deviation ??


        // Dealing with altitude :
        //locking position mutex in order to get position
        pthread_mutex_lock(&positionShared.readWriteMutex);
        ServoControlData[0]->consign = positionShared.z;
        //Unlocking mutex now we don't need the position anymore :
        pthread_mutex_unlock(&positionShared.readWriteMutex);
        //Linking distance and bearing to the autopilot Objective ones :
        ServoControlData[1]->consign = autopilotObjective->directionBearing;

        ServoControlData[2]->consign = autopilotObjective->destinationDist;


        for (i=0 ; i < oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
            {
                // Filling-in coefficients :
                if (j==0) ServoControlData[i]->kp = gotoStandardCoeff[i][j];
                if (j==1) ServoControlData[i]->kd = gotoStandardCoeff[i][j];
                if (j==2) ServoControlData[i]->ki = gotoStandardCoeff[i][j];

            }

            // Creating the PID instance :

            ServoControlData[i]->pid = new PID;
            ServoControlData[i]->pid->setConstants(ServoControlData[i]->kp, ServoControlData[i]->ki, ServoControlData[i]->kd);
        }


        break;

    case GOTO_HOVERING: //mode GOTO_HOVERING

        oneWayNumber = 4;

        for (i=0; i< 4; i++);
        {
            ServoControlData[i] = new oneWayServoControl_t;
        }

        // Basic initialization of commands :
        strcpy(ServoControlData[0]->type, "x");
        strcpy(ServoControlData[1]->type, "y");
        strcpy(ServoControlData[2]->type, "yaw");
        strcpy(ServoControlData[3]->type, "dist");

        ServoControlData[0]->consign = autopilotObjective->destinationX;
        ServoControlData[1]->consign = autopilotObjective->destinationY;
        ServoControlData[2]->consign = autopilotObjective->directionBearing;
        ServoControlData[3]->consign = autopilotObjective->destinationDistXY;

        for (i=0 ; i < oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
            {
                // Filling-in coefficients :
                if (j==0) ServoControlData[i]->kp = gotoHoverCoeff[i][j];
                if (j==1) ServoControlData[i]->kd = gotoHoverCoeff[i][j];
                if (j==2) ServoControlData[i]->ki = gotoHoverCoeff[i][j];

            }

            // Creating the PID instance :


            ServoControlData[i]->pid = new PID;
            ServoControlData[i]->pid->setConstants(ServoControlData[i]->kp, ServoControlData[i]->ki, ServoControlData[i]->kd);

        }


        break;

    case POSITION_HOLD: // mode POSITION_HOLD

        // TODO
        oneWayNumber = 3;

        for (i=0; i< 3; i++);
        {
            ServoControlData[i] = new oneWayServoControl_t;
        }

        // Basic initialization of commands :
        strcpy(ServoControlData[0]->type, "x");
        strcpy(ServoControlData[1]->type, "y");
        strcpy(ServoControlData[2]->type, "z");

        //locking position mutex in order to get position
        pthread_mutex_lock(&positionShared.readWriteMutex);

        ServoControlData[0]->consign = positionShared.x;
        ServoControlData[1]->consign = positionShared.y;
        ServoControlData[2]->consign = positionShared.z;


        //Unlocking mutex now we don't need the position anymore :
        pthread_mutex_unlock(&positionShared.readWriteMutex);


        for (i=0 ; i < oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
            {
                // Filling-in coefficients :
                if (j==0) ServoControlData[i]->kp = positionHoldCoeff[i][j];
                if (j==1) ServoControlData[i]->kd = positionHoldCoeff[i][j];
                if (j==2) ServoControlData[i]->ki = positionHoldCoeff[i][j];

            }

            // Creating the PID instance :


            ServoControlData[i]->pid = new PID;
            ServoControlData[i]->pid->setConstants(ServoControlData[i]->kp, ServoControlData[i]->ki, ServoControlData[i]->kd);

        }

        break;

    default: // Notify the objective is not recognized

        printDebug("Invalid autopilot Objective code !");

        break;
    }

}

servoControl_t::~servoControl_t()
{
    //dtor
    int i;
    for (i=0; i < oneWayNumber; i++) free(ServoControlData[i]);
}

void servoControl_t::makeAsserv(autopilotObjective_t* relativeObjective)
{
    int i;
    double command;
    double value;


    if (oneWayNumber == 0)
    {
        printDebug("Trying to make the asserv of a NULL servoControl_t structure");
        return ;
    }


    for (i=0; i<oneWayNumber; i++ )
    {
        if (ServoControlData[i]->type == "yaw")
        {
            // Locking the position mutex in order to obtain the bearing value :
            pthread_mutex_lock(&flightStateShared.readWriteMutex);

            value = flightStateShared.yaw;

            // Unlocking the mutex :
            pthread_mutex_unlock(&flightStateShared.readWriteMutex);

            // Checking if the relative autopilot objective is null :
            if (relativeObjective != NULL) ServoControlData[i]->consign = relativeObjective ->directionBearing;


            // Now we have everything, lets compute :
            command =ServoControlData[i]->pid->compute(value, ServoControlData[i]->consign);

            // Now we've computed, let's check the command and write it :
            if (command > 100) command = 100;
            if (command < 0) command = 0;

            //Checking if the command is authorized :

            if (command > 50 + MAXYAW/2 ) command = 50 + MAXYAW/2;
            if (command < 50 - MAXYAW/2 ) command = 50 - MAXYAW/2;

            pthread_mutex_lock(&pilotCommandsShared.readWrite);

            pilotCommandsShared.chan1 = command; //TODO : identify the right channel

            //We're done, unlocking the mutex :
            pthread_mutex_unlock(&pilotCommandsShared.readWrite);
        }


        else if (ServoControlData[i]->type == "x")
        {
            // Locking the position mutex in order to obtain the x value :
            pthread_mutex_lock(&positionShared.readWriteMutex);

            value = positionShared.x;

            // Unlocking the mutex :
            pthread_mutex_unlock(&positionShared.readWriteMutex);

            // Now we have everything, lets compute :
            command =ServoControlData[i]->pid->compute(value, ServoControlData[i]->consign);

            // Now we've computed, let's check the command and write it :
            if (command > 100) command = 100;
            if (command < 0) command = 0;

            //Checking if the command is authorized :

            if (command > 50 + MAXELEV/2 ) command = 50 + MAXELEV/2;
            if (command < 50 - MAXELEV/2 ) command = 50 - MAXELEV/2;

            pthread_mutex_lock(&pilotCommandsShared.readWrite);

            pilotCommandsShared.chan2 = command; //TODO : identify the right channel(FORWARD/BACK)

            //We're done, unlocking the mutex :
            pthread_mutex_unlock(&pilotCommandsShared.readWrite);


        }

        else if (ServoControlData[i]->type == "y")
        {
            // Locking the position mutex in order to obtain the x value :
            pthread_mutex_lock(&positionShared.readWriteMutex);

            value = positionShared.y;

            // Unlocking the mutex :
            pthread_mutex_unlock(&positionShared.readWriteMutex);

            // Now we have everything, lets compute :
            command =ServoControlData[i]->pid->compute(value, ServoControlData[i]->consign);

            // Now we've computed, let's check the command and write it :
            if (command > 100) command = 100;
            if (command < 0) command = 0;

            //Checking if the command is authorized :

            if (command > 50 + MAXROLL/2 ) command = 50 + MAXROLL/2;
            if (command < 50 - MAXROLL/2 ) command = 50 - MAXROLL/2;

            pthread_mutex_lock(&pilotCommandsShared.readWrite);

            pilotCommandsShared.chan3 = command; //TODO : identify the right channel (RIGHT/LEFT)

            //We're done, unlocking the mutex :
            pthread_mutex_unlock(&pilotCommandsShared.readWrite);


        }

        else if (ServoControlData[i]->type == "z")
        {
            // Locking the position mutex in order to obtain the x value :
            pthread_mutex_lock(&positionShared.readWriteMutex);

            value = positionShared.z;

            // Unlocking the mutex :
            pthread_mutex_unlock(&positionShared.readWriteMutex);

            // Now we have everything, lets compute :
            command =ServoControlData[i]->pid->compute(value, ServoControlData[i]->consign);

            // Now we've computed, let's check the command and write it :
            if (command > 100) command = 100;
            if (command < 0) command = 0;

            //Checking if the command is authorized :

            if (command > 50 + MAXGAZ/2 ) command = 50 + MAXGAZ/2;
            if (command < 50 - MAXGAZ/2 ) command = 50 - MAXGAZ/2;

            pthread_mutex_lock(&pilotCommandsShared.readWrite);

            pilotCommandsShared.chan4 = command; //TODO : identify the right channel (GAZ)

            //We're done, unlocking the mutex :
            pthread_mutex_unlock(&pilotCommandsShared.readWrite);

        }

        else if (ServoControlData[i]->type == "dist")
        {
            // Checking if the relative autopilot objective is null :
            if (relativeObjective != NULL) ServoControlData[i]->consign = relativeObjective->destinationDistXY;


            // Now we have everything, lets compute :
            command =ServoControlData[i]->pid->compute(value, ServoControlData[i]->consign);

            // Now we've computed, let's check the command and write it :
            if (command > 100) command = 100;
            if (command < 0) command = 0;

            //Checking if the command is authorized :

            if (command > 50 + MAXELEV/2 ) command = 50 + MAXELEV/2;
            if (command < 50 - MAXELEV/2 ) command = 50 - MAXELEV/2;

            pthread_mutex_lock(&pilotCommandsShared.readWrite);

            pilotCommandsShared.chan2 = command; //TODO : identify the right channel(FORWARD/BACK)

            //We're done, unlocking the mutex :
            pthread_mutex_unlock(&pilotCommandsShared.readWrite);

        }




    }


}


oneWayServoControl_t::oneWayServoControl_t()
{
    //ctor
}

oneWayServoControl_t::~oneWayServoControl_t()
{
    //dtor
}

int insertObjective(autopilotObjective_t* objectiveToInsert, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t* currentObjective;
    autopilotObjective_t* lastObjective;

    if(objectiveToInsert->priority < 0 ) return -1; // Negative priority : returning -1

    // Dynamic copy of the parameter :

    autopilotObjective_t* objective = (autopilotObjective_t*)malloc(sizeof(autopilotObjective_t));
    objective = objectiveToInsert;

    // Checking if fifo is empty :

    if (autopilotObjectiveFifo.firstObjective == NULL)
    {
        autopilotObjectiveFifo.firstObjective = objective;
        objective->previousObjective = NULL;
        objective->nextObjective = NULL;
        autopilotObjectiveFifo.numberOfObjectivesPending++;

        return 1;

    }


    // Locating last objective :

    currentObjective = autopilotObjectiveFifo.firstObjective;
    while (currentObjective->nextObjective != NULL) currentObjective = currentObjective->nextObjective;
    lastObjective = currentObjective;


    // Checking last objective priority :

    if (lastObjective->priority < objective->priority)
    {
        // Now we have to search the last objective corresponding to priority :

        currentObjective = lastObjective;
        while (currentObjective->previousObjective != NULL && currentObjective->priority < objective->priority) currentObjective = currentObjective->previousObjective;

        if (currentObjective->previousObjective == NULL) // In case we have reached the first element of the fifo
        {
            currentObjective->previousObjective = objective;
            autopilotObjectiveFifo.firstObjective = objective;
            objective->previousObjective = NULL;
            objective->nextObjective = currentObjective;
        }
        else // We need to insert our objective between two
        {
            // ¨Preparing our objective :
            objective->nextObjective= currentObjective->nextObjective;
            objective->previousObjective = currentObjective;

            // Inserting it :
            currentObjective->nextObjective->previousObjective = objective;
            currentObjective->nextObjective = objective;
        }

    }
    else
    {
        // Preparing our objective :
        objective->previousObjective = lastObjective;
        objective->nextObjective = NULL;

        // Inserting it :
        lastObjective->nextObjective = objective;
    }

    autopilotObjectiveFifo.numberOfObjectivesPending++;
    //printf("Sended new objective not as first element :%s\n", objective->objective);

    return 0;
}


int removeSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t* currentObjective;

    if (autopilotObjectiveFifo.numberOfObjectivesPending < objectiveNumber)
    {
        printDebug("[e] Autopilot : there is objectives less than asked");
        return -1;
    }

    else
    {
        if (objectiveNumber == 0)  removeCurrentObjective(autopilotObjectiveFifo);
        else
        {
            currentObjective = autopilotObjectiveFifo.firstObjective;

            for (int i=0; i <= objectiveNumber; i++)
            {
                currentObjective = currentObjective->nextObjective;
            }


            currentObjective->nextObjective->previousObjective = currentObjective->previousObjective;
            currentObjective->previousObjective->nextObjective = currentObjective->nextObjective;
            currentObjective->previousObjective = NULL;
            currentObjective->nextObjective = NULL;
            freeAutopilotObjective(currentObjective);
            autopilotObjectiveFifo.numberOfObjectivesPending--;

            return 0;
        }
    }
}

int flushFifo(autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    if (autopilotObjectiveFifo.numberOfObjectivesPending !=0)
    {

        autopilotObjective_t* currentObjective;
        autopilotObjective_t* nextCurrentObjective;

        currentObjective =  autopilotObjectiveFifo.firstObjective;

        while(currentObjective != NULL)
        {
            nextCurrentObjective = currentObjective->nextObjective;
            free(currentObjective); // Freiing the ressources
            autopilotObjectiveFifo.numberOfObjectivesPending--;

            currentObjective = nextCurrentObjective;

        }
        autopilotObjectiveFifo.firstObjective = NULL;
        autopilotObjectiveFifo.currentObjectivePriority = 0;
        return 1;
    }
    else return -1;
}

autopilotObjective_t* readCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    return autopilotObjectiveFifo.firstObjective;
}


int removeCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t* objective;
    objective = autopilotObjectiveFifo.firstObjective->nextObjective;

    autopilotObjectiveFifo.firstObjective->nextObjective->previousObjective = NULL;
    freeAutopilotObjective(autopilotObjectiveFifo.firstObjective);

    autopilotObjectiveFifo.firstObjective = objective;
    autopilotObjectiveFifo.numberOfObjectivesPending--;
    autopilotObjectiveFifo.currentObjectivePriority = autopilotObjectiveFifo.firstObjective->priority; //TODO : clarify priorities
    return 1;
}


autopilotObjective_t* readSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t* currentObjective;



    if (autopilotObjectiveFifo.numberOfObjectivesPending < objectiveNumber)
    {
        printDebug("[e] Autopilot : not as many objectives as asked");
        return NULL;
    }
    else
    {

        if (objectiveNumber == 0)  return autopilotObjectiveFifo.firstObjective;
        else
        {
            currentObjective = autopilotObjectiveFifo.firstObjective;

            for (int i=0; i <= objectiveNumber; i++)
            {
                currentObjective = currentObjective->nextObjective;
            }

            removeSpecificObjectivebyNumber(objectiveNumber, autopilotObjectiveFifo);
            return currentObjective;
        }
    }
}


autopilotObjective_t* readSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t* currentObjective;
    currentObjective = autopilotObjectiveFifo.firstObjective;

    int i = 1;

    while(currentObjective != NULL)
    {
        if(!strcmp(objectiveName, currentObjective->name))
        {
            removeSpecificObjectivebyNumber(i, autopilotObjectiveFifo);
            return currentObjective;
        }
        else
        {
            currentObjective = currentObjective->nextObjective;
            i++;
        }
    }
    return NULL;
}



void freeAutopilotObjective (autopilotObjective_t* autopilotObjective)
{
    free(autopilotObjective);
}

int initCalculation(autopilotObjective_t* autopilotObjective)
{
    if(convertPlanar(&autopilotObjective->destinationX, &autopilotObjective->destinationY, autopilotObjective->destinationLat, autopilotObjective->destinationLong))
    {
        printDebug("Error in autopilot objective initialization");
        exit(1);
    }
    autopilotObjective->maxSpeed = MAXSPEED;
    autopilotObjective->maxSpeedXY = MAXSPEEDXY;
    updateCalculation(autopilotObjective);
    exit(0);


}
// does the first bearing calculation and projections


int updateCalculation(autopilotObjective_t* autopilotObjective)
{
    // Locking the position relative mutex :
    pthread_mutex_lock(&positionShared.readWriteMutex);


    if (autopilotObjective->code == GOTO_STANDARD)
    {

        //Updating bearing :
        autopilotObjective->directionBearing = calculateBearing(positionShared.x, positionShared.y, autopilotObjective->destinationX, autopilotObjective->destinationY);

    }

    // Updating distances to Objective :
    autopilotObjective->destinationDistXY =sqrt( pow(2, autopilotObjective->destinationX - positionShared.x) + pow(2, autopilotObjective->destinationY - positionShared.y) );
    autopilotObjective->destinationDist =sqrt( pow(2, autopilotObjective->destinationX - positionShared.x) + pow(2, autopilotObjective->destinationY - positionShared.y) + pow(2, autopilotObjective->destinationZ - positionShared.z) );

    //Now we're done, unlocking the position mutex :
    pthread_mutex_unlock(&positionShared.readWriteMutex);

    //Objective reaching determination :

    if (autopilotObjective->destinationDist == 0)
    {
        return 1;
    }

    else return 0;

    //TODO : algotithme de modification de maxspeed en fonction de la distance restante

   /* if(autopilotObjective->destinationDistXY>)
    {
        autopilotObjective->maxSpeed =;
    }
    else if(autopilotObjective->destinationDistXY>&&autopilotObjective->destinationDistXY<)
    {
        autopilotObjective->maxSpeed =;
    }
    */
} // For now just computes the bearing and several distances, will modify max_speed in the future relative to distance to objective. Returns a boolean that indicates when objective is reached


void* autopilotHandler(void* arg)
{
    // Initialization :

    int objectiveReached=0;

    autopilotObjectiveFifo_t autopilotObjectiveFifo;
    autopilotObjectiveFifo.currentObjectivePriority =0;
    autopilotObjectiveFifo.numberOfObjectivesPending = 0;
    autopilotObjectiveFifo.firstObjective = NULL;

    bidirectionnalHandler_t* bidirectionnalHandler;
    bidirectionnalHandler = (bidirectionnalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* autopilotITMHandler;

    mainITMHandler = bidirectionnalHandler->mainITMHandler;
    autopilotITMHandler = bidirectionnalHandler->componentITMHandler;

    message_t* receivedMessage;
    message_t currentMessage;
    message_t message;

    servoControl_t* currentServoControl;
    currentServoControl = new servoControl_t;

    strcpy(currentMessage.message, "main_autopilot_info_init");
    currentMessage.priority = 20;

    //Send init message to main
    sendMessage(mainITMHandler, currentMessage);


    double distanceToObjective;

    int tickCounter=0;
    int objectiveBypass=0;

    // Coefficients definitions :

    landTakeOffCoeff[4][3];
    gotoHoverCoeff[4][3];
    gotoStandardCoeff[2][3];


    landTakeOffCoeff[0][0]=LANDTAKEOFFXP;
    landTakeOffCoeff[0][1]=LANDTAKEOFFXPD;
    landTakeOffCoeff[0][2]=LANDTAKEOFFXPI;

    landTakeOffCoeff[1][0]=LANDTAKEOFFYP;
    landTakeOffCoeff[1][1]=LANDTAKEOFFYPD;
    landTakeOffCoeff[1][2]=LANDTAKEOFFYPI;

    landTakeOffCoeff[2][0]=LANDTAKEOFFZP;
    landTakeOffCoeff[2][1]=LANDTAKEOFFZPD;
    landTakeOffCoeff[2][2]=LANDTAKEOFFZPI;

    landTakeOffCoeff[3][0]=LANDTAKEOFFYAWP;
    landTakeOffCoeff[3][1]=LANDTAKEOFFYAWPD;
    landTakeOffCoeff[3][2]=LANDTAKEOFFYAWPI;

    gotoHoverCoeff[0][0]=GOTOHOVERXP;
    gotoHoverCoeff[0][1]=GOTOHOVERXPD;
    gotoHoverCoeff[0][2]=GOTOHOVERXPI;

    gotoHoverCoeff[1][0]=GOTOHOVERYP;
    gotoHoverCoeff[1][1]=GOTOHOVERYPD;
    gotoHoverCoeff[1][2]=GOTOHOVERYPI;

    gotoHoverCoeff[2][0]=GOTOHOVERYAWP;
    gotoHoverCoeff[2][1]=GOTOHOVERYAWPD;
    gotoHoverCoeff[2][2]=GOTOHOVERYAWPI;

    gotoHoverCoeff[3][0]=GOTOHOVERDISTP;
    gotoHoverCoeff[3][1]=GOTOHOVERDISTPD;
    gotoHoverCoeff[3][2]=GOTOHOVERDISTPI;

    gotoStandardCoeff[0][0]=GOTOSTANDARDZP;
    gotoStandardCoeff[0][1]=GOTOSTANDARDZPD;
    gotoStandardCoeff[0][2]=GOTOSTANDARDZPI;

    gotoStandardCoeff[1][0]=GOTOSTANDARDYAWP;
    gotoStandardCoeff[1][1]=GOTOSTANDARDYAWPD;
    gotoStandardCoeff[1][2]=GOTOSTANDARDYAWPI;

    gotoStandardCoeff[2][0]=GOTOSTANDARDDISTP;
    gotoStandardCoeff[2][1]=GOTOSTANDARDDISTPD;
    gotoStandardCoeff[2][2]=GOTOSTANDARDDISTPI;

    positionHoldCoeff[0][0]=POSITIONHOLDXP;
    positionHoldCoeff[0][1]=POSITIONHOLDXPD;
    positionHoldCoeff[0][2]=POSITIONHOLDXPI;

    positionHoldCoeff[1][0]=POSITIONHOLDYP;
    positionHoldCoeff[1][1]=POSITIONHOLDYPD;
    positionHoldCoeff[1][2]=POSITIONHOLDYPI;

    positionHoldCoeff[2][0]=POSITIONHOLDZP;
    positionHoldCoeff[2][1]=POSITIONHOLDZPD;
    positionHoldCoeff[2][2]=POSITIONHOLDZPI;

    // State initializatin :

    initialize_mutex(&autopilotSharedState.readWrite);
    pthread_mutex_lock(&autopilotSharedState.readWrite);



    autopilotSharedState.landed = 1;  // Assuming we're landed on startup, TODO : actualize in case of in-flight relaunch
    autopilotSharedState.crashed = 0;
    autopilotSharedState.stressLevel = 1;
    autopilotSharedState.stressLevel = 1;
    autopilotSharedState.currentObjectivePriority = 0;

    pthread_mutex_unlock(&autopilotSharedState.readWrite);


    FILE* writtenObjectives = NULL;
    char readLine[1024];
    int lineNumber = 1;

    char readObjectiveName[64];
    int readObjectiveCode=0;
    double readObjectiveDestinationLat=0;
    double readObjectiveDestinationLong=0;
    double readObjectiveDestinationAlt=0;
    double readObjectiveMaxSpeed=5;

    autopilotObjective_t readObjective;
    autopilotObjective_t* currentObjective;
    autopilotObjective_t* insertedObjective;
    autopilotObjective_t* tempObjective;

    autopilotObjective_t* newObjective;

    // File reading for basic configuration

    writtenObjectives = fopen(OBJECTIVES_PATH,"r");
    fprintf(stderr, "%s\n", strerror(errno));

    if (writtenObjectives == NULL)
    {
        printDebug("[e]Autopilot objective file not found");
        strcpy(currentMessage.message, "main_autopilot_info_objectivefilenotfound");
        currentMessage.priority = 5;
        sendMessage(mainITMHandler, currentMessage);
    }
    else
    {
        while (fscanf(writtenObjectives, "%s %d %lf %lf %lf %lf", readObjectiveName, &readObjectiveCode, &readObjectiveDestinationLat, &readObjectiveDestinationLong, &readObjectiveDestinationAlt, &readObjectiveMaxSpeed) != 0)
        {
            // Verifying the readed objective :
            if ((readObjectiveCode == GOTO_STANDARD) || (readObjectiveCode == GOTO_HOVERING) || (readObjectiveCode == LAND_TAKEOFF) || (readObjectiveCode == POSITION_HOLD))
            {
                if ((readObjectiveDestinationLat >= 0) && (readObjectiveDestinationLong >= 0) && (readObjectiveDestinationAlt >= 0) && (readObjectiveMaxSpeed >= 0))
                {
                    // Now we're clear, we have to fill in the readObjective strucuture
                    readObjective.code = readObjectiveCode;
                    readObjective.destinationLat = readObjectiveDestinationLat;
                    readObjective.destinationLong = readObjectiveDestinationLong;
                    readObjective.destinationAlt = readObjectiveDestinationAlt;
                    readObjective.maxSpeed = readObjectiveMaxSpeed;

                    // Now we add the objective to the fifo
                    if (insertObjective(&readObjective, autopilotObjectiveFifo))
                    {
                        printDebug("[i] Insertion of a new autopilot objective success !");
                        lineNumber++;
                    }
                    else printDebug("[e] Insertion of a new autopilot objective error");
                }



            }
            printDebug("[i] Objectives added to Autopilot FIFO");
            lineNumber = 0;
        }
    }
    if (writtenObjectives != NULL) fclose(writtenObjectives);

    //Notify main thread of end of init


    strcpy(currentMessage.message, "main_autopilot_info_endofinit");
    currentMessage.priority = 20;

    sendMessage(mainITMHandler, currentMessage);

    while (1) // This loop iterates each time an objective is reached
    {
        printDebug("[i] Autopilot is beginning new objective update");

        if(!objectiveBypass)
        {


            currentObjective = readCurrentObjective(autopilotObjectiveFifo);
            if (currentObjective == NULL)
            {
                // If we don't have any objective :
                pthread_mutex_lock(&autopilotSharedState.readWrite);    // Locking shared state mutex, because we need state
                if (autopilotSharedState.landed == 1 || autopilotSharedState.crashed == 1)
                {
                    if (autopilotSharedState.engaged)
                    {
                        // let's Put the autopilot in pause mode :
                        autopilotSharedState.engaged = 0;
                        printDebug("[i] Autopilot was put in pause due to landed state and no objective");

                    }

                }
                else
                {
                    // We build a position hold objective :

                    currentObjective->code = POSITION_HOLD;

                    // Locking position mutex to get position :

                    pthread_mutex_lock(&positionShared.readWriteMutex);

                    currentObjective->destinationLat = rawPositionShared.latitude;
                    currentObjective->destinationLong = rawPositionShared.longitude;
                    currentObjective->destinationAlt = rawPositionShared.altitude;

                    // Unlocking position mutex :

                    pthread_mutex_unlock(&positionShared.readWriteMutex);

                    currentObjective->maxSpeed = MAXSPEED;
                    currentObjective->maxSpeedXY = MAXSPEEDXY;
                    currentObjective->priority = 100;

                    printDebug("[i] A position hold order was issued due to in flight state and no objective");
                }
                pthread_mutex_unlock(&autopilotSharedState.readWrite);

            }


        }

        else
        {
            objectiveBypass = 0;
        }

        pthread_mutex_lock(&autopilotSharedState.readWrite);
        if(autopilotSharedState.engaged)
        {

            pthread_mutex_unlock(&autopilotSharedState.readWrite);
            initCalculation(currentObjective);

            currentServoControl = new servoControl_t(currentObjective);

            if (currentServoControl == NULL)
            {
                printDebug("[e] A servo control structure was returned null to autopilot main Thread, skipping objective...");
                freeAutopilotObjective(currentObjective);
                continue;
            }

        }
        else
        {
            pthread_mutex_unlock(&autopilotSharedState.readWrite);
        }


        while (1) // This loop iterates after each ITMhandler execution and calculation
        {
            printDebug("[i] Autopilot is beginning new ITMHandler/Calculation");

            if(tickCounter == MESSAGE_CHECKING_LIMIT)
            {
                printDebug("[i] Autopilot is checking messages");
                tickCounter=0;

                receivedMessage = retrieveMessage(autopilotITMHandler);
                if (receivedMessage != NULL)
                {
                    // TODO : process message and decide priorities
                    printDebug("[i] New ITM message received by autopilot");


                    if(!strcmp(receivedMessage->message, "newobjective"))
                    {
                        if(receivedMessage->data != NULL && receivedMessage->dataSize == sizeof(autopilotObjective_t))
                        {
                            // Dynamic allocation and copying :
                            insertedObjective = (autopilotObjective_t*)malloc(sizeof(autopilotObjective_t));
                            autopilotObjective_t* to = (autopilotObjective_t*) receivedMessage->data;
                            *insertedObjective = *to;

                            // Locking state mutex because we need priority :

                            if (insertedObjective->priority > autopilotSharedState.currentObjectivePriority)
                            {
                                printDebug("[i] We've got a prioritary objective, removing the current objective and restarting autopilot");

                                free(currentObjective);
                                free(currentServoControl);
                                currentObjective = (autopilotObjective_t*)malloc(sizeof(autopilotObjective_t));
                                tempObjective = (autopilotObjective_t*) receivedMessage->data;
                                *currentObjective = *tempObjective;
                                free(insertedObjective);

                                objectiveBypass = 1;
                                break;

                            }
                            else
                            {
                                printDebug("[i] We've got a new objective, but its priority was not to high so we inserted it in the fifo");
                                //TODO : insert objective
                                insertObjective(insertedObjective, autopilotObjectiveFifo);

                            }

                        }

                        else
                        {
                            printDebug("[e] Autopilot received a message to insert an objective, but the associated data was corrupted");
                        }

                    }

                    else if (!strcmp(receivedMessage->message, "emergencylanding"))
                    {

                        //TODO : behaviour
                        disarmQuadcopter();//dcrémenter par 4, 10 ... pour l'urgence ?
                        //Message
                        printDebug("Need to land in emergency");
                        currentMessage.dataSize=0;
                        strcpy(currentMessage.message, "main_autopilot_info_emergencylanding");
                        sendMessage(mainITMHandler, currentMessage);

                    }

                    //TODO : add all the events handling
                    else if(!strcmp(receivedMessage->message, "landing"))
                    {
                        //TODO : behaviour
                        printDebug("Land quadcopter");
                        strcpy(currentMessage.message, "main_autopilot_info_landing");
                        sendMessage(mainITMHandler, currentMessage);
                        disarmQuadcopter();

                    }

                    else if(!strcmp(receivedMessage->message, "play"))
                    {
                        // Locking state mutex :

                        pthread_mutex_lock(&autopilotSharedState.readWrite);

                        if (!autopilotSharedState.engaged)
                        {
                            printDebug("[i] Autopilot is resuming....");
                            autopilotSharedState.engaged = 1;

                            if (!autopilotSharedState.landed) objectiveBypass = 1;
                            pthread_mutex_unlock(&autopilotSharedState.readWrite);

                            break;
                        }

                        else
                        {
                            printDebug("[e] Autopilot was asked to resume and was already engaged");
                            pthread_mutex_unlock(&autopilotSharedState.readWrite);
                        }




                    }

                    else if(!strcmp(receivedMessage->message, "pause"))
                    {
                        pthread_mutex_lock(&autopilotSharedState.readWrite);

                        if (autopilotSharedState.engaged)
                        {
                            printDebug("[i] Autopilot is pausing....");
                            autopilotSharedState.engaged = 0;
                        }

                        else
                        {
                            printDebug("[e] Autopilot was asked to pause and was already disengaged");
                        }


                        pthread_mutex_unlock(&autopilotSharedState.readWrite);

                    }

                    else
                    {
                        printDebug("Autopilot received an unrecognized ITM message");
                    }

                    free(receivedMessage);

                }

                else
                {

                    usleep(AUTOPILOT_REFRESHING_PERIOD);

                }

            }

            else
            {
                tickCounter++;
                usleep(AUTOPILOT_REFRESHING_PERIOD);
            }

            pthread_mutex_lock(&autopilotSharedState.readWrite);

            if(autopilotSharedState.engaged)
            {
                pthread_mutex_unlock(&autopilotSharedState.readWrite);
                // Calculation Area
                objectiveReached = updateCalculation(currentObjective);
                currentServoControl->makeAsserv(currentObjective);

                if(objectiveReached)
                {
                    //TODO : notify main of the objective completion

                    strcpy(message.message, "main_autopilot_info_objectivecompleted");
                    sendMessage(mainITMHandler, message );
                    // Now the objective is reached, we need to free the ressources used :
                    currentServoControl->~servoControl_t();
                    freeAutopilotObjective(currentObjective);


                }

            }
            else pthread_mutex_unlock(&autopilotSharedState.readWrite);


        }



    }



}

