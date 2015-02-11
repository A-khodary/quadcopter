#include "shared_librairies.h"
#define PI 3.14159265


void initialize_mutex(pthread_mutex_t* mutex);
void printDebug(char string[64]);

// Global structures :

typedef struct bidirectionalHandler_t
{
    handler_t* mainITMHandler;
    handler_t* componentITMHandler;
}birectionnalHandler_t;

//Some Nav functions :

//Projecting from Lat/Long to XY function :

int convertPlanar(double* x, double* y, double latitude, double longitude);
int convertPlanarToHome(double* x, double* y, double latitude, double longitude);
<<<<<<< HEAD
=======
double calculateBearing(double positionX, double positionY, double destinationX, double destinationY) // returns the necessary bearing to meet objective in degrees
>>>>>>> origin/master
