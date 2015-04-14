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

#include "RTIMUSettings.h"
#include "RTIMUMPU6050HMC5883L.h"

#define RATE_TIMER_INTERVAL 2

RTIMUSettings::RTIMUSettings(const char *productType)
{
    if ((strlen(productType) > 200) || (strlen(productType) == 0)) {
        HAL_ERROR("Product name too long or null - using default\n");
        strcpy(m_filename, "RTIMULib.ini");
    } else {
        sprintf(m_filename, "%s.ini", productType);
    }
    loadSettings();
}

bool RTIMUSettings::discoverIMU(int& imuType, unsigned char& slaveAddress)
{
    unsigned char result;
    unsigned char altResult;

    setI2CBus(m_I2CBus);
    if (!I2COpen()) {
        HAL_ERROR1("Failed to open I2C bus %d\n", m_I2CBus);
        return false;
    }

    if (I2CRead(MPU6050_ADDRESS0, MPU6050_WHO_AM_I, 1, &result, "")) {
        if (result == MPU6050_ID) {
            imuType = RTIMU_TYPE_MPU6050;
            slaveAddress = MPU6050_ADDRESS0;
            I2CClose();
            HAL_INFO("Detected MPU6050 at standard address\n");
            return true;
        }
    }

    if (I2CRead(MPU6050_ADDRESS1, MPU6050_WHO_AM_I, 1, &result, "")) {
        if (result == MPU6050_ID) {
            imuType = RTIMU_TYPE_MPU6050;
            slaveAddress = MPU6050_ADDRESS1;
            I2CClose();
            HAL_INFO("Detected MPU6050 at option address\n");
            return true;
        }
    }

    I2CClose();

    HAL_ERROR("No IMU detected\n");
    return false;
}

bool RTIMUSettings::loadSettings()
{
    char buf[200];
    char key[200];
    char val[200];
    RTFLOAT ftemp;

    //  preset general defaults

    m_I2CSlaveAddress = MPU6050_ADDRESS0 ; //ou MPU6050_ADDRESS1
    m_I2CBus = 1;
    m_fusionType = RTFUSION_TYPE_RTQF;
    m_compassCalValid = false;

    //  MPU6050 defaults

    m_MPU6050GyroAccelSampleRate = 50;
    m_MPU6050CompassSampleRate = 25;
    m_MPU6050GyroAccelLpf = MPU6050_LPF_20;
    m_MPU6050GyroFsr = MPU6050_GYROFSR_1000;
    m_MPU6050AccelFsr = MPU6050_ACCELFSR_8;

    //  check to see if settings file exists

    if (!(m_fd = fopen(m_filename, "r"))) {
        HAL_INFO("Settings file not found. Using defaults and creating settings file\n");
        return saveSettings();
    }

    while (fgets(buf, 200, m_fd)) {
        if ((buf[0] == '#') || (buf[0] == ' ') || (buf[0] == '\n'))
            // just a comment
            continue;

        if (sscanf(buf, "%[^=]=%s", key, val) != 2) {
            HAL_ERROR1("Bad line in settings file: %s\n", buf);
            fclose(m_fd);
            return false;
        }

        //  now decode keys

        //  general config

        if (strcmp(key, RTIMULIB_IMU_TYPE) == 0) {
            m_imuType = atoi(val);
        } else if (strcmp(key, RTIMULIB_FUSION_TYPE) == 0) {
            m_fusionType = atoi(val);
        } else if (strcmp(key, RTIMULIB_I2C_BUS) == 0) {
            m_I2CBus = atoi(val);
        } else if (strcmp(key, RTIMULIB_I2C_SLAVEADDRESS) == 0) {
            m_I2CSlaveAddress = atoi(val);

        // compass calibration

        } else if (strcmp(key, RTIMULIB_COMPASSCAL_VALID) == 0) {
            m_compassCalValid = strcmp(val, "true") == 0;
        } else if (strcmp(key, RTIMULIB_COMPASSCAL_MINX) == 0) {
            sscanf(val, "%f", &ftemp);
            m_compassCalMin.setX(ftemp);
        } else if (strcmp(key, RTIMULIB_COMPASSCAL_MINY) == 0) {
            sscanf(val, "%f", &ftemp);
            m_compassCalMin.setY(ftemp);
        } else if (strcmp(key, RTIMULIB_COMPASSCAL_MINZ) == 0) {
            sscanf(val, "%f", &ftemp);
            m_compassCalMin.setZ(ftemp);
        } else if (strcmp(key, RTIMULIB_COMPASSCAL_MAXX) == 0) {
            sscanf(val, "%f", &ftemp);
            m_compassCalMax.setX(ftemp);
        } else if (strcmp(key, RTIMULIB_COMPASSCAL_MAXY) == 0) {
            sscanf(val, "%f", &ftemp);
            m_compassCalMax.setY(ftemp);
        } else if (strcmp(key, RTIMULIB_COMPASSCAL_MAXZ) == 0) {
            sscanf(val, "%f", &ftemp);
            m_compassCalMax.setZ(ftemp);

        //  MPU6050 settings

        } else if (strcmp(key, RTIMULIB_MPU6050_GYROACCEL_SAMPLERATE) == 0) {
            m_MPU6050GyroAccelSampleRate = atoi(val);
        } else if (strcmp(key, RTIMULIB_MPU6050_COMPASS_SAMPLERATE) == 0) {
            m_MPU6050CompassSampleRate = atoi(val);
        } else if (strcmp(key, RTIMULIB_MPU6050_GYROACCEL_LPF) == 0) {
            m_MPU6050GyroAccelLpf = atoi(val);
        } else if (strcmp(key, RTIMULIB_MPU6050_GYRO_FSR) == 0) {
            m_MPU6050GyroFsr = atoi(val);
        } else if (strcmp(key, RTIMULIB_MPU6050_ACCEL_FSR) == 0) {
            m_MPU6050AccelFsr = atoi(val);

        //  Handle unrecognized key

        } else {
            HAL_ERROR1("Unrecognized key in settings file: %s\n", buf);
        }
    }
    HAL_INFO1("Settings file %s loaded\n", m_filename);
    fclose(m_fd);
    return saveSettings();                                  // make sure settings file is correct and complete
}

bool RTIMUSettings::saveSettings()
{
    if (!(m_fd = fopen(m_filename, "w"))) {
        HAL_ERROR("Failed to open settings file for save");
        return false;
    }

    //  General settings

    setComment("#####################################################################");
    setComment("");
    setComment("RTIMULib settings file");
    setBlank();
    setComment("General settings");
    setComment("");

    setBlank();
    setComment("IMU type - ");
    setComment("  0 = Auto discover");
    setComment("  1 = Null (used when data is provided from a remote IMU");
    setComment("  2 = InvenSense MPU-6050");
    setValue(RTIMULIB_IMU_TYPE, m_imuType);

    setBlank();
    setComment("");
    setComment("Fusion type type - ");
    setComment("  0 - Null. Use if only sensor data required without fusion");
    setComment("  1 - Kalman STATE4");
    setComment("  2 - RTQF");
    setComment("  3 - Kalman STATE7");
    setValue(RTIMULIB_FUSION_TYPE, m_fusionType);

    setBlank();
    setComment("");
    setComment("I2C Bus (between 0 and 7) ");
    setValue(RTIMULIB_I2C_BUS, m_I2CBus);

    setBlank();
    setComment("");
    setComment("I2C slave address (filled in automatically by auto discover) ");
    setValue(RTIMULIB_I2C_SLAVEADDRESS, m_I2CSlaveAddress);

    //  Compass calibration settings

    setBlank();
    setComment("#####################################################################");
    setComment("");

    setBlank();
    setComment("Compass calibration");
    setValue(RTIMULIB_COMPASSCAL_VALID, m_compassCalValid);
    setValue(RTIMULIB_COMPASSCAL_MINX, m_compassCalMin.x());
    setValue(RTIMULIB_COMPASSCAL_MINY, m_compassCalMin.y());
    setValue(RTIMULIB_COMPASSCAL_MINZ, m_compassCalMin.z());
    setValue(RTIMULIB_COMPASSCAL_MAXX, m_compassCalMax.x());
    setValue(RTIMULIB_COMPASSCAL_MAXY, m_compassCalMax.y());
    setValue(RTIMULIB_COMPASSCAL_MAXZ, m_compassCalMax.z());

    //  MPU-6050 settings

    setBlank();
    setComment("#####################################################################");
    setComment("");
    setComment("MPU-6050 settings");
    setComment("");

    setBlank();
    setComment("Gyro sample rate (between 5Hz and 1000Hz) ");
    setValue(RTIMULIB_MPU6050_GYROACCEL_SAMPLERATE, m_MPU6050GyroAccelSampleRate);

    setBlank();
    setComment("");
    setComment("Compass sample rate (between 1Hz and 100Hz) ");
    setValue(RTIMULIB_MPU6050_COMPASS_SAMPLERATE, m_MPU6050CompassSampleRate);

    setBlank();
    setComment("");
    setComment("Gyro/accel low pass filter - ");
    setComment("  0 - gyro: 256Hz, accel: 260Hz");
    setComment("  1 - gyro: 188Hz, accel: 184Hz");
    setComment("  2 - gyro: 98Hz, accel: 98Hz");
    setComment("  3 - gyro: 42Hz, accel: 44Hz");
    setComment("  4 - gyro: 20Hz, accel: 21Hz");
    setComment("  5 - gyro: 10Hz, accel: 10Hz");
    setComment("  6 - gyro: 5Hz, accel: 5Hz");
    setValue(RTIMULIB_MPU6050_GYROACCEL_LPF, m_MPU6050GyroAccelLpf);

    setBlank();
    setComment("");
    setComment("Gyro full scale range - ");
    setComment("  0  - +/- 250 degress per second");
    setComment("  8  - +/- 500 degress per second");
    setComment("  16 - +/- 1000 degress per second");
    setComment("  24 - +/- 2000 degress per second");
    setValue(RTIMULIB_MPU6050_GYRO_FSR, m_MPU6050GyroFsr);

    setBlank();
    setComment("");
    setComment("Accel full scale range - ");
    setComment("  0  - +/- 2g");
    setComment("  8  - +/- 4g");
    setComment("  16 - +/- 8g");
    setComment("  24 - +/- 16g");
    setValue(RTIMULIB_MPU6050_ACCEL_FSR, m_MPU6050AccelFsr);

}

void RTIMUSettings::setBlank()
{
    fprintf(m_fd, "\n");
}

void RTIMUSettings::setComment(const char *comment)
{
    fprintf(m_fd, "# %s\n", comment);
}

void RTIMUSettings::setValue(const char *key, const bool val)
{
    fprintf(m_fd, "%s=%s\n", key, val ? "true" : "false");
}

void RTIMUSettings::setValue(const char *key, const int val)
{
    fprintf(m_fd, "%s=%d\n", key, val);
}

void RTIMUSettings::setValue(const char *key, const RTFLOAT val)
{
    fprintf(m_fd, "%s=%f\n", key, val);
}


