cmake_minimum_required(VERSION 2.8)
project( quadcopter )
SET(COMPILE_DEFINITIONS -Werror)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
find_package( OpenCV REQUIRED )
#include_directories(/opt/vc/userland/host_applications/linux/libs/bcm_host/include)
#include_directories(/opt/vc/userland/interface/vcos)
#include_directories(/opt/vc/userland)
#include_directories(/opt/vc/userland/interface/vcos/pthreads)
#include_directories(/opt/vc/userland/interface/vmcs_host/linux)
#include_directories(
  #  /opt/vc/include
    #/opt/vc/include/interface/vcos/pthreads
    #/opt/vc/include/interface/vmcs_host/linux
    #${CMAKE_SOURCE_DIR}/libs/glm/
    #${CMAKE_SOURCE_DIR}/libs/ilclient
    #${CMAKE_SOURCE_DIR}/common
#)


#link_directories(
 #   ${CMAKE_SOURCE_DIR}/libs/ilclient
 #   /opt/vc/lib
)#

#set(GL_LIBS
# openGLES
#   GLESv2
#    EGL
#)

#set(ILC_LIBS
# ilclient
#    ilclient
#    openmaxil
#)


# common library
add_subdirectory(RTIMULib)
# subdirectories
#add_subdirectory(affichage_cam)
#add_executable()
target_link_libraries(quadcopter) #${OpenCV_LIBS} ${GL_LIBS} ${ILC_LIBS}

