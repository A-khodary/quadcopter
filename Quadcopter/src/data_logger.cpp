#include "data_logger.h"


/*
##############################################
#   Data logger functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

//flightStateShared_t flightStateShared;
//rawPositionShared_t rawPositionShared;
//positionShared_t positionShared;

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

    float roll, pitch, yaw, rollSpeed, pitchSpeed, yawSpeed,x,y,z,vx,vy,vz;

    int32_t lat,lon,alt,relative_alt;

    int16_t vxBis,vyBis,vzBis;

    uint16_t hdg;//Compass heading in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX

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

        // Sending heartbeat :
        printDebug("[i] Sending heartbeat...");

        //mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_AUTO_ARMED, 0, MAV_STATE_BOOT);
		mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, heartBeatMode, 0, heartBeatState);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));

		/* Send attitude */

        pthread_mutex_lock(&flightStateShared.readWriteMutex);

        roll = flightStateShared.roll;
        pitch = flightStateShared.pitch;
        yaw = flightStateShared.yaw;
        rollSpeed = 0;//integer with previous roll value ?
        pitchSpeed = 0;
        yawSpeed = 0;

        pthread_mutex_unlock(&flightStateShared.readWriteMutex);

        mavlink_msg_attitude_pack(1, 200, &msg, microsSinceEpoch(), roll, pitch, yaw, rollSpeed, pitchSpeed, yawSpeed);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));

        //Send local position

        pthread_mutex_lock(&positionShared.readWriteMutex);

        x = positionShared.x;
        y = positionShared.y;
        z = positionShared.z;
        vx = 0;//integer with previous x value ?
        vy = 0;
        vz= 0;

        pthread_mutex_unlock(&positionShared.readWriteMutex);

        mavlink_msg_local_position_ned_pack(1, 200, &msg, microsSinceEpoch(), x, y, z, vx, vy, vz);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));

        //Send global position

        pthread_mutex_lock(&positionShared.readWriteMutex);
        pthread_mutex_lock(&rawPositionShared.readWriteMutex);

        lat = rawPositionShared.latitude;
        lon = rawPositionShared.longitude;
        alt = rawPositionShared.altitude;
        relative_alt = positionShared.distanceFromGround;
        vxBis = 0;//integer with previous x value ?
        vyBis = 0;
        vzBis = 0;
        hdg = 65535;//UINT16_MAX

        pthread_mutex_unlock(&positionShared.readWriteMutex);
        pthread_mutex_unlock(&rawPositionShared.readWriteMutex);

        mavlink_msg_global_position_int_pack(1, 200, &msg, microsSinceEpoch(), lat, lon, alt, relative_alt, vxBis, vyBis, vzBis, hdg);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));


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
                        //MAV_MODE_FLAG_SAFETY_ARMED

                    }

                    if (!strcmp(currentDecoded.message, "disarmed"))
                    {
                        motorsArmed = 0;
                    }


                    if (!strcmp(currentDecoded.message, "takeoffed"))
                    {
                        heartBeatState = MAV_STATE_ACTIVE;//?
                    }



                    if (!strcmp(currentDecoded.message, "nowork"))
                    {

                    }

                    if (!strcmp(currentDecoded.message, "manual"))
                    {

                    }

                    if (!strcmp(currentDecoded.message, "autopilot"))
                    {

                    }


                    if (!strcmp(currentDecoded.message, "stabilized"))
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
                    // Manual
                    // Autopilot
                    // Stabilized




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
                if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))//MAVLINK_COMM_0 = ???
                {
                    // Packet received
                    printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);

                    //TODO : Message handling
                    switch(msg.msgid)
                    {
                        case MAVLINK_MSG_ID_SET_POSITION_TARGET_GLOBAL_INT :
                        {
                            // Decode message received

                            mavlink_set_position_target_global_int_t global_position;
                            mavlink_msg_set_position_target_global_int_decode(&msg, &global_position);

                            // Actualize current objective

                            currentMessage.priority = 1;
                            strcpy(currentMessage.message, "autopilot_datalogger_order_newobjective");
                            currentMessage.dataSize = sizeof(autopilotObjective_t);
                            autopilotObjective_t* objective;
                            strcpy(objective->name, "newobjective") ;
                            //objective->code = ;
                            objective->priority = 1;
                            objective->destinationAlt = global_position.alt;//Altitude in meters in AMSL altitude, not WGS84 if absolute or relative, above terrain if GLOBAL_TERRAIN_ALT_INT
                            objective->destinationLat = global_position.lat_int;
                            objective->destinationLong = global_position.lon_int;

                            message_t* pCurrentMessage = &currentMessage;//Dynamic copy ???
                            pCurrentMessage->data = (autopilotObjective_t*)malloc(sizeof(autopilotObjective_t));

                            pCurrentMessage->data = objective;

                            sendMessage(mainITMHandler, currentMessage);


                        }

                            break;

                    default:
                        //Do nothing
                        break;
                    }
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







