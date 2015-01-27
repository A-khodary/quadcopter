#set(CMAKE_C_COMPILER "HOME/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc")
#set(CMAKE_CXX_COMPILER "HOME/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++")

cmake_minimum_required(VERSION 2.8.3)

project( quadcopter )

SET(COMPILE_DEFINITIONS -Werror)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CC_FLAGS_DEBUG "${CMAKE_CC_FLAGS_DEBUG} -Wall")

add_subdirectory(RTIMULib)

add_library (QuadcopterLib quadcopter.c)
#target_include_directories (QuadcopterLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable (quadcopter_exec quadcopter.c)
target_link_libraries (quadcopter_exec LINK_PUBLIC QuadcopterLib)

