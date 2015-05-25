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

#define SAMPLING_PERIOD_MS 10

// Global variables definitions :

receivedCommandsShared_t receivedCommands;

// Functions :

void* readerHandler(void* arg)
{
    printDebug("[i] New reader thread launched !");

    int isUltrasonicOn = 0;
    sampleList_t* ultrasonicSampleList;
    float filteredValue=0;
    float ultrasonic=0;
    float ultrasonicTemp=0;
    int testpwm = 0;
    int testUltrasonic = 0;


    bidirectionnalHandler_t* bidirectionnalHandler;
    bidirectionnalHandler = (bidirectionnalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* readerITMHandler;

    mainITMHandler = bidirectionnalHandler->mainITMHandler;
    readerITMHandler = bidirectionnalHandler->componentITMHandler;

    message_t* receivedMessage;
    message_t currentMessage;

    messageDecoded_t currentDecoded;

    int fd ;
    char c;
    char buffer[32];
    char *data;
    char *numb;
    char data_c[16];
    char numb_c[16]; // Disarming motors on startup to patch armed motors bug :
    int i;
    int finished, started, j, k, number = 0;


    strcpy(currentMessage.message, "main_reader_info_init");
    currentMessage.priority=20;

    sendMessage(mainITMHandler, currentMessage);


    initialize_mutex(&receivedCommands.readWriteMutex);
    pthread_mutex_lock(&receivedCommands.readWriteMutex);


    for (i=0; i<=8; i++) receivedCommands.commands[i]=0;

    pthread_mutex_unlock(&receivedCommands.readWriteMutex);
    i=0;


    if ((fd = serialOpen("/dev/ttyAMA0", BAUDRATE)) < 0) //fd = wiringPiI2CSetup(ARDUINO_ADDRESS) < 0
    {
        printDebug("[e] Reader : error connecting to Arduino via Serial");
        strcpy(currentMessage.message, "main_reader_info_initfailed");
        currentMessage.priority=20;
        sleep(1);

        sendMessage(mainITMHandler, currentMessage);

        pthread_exit(NULL);
    }
    // Notifying main thread init sucedded :

    strcpy(currentMessage.message, "main_reader_info_endofinit");
    currentMessage.priority=20;


    // Now we're connected, processing incoming data :

    while(1)
    {
        // Serial processing AREA :


        while (!finished)
        {

            c = serialGetchar(fd);
            if(c == '_')
            {
                if (started)
                {
                    buffer[i]='\0';
                    data =  strtok( buffer, "=");
                    numb = strtok(NULL, "=");
                    if (data != NULL && numb != NULL)
                    {
                        finished = 1;

                        if (!strcmp(data, "pwm1"))
                        {
                            //printDebug("[i] Got pw1 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[0] = strtof(numb, NULL);
                            //if (testpwm) printf("[i] PWM 1 :%f\n", receivedCommands.commands[0]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "pwm2"))
                        {
                            //printDebug("[i] Got pw2 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[1] = strtof(numb, NULL);
                            //if (testpwm) printf("[i] PWM 2 :%f\n", receivedCommands.commands[1]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "pwm3"))
                        {
                            //printDebug("[i] Got pw3 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[2] = strtof(numb, NULL);
                            //if (testpwm) printf("[i] PWM 3 :%f\n", receivedCommands.commands[2]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "pwm4"))
                        {
                            //printDebug("[i] Got pw4 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[3] = strtof(numb, NULL);
                            //if (testpwm) printf("[i] PWM 4 :%f\n", receivedCommands.commands[3]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "pwm5"))
                        {
                            //printDebug("[i] Got pw5 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[4] = strtof(numb, NULL);
                            if (testpwm) printf("[i] PWM 5 :%f\n", receivedCommands.commands[4]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "pwm6"))
                        {
                            //printDebug("[i] Got pw6 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[5] = strtof(numb, NULL);
                            if (testpwm) printf("[i] PWM 6 :%f\n", receivedCommands.commands[5]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "pwm7"))
                        {
                            //printDebug("[i] Got pw7 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[6] = strtof(numb, NULL);
                            //if (testpwm) printf("[i] PWM 7 :%f\n", receivedCommands.commands[6]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "pwm8"))
                        {
                            //printDebug("[i] Got pw8 value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.commands[7] = strtof(numb, NULL);
                            //if (testpwm) printf("[i] PWM 8 :%f\n", receivedCommands.commands[7]);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }


                        else if (!strcmp(data, "ultradist"))
                        {
                            ultrasonicTemp = strtof(numb, NULL);
                            ultrasonic = ultrasonicTemp;
                            //if (testUltrasonic) printf("[i] Ultrasonic :%f\n", ultrasonicTemp);
                        }

                        else if (!strcmp(data, "status"))
                        {
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            if (!strcmp(numb, "fix")) receivedCommands.gpsStatus = FIX;
                            if (!strcmp(numb, "nofix")) receivedCommands.gpsStatus = NO_FIX;

                            //if (testpwm) printf("Got GPS Status : %d\n", receivedCommands.gpsStatus);

                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "latitude"))
                        {
                            //printDebug("[i] Got ultrasonic value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.latitude = strtof(numb, NULL)/10000000;
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "longitude"))
                        {
                            //printDebug("[i] Got ultrasonic value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.longitude = strtof(numb, NULL)/10000000;
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "altitude"))
                        {
                            //printDebug("[i] Got ultrasonic value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.altitude = strtof(numb, NULL);
                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }

                        else if (!strcmp(data, "voltage"))
                        {
                            //printDebug("[i] Got ultrasonic value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.voltage = strtof(numb, NULL);

                            //if (testpwm) printf("Got voltage : %f\n", receivedCommands.voltage);

                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);

                        }

                        else if (!strcmp(data, "current"))
                        {
                            //printDebug("[i] Got ultrasonic value");
                            pthread_mutex_lock(&receivedCommands.readWriteMutex);
                            receivedCommands.current = strtof(numb, NULL);

                            //if (testpwm) printf("Got current : %f\n", receivedCommands.current);

                            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
                        }



                        //if(testpwm) sleep(1);




                    }



                    strcpy(buffer, "");
                    i=0;
                }
                else
                {
                    started=1;
                    strcpy(buffer, "");
                }
            }

            else
            {
                buffer[i]=c;
                i++;
            }

        }
        finished = 0;



        // Message processing AREA :
        receivedMessage = retrieveMessage(readerITMHandler);
        if (receivedMessage != NULL)

{
        currentDecoded = decodeMessageITM(receivedMessage);



        if(!strcmp(currentDecoded.message,"ultrasonicon"))
        {
            if (isUltrasonicOn)
            {
                printDebug("[e] Something strange in reader : asked to turn on an already on ultrasonic");
            }
            else printDebug("[i] Turning ultrasonic filtering on");
            ultrasonicSampleList = initUltrasonic();
            isUltrasonicOn = 1;

            pthread_mutex_lock(&receivedCommands.readWriteMutex);
            receivedCommands.ultrasonicTelemeter = -1;
            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
        }

        else if (!strcmp(currentDecoded.message, "ultrasonicoff"))
        {
            if(!isUltrasonicOn)
            {
                printDebug("[e] Something strange in reader : asked to turn off an already off ultrasonic");
            }
            else printDebug("[i] Turning ultrasonic filtering off");
            shutdownUltrasonic(ultrasonicSampleList);
            isUltrasonicOn = 0;

            pthread_mutex_lock(&receivedCommands.readWriteMutex);
            receivedCommands.ultrasonicTelemeter = -1;
            pthread_mutex_unlock(&receivedCommands.readWriteMutex);
        }

         else if (!strcmp(currentDecoded.message, "testpwm"))
         {
             if (!testpwm)
             {
                 testpwm = 1;
                 printDebug("[i] Turning on pwm reading test");

             }

             else
             {
                 testpwm = 0;
                 printDebug("[i] Turning off pwm reading test");
             }
         }

         else if (!strcmp(currentDecoded.message, "testultrasonic"))
         {
             if (!testUltrasonic)
             {
                 testUltrasonic = 1;
                 printDebug("[i] Turning on ultrasonic test");

             }

             else
             {
                 testUltrasonic = 0;
                 printDebug("[i] Turning off ultrasonic reading test");
             }
         }



}


        if (isUltrasonicOn)
        {
            if (ultrasonic != -1) addToSampleList(ultrasonic, ultrasonicSampleList);
            ultrasonicTemp = getFilteredUltrasonic(*ultrasonicSampleList);

            pthread_mutex_lock(&receivedCommands.readWriteMutex);

            if (ultrasonicTemp != -1)
            {
                filteredValue = ultrasonicTemp;
                if (testUltrasonic) printDebug("[i]New ultrasonic filtered value");
                if (testUltrasonic) printf("%f\n", filteredValue);

                receivedCommands.ultrasonicTelemeter = filteredValue;


            }
            else
            {
                if(testUltrasonic) printDebug("Not enough elements in sample list");
                receivedCommands.ultrasonicTelemeter = -1;
            }

            pthread_mutex_unlock(&receivedCommands.readWriteMutex);


        }


        usleep(SAMPLING_PERIOD_MS*1000);

    }

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
    if (sampleList.numberOfSamples != SAMPLESIZE)
    {
        return -1;
    }

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

int notifyPwmChannelNull()
{
    return 0;
}






