#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>


typedef struct dataGPS_t
{
    int hours;
    int mins;
    int secs;
    double longitude;
    double latitude;
    int nuSat;//Number of satellites being tracked
    int alt;//Altitude, Meters, above mean sea level

    /*char hours[2];
    char mins[2];
    char secs[2];
    char longitude[10];
    char latitude[11];
    char nuSat[2];//Number of satellites being tracked
    char alt[5];//Altitude, Meters, above mean sea level*/

}dataGPS;
