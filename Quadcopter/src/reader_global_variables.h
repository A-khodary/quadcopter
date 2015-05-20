#ifndef READER_GLOBAL
#define READER_GLOBAL

// Some defines for the GPS status :

#define NO_FIX 1
#define FIX 2

// Global variables structures declaration

typedef struct  receivedCommandsShared_t {


        // Commands defined to be 0-100% for each channel

        float commands[9];
        float ultrasonicTelemeter;

        int gpsStatus;
        int gpsNumberOfSats;
        float latitude;
        float longitude;
        float altitude;

        float voltage;
        float current;


        pthread_mutex_t readWriteMutex;

}receivedCommandsShared_t;


// Global variables definitions

extern receivedCommandsShared_t receivedCommands;

#endif
