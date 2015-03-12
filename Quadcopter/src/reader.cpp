#include "reader.h"

/*
##############################################
#   Reader functionality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/


// Software defines :

#define ARDUINO_ADDRESS 1
#define SAMPLING_PERIOD_MS 10

// Global variables definitions :

receivedCommandsShared_t receivedCommands;

// Functions :

void* readerHandler(void* arg)
{
    printDebug("[i] New reader thread launched !");

    int i;
    int isUltrasonicOn = 0;
    sampleList_t* ultrasonicSampleList;
    float filteredValue;


    bidirectionnalHandler_t* bidirectionnalHandler;
    bidirectionnalHandler = (bidirectionnalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* readerITMHandler;

    mainITMHandler = bidirectionnalHandler->mainITMHandler;
    readerITMHandler = bidirectionnalHandler->componentITMHandler;

    message_t* receivedMessage;
    message_t currentMessage;

    strcpy(currentMessage.message, "main_reader_info_init");
    currentMessage.priority=20;

    sendMessage(mainITMHandler, currentMessage);



    long buffer[9];
    int fd;

    pthread_mutex_lock(&receivedCommands.readWriteMutex);


    for (i=1; i<=9; i++) receivedCommands.commands[i]=0;

    pthread_mutex_unlock(&receivedCommands.readWriteMutex);


    if (true) //fd = wiringPiI2CSetup(ARDUINO_ADDRESS) < 0
    {
        printDebug("[e] Reader : error connecting to Arduino via I2C");
        strcpy(currentMessage.message, "main_reader_info_initfailed");
        currentMessage.priority=20;
        sleep(1);

        sendMessage(mainITMHandler, currentMessage);

        pthread_exit(NULL);
    }

    // Now we're connected, processing incoming data :

    while(1)
{
    // Message processing AREA :
    receivedMessage = retrieveMessage(readerITMHandler);


        //TODO : make message parser
        /*

        if(receivedMessage.message == "ultrasonicon")
        {
            if (isUltrasonicOn)
            {
                printDebug("Something strange in reader : asked to turn on an already on ultrasonic")
            }            ultrasonicSampleList = initUltrasonic();
            isUltrasonicOn = 1;
        }

        if (receivedMessage.message == "ultrasonicoff")
        {
            if(!isUltrasonicOn)
            {
                printDebug("Something strange in reader : asked to turn off an already off ultrasonic")
            }
            shutdownUltrasonic(ultrasonicSampleList);
            isUltrasonicOn = 0;
        }

        */



        read(fd, buffer, 4*9); //TODO implement connection test and ultrasonic reading

        if (isUltrasonicOn)
        {
            //addToSampleList(VALUE. ultrasonicSampleList);
            filteredValue = getFilteredUltrasonic(*ultrasonicSampleList);
        }

        // TODO : integrate ultrasonic

        pthread_mutex_lock(&receivedCommands.readWriteMutex);

        for (i=0; i<=8; i++)
        {
            receivedCommands.commands[i] = buffer[i];
        }

        receivedCommands.ultrasonicTelemeter = filteredValue;



        pthread_mutex_unlock(&receivedCommands.readWriteMutex);

        sleep(SAMPLING_PERIOD_MS/1000);

    }
    close(fd);

}

sampleList_t* initUltrasonic()
{
    sampleList_t* list;
    list = (sampleList_t*)malloc(SAMPLESIZE*sizeof(sampleList_t));
    list->lastEntryIndex = -1;
    list->numberOfSamples = 0;

    return list;
}


void addToSampleList(float sample, sampleList_t* list)
{
    int toBeInsertedIndex;
    int i;


    if (list->lastEntryIndex + 1 > SAMPLESIZE-1) toBeInsertedIndex = 0;
    else toBeInsertedIndex = list->lastEntryIndex +1;

    list->list[toBeInsertedIndex] = sample;
    list->lastEntryIndex = toBeInsertedIndex;

    if (list->numberOfSamples < SAMPLESIZE) list->numberOfSamples++;
}

float getFilteredUltrasonic(sampleList_t sampleList)
{
    int i;
    float result;
    float sampleCopy[SAMPLESIZE];

    for (i=0; i<SAMPLESIZE; i++) sampleCopy[i] = sampleList.list[i]; // In order to sort the table, we need to copy it
    qsort(sampleCopy, SAMPLESIZE, sizeof(float), comp);

    result = (sampleCopy[SAMPLESIZE/2] + sampleCopy[SAMPLESIZE/2 +1])/2; // As it it as pair number, me take this as a median

    return result;
}

void shutdownUltrasonic(sampleList_t* sampleList)
{
    free(sampleList);

}

int comp(const void *a, const void *b)
{
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}






