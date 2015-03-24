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

    int tickCounter=0;

    // Message testing area :

    char readBuffer[128];


    //TODO : notify main handler of end of init

    while(1)
    {
        // External connection management Area : (Mavlink)




        // Message retrieving and handling area :

//        receivedMessage = retrieveMessage(dataLoggerITMHandler);
//        if (receivedMessage != NULL)
//        {
//            currentDecoded = decodeMessageITM(receivedMessage);
//            if (!strcmp(currentDecoded.destination, "datalogger"))
//            {
//                if (currentDecoded.operation == ORDER)
//                {
//                    if (!strcmp(currentDecoded.message, "testreader"))
//                {
//                        printDebug("[i] Beginning reader test");
//                        pthread_mutex_lock(&receivedCommands.readWriteMutex);
//                        printf("PWM 1 : %f\n", receivedCommands.commands[0]);
//                        printf("PWM 2 : %f\n", receivedCommands.commands[1]);
//                        printf("PWM 3 : %f\n", receivedCommands.commands[2]);
//                        printf("PWM 4 : %f\n", receivedCommands.commands[3]);
//                        printf("PWM 5 : %f\n", receivedCommands.commands[4]);
//                        printf("PWM 6 : %f\n", receivedCommands.commands[5]);
//                        printf("PWM 7 : %f\n", receivedCommands.commands[6]);
//                        printf("PWM 8 : %f\n", receivedCommands.commands[7]);
//                        printf("PWM 9 : %f\n", receivedCommands.commands[8]);
//
//                        printf("Ultrasonic : %f\n", receivedCommands.ultrasonicTelemeter);
//                        printf("GPS status : %f\n", receivedCommands.gpsStatus);
//                        printf("Latitude : %f\n", receivedCommands.latitude);
//                        printf("Longitude : %f\n", receivedCommands.longitude);
//                        printf("Altiude : %f\n", receivedCommands.altitude);
//
//                        pthread_mutex_unlock(&receivedCommands.readWriteMutex);
//
//                    }
//
//
//                }
//
//            }
//            else
//            {
//                printDebug("[e] Datalogger received a message it should not");
//
//            }
//            free(receivedMessage);
//        }



        // DataLogging area :




        // Message sending function for testing :

        printDebug("\n[i] Enter a command :");

        scanf("%s", readBuffer);
        strcpy(currentMessage.message, readBuffer);

        sendMessage(mainITMHandler, currentMessage);
        usleep(500000);
    }


}





/*

sendData()
{
    //The default UART header for your MCU
#include "uart.h" // TODO : adapt to Rpi
#include <mavlink/v1.0/common/mavlink.h>

mavlink_system_t mavlink_system;

mavlink_system.sysid = 20;                   ///< ID 20 for this airplane
mavlink_system.compid = MAV_COMP_ID_IMU;     ///< The component sending the message is the IMU, it could be also a Linux process

// Define the system type, in this case an airplane
uint8_t system_type = MAV_TYPE_FIXED_WING;
uint8_t autopilot_type = MAV_AUTOPILOT_GENERIC;

uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
uint32_t custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
uint8_t system_state = MAV_STATE_STANDBY; ///< System ready for flight

// Initialize the required buffers
mavlink_message_t msg;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];

// Pack the message
mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, system_type, autopilot_type, system_mode, custom_mode, system_state);

// Copy the message to the send buffer
uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

// Send the message with the standard UART send function
// uart0_send might be named differently depending on
// the individual microcontroller / library in use.
uart0_send(buf, len); // TODO : adapt to Rpi

}

receivedData()
{
    #include <mavlink/v1.0/common/mavlink.h>

// Example variable, by declaring them static they're persistent
// and will thus track the system state
static int packet_drops = 0;
static int mode = MAV_MODE_UNINIT; /* Defined in mavlink_types.h, which is included by mavlink.h


static void communication_receive(void)
{
	mavlink_message_t msg;
	mavlink_status_t status;

	// COMMUNICATION THROUGH EXTERNAL UART PORT (XBee serial)

	while(uart0_char_available())
	{
		uint8_t c = uart0_get_char();
		// Try to get a new message
		if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
			// Handle message

			switch(msg.msgid)
			{
			        case MAVLINK_MSG_ID_HEARTBEAT:
			        {
				  // E.g. read GCS heartbeat and go into
                                  // comm lost mode if timer times out
			        }
			        break;
			case MAVLINK_MSG_ID_COMMAND_LONG:
				// EXECUTE ACTION
				break;
			default:
				//Do nothing
				break;
			}
		}

		// And get the next one
	}

	// Update global packet drops counter
	packet_drops += status.packet_rx_drop_count;

	// COMMUNICATION THROUGH SECOND UART

	while(uart1_char_available())
	{
		uint8_t c = uart1_get_char();
		// Try to get a new message
		if(mavlink_parse_char(MAVLINK_COMM_1, c, &msg, &status))
		{
			// Handle message the same way like in for UART0
                        // you can also consider to write a handle function like
                        // handle_mavlink(mavlink_channel_t chan, mavlink_message_t* msg)
                        // Which handles the messages for both or more UARTS
		}

		// And get the next one
	}

	// Update global packet drops counter
	packet_drops += status.packet_rx_drop_count;
}

}
*/
