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
#define ESC_MAX_PWM_VALUE 2.1
#define GLOBAL_OFFSET 0.07

#define REFRESHING_PERIOD_DEFAULT 500000 // PWM refreshing default period
#define MAX_PWM 4096

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

#define YAW_CHAN 4
#define GAZ_CHAN 3
#define ROLL CHAN 1
#define PITCH_CHAN 2

// Global variables definitions :

pilotCommandsShared_t pilotCommandsShared;
pilotStateShared_t pilotStateShared;


int calcTicks(float impulseMs, int hertz)
{
	float cycleMs = 1000.0f / hertz;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

void armQuadcopter()
{
    printDebug("[i] Arming quadcopter motors....");
    //pthread_mutex_lock(&pilotCommandsShared.readWrite);

    pilotCommandsShared.chan3 = 0;
    pilotCommandsShared.chan4 = 1;

    writeCommands();

    usleep(1000000);

    printDebug("DONE");

    pthread_mutex_unlock(&pilotCommandsShared.readWrite);
    printDebug("[i] Quadcopter motors are now armed !");
}

void disarmQuadcopter()
{
    printDebug("[i] Disarming quadcopter motors....");
    pthread_mutex_lock(&pilotCommandsShared.readWrite);

    // TODO : implement disarming sequence

//    while((pilotCommandsShared.chan1 != 0)&&(pilotCommandsShared.chan2 != 0)&&(pilotCommandsShared.chan3 != 0)&&(pilotCommandsShared.chan4 != 0)&&(pilotCommandsShared.chan5 != 0)&&(pilotCommandsShared.chan6 != 0)&&(pilotCommandsShared.chan7 != 0)&&(pilotCommandsShared.chan8 != 0)&&(pilotCommandsShared.chan9 != 0))
//    {
//        if((pilotCommandsShared.chan1 - 2)>0)
//        {
//            pilotCommandsShared.chan1 = pilotCommandsShared.chan1 - 2;
//        }
//        else {pilotCommandsShared.chan1 = 0;}
//        if((pilotCommandsShared.chan2 - 2)>0)
//        {
//            pilotCommandsShared.chan2 = pilotCommandsShared.chan2 - 2;
//        }
//        else {pilotCommandsShared.chan2 = 0;}
//        if((pilotCommandsShared.chan3 - 2)>0)
//        {
//            pilotCommandsShared.chan3 = pilotCommandsShared.chan3 - 2;
//        }
//        else {pilotCommandsShared.chan3 = 0;}
//        if((pilotCommandsShared.chan4 - 2)>0)
//        {
//            pilotCommandsShared.chan4 = pilotCommandsShared.chan4 - 2;
//        }
//        else {pilotCommandsShared.chan4 = 0;}
//        if((pilotCommandsShared.chan5 - 2)>0)
//        {
//            pilotCommandsShared.chan5 = pilotCommandsShared.chan5 - 2;
//        }
//        else {pilotCommandsShared.chan5 = 0;}
//        if((pilotCommandsShared.chan6 - 2)>0)
//        {
//            pilotCommandsShared.chan6 = pilotCommandsShared.chan6 - 2;
//        }
//        else {pilotCommandsShared.chan6 = 0;}
//        if((pilotCommandsShared.chan7 - 2)>0)
//        {
//            pilotCommandsShared.chan7 = pilotCommandsShared.chan7 - 2;
//        }
//        else {pilotCommandsShared.chan7 = 0;}
//        if((pilotCommandsShared.chan8 - 2)>0)
//        {
//            pilotCommandsShared.chan8 = pilotCommandsShared.chan8 - 2;
//        }
//        else {pilotCommandsShared.chan8 = 0;}
//        if((pilotCommandsShared.chan9 - 2)>0)
//        {
//            pilotCommandsShared.chan9 = pilotCommandsShared.chan9 - 2;
//        }
//        else {pilotCommandsShared.chan9 = 0;} WTF ???
        writeCommands();
        usleep(100000);//temps à déternimer pour avoir baisse du régime progressive
 //   }

    pthread_mutex_unlock(&pilotCommandsShared.readWrite);
    printDebug("[i] Quadcopter motors are no disarmed !");

}



void writeCommands()
{
    double pwm1, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8, pwm9;

    pthread_mutex_lock(&pilotCommandsShared.readWrite); // Locking mutex to prevent race conditions

    pwm1 = pilotCommandsShared.chan1*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm2 = pilotCommandsShared.chan2*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm3 = pilotCommandsShared.chan3*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm4 = pilotCommandsShared.chan4*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm5 = pilotCommandsShared.chan5*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm6 = pilotCommandsShared.chan6*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm7 = pilotCommandsShared.chan7*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm8 = pilotCommandsShared.chan8*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;
    pwm9 = pilotCommandsShared.chan9*(ESC_MAX_PWM_VALUE - ESC_MIN_PWM_VALUE) + ESC_MIN_PWM_VALUE + GLOBAL_OFFSET;


        pwmWrite(PCA_PINBASE + 0, calcTicks(pwm1, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 1, calcTicks(pwm2, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 2, calcTicks(pwm3, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 3, calcTicks(pwm4, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 4, calcTicks(pwm5, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 5, calcTicks(pwm6, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 6, calcTicks(pwm7, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 7, calcTicks(pwm8, PCA_FREQUENCY));
        pwmWrite(PCA_PINBASE + 8, calcTicks(pwm9, PCA_FREQUENCY));


    pthread_mutex_unlock(&pilotCommandsShared.readWrite);
}

void* pilotHandler(void* arg)
{
    int isfirst = 1;

    // variables needed for the pilot test function :

    double testCommand = 1.3;

    // Handler global variables init :

    initialize_mutex(&pilotCommandsShared.readWrite);


    pilotCommandsShared.chan1 = 0;
    pilotCommandsShared.chan2 = 0;
    pilotCommandsShared.chan3 = 0;
    pilotCommandsShared.chan4 = 0;
    pilotCommandsShared.chan5 = 0;
    pilotCommandsShared.chan6 = 0;
    pilotCommandsShared.chan7 = 0;
    pilotCommandsShared.chan8 = 0;
    pilotCommandsShared.chan9 = 0;


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
    char* dataDecoded;



    int fd = pca9685Setup(PCA_PINBASE, PCA_I2C_ADDRESS, PCA_FREQUENCY);
    if(fd <0)
    {
        strcpy(message.message,"main_pilot_info_initfailed");
        message.priority = 20;
        sleep(1);
        sendMessage(mainITMHandler, message);

        pthread_exit(NULL);
    }
    pca9685PWMReset(fd);

    //armQuadcopter();

    // Waiting 500ms for ESC to init :
    usleep(500000);

    while(1)
    {


        receivedMessage =retrieveMessage(pilotITMHandler);
        if (receivedMessage == NULL)
        {
            //printDebug("[i] No message for pilot");
        }
        else
        {


            decoded = decodeMessageITM(receivedMessage);

            if (strcmp(decoded.destination,"pilot"))
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

                    else if (!strcmp(decoded.message, "arm"))
                    {
                        printDebug("[i] Received the order to arm motors, arming");
                        armQuadcopter();

                    }

                    else if (!strcmp(decoded.message, "disarm"))
                    {
                        printDebug("[i] Received the order to disarm motors, disarming");
                        disarmQuadcopter();
                    }

                    else if (!strcmp(decoded.message, "increasepwm"))
                    {
                        printDebug("[i] Increasing pwm...");
                        testCommand +=0.1;
                        printf("[i] PWM is now %f", testCommand);
                    }

                    else if (!strcmp(decoded.message, "decreasepwm"))
                    {
                        printDebug("[i] Decreasing pwm...");
                        testCommand -=0.1;
                        printf("[i] PWM is now %f", testCommand);
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
//
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

//            strcpy(message.message,"main_pilot_info_autonormal");
//            sendMessage(mainITMHandler, message);
//
//            strcpy(message.message, "autopilot_pilot_order_pause");
//            sendMessage(mainITMHandler, message);
//        }

        pilotStateShared.pilotMode = TEST; // For testing purposes

        pthread_mutex_unlock(&pilotStateShared.readWriteMutex);
        pthread_mutex_unlock(&pilotCommandsShared.readWrite);

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
            writeCommands();//???

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

//            if (testCommand >= 1) testCommand = 0;
//            else testCommand += 0.01;
//            usleep(200000000);


            break;

        }
        writeCommands();
        if (pilotStateShared.pilotMode == CUTOFF) disarmQuadcopter();

        pthread_mutex_unlock(&receivedCommands.readWriteMutex);
        pthread_mutex_unlock(&pilotCommandsShared.readWrite);


        usleep(pilotCommandsShared.refreshingPeriod);




    }

}


