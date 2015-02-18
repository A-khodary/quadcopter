#include "NEO-6M.h"

dataGPS* getDataGPS(void)
{

    dataGPS dataGPS;

    char gps[65]; //gps string array

    char hours[];
    char mins[];
    char secs[];
    char longitude[];
    char latitude[];
    char nuSat[];
    char alt[];

    int fd; //handler
    int flag;
    int i;
    char arr[]="$GPGGA";

    //printf("raspberry gps programme");

    if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0) //device adress, baudrate
    {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    }

    if (wiringPiSetup () == -1)
    {
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    }

    while(1)
    {
        if(serialGetchar(fd)==arr[i])  flag++;
        else flag = 0;

        if(flag==6)
        {
            for(i=0;i<=64;i++)
            {
                gps[i]=serialGetchar(fd);
            }

            hours[0] = gps[1];
            hours[1] = gps[2];
            dataGPS.hours = atoi (&hours);
            //*dataGPS.hours = &hours;

            mins[0] = gps[3];
            mins[1] = gps[4];
            dataGPS.mins = atoi (&mins);
            //*dataGPS;mins = &mins;

            secs[0] = gps[5];
            secs[1] = gps[6];
            dataGPS.secs = atoi (&secs);
            //*dataGPS.secs = &secs;

            longitude[0]=gps[8];
            longitude[1]=gps[9];
            longitude[2]=gps[10];
            longitude[3]=gps[11];
            longitude[4]=gps[12];
            longitude[5]=gps[13];
            longitude[6]=gps[14];
            longitude[7]=gps[15];
            longitude[8]=gps[16];
            //longitude[9]=gps[17];
            //longitude[10]=gps[18];
            dataGPS.longitude = atof (&longitude);
            //*dataGPS.longitude = &longitude;

            latitude[0]=gps[18];
            latitude[1]=gps[19];
            latitude[2]=gps[20];
            latitude[3]=gps[21];
            latitude[4]=gps[22];
            latitude[5]=gps[23];
            latitude[6]=gps[24];
            latitude[7]=gps[25];
            latitude[8]=gps[26];
            latitude[9]=gps[27];
            //latitude[10]=gps[28];
            //latitude[11]=gps[29];
            dataGPS.latitude = atof (&latitude);
            //*dataGPS.latitude = &latitude;

            nuSat[0] = gps[33];
            nuSat[1] = gps[34];
            dataGPS.nuSat = atoi(&nuSat);
            //*dataGPS.nuSat = &nuSat;

            alt[0] = gps [40];
            alt[1] = gps [41];
            alt[2] = gps [42];
            alt[3] = gps [43];
            alt[4] = gps [44];
            dataGPS.alt = atoi(&alt);
            //*dataGPS.alt = &alt;

            //serialClose(fd);
            //flag = 0;

            return dataGPS;

        }

    }
}


