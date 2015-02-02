#include "pilot.h"

/*
##############################################
#   Pilot functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

// Some Software defines :

#define PCA_I2C_ADDRESS 0x40
#define PCA_FREQUENCY 50
#define PCA_PINBASE 300

#define ESC_MIN_PWM_VALUE 200
#define ESC_MAX_PWM_VALUE 1000

#define REFRESHING_PERIOD_DEFAULT 200000 // PWM refreshing default period

// Some Hardware defines :

#define PCA_CHAN_1_PIN 0
#define PCA_CHAN_2_PIN 1
#define PCA_CHAN_3_PIN 2
#define PCA_CHAN_4_PIN 3
#define PCA_CHAN_5_PIN 4
#define PCA_CHAN_6_PIN 5
#define PCA_CHAN_7_PIN 6
#define PCA_CHAN_8_PIN 7
#define PCA_CHAN_9_PIN 8

// Some controlling defines :

#define AUTOPILOT_MANAGE_CHAN 7
#define EL_CHAN 8
#define GAZ_CUTOFF_CHAN 9
#define MANUAL_CHAN 5

int initPca9685()
{
    printDebug("[i] Initialisation du Pca9685");
    if (pca9685Setup(PCA_PINBASE, PCA_I2C_ADDRESS,PCA_FREQUENCY)  < 0)
    {
        printDebug("[e] Error initializing PCA9685 => No PWM output, this is quite terrible !");
        return -1;
    }

    // Returns : 0 : init OK
    //           1 : init FAILED


    return 0;
}



void writeCommands()
{
    int pwm1, pwm2, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8, pmw9;

    pilotRefreshingPeriodShared = REFRESHING_PERIOD_DEFAULT;

    pthread_mutex_lock(quadcopterPilotCommandsShared.readWrite); // Locking mutex to prevent race conditions

    pwm1 = quadcopterPilotCommandsShared.chan1*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm2 = quadcopterPilotCommandsShared.chan2*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm3 = quadcopterPilotCommandsShared.chan3*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm4 = quadcopterPilotCommandsShared.chan4*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm5 = quadcopterPilotCommandsShared.chan5*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm6 = quadcopterPilotCommandsShared.chan6*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm7 = quadcopterPilotCommandsShared.chan7*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm8 = quadcopterPilotCommandsShared.chan8*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm9 = quadcopterPilotCommandsShared.chan9*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;


    pwmWrite(PCA_PINBASE, pwm1);
    pwmWrite(PCA_PINBASE + 1, pwm2);
    pwmWrite(PCA_PINBASE + 2, pwm3);
    pwmWrite(PCA_PINBASE + 3, pwm4);
    pwmWrite(PCA_PINBASE + 4, pwm5);
    pwmWrite(PCA_PINBASE + 5, pwm6);
    pwmWrite(PCA_PINBASE + 6, pwm7);
    pwmWrite(PCA_PINBASE + 7, pwm1);
    pwmWrite(PCA_PINBASE + 8, pwm1);


}

void* pilotHandler(void* arg)
{
    int isfirst = 1;

    // Handler global variables init :

    pilotCommandsShared_t quadcopterPilotCommandsShared;
    quadcopterPilotCommandsShared.chan1 = 0;
    quadcopterPilotCommandsShared.chan2 = 0;
    quadcopterPilotCommandsShared.chan3 = 0;
    quadcopterPilotCommandsShared.chan4 = 0;
    quadcopterPilotCommandsShared.chan5 = 0;
    quadcopterPilotCommandsShared.chan6 = 0;
    quadcopterPilotCommandsShared.chan7 = 0;
    quadcopterPilotCommandsShared.chan8 = 0;
    quadcopterPilotCommandsShared.chan9 = 0;

    pthread_mutex_init(&quadcopterPilotCommandsShared.readWrite);

    quadcopterPilotCommandsShared.refreshingPeriod = REFRESHING_PERIOD_DEFAULT;

    //

    printDebug("[i] New pilot Handler Launched");
    // TODO Event handler initialization


    // End of TODO

    if (initPca9685() <= 0)
    {
        // TODO, broadcast connection error to main


        // END OF TODO
        exit();
    }

    while(1)
    {
        // TODO : event processing
        {



        // Behaviour determination :

        pthread_mutex_lock(&receivedCommands.readWriteMutex);
        pthread_mutex_lock(&pilotStateShared.readWriteMutex);

         if (receivedCommands.commands[GAZ_CUTOFF_CHAN] > 0.5 && pilotStateShared.pilotMode != CUTOFF)
         {
             pilotStateShared.pilotMode = CUTOFF;
             // TODO : notify main of cut off
         }

         else if (receivedCommands.commands[EL_CHAN] > 0.5 && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared != CUTOFF)
         {
             pilotStateShared.pilotMode = AUTOPILOT_EMERGENCY_LANDING;
             // TODO : notify main of emergency landing
         }

         else if (receivedCommands.commands[MANUAL_CHAN] > 0.5 && pilotStateShared.pilotMode != MANUAL && pilotCommandsShared != CUTOFF && pilotCommandsShared != AUTOPILOT_EMERGENCY_LANDING)
        {
            pilotStateShared.pilotMode = MANUAL;
            // TODO : notify main of manual piloting and adapt speed to manual
        }

        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] < 0.33 && pilotCommandsShared != CUTOFF && pilotCommandsShared != AUTOPILOT_EMERGENCY_LANDING && pilotCommandsShared != AUTOPILOT_LANDING)
        {
             pilotStateShared.pilotMode = AUTOPILOT_LANDING;
             // TODO : notify main of normal landing
        }

        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] > 0.33 && receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] < 0.66 && pilotCommandsShared != CUTOFF && pilotCommandsShared != AUTOPILOT_EMERGENCY_LANDING && pilotCommandsShared != AUTOPILOT_RTH)
        {
             pilotStateShared.pilotMode = AUTOPILOT_RTH;
             // TODO : notify main of return to home
        }

        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] > 0.66 && pilotCommandsShared != CUTOFF && pilotCommandsShared != AUTOPILOT_EMERGENCY_LANDING && pilotCommandsShared != AUTOPILOT_NORMAL)
        {
             pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
             // TODO : notify main of return to autopilot normal mode
        }

        pthread_mutex_unlock(&pilotStateShared.readWriteMutex);
        pthread_mutex_lock(&quadcopterPilotCommandsShared.readWrite);

        switch (pilotStateShared.pilotMode)
        {


            case CUTOFF:

                quadcopterPilotCommandsShared.chan1 = 0;
                quadcopterPilotCommandsShared.chan2 = 0;
                quadcopterPilotCommandsShared.chan3 = 0;
                quadcopterPilotCommandsShared.chan4 = 0;
                quadcopterPilotCommandsShared.chan5 = 0;
                quadcopterPilotCommandsShared.chan6 = 0;
                quadcopterPilotCommandsShared.chan7 = 0;
                quadcopterPilotCommandsShared.chan8 = 0;
                quadcopterPilotCommandsShared.chan9 = 0;

            break;

            case AUTOPILOT_EMERGENCY_LANDING:

            break;



            case MANUAL

                quadcopterPilotCommandsShared.chan1 =  receivedCommands.commands[0];
                quadcopterPilotCommandsShared.chan2 =  receivedCommands.commands[1];
                quadcopterPilotCommandsShared.chan3 =  receivedCommands.commands[2];
                quadcopterPilotCommandsShared.chan4 =  receivedCommands.commands[3];

            break;


            case AUTOPILOT_NORMAL

            break;


            case AUTOPILOT_RTH

            break;


            case AUTOPILOT_LANDING

            break;

        }

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);
        pthread_mutex_unlock(&quadcopterPilotCommandsShared.readWrite);

        writeCommands();
        usleep(pilotCommandsShared.refreshingPeriod);

    }


}

