# Install script for directory: /home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Utilities

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmakecommands.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmakecompat.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmakeprops.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmakepolicies.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmakevars.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmakemodules.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/ctest.1"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cpack.1"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/doc/cmake-2.8" TYPE FILE FILES
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-policies.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-properties.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-variables.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-modules.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-commands.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-compatcommands.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/ctest.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cpack.html"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake.docbook"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-policies.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-properties.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-variables.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-modules.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-commands.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cmake-compatcommands.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/ctest.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/ctest.docbook"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cpack.txt"
    "/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Docs/cpack.docbook"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Utilities/Doxygen/cmake_install.cmake")
  INCLUDE("/home/nikita/Scripting/quadcopter/Quadcopter/cmake-2.8.3/Utilities/KWStyle/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

