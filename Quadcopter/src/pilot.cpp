#include "pilot.h"

/*
##############################################
#   Pilot functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

//TODO : debug memory leak in pilot

// Some Software defines :

#define PCA_I2C_ADDRESS 0x40
#define PCA_FREQUENCY 50
#define PCA_PINBASE 300

#define ESC_MIN_PWM_VALUE 1
#define ESC_MAX_PWM_VALUE 2

#define REFRESHING_PERIOD_DEFAULT 500000 // PWM refreshing default period

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

// Global variables definitions :

pilotCommandsShared_t pilotCommandsShared;
pilotStateShared_t pilotStateShared;


int calcTicks(float impulseMs, int hertz)
{
	float cycleMs = 1000.0f / hertz;
	return (int)(ESC_MAX_PWM_VALUE * impulseMs / cycleMs + 0.5f);
}



void writeCommands()
{
    double pwm1, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8, pwm9;

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


        pwmWrite(PCA_PINBASE + 0, calcTicks(pwm1, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 1, calcTicks(pwm2, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 2, calcTicks(pwm3, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 3, calcTicks(pwm4, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 4, calcTicks(pwm5, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 5, calcTicks(pwm6, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 6, calcTicks(pwm7, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 7, calcTicks(pwm8, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 8, calcTicks(pwm9, PCA_FREQUENCY));


}

void* pilotHandler(void* arg)
{
    int isfirst = 1;

    // variables needed for the pilot test function :

    double testCommand = 0;

    // Handler global variables init :


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



    if (pca9685Setup(PCA_PINBASE, PCA_I2C_ADDRESS, PCA_FREQUENCY) < 0)
    {
        strcpy(message.message,"main_pilot_info_initfailed");
        message.priority = 20;
        sleep(1);
        sendMessage(mainITMHandler, message);

        pthread_exit(NULL);
    }

    printDebug("[i]Initializing ESC...");
    writeCommands();

    // Waiting 5ms for ESC to init :
    usleep(5000);

    while(1)
    {


        receivedMessage =retrieveMessage(pilotITMHandler);
        if (receivedMessage == NULL)
        {
            printDebug("[i] No message for pilot");
        }
        else
        {


            decoded = decodeMessageITM(receivedMessage);

            if (strcmp(decoded.destination,"pilot")
            {
                printDebug("[e] pilot component received a message it should not");

            }

            else
            {
                if (decoded.operation == ORDER)
                {
                    if (!strcmp(decoded.message, "cutoff"))
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

                    else if (!strcmp(decoded.message, "manual"))
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

                    else if (!strcmp(decoded.message, "normal"))
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

                    else if (!strcmp(decoded.message, "test"))
                    {

                        strcpy(message.message,"main_pilot_info_test");
                        sendMessage(mainITMHandler, message);


                        printDebug("Pilot received an autopilot test, performing test");
                        pthread_mutex_lock(&pilotStateShared.readWriteMutex);
                        pilotStateShared.pilotMode = TEST;
                        pthread_mutex_unlock(&pilotStateShared.readWriteMutex);
                        strcpy(message.message, "autopilot_pilot_order_pause");
                        sendMessage(mainITMHandler, message);
                    }


                }

                else
                {
                    printDebug("Pilot received a message he was supposed to but didn't recognized it");
                }
            }

            free(receivedMessage);
        }



        // Behaviour determination :

        pthread_mutex_lock(&receivedCommands.readWriteMutex);
        pthread_mutex_lock(&pilotStateShared.readWriteMutex);

//        if (receivedCommands.commands[GAZ_CUTOFF_CHAN] > 0.5 && pilotStateShared.pilotMode != CUTOFF)
//        {
//            pilotStateShared.pilotMode = CUTOFF;
//            // TODO : notify main of cut off and pausing autopilot :
//
//            strcpy(message.message,"main_pilot_info_cutoff");
//            sendMessage(mainITMHandler, message);
//
//            strcpy(message.message, "autopilot_pilot_order_pause");
//            sendMessage(mainITMHandler, message);
//        }
//
//        else if (receivedCommands.commands[EL_CHAN] > 0.5 && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode != CUTOFF)
//        {
//            pilotStateShared.pilotMode = AUTOPILOT_EMERGENCY_LANDING;
//
//            strcpy(message.message,"main_pilot_info_emergencylanding");
//            sendMessage(mainITMHandler, message);
//
//            strcpy(message.message, "autopilot_pilot_order_emergencylanding");
//            sendMessage(mainITMHandler, message);
//
//        }
//
//        else if (receivedCommands.commands[MANUAL_CHAN] > 0.5 && pilotStateShared.pilotMode != MANUAL && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING)
//        {
//            pilotStateShared.pilotMode = MANUAL;
//            // TODO : notify main of manual piloting and adapt speed to manual
//
//            strcpy(message.message,"main_pilot_info_automanual");
//            sendMessage(mainITMHandler, message);
//
//            strcpy(message.message, "autopilot_pilot_order_pause");
//            sendMessage(mainITMHandler, message);
//        }
//
//        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] < 0.33 && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode != AUTOPILOT_LANDING)
//        {
//            pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
//            // TODO : notify main of normal landing
//
//            strcpy(message.message,"main_pilot_info_autolanding");
//            sendMessage(mainITMHandler, message);
//
//            strcpy(message.message, "autopilot_pilot_order_land");
//            sendMessage(mainITMHandler, message);
//        }
//
//        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] > 0.33 && receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] < 0.66 && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode!= AUTOPILOT_RTH)
//        {
//            pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
//            // TODO : notify main of return to home
//
//            strcpy(message.message,"main_pilot_info_gohome");
//            sendMessage(mainITMHandler, message);
//
//            strcpy(message.message, "autopilot_pilot_order_rth");
//            sendMessage(mainITMHandler, message);
//        }
//
//        else if (receivedCommands.commands[AUTOPILOT_MANAGE_CHAN] > 0.66 && pilotStateShared.pilotMode != CUTOFF && pilotStateShared.pilotMode != AUTOPILOT_EMERGENCY_LANDING && pilotStateShared.pilotMode != AUTOPILOT_NORMAL)
//        {
//            pilotStateShared.pilotMode = AUTOPILOT_NORMAL;
//            message_t message;
//            strcpy(message.message,"main_pilot_info_autonormal");
//            sendMessage(mainITMHandler, message);
//
//            strcpy(message.message, "autopilot_pilot_order_pause");
//            sendMessage(mainITMHandler, message);
//        }

        pilotStateShared.pilotMode = TEST;

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

        // Doing nothing to commands : autopilot has control

            break;



        case MANUAL:

            pilotCommandsShared.chan1 =  receivedCommands.commands[0];
            pilotCommandsShared.chan2 =  receivedCommands.commands[1];
            pilotCommandsShared.chan3 =  receivedCommands.commands[2];
            pilotCommandsShared.chan4 =  receivedCommands.commands[3];

            break;


        case AUTOPILOT_NORMAL:

        // Doing nothing to commands : autopilot has control

            break;


        case AUTOPILOT_RTH:

         // Doing nothing to commands : autopilot has control

            break;


        case AUTOPILOT_LANDING:

         // Doing nothing to commands : autopilot has control

            break;

        case TEST:

            // Some testing functions that increases and stops all channels

            pilotCommandsShared.chan1 =  testCommand;
            pilotCommandsShared.chan2 =  testCommand;
            pilotCommandsShared.chan3 =  testCommand;
            pilotCommandsShared.chan4 =  testCommand;

            if (testCommand == 1) testCommand = 0;
            else testCommand += 0.01;


            break;

        }

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);
        pthread_mutex_unlock(&pilotCommandsShared.readWrite);

        writeCommands();
        usleep(pilotCommandsShared.refreshingPeriod);




    }

}

