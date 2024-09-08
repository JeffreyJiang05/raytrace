cmake_minimum_required(VERSION 3.5)

project(gpu_raytrace VERSION 0.1
                     DESCRIPTION "GPU-accelerated raytracing library"
                     LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES native)
set(CMAKE_CUDA_FLAGS "--expt-relaxed-constexpr")

include_directories(include)

find_package(FFMPEG REQUIRED)
link_directories(${FFMPEG_LIBRARY_DIRS})
include_directories(${FFMPEG_INCLUDE_DIRS})
link_libraries(${FFMPEG_LIBRARIES})

find_package(fmt REQUIRED)
link_libraries(fmt::fmt)

find_package(GTest CONFIG REQUIRED)
# link_libraries(GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_executable(ffmpeg_video_test
        src/prog/encode_video.cpp)

add_executable(ffmpeg_encap
        src/prog/video_builder_test.cpp
        include/media/video_builder.hpp
        include/media/constants.hpp
        src/media/video_builder.cpp
        include/base/image.hpp
        src/base/image.cpp
        include/math/sampling.hpp
        include/math/floats.hpp
        include/math/impl/floats.inl
        include/math/impl/sampling.inl
        include/math/linalg/vec.hpp
        include/math/linalg/impl/swizzle_vec.inl
        include/math/linalg/impl/vec_base.inl
        include/math/linalg/impl/vec.inl
        include/math/linalg/fmt.hpp
        include/math/linalg/impl/vec_func.inl
        include/math/functions.hpp)

add_executable(float_test
        src/test/float_test.cpp
        include/math/floats.hpp
        src/test/math_functions_test.cpp)
target_link_libraries(float_test GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_executable(vec_test
        src/test/vec_test.cpp
        include/math/sampling.hpp
        include/math/floats.hpp
        include/math/impl/floats.inl
        include/math/impl/sampling.inl
        include/math/linalg/vec.hpp
        include/math/linalg/impl/swizzle_vec.inl
        include/math/linalg/impl/vec_base.inl
        include/math/linalg/impl/vec.inl
        include/math/linalg/impl/vec_func.inl
        include/math/linalg/fmt.hpp
        include/math/functions.hpp
        include/math/impl/functions.inl)
target_link_libraries(vec_test GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_executable(cuda_test src/prog/cuda_test.cu
        include/math/sampling.hpp
        include/math/floats.hpp
        include/math/impl/floats.inl
        include/math/impl/sampling.inl
        include/math/linalg/vec.hpp
        include/math/linalg/impl/swizzle_vec.inl
        include/math/linalg/impl/vec_base.inl
        include/math/linalg/impl/vec.inl
        include/math/linalg/impl/vec_func.inl
        include/math/linalg/fmt.hpp
        include/math/functions.hpp
        include/math/impl/functions.inl
        include/gpu/gpu.hpp)
