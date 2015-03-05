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
    int pwm1, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8, pwm9;

    int pilotRefreshingPeriodShared = REFRESHING_PERIOD_DEFAULT;

    pthread_mutex_lock(&pilotCommandsShared.readWrite); // Locking mutex to prevent race conditions

    pwm1 = pilotCommandsShared.chan1*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm2 = pilotCommandsShared.chan2*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm3 = pilotCommandsShared.chan3*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm4 = pilotCommandsShared.chan4*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm5 = pilotCommandsShared.chan5*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm6 = pilotCommandsShared.chan6*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm7 = pilotCommandsShared.chan7*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm8 = pilotCommandsShared.chan8*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;
    pwm9 = pilotCommandsShared.chan9*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE;

/* FIX : undefined
    pwmWrite(PCA_PINBASE, pwm1);
    pwmWrite(PCA_PINBASE + 1, pwm2);
    pwmWrite(PCA_PINBASE + 2, pwm3);
    pwmWrite(PCA_PINBASE + 3, pwm4);
    pwmWrite(PCA_PINBASE + 4, pwm5);
    pwmWrite(PCA_PINBASE + 5, pwm6);
    pwmWrite(PCA_PINBASE + 6, pwm7);
    pwmWrite(PCA_PINBASE + 7, pwm1);
    pwmWrite(PCA_PINBASE + 8, pwm1);
*/

}

void* pilotHandler(void* arg)
{
    int isfirst = 1;

    // Handler global variables init :

    pilotCommandsShared_t pilotCommandsShared;

    pilotCommandsShared.chan1 = 0;
    pilotCommandsShared.chan2 = 0;
    pilotCommandsShared.chan3 = 0;
    pilotCommandsShared.chan4 = 0;
    pilotCommandsShared.chan5 = 0;
    pilotCommandsShared.chan6 = 0;
    pilotCommandsShared.chan7 = 0;
    pilotCommandsShared.chan8 = 0;
    pilotCommandsShared.chan9 = 0;

   initialize_mutex(&pilotCommandsShared.readWrite);

    pilotCommandsShared.refreshingPeriod = REFRESHING_PERIOD_DEFAULT;


    printDebug("[i] New pilot Handler Launched");


    bidirectionnalHandler_t* bidirectionnalHandler;
    bidirectionnalHandler = (bidirectionnalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* pilotITMHandler;

    mainITMHandler = bidirectionnalHandler->mainITMHandler;
    pilotITMHandler = bidirectionnalHandler->componentITMHandler;

    message_t* receivedMessage;
    message_t message;
    messageDecoded_t decoded;



    if (initPca9685() <= 0)
    {
        // TODO, broadcast connection error to main + priority

        strcpy(message.message,"main_pilot_info_initfailed");
        sendMessage(mainITMHandler, message);
        // END OF TODO
        exit(0);
    }

    while(1)
    {

        {
            receivedMessage =retrieveMessage(pilotITMHandler);
            decoded = decodeMessageITM(receivedMessage);

            if (decoded.destination != "pilot")
            {
                printDebug("Error : pilot component received a message it should not");

            }

            else
            {
                if (decoded.operation == ORDER)
                {
                    if (decoded.message == "cutoff")
                    {
                        printDebug("Pilot received a cutoff order and cuts everything off");

                        strcpy(message.message,"main_pilot_info_cutoff");
                        sendMessage(mainITMHandler, message);

                        pthread_mutex_lock(&pilotStateShared.readWriteMutex);
                        pilotStateShared.pilotMode = CUTOFF;
                        pthread_mutex_unlock(&pilotStateShared.readWriteMutex);
                        strcpy(message.message, "autopilot_pilot_order_pause");
                        sendMessage(mainITMHandler, message);
                    }

                    else if (decoded.message == "manual")
                    {
                        strcpy(message.message,"main_pilot_info_automanual");
                        sendMessage(mainITMHandler, message);


                        printDebug("Pilot received a manual order, switching to manual and notifying autopilot");
                         pthread_mutex_lock(&pilotStateShared.readWriteMutex);
                        pilotStateShared.pilotMode = MANUAL;
                        pthread_mutex_unlock(&pilotStateShared.readWriteMutex);
                        strcpy(message.message, "autopilot_pilot_order_pause");
                        sendMessage(mainITMHandler, message);

                    }

                    else if (decoded.message == "normal")
                    {

                        strcpy(message.message,"main_pilot_info_autonormal");
                        sendMessage(mainITMHandler, message);


                        printDebug("Pilot received an autopilot normal order, switching to autopilot normal mode and notifying autopilot");
                        pthread_mutex_lock(&pilotStateShared.readWriteMutex);
                        pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
                        pthread_mutex_unlock(&pilotStateShared.readWriteMutex);
                        strcpy(message.message, "autopilot_pilot_order_play");
                        sendMessage(mainITMHandler, message);

                    }

                }

                else
                {
                    printDebug("Pilot received a message he was supposed to but didn't recognized it");
                }
            }

            free(receivedMessage);



        // Behaviour determination :

        pthread_mutex_lock(&receivedCommands.readWriteMutex);
        pthread_mutex_lock(&pilotStateShared.readWriteMutex);

         if (receivedCommands.commands[GAZ_CUTOFF_CHAN] > 0.5 && pilotStateShared.pilotMode != CUTOFF)
         {
             pilotStateShared.pilotMode = CUTOFF;
             // TODO : notify main of cut off and pausing autopilot :

            strcpy(message.message,"main_pilot_info_cutoff");
            sendMessage(mainITMHandler, message);

            strcpy(message.message, "autopilot_pilot_order_pause");
            sendMessage(mainITMHandler, message);
         }

         else if (receivedCommands.commands[EL_CHAN] > 0.5 && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode != CUTOFF)
         {
             pilotStateShared.pilotMode = AUTOPILOT_EMERGENCY_LANDING;

            strcpy(message.message,"main_pilot_info_emergencylanding");
            sendMessage(mainITMHandler, message);

            strcpy(message.message, "autopilot_pilot_order_emergencylanding");
            sendMessage(mainITMHandler, message);

         }

         else if (receivedCommands.commands[MANUAL_CHAN] > 0.5 && pilotStateShared.pilotMode != MANUAL && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING)
        {
            pilotStateShared.pilotMode = MANUAL;
            // TODO : notify main of manual piloting and adapt speed to manual

            strcpy(message.message,"main_pilot_info_automanual");
            sendMessage(mainITMHandler, message);

            strcpy(message.message, "autopilot_pilot_order_pause");
            sendMessage(mainITMHandler, message);
        }

        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] < 0.33 && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode != AUTOPILOT_LANDING)
        {
             pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
             // TODO : notify main of normal landing

            strcpy(message.message,"main_pilot_info_autolanding");
            sendMessage(mainITMHandler, message);

            strcpy(message.message, "autopilot_pilot_order_land");
            sendMessage(mainITMHandler, message);
        }

        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] > 0.33 && receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] < 0.66 && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode!= AUTOPILOT_RTH)
        {
             pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
             // TODO : notify main of return to home

            strcpy(message.message,"main_pilot_info_gohome");
            sendMessage(mainITMHandler, message);

            strcpy(message.message, "autopilot_pilot_order_rth");
            sendMessage(mainITMHandler, message);
        }

        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] > 0.66 && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode != AUTOPILOT_NORMAL)
        {
            pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
            message_t message;
            strcpy(message.message,"main_pilot_info_autonormal");
            sendMessage(mainITMHandler, message);

            strcpy(message.message, "autopilot_pilot_order_pause");
            sendMessage(mainITMHandler, message);
        }

        pthread_mutex_unlock(&pilotStateShared.readWriteMutex);
        pthread_mutex_lock(&pilotCommandsShared.readWrite);

        switch (pilotStateShared.pilotMode)
        {


            case CUTOFF:

                pilotCommandsShared.chan1 = 0;
                pilotCommandsShared.chan2 = 0;
                pilotCommandsShared.chan3 = 0;
                pilotCommandsShared.chan4 = 0;
                pilotCommandsShared.chan5 = 0;
                pilotCommandsShared.chan6 = 0;
                pilotCommandsShared.chan7 = 0;
                pilotCommandsShared.chan8 = 0;
                pilotCommandsShared.chan9 = 0;

            break;

            case AUTOPILOT_EMERGENCY_LANDING:

            break;



            case MANUAL:

                pilotCommandsShared.chan1 =  receivedCommands.commands[0];
                pilotCommandsShared.chan2 =  receivedCommands.commands[1];
                pilotCommandsShared.chan3 =  receivedCommands.commands[2];
                pilotCommandsShared.chan4 =  receivedCommands.commands[3];

            break;


            case AUTOPILOT_NORMAL:

            break;


            case AUTOPILOT_RTH:

            break;


            case AUTOPILOT_LANDING:

            break;

        }

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);
        pthread_mutex_unlock(&pilotCommandsShared.readWrite);

        writeCommands();
        usleep(pilotCommandsShared.refreshingPeriod);

    }


}

}

