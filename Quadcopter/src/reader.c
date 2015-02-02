#include reader.h

/*
##############################################
#   Reader functionality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/


/* Function return documentation :

-1 : Arduino not detected on I2C


*/





// Library includes :




// Hardware defines :

# define RUDDER_PIN 1
# define THROTTLE_PIN 2
# define AILERONS_PIN 3
# define ELEVATOR_PIN 4

// Software defines :

#define ARDUINO_ADDRESS 1
#define SAMPLING_PERIOD_MS 10

// Functions :

void* readerHandler(void* arg)
{
    long buffer[9];
    int fd;

    receivedCommandsShared_t receivedCommands;
    userCommands.ch1=0;
    userCommands.ch2=0;
    userCommands.ch3=0;
    userCommands.ch4=0;
    userCommands.ch5=0;
    userCommands.ch6=0;
    userCommands.ch7=0;
    userCommands.ch8=0;
    userCommands.ch9=0;


    if (fd = wiringPiI2CSetup (ARDUINO_ADDRESS)) < 0)
    {
        printDebug("[e] Reader : error connecting to Arduino via I2C")
        return -1; // Connecting to Arduino
    }

    // Now we're connected, processing incoming data :

    while(continue)
{
    read(fd, buffer, 4*9); //Todo implement connection test

    pthread_mutex_lock(userCommands.readWriteMutex);

    userCommands.ch1=buffer[0];
    userCommands.ch2=buffer[1];
    userCommands.ch3=buffer[2];
    userCommands.ch4=buffer[3];
    userCommands.ch5=buffer[4];
    userCommands.ch6=buffer[5];
    userCommands.ch7=buffer[6];
    userCommands.ch8=buffer[7];
    userCommands.ch9=buffer[8];

    pthread_mutex_unlock(userCommands.readWriteMutex);

    sleep(SAMPLING_PERIOD_MS/1000);

}
    close(fd);

}

