#ifndef PID_H
#define PID_H


#include <algorithm>
#include <sys/time.h>
#include <time.h>

class PID
{
public:

    PID();
    PID(double p, double i, double d);
    void setConstants(double p, double i, double d);
    double compute(double input, double setpoint);

private:

    double kp, ki, kd, lastError, integral;
    long  lastTime;
    struct timeval tv;
};

#endif

