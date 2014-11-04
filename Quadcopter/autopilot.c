/*
##############################################
#   Data logger functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

void asserv_trajectoire( autopilotObjective* autopilotObjective) {

double destinationX, destinationY, destinationZ, currentPositionX,currentPositionY, currentPositionZ, x_computed, y_computed, z_computed, yaw_computed, roll_computed, pitch_computed, directionYAW, currentYAW, currentROLL, currentPITCH;

switch(autopilotObjective.code){

case 1 :

    PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0), yaw_pid(2,0,0);// coefficients à trouver empiriquement

    currentPositionX = positionShared.x;
    currentPositionY = positionShared.y;
    currentPositionZ = positionShared.z;
    currentYAW =  flightStateShared.yaw;
    currentROLL =  flightStateShared.roll;
    currentPITCH =  flightStateShared.pitch;


    destinationX = autopilotObjective.destinationX;
    destinationY = autopilotObjective.destinationY;
    destinationZ = autopilotObjective.destinationZ;
    directionYAW = autopilotObjective.directionYAW;

    x_computed = x_pid.compute(currentPositionX, destinationX);
    y_computed = y_pid.compute(currentPositionY, destinationY);
    z_computed = z_pid.compute(currentPositionZ, destinationZ);
    yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

    break;

case 2:

    PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0), yaw_pid(2,0,0);// coefficients à trouver empiriquement
,
    currentPositionX = positionShared.x;
    currentPositionY = positionShared.y;
    currentPositionZ = positionShared.z;
    currentYAW =  flightStateShared.yaw;
    currentROLL =  flightStateShared.roll;
    currentPITCH =  flightStateShared.pitch;

    destinationX = autopilotObjective.destinationX;
    destinationY = autopilotObjective.destinationY;
    destinationZ = autopilotObjective.destinationZ;
    directionYAW = autopilotObjective.directionYAW;

    x_computed = x_pid.compute(currentPositionX, destinationX);
    y_computed = y_pid.compute(currentPositionY, destinationY);
    z_computed = z_pid.compute(currentPositionZ, destinationZ);
    yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

    break;

case 3 :

    PID x_pid(2,0,0), y_pid(2,0,0), z_pid(2,0,0), yaw_pid(2,0,0);// coefficients à trouver empiriquement

    currentPositionX = positionShared.x;
    currentPositionY = positionShared.y;
    currentPositionZ = positionShared.z;
    currentYAW =  flightStateShared.yaw;
    currentROLL =  flightStateShared.roll;
    currentPITCH =  flightStateShared.pitch;

    destinationX = autopilotObjective.destinationX;
    destinationY = autopilotObjective.destinationY;
    destinationZ = autopilotObjective.destinationZ;
    directionYAW = autopilotObjective.directionYAW;

    x_computed = x_pid.compute(currentPositionX, destinationX);
    y_computed = y_pid.compute(currentPositionY, destinationY);
    z_computed = z_pid.compute(currentPositionZ, destinationZ);
    yaw_computed = yaw_pid.compute(currentYAW,  directionYAW);

    break;

case 4 :

    break;

default :


    break;
}
