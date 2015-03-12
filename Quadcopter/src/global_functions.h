#ifndef GLOBAL_FUNC
#define GLOBAL_FUNC


#define PI 3.14159265

#include "shared_librairies.h"
#include "imu_globals_variables.h"
#include "autopilot.h"

// Some colour defines :

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

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
