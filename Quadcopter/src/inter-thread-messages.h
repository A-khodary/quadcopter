#include "shared_librairies.h"

#ifndef ITM_H
#define ITM_H


// Some defines

#define INFO 1
#define ORDER 2
#define ANSW 3


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

typedef struct messageDecoded_t
{
    char destination[16];
    char source[16];
    char message[64];
    int operation;

} messageDecoded_t;


// Functions declarations :

handler_t* initializeHandler();
int sendMessage(handler_t* Handler, message_t message);
message_t* retrieveMessage(handler_t* Handler);
messageDecoded_t decodeMessageITM(message_t* message);

#endif
