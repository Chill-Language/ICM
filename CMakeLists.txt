cmake_minimum_required(VERSION 3.0)
project(ICM)

include(cotire.cmake)

set(PRILIB Common)
add_definitions(-DPRILIB_NAME=Common)
add_subdirectory(prilib prilib)

include_directories(include)
aux_source_directory(source SOURCE_FILES)
aux_source_directory(source/parser PARSER_SOURCE_FILES)
aux_source_directory(source/runtime RUNTIME_SOURCE_FILES)
aux_source_directory(source/compiler COMPILER_SOURCE_FILES)

add_executable(icm ${SOURCE_FILES} ${PARSER_SOURCE_FILES} ${RUNTIME_SOURCE_FILES} ${COMPILER_SOURCE_FILES})
set_property(TARGET icm PROPERTY CXX_STANDARD 14)

target_link_libraries(icm prilib)

set_target_properties(icm PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "include/basic.h")
cotire(icm)
