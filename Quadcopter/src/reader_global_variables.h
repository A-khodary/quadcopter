#ifndef READER_GLOBAL
#define READER_GLOBAL


// Global variables structures declaration

typedef struct  receivedCommandsShared_t {


        // Commands defined to be 0-100% for each channel

        long commands[9];
        long ultrasonicTelemeter;

        pthread_mutex_t readWriteMutex;

}receivedCommandsShared_t;


// Global variables definitions

extern receivedCommandsShared_t receivedCommands;

#endif
