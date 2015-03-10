set(CMAKE_C_COMPILER ${C_COMPILER_PATH})
set(CMAKE_CXX_COMPILER ${CXX_COMPILER_PATH})

cmake_minimum_required(VERSION 2.8.3)


# project declaration

project(quadcopter)
set(EXECUTABLE_OUTPUT_PATH build/${CMAKE_BUILD_TYPE})

# option declarations

SET(COMPILE_DEFINITIONS -Werror)
set(CMAKE_CXX_FLAGS_DEBUG  -Wall -g")
set(CMAKE_CC_FLAGS_DEBUG  -Wall -g")

set(CMAKE_LINKER_FLAGS "-pthread" CACHE STRING "Linker Flags" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_LINKER_FLAGS}" CACHE STRING "" FORCE)

# Source list generation :

file(GLOB_RECURSE source_files src/*)


# Executable declaration

add_executable(Quadcopter_${CMAKE_BUILD_TYPE} ${source_files})


# Libraries includes and executable configuration


#	Pthread

include_directories(${PTHREAD_INCLUDE_DIRS})
link_directories(${PTHREAD_LIBRARY_DIRS})
target_link_libraries(Quadcopter_${CMAKE_BUILD_TYPE} ${PTHREAD_NAME})


#	Wiring Pi I2C

include_directories(${WIRING_PI_I2C_INCLUDE_DIRS})
link_directories(${WIRING_PI_I2C_LIBRARY_DIRS})
target_link_libraries(Quadcopter_${CMAKE_BUILD_TYPE} ${WIRING_PI_I2C_NAME})

#	Proj

include_directories(${PROJ_INCLUDE_DIRS})
link_directories(${PROJ_LIBRARY_DIRS})
target_link_libraries(Quadcopter_${CMAKE_BUILD_TYPE} ${PROJ_NAME})
