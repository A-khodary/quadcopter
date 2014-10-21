// Global variables structures declarations :


typedef struct autopilotObjective
{
    char name[64]; // the objective name that will be displayed
    int priority; // Priority from 0 to 100

    int code; // Objective code : for a complete list, refer to documentation

    // Those elements may not be necessary ( depending from the objective code )

    double destinationX;
    double destinationY;
    double destinationZ;

     double max_speed; // Max speed in ms-1



    pthread_mutex_t readWrite;

}autopilotObjective;


autopilotObjective* ObjectiveFifo[256];


// Global functions declarations :

int insertObjective(autopilotObjective* objective);
autopilotObjective* readCurrentObjective();
int removeCurrentObjective();

autopilotObjective* readSpecificObjective(int objectiveNumber);
int removeSpecificObjective(int objectiveNumber);


