/*
##############################################
#   Data logger functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

void asserv_trajectoire( autopilotObjective* autopilotObjective) {

double destinationX, destinationY, destinationZ, currentPositionX,currentPositionY, currentPositionZ, x_computed, y_computed, z_computed;
PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0);// coefficients à trouver empiriquement

currentPositionX = positionShared.x;
currentPositionY = positionShared.y;
currentPositionZ = positionShared.z;

destinationX = autopilotObjective.destinationX;
destinationY = autopilotObjective.destinationY;
destinationZ = autopilotObjective.destinationZ;

x_computed = x_pid.compute(currentPositionX, destinationX);
y_computed = y_pid.compute(currentPositionY, destinationY);
z_computed = z_pid.compute(currentPositionZ, destinationZ);

//ensuite asserv des moteurs avec x,y,z computed
}
