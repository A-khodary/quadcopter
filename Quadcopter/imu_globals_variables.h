// Global variables structures declaration

typedef struct  flightStateShared {
        double yaw;
        double pitch;
        double roll;

        pthread_mutex_t readWriteMutex;

}flightStateShared;



typedef struct rawPositionShared {

    double latitude;
    double longitude;
    double altitude;

    pthread_mutex_t readWriteMutex;

}rawPositionShared;

typedef struct positionShared {

    double x;
    double y;
    double z;

    pthread_mutex_t readWriteMutex;

}positionShared



// Global variables definitions

flightStateShared quadcopterFlightStateShared;
rawPositionShared quadcopteRawPositionShared;
positionShared quadcopterPositionFromHomeShared;


