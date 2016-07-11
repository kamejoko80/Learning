cmake_minimum_required(VERSION 3.2)
project(project_name)

#find_package(OpenCV_REQUIRED)
set(OpenCV_BASE_DIRS /path_to/opencv/)
set(OpenCV_INCLUDE_DIRS ${OpenCV_BASE_DIRS}/include)
set(OpenCV_LIBRARY_DIRS ${OpenCV_BASE_DIRS}/lib)
set(OpenCV_LIBS opencv_videostab opencv_video opencv_ts opencv_superres opencv_stitching opencv_photo opencv_ocl
        opencv_objdetect opencv_ml opencv_legacy opencv_imgproc opencv_highgui opencv_gpu opencv_flann
        opencv_features2d opencv_core opencv_contrib opencv_calib3d)

link_directories(${OpenCV_LIBRARY_DIRS} ${CMAKE_SOURCE_DIR}/lib /usr/local/lib)
set(LIBS ${LIBS}  gflags pthread glog leveldb snappy lmdb hdf5_hl hdf5 m openblas)
set(LIBS ${LIBS} ${OpenCV_LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -MMD -MP -DNDEBUG")

add_subdirectory(your_subdir)
