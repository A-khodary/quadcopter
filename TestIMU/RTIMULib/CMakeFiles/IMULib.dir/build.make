# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib

# Include any dependencies generated for this target.
include CMakeFiles/IMULib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/IMULib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/IMULib.dir/flags.make

CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o: RTFusionKalman4.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusionKalman4.cpp

CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusionKalman4.cpp > CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.i

CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusionKalman4.cpp -o CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.s

CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.requires

CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.provides: CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.provides

CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o: RTFusionRTQF.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusionRTQF.cpp

CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusionRTQF.cpp > CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.i

CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusionRTQF.cpp -o CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.s

CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.requires

CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.provides: CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.provides

CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTFusion.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTFusion.cpp.o: RTFusion.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTFusion.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTFusion.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusion.cpp

CMakeFiles/IMULib.dir/RTFusion.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTFusion.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusion.cpp > CMakeFiles/IMULib.dir/RTFusion.cpp.i

CMakeFiles/IMULib.dir/RTFusion.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTFusion.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTFusion.cpp -o CMakeFiles/IMULib.dir/RTFusion.cpp.s

CMakeFiles/IMULib.dir/RTFusion.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTFusion.cpp.o.requires

CMakeFiles/IMULib.dir/RTFusion.cpp.o.provides: CMakeFiles/IMULib.dir/RTFusion.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTFusion.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTFusion.cpp.o.provides

CMakeFiles/IMULib.dir/RTFusion.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTFusion.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTFusion.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTIMU.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTIMU.cpp.o: RTIMU.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTIMU.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTIMU.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMU.cpp

CMakeFiles/IMULib.dir/RTIMU.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTIMU.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMU.cpp > CMakeFiles/IMULib.dir/RTIMU.cpp.i

CMakeFiles/IMULib.dir/RTIMU.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTIMU.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMU.cpp -o CMakeFiles/IMULib.dir/RTIMU.cpp.s

CMakeFiles/IMULib.dir/RTIMU.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTIMU.cpp.o.requires

CMakeFiles/IMULib.dir/RTIMU.cpp.o.provides: CMakeFiles/IMULib.dir/RTIMU.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTIMU.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTIMU.cpp.o.provides

CMakeFiles/IMULib.dir/RTIMU.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTIMU.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTIMU.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTIMUHal.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTIMUHal.cpp.o: RTIMUHal.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTIMUHal.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTIMUHal.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUHal.cpp

CMakeFiles/IMULib.dir/RTIMUHal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTIMUHal.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUHal.cpp > CMakeFiles/IMULib.dir/RTIMUHal.cpp.i

CMakeFiles/IMULib.dir/RTIMUHal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTIMUHal.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUHal.cpp -o CMakeFiles/IMULib.dir/RTIMUHal.cpp.s

CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.requires

CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.provides: CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.provides

CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTIMUHal.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o: RTIMUMPU6050HMC5883L.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUMPU6050HMC5883L.cpp

CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUMPU6050HMC5883L.cpp > CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.i

CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUMPU6050HMC5883L.cpp -o CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.s

CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.requires

CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.provides: CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.provides

CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTIMUNull.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTIMUNull.cpp.o: RTIMUNull.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTIMUNull.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTIMUNull.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUNull.cpp

CMakeFiles/IMULib.dir/RTIMUNull.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTIMUNull.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUNull.cpp > CMakeFiles/IMULib.dir/RTIMUNull.cpp.i

CMakeFiles/IMULib.dir/RTIMUNull.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTIMUNull.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUNull.cpp -o CMakeFiles/IMULib.dir/RTIMUNull.cpp.s

CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.requires

CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.provides: CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.provides

CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTIMUNull.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o: RTIMUSettings.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUSettings.cpp

CMakeFiles/IMULib.dir/RTIMUSettings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTIMUSettings.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUSettings.cpp > CMakeFiles/IMULib.dir/RTIMUSettings.cpp.i

CMakeFiles/IMULib.dir/RTIMUSettings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTIMUSettings.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTIMUSettings.cpp -o CMakeFiles/IMULib.dir/RTIMUSettings.cpp.s

CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.requires

CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.provides: CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.provides

CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.provides.build

CMakeFiles/IMULib.dir/RTMath.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/RTMath.cpp.o: RTMath.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/RTMath.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/RTMath.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTMath.cpp

CMakeFiles/IMULib.dir/RTMath.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/RTMath.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTMath.cpp > CMakeFiles/IMULib.dir/RTMath.cpp.i

CMakeFiles/IMULib.dir/RTMath.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/RTMath.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/RTMath.cpp -o CMakeFiles/IMULib.dir/RTMath.cpp.s

CMakeFiles/IMULib.dir/RTMath.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/RTMath.cpp.o.requires

CMakeFiles/IMULib.dir/RTMath.cpp.o.provides: CMakeFiles/IMULib.dir/RTMath.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/RTMath.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/RTMath.cpp.o.provides

CMakeFiles/IMULib.dir/RTMath.cpp.o.provides.build: CMakeFiles/IMULib.dir/RTMath.cpp.o
.PHONY : CMakeFiles/IMULib.dir/RTMath.cpp.o.provides.build

CMakeFiles/IMULib.dir/main.cpp.o: CMakeFiles/IMULib.dir/flags.make
CMakeFiles/IMULib.dir/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/IMULib.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/IMULib.dir/main.cpp.o -c /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/main.cpp

CMakeFiles/IMULib.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IMULib.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/main.cpp > CMakeFiles/IMULib.dir/main.cpp.i

CMakeFiles/IMULib.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IMULib.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/main.cpp -o CMakeFiles/IMULib.dir/main.cpp.s

CMakeFiles/IMULib.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/IMULib.dir/main.cpp.o.requires

CMakeFiles/IMULib.dir/main.cpp.o.provides: CMakeFiles/IMULib.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/IMULib.dir/build.make CMakeFiles/IMULib.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/IMULib.dir/main.cpp.o.provides

CMakeFiles/IMULib.dir/main.cpp.o.provides.build: CMakeFiles/IMULib.dir/main.cpp.o
.PHONY : CMakeFiles/IMULib.dir/main.cpp.o.provides.build

# Object files for target IMULib
IMULib_OBJECTS = \
"CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o" \
"CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o" \
"CMakeFiles/IMULib.dir/RTFusion.cpp.o" \
"CMakeFiles/IMULib.dir/RTIMU.cpp.o" \
"CMakeFiles/IMULib.dir/RTIMUHal.cpp.o" \
"CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o" \
"CMakeFiles/IMULib.dir/RTIMUNull.cpp.o" \
"CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o" \
"CMakeFiles/IMULib.dir/RTMath.cpp.o" \
"CMakeFiles/IMULib.dir/main.cpp.o"

# External object files for target IMULib
IMULib_EXTERNAL_OBJECTS =

libIMULib.a: CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTFusion.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTIMU.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTIMUHal.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTIMUNull.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/RTMath.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/main.cpp.o
libIMULib.a: CMakeFiles/IMULib.dir/build.make
libIMULib.a: CMakeFiles/IMULib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libIMULib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/IMULib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/IMULib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/IMULib.dir/build: libIMULib.a
.PHONY : CMakeFiles/IMULib.dir/build

CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTFusionKalman4.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTFusionRTQF.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTFusion.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTIMU.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTIMUHal.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTIMUMPU6050HMC5883L.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTIMUNull.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTIMUSettings.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/RTMath.cpp.o.requires
CMakeFiles/IMULib.dir/requires: CMakeFiles/IMULib.dir/main.cpp.o.requires
.PHONY : CMakeFiles/IMULib.dir/requires

CMakeFiles/IMULib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/IMULib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/IMULib.dir/clean

CMakeFiles/IMULib.dir/depend:
	cd /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib /home/nikita/Scripting/quadcopter/TestIMU/RTIMULib/CMakeFiles/IMULib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/IMULib.dir/depend

