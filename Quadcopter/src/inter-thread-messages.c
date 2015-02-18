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

int removeCurrentMessage(handler_t* handler)
{
    handler_t Handler;
    Handler = handler->fifoFirstElement->previousMessage;
    handler->fifoFirstElement->previousMessage->nextMessage = NULL;
    handler->fifoFirstElement->previousMessage = NULL;
    handler->fifoFirstElement = Handler;
    handler.messagesToProcess--;
    handler.fifoProcessingPriority = autopilotObjectiveFifo->fifoFirstElement.priority;
    return 1;
}


char[] retrieveDestination(message_t* message)
{
    int i;
    char[] destination;

    for (i=0;i<30;i++)
    {
        while(message.message!="_")
        {
            destnination[i] = message.message[i];
        }
    return destination;
    }
}

char[] retrieveSender(message_t* message)
{
    int i,j;
    char[] sender;

    for (i=0;i<30;i++)
    {
        while(message.message[i]=="_")
        {
            for (j=0;j<30;j++)
            {
                destnination[i+j] = message.message[i+j];
            }
        return sender;
        }
    }
}

char[] retrieveResponseOrder(message_t* message)
{
    int i,j,k;
    char[] responseorder;

    for (i=0;i<30;i++)
    {
        while(message.message[i]=="_")
        {
            for (j=0;j<30;j++)
            {
                while(message.message[i+j]=="_")
                {
                    for (k=0;k<30;k++)
                    {
                        destnination[i+j+k] = message.message[i+j+k];
                    }
                    return responseorder;
                }
            }
        }
    }
}


char[] retrieveMessageOfMessage(message_t* message)
{
    int i,j,k,l;
    char[] MessageOfMessage;

    for (i=0;i<30;i++)
    {
        while(message.message[i]=="_")
        {
            for (j=0;j<30;j++)
            {
                while(message.message[i+j]=="_")
                {
                    for (k=0;k<30;k++)
                    {
                        while(message.message[i+j+k]=="_")
                        {
                            for (l=0;l<30;l++)
                            {
                                destnination[i+j+k+l] = message.message[i+j+k+l];
                            }
                            return MessageOfMessage;
                        }

                    }
                }
            }
        }
    }
}

