#ifndef imu_globals_variables_H
#define imu_globals_variables_H


// Global variables structures declaration

typedef struct
{
    double yaw;
    double pitch;
    double roll;

    pthread_mutex_t readWriteMutex;

} flightStateShared_t;



typedef struct
{

    double latitude;
    double longitude;
    double altitude;

    pthread_mutex_t readWriteMutex;

} rawPositionShared_t;

typedef struct
{
    // Position from home

    double x;
    double y;
    double z;

    pthread_mutex_t readWriteMutex;

} positionShared_t;

typedef struct
{
    // Position of home in the 1866 Clark ellipsoid format


    double x;
    double y;

    double latitude; // in °
    double longitude; // in °

} homePosition_t;



// Global variables declaration

extern flightStateShared_t flightStateShared;
extern rawPositionShared_t rawPositionShared;
extern positionShared_t positionShared;
extern rawPositionShared_t homeRawPosition;
extern homePosition_t homePosition;

#endif
