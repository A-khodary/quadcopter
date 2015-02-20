#include "autopilot.h"


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

    // Locating last objective :


    currentObjective = autopilotObjectiveFifo.firstObjective;

    if (currentObjective->priority > objective->priority )
    {

        autopilotObjectiveFifo.firstObjective = objective
        autopilotObjectiveFifo->firstObjective = objective;

        objective->previousObjective = NULL;
        objective->nextObjective = currentObjective;
        currentObjective->previousObjective = objective

    }


    while (currentObjective->nextObjective != NULL) currentObjective = currentObjective->nextObjective;
    lastObjective = currentObjective;

    // Checking last objective priority :

    if (lastObjective->priority < objective->priority)
    {
        // Now we have to search the last objective corresponding to priority :

        currentObjective = lastObjective;
        while (currentObjective->previousObjective != NULL && currentObjective->priority < objective->priority) currentObjective = currentObjective->previousObjective;

        if (currentObjective->previousObjective == NULL)
        {
            currentObjective->previousObjective = objective; // We have reached the first element of the fifo // TODO : debug on first objective
        }

        else // We need to insert our objective between the two
        {
            // Preparing our objective :
            objective->nextObjective= currentObjective->nextObjective;
            objective->previousObjective = currentObjective;

            // Inserting it :
            currentObjective->nextObjective->previousObjective = objective;
            currentObjective->nextObjective = objective;
        }

    }
    else
    {
        // Preparing our Objective :
        objective->previousObjective = lastObjective;
        objective->nextObjective = NULL;

        // Inserting it :
        lastObjective->nextObjective = objective;
    }

    autopilotObjectiveFifo.numberOfObjectivesPending++;
    return 1;
}


servoControl_t* buildServoControl(autopilotObjective_t* autopilotObjective)
{
    int i,j;

    if (autopilotObjective == NULL)
    {
        printDebug("The autopilot objective passed to buildServoControl was NULL");

        return NULL;
    }

    servoControl_t* currentServoControl;
    currentServoControl = (servoControl_t*)malloc(sizeof(servoControl_t));


    switch(autopilotObjective->code)
    {

    case LAND_TAKEOFF : //mode  LAND_TAKEOFF

        currentServoControl->oneWayNumber = 4;

        currentServoControl->ServoControlData = (malloc(4*sizeof(oneWayServoControl_t)));

                                                // Basic initialization of commands :
                                                currentServoControl->ServoControlData[0]->type="x";
                                                currentServoControl->ServoControlData[1]->type="y";
                                                currentServoControl->ServoControlData[2]->type="z";
                                                currentServoControl->ServoControlData[3]->type="yaw";

                                                //locking position mutex in order to get position
                                                pthread_mutex_lock(&positionShared->readWriteMutex);

                                                currentServoControl->ServoControlData[0]->consign = positionShared->x;
                                                currentServoControl->ServoControlData[1]->consign = positionShared->y;

                                                //Unlocking mutex now we don't need the position anymore :
                                                pthread_mutex_unlock(&positionShared->readWriteMutex);

                                                currentServoControl->ServoControlData[2]->consign = autopilotObjective->destinationZ;
                                                currentServoControl->ServoControlData[3]->consign = autopilotObjective->directionBearing;

        for (i=0 ; i < currentServoControl->oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
            {
                // Filling-in coefficients :
                if (j==0) currentServoControl->servoControlData[i]->kp landTakeOffCoeff[i][j];
                if (j==1) currentServoControl->servoControlData[i]->kd landTakeOffCoeff[i][j];
                if (j==2) currentServoControl->servoControlData[i]->ki landTakeOffCoeff[i][j];

            }

            // Creating the PID instance :
            currentServoControl->servoControlData[i]->pid = new PID(currentServoControl->servoControlData[i]->kp, currentServoControl->servoControlData[i]->ki, currentServoControl->servoControlData[i]->kd);

        }


    break;

    case GOTO_STANDARD: //mode GOTO_STANDARD

            currentServoControl->oneWayNumber = 3;
            currentServoControl->ServoControlData = (malloc(2*sizeof(oneWayServoControl));

                                                    // Basic initialization of commands :
                                                    currentServoControl->ServoControlData[0]->type="z";
                                                    currentServoControl->ServoControlData[1]->type="yaw";
                                                    currentServoControl->ServoControlData[2]->type="dist";


                                                    currentServoControl->ServoControlData[0]->consign = autopilotObjective->destinationZ;

                                                    //Linking distance and bearing to the autopilot Objective ones :
                                                    currentServoControl->ServoControlData[1]->consign = autopilotObjective->destinationBearing;

                                                    // Dealing with altitude :
                                                    currentServoControl->ServoControlData[2]->consign = autopilotObjective->destinationZ;


        for (i=0 ; i < currentServoControl->oneWayNumber ; i++)
        {

            for (j=0 ; j < 3 ; j++)
                {
                    // Filling-in coefficients :
                    if (j==0) currentServoControl->servoControlData[i]->kp = landTakeOffCoeff[i][j];
                    if (j==1) currentServoControl->servoControlData[i]->kd = landTakeOffCoeff[i][j];
                    if (j==2) currentServoControl->servoControlData[i]->ki = landTakeOffCoeff[i][j];

                }

            // Creating the PID instance :
            currentServoControl->servoControlData[i]->pid = new PID(currentServoControl->servoControlData[i]->kp, currentServoControl->servoControlData[i]->ki, currentServoControl->servoControlData[i]->kd);
        }


    break;

    case GOTO_HOVERING: //mode GOTO_HOVERING

            // TODO


    break;

    case POSITION_HOLD: // mode POSITION_HOLD

                // TODO

    break;

    default: // Notify the objective is not recognized

        printDebug("Invalid autopilot Objective code !");
        return NULL;


    break;
    }

return currentServoControl;
}


void freeServoControl(servoControl_t* servoControl)
{
    int i;
    for (i=0; i < servoControl->oneWayNumber; i++) free(servoControl->ServoControlData[i]);
    free(servoControl);
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
            autopilotObjectiveFifo.numberOfObjectivesPending--

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

        currentObjective =  autopilotObjectiveFifo->firstObjective;

        while(currentObjective != NULL)
        {
            nextCurrentObjective = currentObjective->nextObjective;
            free(currentObjective); // Freiing the ressources
            autopilotObjectiveFifo.numberOfObjectivesPending--;

            currentObjective = nextCurrentObjective;

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
    autopilotObjective_t* objective;
    objective = autopilotObjectiveFifo->firstObjective->nextObjective;

    autopilotObjectiveFifo->firstObjective->nextObjective->previousObjective = NULL;
    freeAutopilotObjective(autopilotObjectiveFifo->firstObjective);

    autopilotObjectiveFifo->firstObjective = objective;
    autopilotObjectiveFifo.numberOfObjectivesPending--;
    autopilotObjectiveFifo.currentObjectivePriority = autopilotObjectiveFifo->firstObjective.priority; //TODO : clarify priorities
    return 1;
}


autopilotObjective_t* readSpecificObjectivebyNumber(int objectiveNumber, autopilotObjectiveFifo_t autopilotObjectiveFifo)
{
    autopilotObjective_t currentObjective;



    if (autopilotObjectiveFifo.numberOfObjectivesPending < objectiveNumber))
    {
        printDebug("[e] Autopilot : not as many objectives as asked");
        return NULL;
    }
    else
    {

        if (objectiveNumber == 0)  return autopilotObjectiveFifo->firstObjective;
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
    currentObjective = autopilotObjectiveFifo->firstObjective;

    int i = 1;

    while(currentObjective != NULL)
    {
        if(!strcmp(objectiveName, currentObjective->name))
        {
            removeSpecificObjectivebyNumber(i, autopilotObjectiveFifo)
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
    autopilotObjective->destinationDistXY =sqrt( pow(2, autopilotObjective->destinationX - positionShared.x) + pow(2, autopilotObjective->destinationY - positionShared.Y) );
    autopilotObjective->destinationDist =sqrt( pow(2, autopilotObjective->destinationX - positionShared.x) + pow(2, autopilotObjective->destinationY - positionShared.Y) + pow(2, autopilotObjective->destinationZ - positionShared.Z) );

    //Now we're done, unlocking the position mutex :
    pthread_mutex_unlock(&positionShared.readWriteMutex);

     //Objective reaching determination :

     //TODO

     return 0;


} // For now just computes the bearing and several distances, will modify max_speed in the future relative to distance to objective. Returns a boolean that indicates when objective is reached


void makeAsserv(servoControl_t* currentServoControl, autopilotObjective_t* relativeObjective) //If the autopilotObjective_t passed is not null, makeAsserv will do the servo control relatively (allows dynamic setpoint)
{
    int i,
    double command;
    double value;


    if (currentServoControl == NULL)
    {
        printDebug("Trying to make the asserv of a NULL servoControl_t structure");
        return -1;
    }


    for (i=0; i< currentServoControl->oneWayNumber; i++ )
    {
        if (currentServoControl->ServoControlData[i]->type == "yaw")
        {
            // Locking the position mutex in order to obtain the bearing value :
            pthread_mutex_lock(&flightStateShared.readWriteMutex);

            value = flightStateShared.yaw;

            // Unlocking the mutex :
            pthread_mutex_unlock(&flightStateShared.readWriteMutex);

            // Checking if the relative autopilot objective is null :
            if (relativeObjective != NULL) currentServoControl->servoControlData[i]->consign = relativeObjective ->directionBearing;


            // Now we have everything, lets compute :
            command =currentServoControl->servoControlData[i]->pid.compute(value, currentServoControl->servoControlData[i]->consign);

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


        else if (currentServoControl->ServoControlData[i]->type == "x")
        {
            // Locking the position mutex in order to obtain the x value :
            pthread_mutex_lock(&positionShared.readWriteMutex);

            value = positionShared.x;

            // Unlocking the mutex :
            pthread_mutex_unlock(&positionShared.readWriteMutex);

            // Now we have everything, lets compute :
            command =currentServoControl->servoControlData[i]->pid.compute(value, currentServoControl->servoControlData[i]->consign);

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

        else if (currentServoControl->ServoControlData[i]->type == "y")
        {
            // Locking the position mutex in order to obtain the x value :
            pthread_mutex_lock(&positionShared.readWriteMutex);

            value = positionShared.y;

            // Unlocking the mutex :
            pthread_mutex_unlock(&positionShared.readWriteMutex);

            // Now we have everything, lets compute :
            command =currentServoControl->servoControlData[i]->pid.compute(value, currentServoControl->servoControlData[i]->consign);

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

        else if (currentServoControl->ServoControlData[i]->type == "z")
        {
            // Locking the position mutex in order to obtain the x value :
            pthread_mutex_lock(&positionShared.readWriteMutex);

            value = positionShared.z;

            // Unlocking the mutex :
            pthread_mutex_unlock(&positionShared.readWriteMutex);

            // Now we have everything, lets compute :
            command =currentServoControl->servoControlData[i]->pid.compute(value, currentServoControl->servoControlData[i]->consign);

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

        else if (currentServoControl->ServoControlData[i]->type == "dist")
        {
            // Checking if the relative autopilot objective is null :
            if (relativeObjective != NULL) currentServoControl->servoControlData[i]->consign = relativeObjective->destinationDistXY;


            // Now we have everything, lets compute :
            command =currentServoControl->servoControlData[i]->pid.compute(value, currentServoControl->servoControlData[i]->consign);

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




void* autopilotHandler(void* arg)
{
    // Initialization :

    autopilotObjectiveFifo_t autopilotObjectiveFifo;
    autopilotObjectiveFifo.currentObjectivePriority =0;
    autopilotObjectiveFifo.numberOfObjectivesPending = 0;
    autopilotObjectiveFifo.firstObjective = NULL;

    bidirectionalHandler_t* bidirectionalHandler;
    bidirectionalHandler = (bidirectionalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* autopilotITMHandler;

    mainITMHandler = bidirectionalHandler.mainITMHandler;
    autopilotITMHandler = bidirectionalHandler.componentITMHandler;

    message_t* receivedMessage;
    message_t currentMessage;

    servoControl_t* currentServoControl;

    strcpy(currentMessage.message, "main_autopilot_init");
    currentMessage.priority = 20;

    //Send init message to main
    sendMessage(mainITMHandler, currentMessage);


    double distanceToObjective;

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
                        printDebug("Insertion of a new autopilot objective success !");
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
        fclose(writtenObjectives);
    }

    //Notify main thread of end of init


    strcpy(currentMessage.message, "main_autopilot_info_endofinit");
    currentMessage.priority = 20

    sendMessage(mainITMHandler, currentMessage);

    while (1) // This loop iterates each time an objective is reached
    {
        receivedMessage = retrieveMessage(autopilotITMHandler);
        if (receivedMessage != NULL)
        {
            // TODO : process message and decide priorities
            printDebug("New ITM message received by autopilot");
            if(receivedMessage->message == "???_autopilot_order_newobjective")
            {
                //TODO : behaviour
                 insertObjective(receivedMessage->data,autopilotObjectiveFifo);//autopilot_new_objective sous forme de message.data ?
            }

            /*else if (receivedMessage->message == "autopilot_emergency_landing")
            {
                // TODO : behaviour
                 insertObjective(receivedMessage->data,autopilotObjectiveFifo);
            }*/

             //TODO : add all the events handling
             else if(receivedMessage->message == "autopilot_imu_order_???")
             {
                //TODO : behaviour

             }
            else
            {
                printDebug("Autopilot received an unrecognized ITM message")
                //TODO : send event to main
                message_t message;
                strcpy(message.message, "main_autopilot_info_wrongmessage");
                sendMessage(mainITMHandler, message );
            }
        }


        currentObjective = readCurrentObjective();
        initCalculation(currentObjective);
        currentServoControl = buildServoControl(currentObjective);
        if (currentServoControl == NULL)
        {
            printDebug("A servo control structure was returned null to autopilot main Thread, skipping objective...");
            freeAutopilotObjective(currentObjective);
            continue;
        }


        while (1) // This loop iterates after each ITMhandler execution and calculation
    {
        if(tickCounter == MESSAGE_CHECKING_LIMIT)
            {
                tickCounter=0;

                receivedMessage = retrieveMessage(autopilotITMHandler);
                if (receivedMessage != NULL)
                {
                    // TODO : process message and decide priorities
                    printDebug("New ITM message received by autopilot");

                    if(receivedMessage->message == "???_autopilot_order_newobjective")
                    {
                        //TODO : behaviour
                         insertObjective(receivedMessage->data,autopilotObjectiveFifo);//autopilot_new_objective sous forme de message.data ?
                    }

                    /*else if (receivedMessage->message == "autopilot_emergency_landing")
                    {
                        // TODO : behaviour
                         insertObjective(receivedMessage->data,autopilotObjectiveFifo);
                    }*/

                     //TODO : add all the events handling
                     else if(receivedMessage->message == "autopilot_imu_order_???")
                     {
                        //TODO : behaviour

                     }
                    else
                    {
                        printDebug("Autopilot received an unrecognized ITM message")
                        //TODO : send event to main
                        message_t message;
                        strcpy(message.message, "main_autopilot_info_wrongmessage");
                        sendMessage(mainITMHandler, message );
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
            ObjectiveReached = updateCalculation(currentOjective);
            makeAsserv(currentServoControl, currentObjective);

            if(ObjectiveReached)
            {
                //TODO : notify main of the objective completion
                message_t message1;
                strcpy(message1.message, "main_autopilot_info_objectivecompleted");
                sendMessage(mainITMHandler, message1 );
                // Now the objective is reached, we need to free the ressources used :
                freeServoControl(currentServoControl);
                freeAutopilotObjective(currentObjective);


            }


        }




    }



}

<<<<<<< HEAD
=======

>>>>>>> fc5bc8ae0b0c071d8df1346a25b7b004393b0ca1
