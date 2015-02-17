#include "autopilot.h"
#include "autopilot_global_variables.h"

/*
##############################################
#   Autopilot functionnality                 #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/


int insertObjective(autopilotObjective_t* objective, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    int objectiveIndex;
    autopilotObjective_t* currentObjective;
    autopilotObjective_t* lastObjective;

    if (autopilotObjectiveFifo.isFifoInitialized != 1)
    {
        printDebug("[e] Autopilot : attempting to insert objective in a non initialized Objective FIFO");
        return -1;
    }

    if (autopilotObjectiveFifo.numberOfObjectivesPending >= AUTOPILOT_OBJECTIVE_FIFO_SIZE)
    {
        printDebug("[e] Autopilot : attempting to insert objective in a full Objective FIFO");
        return -2;
    }

    // Locating last objective :

    currentObjective = autopilotObjectiveFifo->firstObjective;
    while (currentObjective->nextObjective != NULL) currentObjective = currentObjective->nextObjective;
    lastObjective = currentObjective;

     // Checking last objective priority :

    if (lastObjective->priority < Objective>priority)
    {
        // Now we have to search the last objective corresponding to priority :

        currentObjective = lastObjective;
        while (currentObjective->previousObjective != NULL || currentObjective->priority < Objective->priority) currentObjective = currentObjective->previousObjective;

        if (currentObjective->previousObjective == NULL) currentObjective->previousObjective = Objective; // We have reached the first element of the fifo
        else // We need to insert our objective between two
        {
            // Preparing our objective :
            Objective->nextObjective= currentObjective->nextObjective;
            Objective->previousObjective = currentObjective;

            // Inserting it :
            currentObjective->nextObjective->previousObjective = Objective;
            currentObjective->nextObjective = Objective;
        }

    }
    else
    {
        // Preparing our Objective :
        Objective->previousObjective = lastObjective;
        Objective->nextObjective = NULL;

        // Inserting it :
        lastObjective->nextObjective = Objective;
    }

        autopilotObjectiveFifo.numberOfObjectivesPending++;
        return 1;
}

}
/* Return summary :
-1 Fifo non initialized
-2 Fifo is full
 1 Inserting Ok


*/


// TODO Servo controlling functions

servoControl_t* buildServoControl(autopilotObjective_t* autopilotObjective)
{
    int i,j;

    if (autopilotObjective == NULL)
    {
        printDebug("The autopilot objective passed to buildServoControl was NULL");
        return NULL
    }

    servoControl_t* currentServoControl;
    currentServoControl = (servoControl_t)malloc(sizeof(servoControl_t));



    switch(autopilotObjective->code)
    {

    case LAND_TAKEOFF : //mode  LAND_TAKEOFF

        currentServoControl->oneWayNumber = 4;
        currentServoControl->ServoControlData = (malloc(4*sizeof(oneWayServoControl));

        // Basic initialization of commands :
        currentServoControl->ServoControlData[0]->type="x";
        currentServoControl->ServoControlData[1]->type="y";
        currentServoControl->ServoControlData[2]->type="z";
        currentServoControl->ServoControlData[3]->type="yaw";

        //locking position mutex in order to get position
        pthread_mutex_lock(&positionShared->readWriteMutex);

        currentServoControl->ServoControlData[0]->consign = &positionShared->x;
        currentServoControl->ServoControlData[1]->consign = &positionShared->y;

        //Unlocking mutex now we don't need the position anymore :
        pthread_mutex_unlock(&positionShared->readWriteMutex);

        currentServoControl->ServoControlData[2]->consign = &autopilotObjective->destinationZ;
        currentServoControl->ServoControlData[3]->consign = &autopilotObjective->directionBearing;

        for (i=0 ; i < currentServoControl->oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
            {
                // Filling-in coefficients :
                if (j==0) currentServoControl->servoControlData[i]->kp &landTakeOffCoeff[i][j];
                if (j==1) currentServoControl->servoControlData[i]->kd &landTakeOffCoeff[i][j];
                if (j==2) currentServoControl->servoControlData[i]->ki &landTakeOffCoeff[i][j];

            }

            // Creating the PID instance :
            currentServoControl->servoControlData[i]->pid = new PID(*currentServoControl->servoControlData[i]->kp, currentServoControl->servoControlData[i]->ki, currentServoControl->servoControlData[i]->kd);

        }


        break;

    case GOTO_STANDARD: //mode GOTO_STANDARD

        currentServoControl->oneWayNumber = 3;
        currentServoControl->ServoControlData = (malloc(2*sizeof(oneWayServoControl));

        // Basic initialization of commands :
        currentServoControl->ServoControlData[0]->type="z";
        currentServoControl->ServoControlData[1]->type="yaw";
        currentServoControl->ServoControlData[2]->type="dist";


        currentServoControl->ServoControlData[0]->consign = &autopilotObjective->destinationZ;

        //Linking distance and bearing to the autopilot Objective ones :
        currentServoControl->ServoControlData[1]->consign = &autopilotObjective->destinationBearing;

        // Dealing with altitude :
        currentServoControl->ServoControlData[2]->consign = &autopilotObjective->destinationZ;


        for (i=0 ; i < currentServoControl->oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
            {
                // Filling-in coefficients :
                if (j==0) currentServoControl->servoControlData[i]->kp = &landTakeOffCoeff[i][j];
                if (j==1) currentServoControl->servoControlData[i]->kd = &landTakeOffCoeff[i][j];
                if (j==2) currentServoControl->servoControlData[i]->ki = &landTakeOffCoeff[i][j];

            }

            // Creating the PID instance :
            currentServoControl->servoControlData[i]->pid = new PID(currentServoControl->servoControlData[i]->kp, currentServoControl->servoControlData[i]->ki, currentServoControl->servoControlData[i]->kd);

        }


        break;

    case GOTO_HOVERING : //mode GOTO_HOVERING

    // TODO


        break;

    case 4 : // mode POSITION_HOLD

    // TODO

        break;

    default : // Notify the objective is not recognized

    printDebug("Invalid autopilot Objective code !")
    return NULL;


        break;
    }

return currentServoControl;

}

freeServoControl(servoControl_t* servoControl)
{
    int i;
    for (i=0; i < servoControl->oneWayNumber; i++) free(servoControl->ServoControlData[i]);
    free(servoControl);
}

// TODO FIFO Management functions


int removeSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t* currentObjective;

    if (AUTOPILOT_OBJECTIVE_FIFO_SIZE<objectiveNumber*sizeof(autopilotObjective_t))
    {
        printDebug("[e] Autopilot : not as many objectives as asked");
        return -1;
    }

   else
   {
       if (objectiveNumber == 1)  removeCurrentObjective(autopilotObjectiveFifo);
       else
    {
        currentObjective = &autopilotObjectiveFifo + objectiveNumber*sizeof(autopilotObjective_t);
       currentObjective->nextObjective->previousObjective = currentObjective->previousObjective;
       currentObjective->previousObjective->nextObjective = currentObjective->nextObjective;
       currentObjective->previousObjective = NULL;
       currentObjective->nextObjective = NULL;
       autopilotObjectiveFifo.numberOfObjectivesPending--;
   }
    }
}

int flushFifo(autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
     if (autopilotObjectiveFifo.numberOfObjectivesPending !=0)
     {

         autopilotObjective_t* currentObjective;
         autopilotObjective_t* nextCurrentObjective;

         currentObjective =  autopilotObjectiveFifo->firstObjective;

         for (i=0,i< autopilotObjectiveFifo.numberOfObjectivesPending*sizeof(autopilotObjective_t),i++)
         {
            nextCurrentObjective = currentObjective->nextObjective;
            currentObjective->previousObjective = NULL;
            currentObjective->nextObjective = NULL;
            currentObjective = nextCurrentObjective;
            autopilotObjectiveFifo.numberOfObjectivesPending--;
         }
         autopilotObjectiveFifo->firstObjective = NULL;
         autopilotObjectiveFifo.currentObjectivePriority = 0;
         return 1;
     }
     else return -1;
}

autopilotObjective_t* readCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo);
{
    return autopilotObjectiveFifo->firstObjective;
}


int removeCurrentObjective(autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
      autopilotObjective_t objective;
      objective = autopilotObjectiveFifo->firstObjective->previousObjective;
      autopilotObjectiveFifo->firstObjective->previousObjective->nextObjective = NULL;
      autopilotObjectiveFifo->firstObjective->previousObjective = NULL;
      autopilotObjectiveFifo->firstObjective = objective;
      autopilotObjectiveFifo.numberOfObjectivesPending--;
      autopilotObjectiveFifo.currentObjectivePriority = autopilotObjectiveFifo->firstObjective.priority;
      return 1;
}


autopilotObjective_t* readSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
   if (AUTOPILOT_OBJECTIVE_FIFO_SIZE<objectiveNumber*sizeof(autopilotObjective_t)) printDebug("[e] Autopilot : not as many objectives as asked");
   else return &autopilotObjectiveFifo + objectiveNumber*sizeof(autopilotObjective_t);
}


autopilotObjective_t* readSpecificObjectivebyName(char* objectiveName, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t* currentObjective;
    currentObjective = autopilotObjectiveFifo->firstObjective;

    for (i=0,i< autopilotObjectiveFifo.numberOfObjectivesPending*sizeof(autopilotObjective_t)+1,i++)
    {
        if(currentObjective.name == objectiveName)
        {
            return currentObjective;
        }
        else
        {
            currentObjective = currentObjective->previousObjective;
        }
    }
}



int initCalculation(autopilotObjective_t* autopilotObjective)
{
    if(convertPlanar(&autopilotObjective->destinationX, &autopilotObjective->destinationY, autopilotObjective->destinationLat, autopilotObjective->destinationLong))
    {
        printDebug("Error in autopilot objective initialization");
        exit(1);
    }
    updateCalculation(autopilotObjective);
    exit(0);


}
// does the first bearing calculation and projections


void updateCalculation(autopilotObjective_t* autopilotObjective)
{

    if (autopilotObjective->code == GOTO_STANDARD)
    {
         // Locking the position relative mutex :
        pthread_mutex_lock(&positionShared.readWriteMutex);

        //Updating bearing :
        autopilotObjective->directionBearing = calculateBearing(positionShared.x, positionShared.y, autopilotObjective->destinationX, autopilotObjective->destinationY);

        // Updating distance to Objective :
        autopilotObjective->destinationDistXY =sqrt( pow(2, autopilotObjective->destinationX - positionShared.x) + pow(2, autopilotObjective->destinationY - positionShared.Y) );

        //Now we're done, unlocking mutex :
        pthread_mutex_unlock(&positionShared.readWriteMutex);
    }
} // For now just computes the bearing, will modify max_speed in the future relative to distance to objective






void* autopilotHandler(void* arg)
{
    // Initialization :

    autopilotObjectiveFifo_t autopilotObjectiveFifo;

    bidirectionalHandler_t* bidirectionalHandler;
    bidirectionalHandler = (bidirectionalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* autopilotITMHandler;

    mainITMHandler = bidirectionalHandler.mainITMHandler;
    autopilotITMHandler = bidirectionalHandler.componentITMHandler;

    message_t receivedMessage;
    message_t currentMessage;

    servoControl_t currentServoControl;
    currentServoControl.oneWayNumber = 0;
    currentServoControl.

    currentMessage.message = "autopilot_init"
    currentMessage.priority = 20;

    int tickCounter=0;

    // Coefficients definitions :

    landTakeOffCoeff[4][3];
    gotoHoverCoeff[4][3];
    gotoStandardCoeff[2][3];


    landTakeoff[0][0]=LANDTAKEOFFXP;
    landTakeoffXPD[0][1]=LANDTAKEOFFXPD;
    landTakeoffXPI[0][2]=LANDTAKEOFFXPI;

    landTakeoffYP[1][0]=LANDTAKEOFFYP;
    landTakeoffYPD[1][1]=LANDTAKEOFFYPD;
    landTakeoffYPI[1][2]=LANDTAKEOFFYPI;

    landTakeoffZP[2][0]=LANDTAKEOFFZP;
    landTakeoffZPD[2][1]=LANDTAKEOFFZPD;
    landTakeoffZPI[2][2]=LANDTAKEOFFZPI;

    landTakeoffYawP[3][0]=LANDTAKEOFFYAWP;
    landTakeoffYawPD[3][1]=LANDTAKEOFFYAWPD;
    landTakeoffYawPI[3][2]=LANDTAKEOFFYAWPI;

    //TODO : the same for the other modes

    //Send init message to main
    sendMessage(mainITMHandler, currentMessage);

    FILE* writtenObjectives;
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

    autopilotObjectiveFifo_t autopilotObjectiveFifo;
    autopilotObjectiveFifo.firstObjective = NULL;
    autopilotObjectiveFifo.currentObjectivePriority = 0;
    autopilotObjectiveFifo.numberOfObjectivesPending = 0


    // File reading for basic configuration

    writtenObjectives = fopen(OBJECTIVES_PATH, "r");
    if (writtenObjectives == NULL)
    {
        printDebug("Autopilot objective file not found");
        currentMessage.message = "autopilot_objective_file_not_found";
        currentMessage.priority = 5;
        sendMessage(mainITMHandler, currentMessage);
    }
    else
    {
        while (fscanf(writtenObjectives, "%s %d %lf %lf %lf %lf", &readObjectiveName, &readObjectiveCode, &readObjectiveDestinationLat, &readObjectiveDestinationLong, &readObjectiveDestinationAlt, &readObjectiveMaxSpeed) != 0)
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
                    if (insertObjective(&readObjective))
                    {
                        printDebug("Insertion of a new autopilot objective success !")
                        lineNumber++;
                    }
                    else printDebug("Insertion of a new autopilot objective error");
                }
                else printDebug("Autopilot objective speed, or destination is incorrect")

            }
            else printDebug("Autopilot objective type is incorrect");
        }
        printDebug("%d Objectives added to Autopilot FIFO", lineNumber);
        lineNumber = 0;
    }

    //Notify main thread of end of init

    currentMessage.message = "end_of_init"
    currentMessage.priority = 20;

    sendMessage(mainITMHandler, currentMessage);

    while (1) // This loop iterates each time an objective is reached
    {
        receivedMessage = retrieveMessage(autopilotITMHandler
        if (receivedMessage != NULL)
        {
             // TODO : process message
             printDebug("New ITM message received by autopilot");
        }


       currentObjective = readCurrentObjective();
       initCalculation(currentObjective);
       ini


        while () // This loop iterates after each ITMhandler execution and calculation
        {
            if(tickCounter == MESSAGE_CHECKING_LIMIT)
            {
                tickCounter=0;

                receivedMessage = retrieveMessage(autopilotITMHandler);
                if (receivedMessage != NULL)
                {
                    // TODO : process message
                    printDebug("New ITM message received by autopilot");

                    if(receivedMessage->message == "autopilot_new_objective")
                    {
                       //TODO : behaviour
                    }

                    else if (receivedMessage->message == "autopilot_emergency_landing")
                    {
                        // TODO : behaviour
                    }

                    //TODO : add all the events handling

                    else
                    {
                        printDebug("Autopilot received an unrecognized ITM message")
                        //TODO : send event to main
                    }


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
            // Calculation Area


        }


        fclose(writtenObjectives);
    }



}

