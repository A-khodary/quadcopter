#include "autopilot.h"
#include "autopilot_global_variables.h"//variables globales définies 2 fois => choisir fichier

/*
##############################################
#   Autopilot functionnality                 #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

//TODO : adapter les différents PID à utiliser pour chaque mode

void makeAsserv(autopilotObjective_t* autopilotObjective) {

    float landTakeoffXP, landTakeoffXPI, landTakeoffXPD, landTakeoffYP, landTakeoffYPI, landTakeoffYPD, landTakeoffZP, landTakeoffZPI, landTakeoffZPD, landTakeoffYawP, landTakeoffYawPI, landTakeoffYawPD
    //  TODO : use coeff defines in variables

    double destinationX, destinationY, destinationZ, currentPositionX,currentPositionY, currentPositionZ, x_computed, y_computed, z_computed, yaw_computed, roll_computed, pitch_computed, directionYAW, currentYAW, currentROLL, currentPITCH;

    switch(autopilotObjective.code){

    case 1 : //mode  LAND_TAKEOFF

        PID x_pid(landTakeoffXP,landTakeoffXPI,landTakeoffXPD), y_pid(landTakeoffYP,landTakeoffYPI,landTakeoffYPD), z_pid(landTakeoffZP,landTakeoffZPI,landTakeoffZPD), yaw_pid(landTakeoffYawP,landTakeoffYawPI,landTakeoffYawPD);// coefficients à trouver empiriquement

        currentPositionX = positionShared.x;
        currentPositionY = positionShared.y;
        currentPositionZ = positionShared.z;
        currentYAW =  flightStateShared.yaw;
        currentROLL =  flightStateShared.roll;
        currentPITCH =  flightStateShared.pitch;


        destinationX = autopilotObjective.destinationX;
        destinationY = autopilotObjective.destinationY;
        destinationZ = autopilotObjective.destinationZ;
        directionYAW = autopilotObjective.directionYAW;

        x_computed = x_pid.compute(currentPositionX, destinationX);
        y_computed = y_pid.compute(currentPositionY, destinationY);
        z_computed = z_pid.compute(currentPositionZ, destinationZ);
        yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

        break;

    case 2: //mode GOTO_STANDARD

        PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0), yaw_pid(2,0,0);// coefficients à trouver empiriquement
    ,
        currentPositionX = positionShared.x;
        currentPositionY = positionShared.y;
        currentPositionZ = positionShared.z;
        currentYAW =  flightStateShared.yaw;
        currentROLL =  flightStateShared.roll;
        currentPITCH =  flightStateShared.pitch;

        destinationX = autopilotObjective.destinationX;
        destinationY = autopilotObjective.destinationY;
        destinationZ = autopilotObjective.destinationZ;
        directionYAW = autopilotObjective.directionYAW;

        x_computed = x_pid.compute(currentPositionX, destinationX);
        y_computed = y_pid.compute(currentPositionY, destinationY);
        z_computed = z_pid.compute(currentPositionZ, destinationZ);
        yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

        break;

    case 3 : //mode GOTO_HOVERING

        PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0), yaw_pid(2,0,0);// coefficients à trouver empiriquement

        currentPositionX = positionShared.x;
        currentPositionY = positionShared.y;
        currentPositionZ = positionShared.z;
        currentYAW =  flightStateShared.yaw;
        currentROLL =  flightStateShared.roll;
        currentPITCH =  flightStateShared.pitch;

        destinationX = autopilotObjective.destinationX;
        destinationY = autopilotObjective.destinationY;
        destinationZ = autopilotObjective.destinationZ;
        directionYAW = autopilotObjective.directionYAW;

        x_computed = x_pid.compute(currentPositionX, destinationX);
        y_computed = y_pid.compute(currentPositionY, destinationY);
        z_computed = z_pid.compute(currentPositionZ, destinationZ);
        yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

        break;

    case 4 : // mode POSITION_HOLD

        break;

    default : // mode POSITION_HOLD par défault


        break;
}
}

int insertObjective(autopilotObjective_t* objective, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    int objectiveIndex;
    autopilotObjective_t* currentObjective;
    autopilotObjective_t* lastObjective;

    pthread_mutex_lock(autopilotObjectiveFifo.readWrite);

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
        pthread_mutex_unlock(autopilotObjectiveFifo.readWrite);

        return 1;
}

}
/* Return summary :
-1 Fifo non initialized
-2 Fifo is full
 1 Inserting Ok


*/


// TODO Servo controlling functions

servoControl_t buildServoControl(autopilotObjective_t autopilotObjective)
{
    servoControl_t currentServoControl;


    switch(autopilotObjective.code)
    {

    case LAND_TAKEOFF : //mode  LAND_TAKEOFF

        currentServoControl.oneWayNumber = 3;
        currentServoControl.ServoControlData = (malloc(3*sizeof(oneWayServoControl));
        currentServoControl.ServoControlData[0].type=


        break;

    case GOTO_STANDARD: //mode GOTO_STANDARD

        PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0), yaw_pid(2,0,0);// coefficients à trouver empiriquement
    ,
        currentPositionX = positionShared.x;
        currentPositionY = positionShared.y;
        currentPositionZ = positionShared.z;
        currentYAW =  flightStateShared.yaw;
        currentROLL =  flightStateShared.roll;
        currentPITCH =  flightStateShared.pitch;

        destinationX = autopilotObjective.destinationX;
        destinationY = autopilotObjective.destinationY;
        destinationZ = autopilotObjective.destinationZ;
        directionYAW = autopilotObjective.directionYAW;

        x_computed = x_pid.compute(currentPositionX, destinationX);
        y_computed = y_pid.compute(currentPositionY, destinationY);
        z_computed = z_pid.compute(currentPositionZ, destinationZ);
        yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

        break;

    case GOTO_HOVERING : //mode GOTO_HOVERING

        PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0), yaw_pid(2,0,0);// coefficients à trouver empiriquement

        currentPositionX = positionShared.x;
        currentPositionY = positionShared.y;
        currentPositionZ = positionShared.z;
        currentYAW =  flightStateShared.yaw;
        currentROLL =  flightStateShared.roll;
        currentPITCH =  flightStateShared.pitch;

        destinationX = autopilotObjective.destinationX;
        destinationY = autopilotObjective.destinationY;
        destinationZ = autopilotObjective.destinationZ;
        directionYAW = autopilotObjective.directionYAW;

        x_computed = x_pid.compute(currentPositionX, destinationX);
        y_computed = y_pid.compute(currentPositionY, destinationY);
        z_computed = z_pid.compute(currentPositionZ, destinationZ);
        yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

        break;

    case 4 : // mode POSITION_HOLD

        break;

    default : // mode POSITION_HOLD par défault


        break;
    }

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


void initCalculation(autopilotObjective_t* autopilotObjective)
{


}

void updateCalculation(autopilotObjective_t* autopilotObjective)
{


}






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

    currentMessage.message = "autopilot_init"
    currentMessage.priority = 20;

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


        while () // This loop iterates after each ITMhandler execution and calculation
        {
            if(tickCounter == MESSAGE_CHECKING_LIMIT)
            {
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
                usleep(AUTOPILOT_REFRESHING_PERIOD);
            }


        }



    }



}

