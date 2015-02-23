#include "shared_librairies.h"
#include "autopilot.h"


#ifndef GLOBAL_FUNC
#define GLOBAL_FUNC


#define PI 3.14159265


void initialize_mutex(pthread_mutex_t* mutex);
void printDebug(char const *string);

// Global structures :

typedef struct bidirectionnalHandler_t
{
    handler_t* mainITMHandler;
    handler_t* componentITMHandler;
}bidirectionnalHandler_t;

//Some Nav functions :

//Projecting from Lat/Long to XY function :

int convertPlanar(double* x, double* y, double latitude, double longitude);
int convertPlanarToHome(double* x, double* y, double latitude, double longitude);

double calculateBearing(double positionX, double positionY, double destinationX, double destinationY); // returns the necessary bearing to meet objective in degrees

#endif
