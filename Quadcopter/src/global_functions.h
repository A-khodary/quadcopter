#include "shared_librairies.h"

void initialize_mutex(pthread_t* mutex);
void printDebug(char string[64]);

// Global structures :

typedef struct bidirectionalHandler_t
{
    handler_t* mainITMHandler;
    handler_t* componentITMHandler;
}birectionnalHandler_t;

