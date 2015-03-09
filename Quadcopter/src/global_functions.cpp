#include "global_functions.h"

void initialize_mutex(pthread_mutex_t* mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void printDebug(char const *to_print)
{
    printf("%s\n",to_print);
    fflush(stdout);

}

// Some Nav functions :


int convertPlanarToHome(double* x, double* y, double latitude, double longitude)
{
    int success=1;

    success = convertPlanar(x,y,latitude,longitude);
    *x -= homePosition.x;
    *y -= homePosition.y;

    return success;
}

int convertPlanar(double* x, double* y, double latitude, double longitude)
{
    int p;
    projPJ pj_merc, pj_latlong;

            if (!(pj_merc = pj_init_plus("+proj=merc +ellps=clrk66 +lat_ts=49.03")) ) exit(1); // Cergy latitude
            if (!(pj_latlong = pj_init_plus("+proj=latlong +ellps=clrk66")) ) exit(1);
            *x *= DEG_TO_RAD;
            *y *= DEG_TO_RAD;
            p = pj_transform(pj_latlong, pj_merc, 1, 1, x, y, NULL );
            exit(p);
}


double calculateBearing(double positionX, double positionY, double destinationX, double destinationY) // returns the necessary bearing to meet objective in degrees
{
    double bearing;
    double dx;
    double dy;

    dx = destinationX - positionX;
    dy = destinationY - positionY;

    bearing =PI/2 - atan2(dx,dy);

    return bearing*180/PI;
}




