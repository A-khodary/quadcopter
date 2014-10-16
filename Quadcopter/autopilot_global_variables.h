// Global variables structures declarations :

typedef struct autopilotCurrentActionShared
{
    pthread_mutex_t readWrite;
}

typedef struct autopilotCurrentObjectiveShared
{
    pthread_mutex_t readWrite;
}

typedef struct autopilotFinalObjectiveShared
{
    pthread_mutex_t readWrite;
}


