// Global variables structures declaration

typedef struct  receivedCommandsShared {


        // Commands defined to be 0-100% for each channel

        long commands[9];
        pthread_mutex_t readWriteMutex;

}receivedCommandsShared_t;


// Global variables definitions

extern receivedCommandsShared_t receivedCommands;
