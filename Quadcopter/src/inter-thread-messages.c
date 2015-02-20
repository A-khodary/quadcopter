#include "inter-thread-messages.h"

handler_t* initializeHandler()
{
    handler_t* handler = malloc(sizeof(handler_t));
    pthread_mutex_init(&handler->fifoMutex , NULL);


    pthread_mutex_lock(&handler->fifoMutex);

    handler->fifoFirstElement = NULL;
    handler->messagesToProcess = 0;
    handler->fifoProcessingPriority = 1;

    handler->handlerInitialized = 1;

    pthread_mutex_unlock(&handler->fifoMutex);

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

    pthread_mutex_lock(&handler->fifoMutex);

    // Locating last message :

    currentMessage = handler->fifoFirstElement;
    while (currentMessage->nextMessage != NULL) currentMessage = currentMessage->nextMessage;
    lastMessage = currentMessage;

    // Checking if fifo is empty :

    if (handler->fifoFirstElement == NULL)
    {
        handler->fifoFirstElement = message;
        pthread_mutex_unlock(&handler->fifoMutex);
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
        pthread_mutex_unlock(&handler->fifoMutex);

        return 0;
}

message_t* retrieveMessage(handler_t* handler)
{
    message_t* message = malloc(sizeof(message_t));

    if (!handler->handlerInitialized) return NULL;

    pthread_mutex_lock(&handler->fifoMutex);

    if (handler->fifoFirstElement == NULL)
    {
        pthread_mutex_unlock(&handler->fifoMutex);
        return NULL;
    }

    message = handler->fifoFirstElement;
    handler->fifoProcessingPriority = message->priority;

    // Removing the message from Fifo :

    handler->fifoFirstElement->previousMessage == NULL;
    handler->fifoFirstElement = handler->fifoFirstElement->nextMessage;

    handler->messagesToProcess -= 1;

    pthread_mutex_unlock(&handler->fifoMutex);
    return message;
}

/*

int removeCurrentMessage(handler_t* handler)    NON utile : intégré à la fonction retrieveMessage();
{
    handler_t Handler;
    Handler = handler->fifoFirstElement->previousMessage; // Nawak ;)
    handler->fifoFirstElement->previousMessage->nextMessage = NULL; //Non plus : fifFirstElement->previus message = NULL quoi q'il arrive
    handler->fifoFirstElement->previousMessage = NULL;
    handler->fifoFirstElement = Handler;
    handler.messagesToProcess--;
    handler.fifoProcessingPriority = autopilotObjectiveFifo->fifoFirstElement.priority;
    return 1;
}

*/

/*


messageDecoded_t decodeMessageITM(message_t* message)
{
    messageDecoded_t decoded;
    int i=0;
    char comm[16];

    while (message->message[i] != "_")
    {
        decoded.destination[i] = message->message[i];
        i++;
    }
    i++;

    while (message->message[i] != "_")
    {
        decoded.source[i] = message->message[i];
        i++;
    }
    i++;

    while (message->message[i] != "_")
    {
        decoded.source[i] = message->message[i];
        i++;
    }
    i++;

    while (message->message[i] != "_")
    {
        comm = message->message[i];
        i++;
    }

    if(comm == "info")  decoded.operation = INFO;
    else if (comm == "answ") decoded.operation = ANSW;
    else if (comm == "order") decoded.operation = ORDER;
    i++;

        while (message->message[i] != "\0")
    {
        decoded.message = message->message[i];
        i++;
    }

    return decoded;
}

*/
