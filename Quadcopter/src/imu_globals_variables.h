// Global variables structures declaration

typedef struct  flightStateShared {
        double yaw;
        double pitch;
        double roll;

        pthread_mutex_t readWriteMutex;

}flightStateShared_t;



typedef struct rawPositionShared_t {

    double latitude;
    double longitude;
    double altitude;

    pthread_mutex_t readWriteMutex;

}rawPositionShared_t;

typedef struct positionShared_t { // Position from home

    double x;
    double y;
    double z;

    pthread_mutex_t readWriteMutex;

}positionShared_t;

typedef struct homePosition_t { // Position of home in the 1866 Clark ellipsoid format


    double x;
    double y;

    double latitude; // in °
    double longitude; // in °

}homePosition_t;



// Global variables declaration

extern flightStateShared_t quadcopterFlightStateShared;
extern rawPositionShared_t quadcopteRawPositionShared;
extern positionShared_t quadcopterPositionShared;
extern rawPositionShared_t homeRawPosition;
extern homePosition_t homePosition;


