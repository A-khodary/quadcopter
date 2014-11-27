int insertObjective(autopilotObjective* objective);
autopilotObjective* readCurrentObjective();
int removeCurrentObjective();
int flushFifoObjective();

autopilotObjective* readSpecificObjective(int objectiveNumber);
int removeSpecificObjective(int objectiveNumber);
