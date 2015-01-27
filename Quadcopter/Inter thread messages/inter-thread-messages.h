#include "shared_librairies.h"

// Some defines



// Structure declarations :

typedef struct handler_t
{

    int handlerInitialized;

    pthread_mutex_t fifoMutex;
    message_t* fifoFirstElement;

    int fifoProcessingPriority;
    int messagesToProcess;


}handler_t;


typedef struct message_t
{
    char message[128];

    int dataSize;
    void* data;

    int priority;
    message_t* previousMessage;
    message_t* nextMessage;

}message_t;


// Functions declarations :

handler_t* initializeHandler();
int sendMessage(handler_t* Handler, message_t* message);
message_t* retrieveMessage(handler_t* Handler);

