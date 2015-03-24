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


    // Test initialization :





    // End of test initialization


    printDebug("[i]Launching components threads...");


<<<<<<< HEAD
    //pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
    //pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionnalHandler);
    //pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, (void*)&dataLoggerBidirectionnalHandler);
    pthread_create(&autopilotThread, NULL, autopilotHandler, (void*)&autopilotBidirectionnalHandler);
=======
    pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
    //pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionnalHandler);
    pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, (void*)&dataLoggerBidirectionnalHandler);
    //pthread_create(&autopilotThread, NULL, autopilotHandler, (void*)&autopilotBidirectionnalHandler);
>>>>>>> 520e20d8f17be07ec7ffc02e5f94b18f184f2ddd
    //pthread_create(&imuThread, NULL, imuHandler, (void*)mainITMHandler);


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

        printDebug("[i] NewMessage");



        // TODO handling message :

        if(!strcmp(currentDecodedMessage.destination,"autopilot"))
        {
            printDebug("Main Thread is dispatching a message to autopilot");
            sendMessage(autopilotITMHandler, *currentMessage);

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

            if(currentDecodedMessage.operation == INFO)
            {


                if (currentDecodedMessage.message == "emergencylanding") // the autopilot can notify main of such event
                {
                    printDebug("Received order to do an emergency landing, broadcasting to datalogger...");
                    messageToSend.dataSize=0;
                    strcpy(messageToSend.message, "datalogger_main_info_emergencylanding");
                    sendMessage(dataLoggerITMHandler, messageToSend);
                }

                else if (currentDecodedMessage.message == "landed") // the autopilot can notify main of such event
                {
                    printDebug("Received a landed info, broadcasting to datalogger...");
                    messageToSend.dataSize=0;
                    strcpy(messageToSend.message, "datalogger_main_info_landed");
                    sendMessage(dataLoggerITMHandler, messageToSend);
                }

                else if (currentDecodedMessage.message == "crashed") // the autopilot can notify main of such event
                {

                    // TODO

                }

                else if (currentDecodedMessage.message == "takeoffed") // the autopilot can notify main of such event
                {
                    printDebug("Received a takeoffed info, broadcasting to datalogger...");
                    messageToSend.dataSize=0;
                    strcpy(messageToSend.message, "datalogger_main_info_takeoffed");
                    sendMessage(dataLoggerITMHandler, messageToSend);

                }

                else if (currentDecodedMessage.message == "invalidobjectivepath") // the autopilot can notify main of such event
                {
                    printDebug("Autopilot have notified main of invalid objective path for autopilot");
                }

                else if (!strcmp(currentDecodedMessage.message,"initfailed")) // the autopilot can notify main of such event
                {
                    if (!strcmp(currentDecodedMessage.source, "pilot"))
                    {
                        printDebug("[e] Pilot failed its init : this is terrible : no command, restarting the pilot component...");
                        pthread_cancel(pilotThread);
                        pthread_join(pilotThread, NULL); // Join to cleanup (prevents memory leak)
                        pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionnalHandler);
                    }

                    else if (!strcmp(currentDecodedMessage.source, "reader"))
                    {
                        printDebug("[e] Reader failed its init : this is terrible : no user command and no ultrasonic, restarting the reader component...");
                        pthread_cancel(readerThread);
                        pthread_join(readerThread, NULL); // Join to cleanup (prevents memory leak)
                        sleep(10);
                        pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);

                    }
                }

                else if (!strcmp(currentDecodedMessage.message,"init")) // the autopilot can notify main of such event
                {
                    if (!strcmp(currentDecodedMessage.source, "pilot"))
                    {
                        printDebug("[i] Pilot started its init");
                    }

                    else if (!strcmp(currentDecodedMessage.source, "reader"))
                    {
                        printDebug("[i] Reader started its init");
                    }
                }


                else
                {
                printDebug("[e]Main thread received an INFO message, but its content was not recognized");
                }


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
                    //pthread_create(&imuThread, NULL, imuHandler, (void*)mainITMHandler);

                }

                else if (!strcmp(currentDecodedMessage.message, "restartthreadreader"))
                {
                    pthread_cancel(readerThread);
                    pthread_join(readerThread, NULL);
                    pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
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
