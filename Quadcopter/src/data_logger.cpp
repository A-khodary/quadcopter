#include "data_logger.h"


/*
##############################################
#   Data logger functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

void* dataLoggerHandler(void* arg)
{
    // Initialization :

    bidirectionnalHandler_t* bidirectionnalHandler;
    bidirectionnalHandler = (bidirectionnalHandler_t*)arg;

    handler_t* mainITMHandler;
    handler_t* dataLoggerITMHandler;

    mainITMHandler = bidirectionnalHandler->mainITMHandler;
    dataLoggerITMHandler = bidirectionnalHandler->componentITMHandler;

    message_t* receivedMessage;
    message_t currentMessage;
    messageDecoded_t currentDecoded;

    strcpy(currentMessage.message, "main_datalogger_info_init");
    currentMessage.priority = 5;//to choose
    sendMessage(mainITMHandler, currentMessage);

    int tickCounter=0;

    // Message area :

    char readBuffer[128];

    // Mavlink initialization :

    int sock;
    struct sockaddr_in gcAddr;
    mavlink_message_t msg;
    uint16_t len;
    int bytes_sent;
    uint8_t buf[BUFFER_LENGTH];

    int heartBeatMode = MAV_MODE_PREFLIGHT;
    int heartBeatState = MAV_STATE_UNINIT;

    int motorsArmed = 0;

    unsigned int temp = 0;

    ssize_t recsize;
    socklen_t fromlen;

    memset(&gcAddr, 0, sizeof(gcAddr));
	gcAddr.sin_family = AF_INET;
	gcAddr.sin_addr.s_addr = inet_addr(GCS_IP);
	gcAddr.sin_port = htons(GCS_PORT);


    sock = initSocket(GCS_IP, GCS_PORT, gcAddr);
    if (sock == 0)
    {
        printDebug("[e] Error in datalogger : can't set the GSC socket !");

        // Notifying main thread of failure :
        strcpy(currentMessage.message, "main_datalogger_info_initfailed");
        currentMessage.priority = 5;//to choose
        sendMessage(mainITMHandler, currentMessage);

        sleep(1);
        pthread_exit(NULL);
    }


    // Notifying main handler of end of init
    strcpy(currentMessage.message, "main_datalogger_info_endofinit");
    currentMessage.priority = 5;//to choose
    sendMessage(mainITMHandler, currentMessage);

    while(1)
    {
        // Mavlink Area :

        // Senfing heartbeat :
        printDebug("[i] Sending heartbeat...");

        //mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_AUTO_ARMED, 0, MAV_STATE_BOOT);
		mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, heartBeatMode, 0, heartBeatState);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));

        sleep(1);

        //Message retrieving and handling area :

        receivedMessage = retrieveMessage(dataLoggerITMHandler);
        if (receivedMessage != NULL)
        {
            printDebug("[i] Data logger received a new message");
            currentDecoded = decodeMessageITM(receivedMessage);
            if (!strcmp(currentDecoded.destination, "datalogger"))
            {
                if (currentDecoded.operation == ORDER)
                {
                    if (!strcmp(currentDecoded.message, "testreader"))
                {
                        printDebug("[i] Beginning reader test");
                        pthread_mutex_lock(&receivedCommands.readWriteMutex);
                        printf("PWM 1 : %f\n", receivedCommands.commands[0]);
                        printf("PWM 2 : %f\n", receivedCommands.commands[1]);
                        printf("PWM 3 : %f\n", receivedCommands.commands[2]);
                        printf("PWM 4 : %f\n", receivedCommands.commands[3]);
                        printf("PWM 5 : %f\n", receivedCommands.commands[4]);
                        printf("PWM 6 : %f\n", receivedCommands.commands[5]);
                        printf("PWM 7 : %f\n", receivedCommands.commands[6]);
                        printf("PWM 8 : %f\n", receivedCommands.commands[7]);
                        printf("PWM 9 : %f\n", receivedCommands.commands[8]);

                        printf("Ultrasonic : %f\n", receivedCommands.ultrasonicTelemeter);
                        printf("GPS status : %f\n", receivedCommands.gpsStatus);
                        printf("Latitude : %f\n", receivedCommands.latitude);
                        printf("Longitude : %f\n", receivedCommands.longitude);
                        printf("Altiude : %f\n", receivedCommands.altitude);
                        fflush(stdout);

                        pthread_mutex_unlock(&receivedCommands.readWriteMutex);

                    }


                }

                else if (currentDecoded.operation == INFO)
                {

                    // System states messages :

                    if (!strcmp(currentDecoded.message, "engaged"))
                    {
                        heartBeatState = MAV_STATE_ACTIVE;
                    }

                    if (!strcmp(currentDecoded.message, "disengaged"))
                    {
                        heartBeatState = MAV_STATE_STANDBY;
                    }

                    if (!strcmp(currentDecoded.message, "crashed"))
                    {
                        heartBeatState = MAV_STATE_EMERGENCY;
                    }

                    // System mode messages :

                    if (!strcmp(currentDecoded.message, "armed"))
                    {
                        motorsArmed = 1;

                    }

                    if (!strcmp(currentDecoded.message, "disarmed"))
                    {
                        motorsArmed = 0;
                    }


                    if (!strcmp(currentDecoded.message, "takeoffed"))
                    {

                    }



                    if (!strcmp(currentDecoded.message, "nowork"))
                    {

                    }





                    // Messages to handle :
                    // Crashed
                    // Takeoffed
                    // Armed
                    // Disarmed
                    // Nowork
                    // engaged
                    // disengaged




                }


            }
            else
            {
                printDebug("[e] Datalogger received a message it should not");

            }
            free(receivedMessage);
        }



        // DataLogging area :

        memset(buf, 0, BUFFER_LENGTH);
        recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr *)&gcAddr, &fromlen);
        if (recsize > 0)
        {
            // Something received - print out all bytes and parse packet
            mavlink_message_t msg;
            mavlink_status_t status;
            printf("Bytes Received: %d\nDatagram: ", (int)recsize);
            for (int i = 0; i < recsize; ++i)
            {
                temp = buf[i];
                printf("%02x ", (unsigned char)temp);
                if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
                {
                    // Packet received
                    printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);

                    //TODO : Message handling
                }
            }
            printf("\n");
        }
        memset(buf, 0, BUFFER_LENGTH);
        sleep(1); // Sleep one second


         //Message sending function for testing :

        printDebug("\n[i] Enter a command :");
        fflush(stdout);

        scanf("%s", readBuffer);
        strcpy(currentMessage.message, readBuffer);
        currentMessage.priority = 5;

        sendMessage(mainITMHandler, currentMessage);
    }


}


int initSocket(char targetIp[100], int targetPort, struct sockaddr_in gcAddr)
{
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in locAddr;

	memset(&locAddr, 0, sizeof(locAddr));
	locAddr.sin_family = AF_INET;
	locAddr.sin_addr.s_addr = INADDR_ANY;
	locAddr.sin_port = htons(targetPort);

    /* Bind the socket to port 14551 - necessary to receive packets from qgroundcontrol */

	if (-1 == bind(sock,(struct sockaddr *)&locAddr, sizeof(struct sockaddr)))
    {
		printDebug("[e] Error mavlink socket bind failed");
		close(sock);
		return 0;
    }

    /* Attempt to make it non blocking */
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
    {
		printDebug("[e] Error setting mavlink socket nonblocking");
		close(sock);
		return 0;
    }

	return sock;
}








