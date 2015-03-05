set(CMAKE_C_COMPILER ${C_COMPILER_PATH})
set(CMAKE_CXX_COMPILER ${CXX_COMPILER_PATH})

cmake_minimum_required(VERSION 2.8.3)


# project declaration

project(quadcopter)
set(EXECUTABLE_OUTPUT_PATH build/${CMAKE_BUILD_TYPE})

# option declarations 

SET(COMPILE_DEFINITIONS -Werror)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -lpthread -lwiringPi")
set(CMAKE_CC_FLAGS_DEBUG "${CMAKE_CC_FLAGS_DEBUG} -Wall")


# Source list generation :

file(GLOB_RECURSE source_files src/*)


# Executable declaration 

add_executable(Quadcopter_${CMAKE_BUILD_TYPE} ${source_files})


# Libraries includes and executable configuration


#	Pthread

include_directories(${PTHREAD_INCLUDE_DIRS})
link_directories(${PTHREAD_LIBRARY_DIRS})
target_link_libraries(${PTHREAD_NAME} Quadcopter_${CMAKE_BUILD_TYPE})


#	Wiring Pi I2C

include_directories(${WIRING_PI_I2C_INCLUDE_DIRS})
link_directories(${WIRING_PI_I2C_LIBRARY_DIRS})
target_link_libraries(${WIRING_PI_I2C_NAME} Quadcopter_${CMAKE_BUILD_TYPE})




 






#add_subdirectory(RTIMULib)
#add_library (QuadcopterLib quadcopter.c)
#target_include_directories (QuadcopterLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable (quadcopter_exec quadcopter.c)
#target_link_libraries (quadcopter_exec LINK_PUBLIC QuadcopterLib)

