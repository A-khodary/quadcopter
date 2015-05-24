#include "global_functions.h"

void initialize_mutex(pthread_mutex_t* mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void printDebug(char const *to_print)
{
    char destination[256];

    if(strstr(to_print, "[i]") != NULL) sprintf(destination, "%s%s", KGRN, to_print);
    else if(strstr(to_print, "[e]") != NULL) sprintf(destination, "%s%s", KRED, to_print);
    else strcpy(destination, to_print);

    printf("%s\n",destination);
    printf("%s", KNRM);

    fflush(stdout);

}

// Some Nav functions :


int convertPlanarToHome(double* x, double* y, double latitude, double longitude)
{
    int success=1;

    printf("Incoming latitude / longitude %f, %f \n", latitude, longitude);
    success = convertPlanar(x,y,latitude,longitude);
    printf("Resulting position : %f, %f\n", *x, *y);
    printf("Home position : %f, %f\n", homePosition.x, homePosition.y);
    *x -= homePosition.x;
    *y -= homePosition.y;

    return success;
}

int convertPlanar(double* x, double* y, double latitude, double longitude)
{
    int p;
    double xcpy, ycpy = 0;
    printf("x, y : %f, %f\n", *x, *y);
    xcpy = *x;
    ycpy = *y;

    projPJ pj_merc, pj_latlong;

    pj_merc = pj_init_plus("+proj=merc +ellps=clrk66 +lat_ts=33");
    pj_latlong = pj_init_plus("+proj=latlong +ellps=clrk66");

    printf("xcpy, ycpy : %f, %f\n", xcpy, ycpy);

    xcpy *= DEG_TO_RAD;
    ycpy *= DEG_TO_RAD;

    printf("xcpy, ycpy : %f, %f\n", xcpy, ycpy);
    p = pj_transform(pj_latlong, pj_merc, 1, 1, &xcpy, &ycpy, NULL );
    printf("xcpy, ycpy : %f, %f\n", xcpy, ycpy);

    *x = xcpy;
    *y = ycpy;

    return p;
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

uint64_t microsSinceEpoch()
{

	struct timeval tv;

	uint64_t micros = 0;

	gettimeofday(&tv, NULL);
	micros =  ((uint64_t)tv.tv_sec) * 1000000 + tv.tv_usec;

	return micros;
}




