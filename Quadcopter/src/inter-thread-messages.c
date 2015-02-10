#include "inter-thread-messages.h"

handler_t* initializeHandler()
{
    handler_t* handler = malloc(sizeof(handler_t));
    pthread_mutex_init(handler->fifoMutex);


    pthread_mutex_lock(handler->fifoMutex);

    handler->fifoFirstElement = NULL;
    handler->messagesToProcess = 0;
    handler->fifoProcessingPriority = 1;

    handler->handlerInitialized = 1;

    pthread_mutex_unlock(handler->fifoMutex);

    return handler;
}

int sendMessage(handler_t* handler, message_t messageByValues)
{
    message_t* currentMessage;
    message_t* lastMessage;

    // Dynamic copy of the parameter :

    message_t* message = malloc(sizeof(message_t));
    *message = messageByValues;


    if (!handler->handlerInitialized) return -1; // Not initialized : returning -1

    pthread_mutex_lock(handler->fifoMutex);

    // Locating last message :

    currentMessage = handler->fifoFirstElement;
    while (currentMessage->nextMessage != NULL) currentMessage = currentMessage->nextMessage;
    lastMessage = currentMessage;

    // Checking if fifo is empty :

    if (handler->fifoFirstElement == NULL)
    {
        handler->fifoFirstElement = message;
        pthread_mutex_unlock(handler->fifoMutex);
        return 1;
    }

    // Checking last message priority :

    if (lastMessage->priority < message->priority)
    {
        // Now we have to search the last message corresponding to priority :

        currentMessage = lastMessage;
        while (currentMessage->previousMessage != NULL || currentMessage->priority < message->priority) currentMessage = currentMessage->previousMessage;

        if (currentMessage->previousMessage == NULL) currentMessage->previousMessage = message; // We have reached the first element of the fifo
        else // We need to insert our message between two
        {
            // ¨Preparing our message :
            message->nextMessage= currentMessage->nextMessage;
            message->previousMessage = currentMessage;

            // Inserting it :
            currentMessage->nextMessage->previousMessage = message;
            currentMessage->nextMessage = message;
        }

    }
    else
    {
        // Preparing our message :
        message->previousMessage = lastMessage;
        message->nextMessage = NULL;

        // Inserting it :
        lastMessage->nextMessage = message;
    }

        handler->messagesToProcess++;
        pthread_mutex_unlock(handler->fifoMutex)

        return 0;
}

message_t* retrieveMessage(handler_t* handler)
{
    message_t* message = malloc(sizeof(message_t));

    if (!handler->handlerInitialized) return NULL;

    pthread_mutex_lock(handler->fifoMutex);

    if (handler->fifoFirstElement == NULL)
    {
        pthread_mutex_unlock(handler->fifoMutex);
        return NULL;
    }

    message = handler->fifoFirstElement;
    handler->fifoProcessingPriority = message->priority;
    handler->messagesToProcess -= 1;

    pthread_mutex_unlock(handler->fifoMutex);
    return message;
}
