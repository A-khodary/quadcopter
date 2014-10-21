// Global variables structures declaration

typedef struct  receivedCommandsShared {


        // Commands defined to be 0-100% for each channel

        double ch1;
        double ch2;
        double ch3;
        double ch4;
        double ch5;
        double ch6;
        double ch7;
        double ch8;
        double ch9;

        pthread_mutex_t readWriteMutex;

}receivedCommandsShared;


// Global variables definitions

receivedCommandsShared userCommands;
