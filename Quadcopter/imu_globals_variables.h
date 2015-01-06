// Global variables structures declaration

typedef struct  flightStateShared {
        double yaw;
        double pitch;
        double roll;

        pthread_mutex_t readWriteMutex;

}flightStateShared_t;



typedef struct rawPositionShared {

    double latitude;
    double longitude;
    double altitude;

    pthread_mutex_t readWriteMutex;

}rawPositionShared_t;

typedef struct positionShared { // Position from home

    double x;
    double y;
    double z;

    pthread_mutex_t readWriteMutex;

}positionShared_t;



// Global variables declaration

extern flightStateShared_t quadcopterFlightStateShared;
extern rawPositionShared_t quadcopteRawPositionShared;
extern positionShared_t quadcopterPositionShared;
extern rawPositionShared_t homeRawPosition;


