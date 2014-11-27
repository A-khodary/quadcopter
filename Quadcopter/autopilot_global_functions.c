int insertObjective(autopilotObjective* objective)
{
    extern autopilotObjectiveFifo;
    int objectiveIndex;

    pthread_mutex_lock(autopilotObjectiveFifo.readWrite);

    if (autopilotObjectiveFifo.isFifoInitialized != 1)
    {
        printDebug("[e] Autopilot : attempting to insert objective in a non initialized Objective FIFO");
        return -1;
    }

    if (autopilotObjectiveFifo.numberOfObjectivesPending >= AUTOPILOT_OBJECTIVE_FIFO_SIZE)
    {
        printDebug("[e] Autopilot : attempting to insert objective in a full Objective FIFO");
        return -2;
    }










}
/* Return summary :
-1 Fifo non initialized
-2 Fifo is full
 1 Inserting Ok


*/


autopilotObjective* readCurrentObjective()
{

}


int removeCurrentObjective()
{

}


autopilotObjective* readSpecificObjective(int objectiveNumber)
{

}


int removeSpecificObjective(int objectiveNumber)
{

}

int flushFifoObjective()
{


}
