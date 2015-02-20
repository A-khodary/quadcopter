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


#include "shared_librairies.h"


//          ###     Components includes     ###

#include "autopilot.h"
#include "data_logger.h"
#include "RTIMULib.h"
#include "osd.h"
#include "pilot.h"
#include "reader_global_functions.h"

//          ###     Shared functions includes      ###

#include "global_functions.h"
#include "autopilot_global_functions.h"
#include "data_logger_global_functions.h"


//          ###     Shared variables includes      ###

#include "reader_global_variables.h"
#include 'pilot_global_variables.h"


int main()
{

    printDebug("Main thread is initializing...")


    // Threads declaration

    pthread readerThread, writerThread, pilotThread, dataLoggerThread, autopilotThread, imuThread;

    // Message declarations :

    message_t* currentMessage;
    messageDecoded_t currentDecodedMessage;

    // Global variables definition : (normally defined in specific component handler init)


    // Main handler init :

    handler_t* mainITMHandler = initializeHandler();
    if (mainITMHandler == NULL)
    {
        printDebug("Main handler init error");
        return 0;
    }

    // ITM handler of components filling in :

    //  Autopilot :

    handler_t* autopilotITMHandler() = initializeHandler();
    if (autopilotITMHandler == NULL) printDebug("Autopilot handler init error");


    bidirectionalHandler_t autopilotBidirectionalHandler;
    autopilotBidirectionalHandler.mainITMHandler = &mainITMHandler;
    autopilotBidirectionalHandler.componentITMHandler = &autopilotITMHandler;

    //  Pilot

    handler_t* pilotITMHandler() = initializeHandler();
    if (pilotITMHandler == NULL) printDebug("Pilot handler init error");

    bidirectionalHandler_t pilotBidirectionalHandler;
    pilotBidirectionalHandler.mainITMHandler = &mainITMHandler;
    pilotBidirectionalHandler.componentITMHandler = &pilotITMHandler;

    // Data logger :

    handler_t* dataLoggerITMHandler() = initializeHandler();
    if (dataLoggerITMHandler == NULL) printDebug("Data Logger handler init error");


    bidirectionalHandler_t dataLoggerBidirectionalHandler;
    dataLoggerBidirectionalHandler.mainITMHandler = &mainITMHandler;
    dataLoggerBidirectionalHandler.componentITMHandler = &dataLoggerITMHandler;


    // Reader

    handler_t* readerITMHandler() = initializeHandler();
    if (readerITMHandler == NULL) printDebug("Reader handler init error");

    bidirectionalHandler_t readerBidirectionalHandler;
    readerBidirectionalHandler.mainITMHandler = &mainITMHandler;
    readerBidirectionalHandler.componentITMHandler = &readerITMHandler;


    // Test initialization :





    // End of test initialization


    printDebug("Launching components threads...")


    pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
    pthread_create(&writerThread, NULL, writerHandler, (void*)mainITMHandler);
    pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionalHandler);
    pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, (void*)&dataLoggerBidirectionnalHandler);
    pthread_create(&autopilotThread, NULL, autopilotHandler, (void*)&autopilotBidirectionalHandler);
    pthread_create(&imuThread, NULL, imuHandler, (void*)mainITMHandler);


    while(1)
    {
        // Message processing Area :

        currentMessage = retrieveMessage(mainITMHandler);
        currentDecodedMessage = decodeMessageITM()(currentMessage);



        // TODO handling message :

        if(currentDecodedMessage.destination == "autopilot")
        {
            printDebug("Main Thread is dispatching a message to autopilot : %s", currentDecodedMessage.message);
            sendMessage(&autopilotITMHandler, currentMessage);// autopilotITMHandler deja pointeur => &???

        }

        if(currentDecodedMessage.destination == "pilot")
        {
            printDebug("Main Thread is dispatching a message to pilot : %s", currentDecodedMessage.message);
            sendMessage(&pilotITMHandler, currentMessage);

        }

        if(currentDecodedMessage.destination == "datalogger")
        {
            printDebug("Main Thread is dispatching a message to datalogger : %s", currentDecodedMessage.message);
            sendMessage(&dataLoggerITMHandler, currentMessage);
        }

        if(currentDecodedMessage.destination == "reader")
        {
            printDebug("Main Thread is dispatching a message to reader : %s", currentDecodedMessage.message);
            sendMessage(&readerITMHandler, currentMessage);
        }

        if(currentDecodedMessage.destination == "main")
        {

            if(currentDecodedMessage.operation == INFO)
            {

                if (currentDecodedMessage.message == "restartthreadautopilot")
                {
                    pthread_cancel(autopilotThread);
                    // TODO : make restart
                    pthread_create(&autopilotThread, NULL, autopilotHandler, (void*)&autopilotBidirectionalHandler);

                }
                else if (currentDecodedMessage.message == "restartthreaddatalogger")
                {
                    pthread_cancel(dataLoggerThread);
                    // TODO : make restart
                    pthread_create(&dataLoggerThread, NULL, dataLoggerHandler, (void*)&dataLoggerBidirectionnalHandler);
                }

                else if (currentDecodedMessage.message == "restartthreadpilot")
                {
                    pthread_cancel(pilotThead);
                    // TODO : make restart
                    pthread_create(&pilotThread, NULL, pilotHandler, (void*)&pilotBidirectionalHandler);
                }
                // TODO : do it for all threads
                else if (currentDecodedMessage.message == "restartthreadimu")
                {
                    pthread_cancel(imuThread);
                    // TODO : make restart
                    pthread_create(&imuThread, NULL, imuHandler, (void*)mainITMHandler);

                }

                else if (currentDecodedMessage.message == "restartthreadreader")
                {
                    pthread_cancel(readerThread);
                    // TODO : make restart
                    pthread_create(&readerThread, NULL, readerHandler, (void*)&readerBidirectionnalHandler);
                }

                else if (currentDecodedMessage.message == "restartthreadwriter")
                {
                    pthread_cancel(writerThread);
                    // TODO : make restart
                    pthread_create(&writerThread, NULL, writerHandler, (void*)mainITMHandler);
                }

                else if (currentDecodedMessage.message == "emergencylanding") // the autopilot can notify main of such event
                {

                    // TODO
                }

                else if (currentDecodedMessage.message == "landed") // the autopilot can notify main of such event
                {

                    // TODO
                    pthread_cancel(writerThread);
                    pthread_cancel(autopilotThread);
                }

                else if (currentDecodedMessage.message == "crashed") // the autopilot can notify main of such event
                {

                    // TODO
                    pthread_cancel(writerThread);
                    pthread_cancel(autopilotThread);
                }

                else if (currentDecodedMessage.message == "takeoffed") // the autopilot can notify main of such event
                {

                    // TODO
                }

                else if (currentDecodedMessage.message == "invalidobjectivepath") // the autopilot can notify main of such event
                {

                    // TODO
                    printDebug("Invalid objective path for autopilot");
                    currentDecodedMessage.message == "restartthreadautopilot";// ???
                }


            }

            else
            {
                printDebug("Main thread received a message, but it was not recognized");
            }


        }

        else
        {
            printDebug("Invalid destination for message !");
        }


    }




    return 0;
}
