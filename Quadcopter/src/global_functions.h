#include "shared_librairies.h"

void initialize_mutex(pthread_mutex_t* mutex);
void printDebug(char string[64]);

// Global structures :

typedef struct bidirectionalHandler_t
{
    handler_t* mainITMHandler;
    handler_t* componentITMHandler;
}birectionnalHandler_t;

//Projecting from Lat/Long to XY function :

int convertPlanar(double* x, double* y, double latitude, double longitude);
int convertPlanarToHome(double* x, double* y, double latitude, double longitude);
