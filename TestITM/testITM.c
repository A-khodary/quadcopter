#include "testITM.h"
#include "inter-thread-messages.h"



void* itmCharger1(void * arg)
{
    handler_t* dest;
    message_t currentMessage;
    dest = (handler_t*)arg;
    char buffer[68];
    int priority = 2;
    int counter=0;

    while(1)
    {
        if (counter == 1000) counter = 0;
        //printf("Writing Charg 1 message number %d\n", counter);

        sprintf(buffer, "Charg 1 : message %d\n", counter);
        strcpy(currentMessage.message, buffer);
        currentMessage.priority = priority;
        sendMessage(dest, currentMessage);
        usleep(100000);
        if (counter == 30) usleep(1000000000);

        //printf("Writed Charg 1 message number %d\n", counter);


        counter++;
    }

}

void* itmCharger2(void * arg)
{
        handler_t* dest;
    message_t currentMessage;
    dest = (handler_t*)arg;
    char buffer[68];
    int priority = 3;
    int counter=0;

    while(1)
    {
        if (counter == 1000) counter = 0;
        //printf("Writing Charg 1 message number %d\n", counter);

        sprintf(buffer, "Charg 2 : message %d\n", counter);
        strcpy(currentMessage.message, buffer);
        currentMessage.priority = priority;
        sendMessage(dest, currentMessage);
        usleep(100000);
        if (counter == 30) usleep(1000000000);

        //printf("Writed Charg 1 message number %d\n", counter);


        counter++;
    }


}


int main()
{
    handler_t* mainHandler = initializeHandler();
    pthread_t thread1;
    pthread_t thread2;

    message_t* currentMessage;
    pthread_create(&thread1, NULL, itmCharger1, (void*)mainHandler);
    pthread_create(&thread2, NULL, itmCharger2, (void*)mainHandler);


    while(1)
    {
        currentMessage = retrieveMessage(mainHandler);
        if (currentMessage == NULL)
        {
            printf("No message for Handler\n");
        }
        else
        {
            printf("Message received : %s", currentMessage->message);
            printf("Messages to process : %d\n", mainHandler->messagesToProcess);
            free(currentMessage);
        }
        usleep(200000);


    }

    return 0;
}
