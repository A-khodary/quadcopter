
/*
##############################################
#   Pilot functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/



void writeCommands()
{
    pthread_mutex_lock(quadcopterPilotCommandsShared.readWrite); // Locking mutex to prevent race conditions



}
