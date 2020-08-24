cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project (unittest)

#include_directories(/usr/include/python3.5m)

file(GLOB SOURCES "src/*.cpp")
file(GLOB SOURCES_main "./*.cpp")
file(GLOB DMLC_PATH "3rdparty/dmlc-core/include")
file(GLOB DLPACK_PATH "3rdparty/dlpack/include")
file(GLOB INCLUDE_DIR "include/")

include_directories(include)
include_directories(${INCLUDE_DIR})
include_directories(${DMLC_PATH})
include_directories(${DLPACK_PATH})

#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    #message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

#add_compile_options(-std=c++11)
#查找当前目录下的所有源文件
#并将名称保存到DIR_SRCS变量中
aux_source_directory(. DIR_SRCS)
add_executable(unittest  ${SOURCES_main})
#link libray in static mode
target_link_libraries(unittest)


