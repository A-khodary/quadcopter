#include "main.h"

RTIMU *imu;
RTIMU_DATA imuData;

int main(int argc, char** argv) {

std::cout << "\033[1;31m" << std::endl << "--------------------" << std::endl << " raspcopter " << std::endl << "--------------------" << "\033[0m" << std::endl << std::endl;

RTIMUSettings *settings = new RTIMUSettings("raspcopter");
imu = RTIMU::createIMU(settings);imu->IMUInit();

}
