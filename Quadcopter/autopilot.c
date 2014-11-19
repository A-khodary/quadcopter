/*
##############################################
#   Data logger functionnality                     #
#   Nicolas de Maubeuge and Vivien Billaud   #
#   October 2014                             #
#                                            #
##############################################
*/

//TODO : adapter les différents PID à utiliser pour chaque mode

void trajAsserv(autopilotObjective* autopilotObjective) {

    extern float landTakeoffXP, landTakeoffXPI, landTakeoffXPD, landTakeoffYP, landTakeoffYPI, landTakeoffYPD, landTakeoffZP, landTakeoffZPI, landTakeoffZPD, landTakeoffYawP, landTakeoffYawPI, landTakeoffYawPD
    //  TODO : use coeff defines in variables

    double destinationX, destinationY, destinationZ, currentPositionX,currentPositionY, currentPositionZ, x_computed, y_computed, z_computed, yaw_computed, roll_computed, pitch_computed, directionYAW, currentYAW, currentROLL, currentPITCH;

    switch(autopilotObjective.code){

    case 1 : //mode  LAND_TAKEOFF

        PID x_pid(landTakeoffXP,landTakeoffXPI,landTakeoffXPD), y_pid(landTakeoffYP,landTakeoffYPI,landTakeoffYPD), z_pid(landTakeoffZP,landTakeoffZPI,landTakeoffZPD), yaw_pid(landTakeoffYawP,landTakeoffYawPI,landTakeoffYawPD);// coefficients à trouver empiriquement

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

    case 2: //mode GOTO_STANDARD

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

    case 3 : //mode GOTO_HOVERING

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

    case 4 : // mode POSITION_HOLD

        break;

    default : // mode POSITION_HOLD par défault


        break;
}
