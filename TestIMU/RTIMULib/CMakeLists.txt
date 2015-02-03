#set(CMAKE_C_COMPILER "/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc")
#set(CMAKE_CXX_COMPILER "/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-c++")

cmake_minimum_required(VERSION 2.8.3)

project( testIMU )

SET(COMPILE_DEFINITIONS -Werror)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CC_FLAGS_DEBUG "${CMAKE_CC_FLAGS_DEBUG} -Wall")

add_library (IMULib RTFusionKalman4.cpp RTFusionRTQF.cpp RTFusion.cpp RTIMU.cpp RTIMUHal.cpp RTIMUMPU6050HMC5883L.cpp RTIMUNull.cpp RTIMUSettings.cpp RTMath.cpp main.cpp)
#add_executable (IMU_exec RTFusionKalman4.cpp RTFusionRTQF.cpp RTFusion.cpp RTIMU.cpp RTIMUHal.cpp RTIMUMPU6050HMC5883L.cpp RTIMUNull.cpp RTIMUSettings.cpp RTMath.cpp)
#target_link_libraries (IMU_exec LINK_PUBLIC IMULib)

