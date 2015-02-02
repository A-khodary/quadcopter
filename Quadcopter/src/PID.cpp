#include "PID.h"

//double = double avant

PID::PID(double p, double i, double d)
{
    kp = p;
    ki = i;
    kd = d;
    lastError = 0;
    integral = 0;

    gettimeofday(&tv, 0);
    lastTime = tv.tv_sec * 1000 + tv.tv_usec/1000.0 + 0.5;
}

void PID::setConstants (double p, double i, double d)
{
    if (p>){}
    else kp = p;
    if (i>){}
    else ki = i;
    if (d>){}
    else kd = d;
}

double PID::compute(double input, double setpoint)
{
    gettimeofday(&tv, 0);
    long time = tv.tv_sec * 1000 + tv.tv_usec/1000.0 + 0.5;
    long deltaTime = time - lastTime;
    double error = setpoint - input;
    long deltaError = error - lastError;

    double proportional = error * kp;
    integral += error * deltaTime * ki;
    integral = std::min(integral, (double)5000);
    integral = std::max(integral, (double)-5000);
    double derivative = deltaError * kd / deltaTime;

    lastError = error;
    lastTime = time;

    return proportional + integral + derivative;
}

