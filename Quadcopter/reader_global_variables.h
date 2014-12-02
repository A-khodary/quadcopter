// Global variables structures declaration

typedef struct  receivedCommandsShared {


        // Commands defined to be 0-100% for each channel

        long ch1;
        long ch2;
        long ch3;
        long ch4;
        long ch5;
        long ch6;
        long ch7;
        long ch8;
        long ch9;

        pthread_mutex_t readWriteMutex;

}receivedCommandsShared;


// Global variables definitions

extern receivedCommandsShared userCommands;
