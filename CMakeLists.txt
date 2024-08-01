cmake_minimum_required(VERSION 3.2)

project(gpu_raytrace VERSION 0.1
                     DESCRIPTION "GPU-accelerated raytracing library"
                     LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_FLAGS "-arch=sm_86")

include_directories(include)

find_package(FFMPEG REQUIRED)
link_directories(${FFMPEG_LIBRARY_DIRS})
include_directories(${FFMPEG_INCLUDE_DIRS})
link_libraries(${FFMPEG_LIBRARIES})

find_package(fmt REQUIRED)
link_libraries(fmt::fmt)

add_executable(ffmpeg_video_test
        src/prog/encode_video.cpp)

add_executable(ffmpeg_encap
        src/prog/video_builder_test.cpp
        include/media/video_builder.hpp
        include/media/constants.hpp
        src/media/video_builder.cpp
        include/base/image.hpp
        src/base/image.cpp)