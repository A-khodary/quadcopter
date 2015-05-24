/*
###############################################
#                                                                               #
#   Quadcopter V0.0                                                   #
#   Vivien Billaud and Nicolas de Maubeuge                  #
#   Ensea 2014                                                          #
#   All rights reserved                                                 #
#                                                                              #
##############################################


*/

// Quadcopter defines :

#define MAIN_SLEEPING_TIME 100000


#include "shared_librairies.h"


//          ###     Components includes     ###

#include "autopilot.h"
#include "data_logger.h"
#include "RTIMULib/RTIMULib.h"
#include "osd.h"
#include "pilot.h"
#include "imu.h"
#include "reader.h"



//          ###     Shared functions includes      ###

#include "global_functions.h"


//          ###     Shared variables includes      ###

#include "reader_global_variables.h"
#include "pilot_global_variables.h"

//          ###     Module number defines          ###

#define IMU 2
#define AUTOPILOT 1
#define READER 3
#define PILOT 4
#define DATALOGGER 5


//         ###      Behavioural declarations        ###

#define MAX_NUMBER_OF_FAILURE 15

int imuInitComplete;
int readerInitComplete;
int pilotInitComplete;
int dataloggerInitComplete;
int autopilotInitComplete;

int numberOfFailures;

void initBehaviour()
{
    int imuInitComplete=0;
    int readerInitComplete=0;
    int pilotInitComplete=0;
    int dataloggerInitComplete=0;
    int autopilotInitComplete=0;

    int numberOfFailures=0;
}

int notifyInitFailure(int component)
{
    if (numberOfFailures >= MAX_NUMBER_OF_FAILURE)
    {
        printDebug("[e] To much failures, program cannot continue...");
        return 1;
    }
    else return 0;
}

int notifyInitSucess(int component)
{
    if (component == AUTOPILOT) autopilotInitComplete = 1;
    else if (component == IMU) imuInitComplete = 1;
    else if (component == DATALOGGER) dataloggerInitComplete = 1;
    else if (component == READER) readerInitComplete = 1;
    else if (component == PILOT) pilotInitComplete = 1;

    if (autopilotInitComplete && readerInitComplete && pilotInitComplete && dataloggerInitComplete && imuInitComplete) return 1; // In this case, we are ready to go !
    else return 0;
}


int main()
{

    printDebug("[i]Main thread is initializing...");




    // Threads declaration

    pthread_t readerThread, pilotThread, dataLoggerThread, autopilotThread, imuThread;

    // Message declarations :

    message_t* currentMessage;
    messageDecoded_t currentDecodedMessage;
    message_t messageToSend;


    // Global variables definition : (normally defined in specific component handler init)


    // Main handler init :

    handler_t* mainITMHandler = initializeHandler();
    if (mainITMHandler == NULL)
    {
        printDebug("[e]Main handler init error");
        return 0;
    }

    // ITM handler of components filling in :

    //  Autopilot :

    handler_t* autopilotITMHandler = initializeHandler();
    if (autopilotITMHandler == NULL) printDebug("[e]Autopilot handler init error");


    bidirectionnalHandler_t autopilotBidirectionnalHandler;
    autopilotBidirectionnalHandler.mainITMHandler = mainITMHandler;
    autopilotBidirectionnalHandler.componentITMHandler = autopilotITMHandler;

    //  Pilot

    handler_t* pilotITMHandler = initializeHandler();
    if (pilotITMHandler == NULL) printDebug("[e]Pilot handler init error");

    bidirectionnalHandler_t pilotBidirectionnalHandler;
    pilotBidirectionnalHandler.mainITMHandler = mainITMHandler;
    pilotBidirectionnalHandler.componentITMHandler = pilotITMHandler;

    // Data logger :

    handler_t* dataLoggerITMHandler = initializeHandler();
    if (dataLoggerITMHandler == NULL) printDebug("[e]Data Logger handler init error");


    bidirectionnalHandler_t dataLoggerBidirectionnalHandler;
    dataLoggerBidirectionnalHandler.mainITMHandler = mainITMHandler;
    dataLoggerBidirectionnalHandler.componentITMHandler = dataLoggerITMHandler;


    // Reader

    handler_t* readerITMHandler = initializeHandler();
    if (readerITMHandler == NULL) printDebug("[e]Reader handler init error");

    bidirectionnalHandler_t readerBidirectionnalHandler;
    readerBidirectionnalHandler.mainITMHandler = mainITMHandler;
    readerBidirectionnalHandler.componentITMHandler = readerITMHandler;

    //IMU

    handler_t* imuITMHandler = initializeHandler();
    if (imuITMHandler == NULL) printDebug("[e]IMU handler init error");


    printDebug("[i]Launching components threads...");

    pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
    pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionnalHandler);
    pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, (void*)&dataLoggerBidirectionnalHandler);
    pthread_create(&autopilotThread, NULL, autopilotHandler, (void*)&autopilotBidirectionnalHandler);
    if (waitForGPS())
    {
        pthread_create(&imuThread, NULL, imuHandler, (void*)&mainITMHandler);
    }

    // Sending booting info to datalogger :
    messageToSend.dataSize=0;
    strcpy(messageToSend.message, "datalogger_main_info_booting");
    sendMessage(dataLoggerITMHandler, messageToSend);

    while(1)
    {

        usleep(MAIN_SLEEPING_TIME);

        // Message processing Area :

        currentMessage = retrieveMessage(mainITMHandler);

        // In case we don't have any new message :
        if (currentMessage == NULL)
        {
            //printDebug("[i] Main is idle : no messages");
            continue;
        }


        currentDecodedMessage = decodeMessageITM(currentMessage);
        if (!strcmp(currentDecodedMessage.destination, "") || !strcmp(currentDecodedMessage.source, "") || !strcmp(currentDecodedMessage.message, "")   )
        {
            continue;
        }



        //              ### Message redirection AREA    ###


        if(!strcmp(currentDecodedMessage.destination,"autopilot"))
        {
            printDebug("Main Thread is dispatching a message to autopilot");

            if (currentDecodedMessage.message == "newobjective")
            {
                printDebug("[i] Autopilot new objective !");
                messageToSend.priority = 1;
                strcpy(messageToSend.message, "autopilot_datalogger_order_newobjective");
                sendMessage(autopilotITMHandler, messageToSend);
            }
            else sendMessage(autopilotITMHandler, messageToSend);

        }

        else if(!strcmp(currentDecodedMessage.destination,"pilot"))
        {
            printDebug("Main Thread is dispatching a message to pilot");
            sendMessage(pilotITMHandler, *currentMessage);

        }

        else if(!strcmp(currentDecodedMessage.destination,"datalogger"))
        {
            printDebug("[i] Main Thread is dispatching a message to datalogger");
            sendMessage(dataLoggerITMHandler, *currentMessage);
        }

        else if(!strcmp(currentDecodedMessage.destination,"reader"))
        {
            //printDebug("Main Thread is dispatching a message to reader : %s", currentDecodedMessage.message);
            sendMessage(readerITMHandler, *currentMessage);
        }

        else if(!strcmp(currentDecodedMessage.destination,"main"))
        {
            //      ### MAIN messages processing area       ###


            if (!strcmp(currentDecodedMessage.source, "autopilot"))
            {
                if(currentDecodedMessage.operation == INFO)
                {
                    // Initialization relative messages :

                    if (!strcmp(currentDecodedMessage.message,"init")) printDebug("[i] Autopilot started its init");

                    else if(!strcmp(currentDecodedMessage.message,"endofinit"))
                    {
                        printDebug("[i] Autopilot completed its init");
                        if (notifyInitSucess(AUTOPILOT))
                        {
                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "autopilot_main_order_ready");
                            sendMessage(mainITMHandler, messageToSend);

                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "datalogger_main_info_standby");
                            sendMessage(dataLoggerITMHandler, messageToSend);
                        }

                    }

                    else if (!strcmp(currentDecodedMessage.message,"initfailed"))
                    {
                        if (notifyInitFailure(AUTOPILOT)) return 1;
                        printDebug("[e] Autopilot failed its init : this is terrible : no intelligence, restarting the autopilot component...");
                        pthread_cancel(imuThread);
                        pthread_join(imuThread, NULL); // Join to cleanup (prevents memory leak)
                        sleep(1);
                        pthread_create(&imuThread, NULL, imuHandler, (void*)&mainITMHandler);
                    }

                    // Autopilot state notification :

                    else if (currentDecodedMessage.message == "engaged") // the autopilot can notify main of such event
                    {
                        printDebug("[i] Autopilot engaged !");

                        messageToSend.dataSize=0;
                        messageToSend.priority = 20;
                        strcpy(messageToSend.message, "datalogger_main_info_engaged");
                        sendMessage(pilotITMHandler, messageToSend);


                    }

                    else if (currentDecodedMessage.message == "disengaged") // the autopilot can notify main of such event
                    {
                        printDebug("[i] Autopilot disengaged !");

                        messageToSend.dataSize=0;
                        messageToSend.priority = 20;
                        strcpy(messageToSend.message, "datalogger_main_info_disengaged");
                        sendMessage(pilotITMHandler, messageToSend);
                    }

                    else if (currentDecodedMessage.message == "crashed") // the autopilot can notify main of such event
                    {
                        printDebug("Quadcopter has crashed");
                        messageToSend.dataSize=0;
                        messageToSend.priority = 20;
                        strcpy(messageToSend.message, "pilot_main_order_cutoff");
                        sendMessage(pilotITMHandler, messageToSend);

                        // Broadcasting to data logger :

                        strcpy(messageToSend.message, "datalogger_main_info_crashed");
                        messageToSend.priority = 20;
                        sendMessage(dataLoggerITMHandler, messageToSend);
                    }

                    else if (currentDecodedMessage.message == "takeoffed") // the autopilot can notify main of such event
                    {
                        printDebug("[i] Autopilot notified sucessfull takeoff");
                        messageToSend.dataSize=0 ;
                        messageToSend.priority = 20;
                        strcpy(messageToSend.message, "datalogger_main_info_takeoffed");
                        sendMessage(dataLoggerITMHandler, messageToSend);
                    }

                    else if (currentDecodedMessage.message == "landed") // the autopilot can notify main of such event
                    {
                        printDebug("[i] Autopilot notified sucessfull landing");
                        messageToSend.dataSize=0 ;
                        messageToSend.priority = 20;
                        strcpy(messageToSend.message, "datalogger_main_info_landed");
                        sendMessage(dataLoggerITMHandler, messageToSend);
                    }

                    else if (currentDecodedMessage.message == "nowork") // the autopilot can notify main of such event
                    {
                        printDebug("[i] Autopilot notified to be idle");
                        messageToSend.dataSize=0 ;
                        messageToSend.priority = 20;
                        strcpy(messageToSend.message, "datalogger_main_info_nowork");
                        sendMessage(dataLoggerITMHandler, messageToSend);
                    }

                    else if (currentDecodedMessage.message == "objectivereached") // the autopilot can notify main of such event
                    {
                        printDebug("[i] Autopilot notified to have reach an objective!");
                        messageToSend.dataSize=0 ;
                        messageToSend.priority = 20;
                        strcpy(messageToSend.message, "datalogger_main_info_objectivereached");
                        sendMessage(dataLoggerITMHandler, messageToSend);
                    }


                    // Autopilot errors :

                    else if (currentDecodedMessage.message == "invalidobjectivepath") // the autopilot can notify main of such event
                    {

                    }

                    else printDebug("[e] Main received an unknown info from autopilot");





                }

                else printDebug("[e] Main received an unknown action from autopilot");

            }

            if (!strcmp(currentDecodedMessage.source, "pilot"))
            {
                if(currentDecodedMessage.operation == INFO)
                {
                    // Initialization relative messages :
                    if (!strcmp(currentDecodedMessage.message,"init")) printDebug("[i] Pilot started its init");

                    else if(!strcmp(currentDecodedMessage.message,"endofinit"))
                    {
                        printDebug("[i] Pilot completed its init");
                        if (notifyInitSucess(PILOT))
                        {
                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "autopilot_main_order_ready");
                            sendMessage(mainITMHandler, messageToSend);

                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "datalogger_main_info_standby");
                            sendMessage(dataLoggerITMHandler, messageToSend);
                        }
                    }

                    else if (!strcmp(currentDecodedMessage.message,"initfailed"))
                    {
                        if (notifyInitFailure(PILOT)) return 1;
                        printDebug("[e] Pilot failed its init : this is terrible : no command, restarting the pilot component...");
                        pthread_cancel(pilotThread);
                        pthread_join(pilotThread, NULL); // Join to cleanup (prevents memory leak)
                        pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionnalHandler);
                    }

                    else if (!strcmp(currentDecodedMessage.message,"armed"))
                    {
                        printDebug("[i] Pilot has notified armed motors");
                        messageToSend.dataSize=0;
                        messageToSend.priority=20;
                        strcpy(messageToSend.message, "datalogger_main_info_armed");
                        sendMessage(dataLoggerITMHandler, messageToSend);
                    }

                    else if (!strcmp(currentDecodedMessage.message,"disarmed"))
                    {
                        printDebug("[i] Pilot has notified disarmed motors");
                        messageToSend.dataSize=0;
                        messageToSend.priority=20;
                        strcpy(messageToSend.message, "datalogger_main_info_disarmed");
                        sendMessage(dataLoggerITMHandler, messageToSend);
                    }

                    else printDebug("[e] Main received an unknown info from Pilot");

                }

                else
                {
                    printDebug("[e] Main received an unauthorized action from pilot");

                }

            }

            if (!strcmp(currentDecodedMessage.source, "imu"))
            {
                if(currentDecodedMessage.operation == INFO)
                {
                    // Initialization relative messages :
                    if (!strcmp(currentDecodedMessage.message,"init")) printDebug("[i] Imu started its init");

                    else if(!strcmp(currentDecodedMessage.message,"endofinit"))
                {
                    printDebug("[i] Imu completed its init");
                        if (notifyInitSucess(IMU))
                        {
                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "autopilot_main_order_ready");
                            sendMessage(mainITMHandler, messageToSend);

                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "datalogger_main_info_standby");
                            sendMessage(dataLoggerITMHandler, messageToSend);
                        }
                    }

                    else if (!strcmp(currentDecodedMessage.message,"initfailed"))
                    {
                        if (notifyInitFailure(IMU)) return 1;
                        printDebug("[e] IMU failed its init : this is terrible : no user command and no positionning, restarting the imu component...");
                        pthread_cancel(imuThread);
                        pthread_join(imuThread, NULL); // Join to cleanup (prevents memory leak)
                        sleep(1);
                        pthread_create(&imuThread, NULL, imuHandler, (void*)&mainITMHandler);
                    }

                    else printDebug("[e] Main received an unknow info from IMU");


                }

                else
                {
                    printDebug("[e] Main received an unauthorized action from IMU");
                }

            }

            if (!strcmp(currentDecodedMessage.source, "reader"))
            {
                if(currentDecodedMessage.operation == INFO)
                {
                    // Initialization relative messages :
                    if (!strcmp(currentDecodedMessage.message,"init")) printDebug("[i] Reader started its init");

                    else if(!strcmp(currentDecodedMessage.message,"endofinit"))
                    {
                        printDebug("[i] Reader completed its init");
                        if (notifyInitSucess(READER))
                        {
                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "autopilot_main_order_ready");
                            sendMessage(mainITMHandler, messageToSend);

                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "datalogger_main_info_standby");
                            sendMessage(dataLoggerITMHandler, messageToSend);
                        }
                    }

                    else if (!strcmp(currentDecodedMessage.message,"initfailed"))
                    {
                        if (notifyInitFailure(READER)) return 1;
                        printDebug("[e] Reader failed its init : this is terrible : no user command and no ultrasonic, restarting the reader component...");
                        pthread_cancel(readerThread);
                        pthread_join(readerThread, NULL); // Join to cleanup (prevents memory leak)
                        sleep(1);
                        pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
                    }

                    else printDebug("[e] Main received an unknown info from reader");

                }

                else
                {
                    printDebug("[e] Main received an unthaurized action from reader");
                }

            }

            if (!strcmp(currentDecodedMessage.source, "datalogger"))
            {
                if(currentDecodedMessage.operation == INFO)
                {
                    // Initialization relative messages :
                    if (!strcmp(currentDecodedMessage.message,"init")) printDebug("[i] Datalogger started its init");

                    else if(!strcmp(currentDecodedMessage.message,"endofinit"))
                    {
                        printDebug("[i] Datalogger completed its init");
                        if (notifyInitSucess(DATALOGGER))
                        {
                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "autopilot_main_order_ready");
                            sendMessage(mainITMHandler, messageToSend);

                            messageToSend.dataSize=0;
                            messageToSend.priority=5;
                            strcpy(messageToSend.message, "datalogger_main_info_standby");
                            sendMessage(dataLoggerITMHandler, messageToSend);
                        }
                    }

                    else if (!strcmp(currentDecodedMessage.message,"initfailed"))
                    {
                        if (notifyInitFailure(DATALOGGER)) return 1;
                        printDebug("[e] Datalogger failed its init : this is terrible : no user command and logging, restarting the datalogger component...");
                        pthread_cancel(imuThread);
                        pthread_join(imuThread, NULL); // Join to cleanup (prevents memory leak)
                        sleep(1);
                        pthread_create(&imuThread, NULL, imuHandler, (void*)&mainITMHandler);

                    }

                    else printDebug("[e] Main received an unknown info from datalogger");

                }

                else if (currentDecodedMessage.operation == ORDER)
                {
                    if (!strcmp(currentDecodedMessage.message, "restartthreadautopilot"))
                    {
                        pthread_cancel(autopilotThread);
                        pthread_join(autopilotThread,NULL);
                        pthread_create(&autopilotThread, NULL, autopilotHandler, (void*)&autopilotBidirectionnalHandler);

                    }
                    else if (!strcmp(currentDecodedMessage.message, "restartthreaddatalogger"))
                    {
                        pthread_cancel(dataLoggerThread);
                        pthread_join(dataLoggerThread, NULL);
                        pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, (void*)&dataLoggerBidirectionnalHandler);
                    }

                    else if (!strcmp(currentDecodedMessage.message, "restartthreadpilot"))
                    {
                        pthread_cancel(pilotThread);
                        pthread_join(pilotThread, NULL);
                        pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionnalHandler);
                    }

                    else if (!strcmp(currentDecodedMessage.message, "restartthreadimu"))
                    {
                        pthread_cancel(imuThread);
                        pthread_join(imuThread, NULL);
                        /*
                        pthread_create(&imuThread, NULL, imuHandler, (void*)&imuBidirectionnalHandler);
                        */
                        pthread_create(&imuThread, NULL, imuHandler, (void*)&mainITMHandler);

                    }

                    else if (!strcmp(currentDecodedMessage.message, "restartthreadreader"))
                    {
                        pthread_cancel(readerThread);
                        pthread_join(readerThread, NULL);
                        pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
                    }

                    else printDebug("[e] Main received an unknow order from datalogger");
                }

                else
                {
                    printDebug("[e] Main received an unknown operation from datalogger");
                }
            }






            else
            {
                printDebug("[e] Main thread received a message, but it was not recognized. NOTE : main messages have to be of INFO or ORDER type");
            }



        }

        else
        {
            printDebug("[e] Invalid destination for message !");
        }
        free(currentMessage);


    }




    return 0;
}

