#include "global_functions.h"

void initialize_mutex(pthread_mutex_t* mutex)
{
    *mutex = PTHREAD_MUTEX_INITIALIZER;
}

void printDebug(char string[64])
{


}

int convertPlanarToHome(double* x, double* y, double latitude, double longitude)
{
    int success=1;

    success = convertPlanar(x,y,latide,longitude);
    *x -= homePosition.x;
    *y -= homePosition.y;

    return success;
}

int convertPlanar(double* x, double* y, double latitude, double longitude)
{
    projPJ pj_merc, pj_latlong;

            if (!(pj_merc = pj_init_plus("+proj=merc +ellps=clrk66 +lat_ts=33")) ) exit(1);
            if (!(pj_latlong = pj_init_plus("+proj=latlong +ellps=clrk66")) ) exit(1);
            *x *= DEG_TO_RAD;
            *y *= DEG_TO_RAD;
            p = pj_transform(pj_latlong, pj_merc, 1, 1, x, y, NULL );
            exit(0);
}




