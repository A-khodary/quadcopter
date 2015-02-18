#include reader.h

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

// Functions :

void* readerHandler(void* arg)
{
    int isUltrasonicOn = 0;
    float* ultrasonicSampleList;
    float filteredValue;


    bidirectionalHandler_t* bidirectionalHandler;
    bidirectionalHandler = (bidirectionalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* readerITMHandler;

    mainITMHandler = bidirectionalHandler.mainITMHandler;
    readerITMHandler = bidirectionalHandler.componentITMHandler;

    message_t* receivedMessage;
    message_t currentMessage;

    currentMessage.message="main_reader_init";
    currentMessage.priority=20;

    sendMessage(mainITMHandler, currentMessage);



    long buffer[9];
    int fd;

    receivedCommandsShared_t receivedCommands;
    userCommands.ch1=0;
    userCommands.ch2=0;
    userCommands.ch3=0;
    userCommands.ch4=0;
    userCommands.ch5=0;
    userCommands.ch6=0;
    userCommands.ch7=0;
    userCommands.ch8=0;
    userCommands.ch9=0;


    if (fd = wiringPiI2CSetup (ARDUINO_ADDRESS)) < 0)
    {
        printDebug("[e] Reader : error connecting to Arduino via I2C")
        // TODO : notify main thread
    }

    // Now we're connected, processing incoming data :

    while(1)
{
    // Message processing AREA :
    receivedMessage = retrieveMessage(readerITMHandler);

        //TODO : make message parser
        if(receivedMessage.message == "ultrasonicon")
        {
            if (isUltrasonicOn)
            {
                printDebug("Something strange in reader : asked to turn on an already on ultrasonic")
            }
            ultrasonicSampleList = initUltrasonic();
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



        read(fd, buffer, 4*9); //TODO implement connection test and ultrasonic reading

        if (isUltrasonicOn)
        {
            addToSampleList(VALUE. ultrasonicSampleList);
            filteredValue = getFilteredUltrasonic(ultrasonicSampleList);
        }

        // TODO : integrate ultrasonic

        pthread_mutex_lock(receivedCommands.readWriteMutex);

        for (int i=0; i<=8, i++)
        {
            receivedCommands.commands[i] = buffer[i];
        }

        receivedCommands.ultrasonicTelemeter = filteredValue;



        pthread_mutex_unlock(userCommands.readWriteMutex);

        sleep(SAMPLING_PERIOD_MS/1000);

    }
    close(fd);

}

float* initUltrasonic()
{
    float* list;
    list = (float*)malloc(SAMPLESIZE*sizeof(float));
}


void* addToSampleList(float sample, float* sampleList);
float getFilteredUltrasonic(float* sampleList);
shutdownUltrasonic(float* sampleList);



