# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/guan/文档/Manager

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/guan/文档/Manager/build

# Include any dependencies generated for this target.
include CMakeFiles/manager.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/manager.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/manager.dir/flags.make

CMakeFiles/manager.dir/main.cpp.o: CMakeFiles/manager.dir/flags.make
CMakeFiles/manager.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guan/文档/Manager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/manager.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/manager.dir/main.cpp.o -c /home/guan/文档/Manager/main.cpp

CMakeFiles/manager.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/manager.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guan/文档/Manager/main.cpp > CMakeFiles/manager.dir/main.cpp.i

CMakeFiles/manager.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/manager.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guan/文档/Manager/main.cpp -o CMakeFiles/manager.dir/main.cpp.s

CMakeFiles/manager.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/manager.dir/main.cpp.o.requires

CMakeFiles/manager.dir/main.cpp.o.provides: CMakeFiles/manager.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/manager.dir/build.make CMakeFiles/manager.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/manager.dir/main.cpp.o.provides

CMakeFiles/manager.dir/main.cpp.o.provides.build: CMakeFiles/manager.dir/main.cpp.o


# Object files for target manager
manager_OBJECTS = \
"CMakeFiles/manager.dir/main.cpp.o"

# External object files for target manager
manager_EXTERNAL_OBJECTS =

manager: CMakeFiles/manager.dir/main.cpp.o
manager: CMakeFiles/manager.dir/build.make
manager: /usr/local/lib/libboost_system.so
manager: /usr/local/lib/libboost_filesystem.so
manager: /usr/local/lib/libboost_date_time.so
manager: /usr/local/lib/libboost_iostreams.so
manager: /usr/local/lib/libboost_serialization.so
manager: /usr/local/lib/libboost_regex.so
manager: /usr/lib/x86_64-linux-gnu/libqhull.so
manager: /usr/lib/libOpenNI.so
manager: /usr/lib/libOpenNI2.so
manager: /usr/lib/x86_64-linux-gnu/libfreetype.so
manager: /usr/lib/x86_64-linux-gnu/libz.so
manager: /usr/lib/x86_64-linux-gnu/libjpeg.so
manager: /usr/lib/x86_64-linux-gnu/libpng.so
manager: /usr/lib/x86_64-linux-gnu/libtiff.so
manager: /usr/local/lib/libboost_system.so
manager: /usr/local/lib/libboost_thread.so
manager: /usr/local/lib/libboost_chrono.so
manager: /usr/local/lib/libboost_date_time.so
manager: /usr/local/lib/libboost_atomic.so
manager: /usr/lib/x86_64-linux-gnu/libpthread.so
manager: Connect/libConnect.a
manager: /usr/lib/libpcl_segmentation.so
manager: /usr/lib/libpcl_surface.so
manager: /usr/lib/libpcl_keypoints.so
manager: /usr/lib/libpcl_tracking.so
manager: /usr/lib/libpcl_recognition.so
manager: /usr/lib/libpcl_ml.so
manager: /usr/lib/libpcl_registration.so
manager: /usr/lib/libpcl_features.so
manager: /usr/lib/libpcl_filters.so
manager: /usr/lib/libpcl_sample_consensus.so
manager: /usr/lib/libpcl_search.so
manager: /usr/lib/libpcl_kdtree.so
manager: /usr/lib/libpcl_stereo.so
manager: /usr/lib/libpcl_io.so
manager: /usr/lib/libpcl_octree.so
manager: /usr/lib/libpcl_common.so
manager: /usr/lib/x86_64-linux-gnu/libvtkChartsCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkInfovisCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkIOLegacy-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkIOPLY-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkIOGeometry-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkRenderingLOD-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkViewsContext2D-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkRenderingContext2D-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkViewsCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkInteractionWidgets-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersModeling-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkInteractionStyle-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersHybrid-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkImagingGeneral-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkImagingSources-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkImagingHybrid-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkIOImage-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkDICOMParser-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkIOCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkmetaio-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkRenderingAnnotation-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkRenderingFreeType-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkftgl-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libGL.so
manager: /usr/lib/x86_64-linux-gnu/libvtkImagingColor-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkRenderingVolume-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkRenderingCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonColor-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersExtraction-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersStatistics-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkImagingFourier-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkImagingCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkalglib-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersGeometry-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersSources-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersGeneral-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkFiltersCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonExecutionModel-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonComputationalGeometry-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonDataModel-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonMisc-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonTransforms-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonMath-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonSystem-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtkCommonCore-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libvtksys-6.3.so.6.3.0
manager: /usr/lib/x86_64-linux-gnu/libflann_cpp.so
manager: /usr/local/lib/libopencv_dnn.so.3.4.7
manager: /usr/local/lib/libopencv_highgui.so.3.4.7
manager: /usr/local/lib/libopencv_ml.so.3.4.7
manager: /usr/local/lib/libopencv_objdetect.so.3.4.7
manager: /usr/local/lib/libopencv_shape.so.3.4.7
manager: /usr/local/lib/libopencv_stitching.so.3.4.7
manager: /usr/local/lib/libopencv_superres.so.3.4.7
manager: /usr/local/lib/libopencv_videostab.so.3.4.7
manager: /usr/local/lib/libopencv_calib3d.so.3.4.7
manager: /usr/local/lib/libopencv_features2d.so.3.4.7
manager: /usr/local/lib/libopencv_flann.so.3.4.7
manager: /usr/local/lib/libopencv_photo.so.3.4.7
manager: /usr/local/lib/libopencv_video.so.3.4.7
manager: /usr/local/lib/libopencv_videoio.so.3.4.7
manager: /usr/local/lib/libopencv_imgcodecs.so.3.4.7
manager: /usr/local/lib/libopencv_imgproc.so.3.4.7
manager: /usr/local/lib/libopencv_core.so.3.4.7
manager: /usr/local/lib/libboost_system.so
manager: /usr/local/lib/libboost_filesystem.so
manager: /usr/local/lib/libboost_date_time.so
manager: /usr/local/lib/libboost_iostreams.so
manager: /usr/local/lib/libboost_serialization.so
manager: /usr/local/lib/libboost_regex.so
manager: /usr/lib/x86_64-linux-gnu/libqhull.so
manager: /usr/lib/libOpenNI.so
manager: /usr/lib/libOpenNI2.so
manager: /usr/lib/x86_64-linux-gnu/libjpeg.so
manager: /usr/lib/x86_64-linux-gnu/libpng.so
manager: /usr/lib/x86_64-linux-gnu/libtiff.so
manager: /usr/local/lib/libboost_system.so
manager: /usr/local/lib/libboost_filesystem.so
manager: /usr/local/lib/libboost_date_time.so
manager: /usr/local/lib/libboost_iostreams.so
manager: /usr/local/lib/libboost_serialization.so
manager: /usr/local/lib/libboost_regex.so
manager: /usr/lib/x86_64-linux-gnu/libqhull.so
manager: /usr/lib/libOpenNI.so
manager: /usr/lib/libOpenNI2.so
manager: /usr/lib/x86_64-linux-gnu/libjpeg.so
manager: /usr/lib/x86_64-linux-gnu/libpng.so
manager: /usr/lib/x86_64-linux-gnu/libtiff.so
manager: /usr/lib/x86_64-linux-gnu/libfreetype.so
manager: /usr/lib/x86_64-linux-gnu/libz.so
manager: Connect/libEncodeTool.a
manager: CMakeFiles/manager.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/guan/文档/Manager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable manager"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/manager.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/manager.dir/build: manager

.PHONY : CMakeFiles/manager.dir/build

CMakeFiles/manager.dir/requires: CMakeFiles/manager.dir/main.cpp.o.requires

.PHONY : CMakeFiles/manager.dir/requires

CMakeFiles/manager.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/manager.dir/cmake_clean.cmake
.PHONY : CMakeFiles/manager.dir/clean

CMakeFiles/manager.dir/depend:
	cd /home/guan/文档/Manager/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/guan/文档/Manager /home/guan/文档/Manager /home/guan/文档/Manager/build /home/guan/文档/Manager/build /home/guan/文档/Manager/build/CMakeFiles/manager.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/manager.dir/depend

