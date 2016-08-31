cmake_minimum_required(VERSION 3.0)
project(ICM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories(include)
include_directories(include/common)
aux_source_directory(source SOURCE_FILES)
aux_source_directory(source/common COMMON_SOURCE_FILES)
add_executable(icm ${SOURCE_FILES} ${COMMON_SOURCE_FILES})
