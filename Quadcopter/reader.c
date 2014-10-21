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

int readCommands()
{
    extern userCommands;
    userCommands.ch1=0;
    userCommands.ch2=0;
    userCommands.ch3=0;
    userCommands.ch4=0;
    userCommands.ch5=0;
    userCommands.ch6=0;
    userCommands.ch7=0;
    userCommands.ch8=0;
    userCommands.ch9=0;


    if (wiringPiI2CSetup (ARDUINO_ADDRESS)) == -1) return -1; // Connecting to Arduino




}
