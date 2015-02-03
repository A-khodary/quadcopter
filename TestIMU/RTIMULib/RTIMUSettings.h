//
//  Copyright (c) 2014 richards-tech
//
//  This file is part of RTIMULib
//
//  RTIMULib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  RTIMULib is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RTIMULib.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _RTIMUSETTINGS_H
#define _RTIMUSETTINGS_H

#include "RTMath.h"
#include "RTIMUHal.h"

//  Settings keys

#define RTIMULIB_IMU_TYPE                   "IMUType"
#define RTIMULIB_FUSION_TYPE                "FusionType"
#define RTIMULIB_I2C_SLAVEADDRESS           "I2CSlaveAddress"
#define RTIMULIB_I2C_BUS                    "I2CBus"

//  MPU6050 settings keys

#define RTIMULIB_MPU6050_GYROACCEL_SAMPLERATE "MPU6050GyroAccelSampleRate"
#define RTIMULIB_MPU6050_COMPASS_SAMPLERATE "MPU6050CompassSampleRate"
#define RTIMULIB_MPU6050_GYROACCEL_LPF      "MPU6050GyroAccelLpf"
#define RTIMULIB_MPU6050_GYRO_FSR           "MPU6050GyroFSR"
#define RTIMULIB_MPU6050_ACCEL_FSR          "MPU6050AccelFSR"

//  Compass calibration settings keys

#define RTIMULIB_COMPASSCAL_VALID           "CompassCalValid"
#define RTIMULIB_COMPASSCAL_MINX            "CompassCalMinX"
#define RTIMULIB_COMPASSCAL_MAXX            "CompassCalMaxX"
#define RTIMULIB_COMPASSCAL_MINY            "CompassCalMinY"
#define RTIMULIB_COMPASSCAL_MAXY            "CompassCalMaxY"
#define RTIMULIB_COMPASSCAL_MINZ            "CompassCalMinZ"
#define RTIMULIB_COMPASSCAL_MAXZ            "CompassCalMaxZ"

class RTIMUSettings : public RTIMUHal
{
public:
    RTIMUSettings(const char *productType = "RTIMULib");

    //  This function tries to find an IMU. It stops at the first valid one
    //  and return true or else false

   bool discoverIMU(int& imuType, unsigned char& slaveAddress);

    //  This function loads the local variables from the settings file or uses defaults

    bool loadSettings();

    //  This function saves the local variables to the settings file

    bool saveSettings();

    //  These are the local variables

    int m_imuType;                                          // type code of imu in use
    int m_fusionType;                                       // fusion algorithm type code

    unsigned char m_I2CSlaveAddress;                        // I2C slave address of the imu
    unsigned char m_I2CBus;                                 // I2C bus of the imu (eg 1 for Raspberry Pi usually)

    bool m_compassCalValid;                                 // true if there is valid compass calibration data
    RTVector3 m_compassCalMin;                              // the minimum values
    RTVector3 m_compassCalMax;                              // the maximum values

    //  IMU-specific vars

    //  MPU6050

    int m_MPU6050GyroAccelSampleRate;                       // the sample rate (samples per second) for gyro and accel
    int m_MPU6050CompassSampleRate;                         // same for the compass
    int m_MPU6050GyroAccelLpf;                              // low pass filter code for the gyro and accel
    int m_MPU6050GyroFsr;                                   // FSR code for the gyro
    int m_MPU6050AccelFsr;                                  // FSR code for the accel

private:
    void setBlank();
    void setComment(const char *comment);
    void setValue(const char *key, const bool val);
    void setValue(const char *key, const int val);
    void setValue(const char *key, const RTFLOAT val);

    char m_filename[256];                                    // the settings file name

    FILE *m_fd;
};

#endif // _RTIMUSETTINGS_H

