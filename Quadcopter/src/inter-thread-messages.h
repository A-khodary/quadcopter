#ifndef INTER_THREAD_MESSAGES
#define INTER_THREAD_MESSAGES

#include "shared_librairies.h"

// Some defines



// Structure declarations :

typedef struct message_t
{
    char message[128];

    int dataSize;
    void* data;

    int priority;
    struct message_t* previousMessage;
    struct message_t* nextMessage;

}message_t;


typedef struct handler_t
{

    int handlerInitialized;

    pthread_mutex_t fifoMutex;
    message_t* fifoFirstElement;

    int fifoProcessingPriority;
    int messagesToProcess;


}handler_t;


// Functions declarations :

handler_t* initializeHandler();
int sendMessage(handler_t* Handler, message_t message);
message_t* retrieveMessage(handler_t* Handler);

#endif
