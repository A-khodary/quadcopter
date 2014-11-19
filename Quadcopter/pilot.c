
/*
##############################################
#   Pilot functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/





int initPca9685()
{
    printDebug("[i] Initialisation du Pca9685")




    // Returns : 0 : init OK
    //           1 : init FAILED
}



int writeCommands()
{
    pthread_mutex_lock(quadcopterPilotCommandsShared.readWrite); // Locking mutex to prevent race conditions




    // Returns : 0 : write OK
    // Returns : 1 : write FAILED
}

void pilotHandler()
{

}
