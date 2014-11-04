#ifndef PID_H
#define PID_H

#include <sys/time.h>
#include <algorithm>

class PID
{
public:

    PID(double p, double i, double d);
    void setConstants(double p, double i, double d);
    double compute(double input, double setpoint);

private:

    double kp, ki, kd, lastError, integral;
    long  lastTime;
    struct timeval tv;
};

#endif // PID_H

