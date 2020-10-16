cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project (witin_excu)
project(Installation VERSION 0.1)


file(GLOB SOURCES "src/node/*.cpp"
	"src/graph/*.cpp"
	"src/tensor/*.cpp"
	"src/target/*.cpp"
	"src/session/*.cpp"
	"src/sim/*.cpp"
	"src/utils/*.cpp"
	)
file(GLOB SOURCES_BRANCH "src/branch.cpp")

file(GLOB DMLC_PATH "3rdparty/dmlc-core/include")
file(GLOB DLPACK_PATH "3rdparty/dlpack/include")
file(GLOB INCLUDE_DIR "include/")

include_directories(include)
include_directories(${INCLUDE_DIR})
include_directories(${DMLC_PATH})
include_directories(${DLPACK_PATH})

macro(witin_add_example name)
	add_executable(${name} src/${name}.cpp ${SOURCES})
	target_include_directories(${name} PRIVATE ${INCLUDE_DIR})
	target_include_directories(${name} PRIVATE ${DMLC_PATH})
	target_include_directories(${name} PRIVATE ${DLPACK_PATH})
	target_link_libraries(${name} PRIVATE jsoncpp)
endmacro()

include(cmake/modules/Findlibdb_cxx.cmake)
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

add_compile_options(-ljsoncpp)
#add_compile_options(-std=c++11)
#查找当前目录下的所有源文件
#并将名称保存到DIR_SRCS变量中
aux_source_directory(. DIR_SRCS)
#为源文件的编译添加选项
#ADD_DEFINITIONS(-g -W -Wall -o2 )
ADD_DEFINITIONS(-g)
#link_directories(./lib)
message(STATUS "add exectable")

add_definitions(-DDMLC_LOG_DEBUG=1)
#set(CMAKE_SOURCE_DIR ..)
set(CMAKE_MODULE_PATH ${CMAKE_ROOT}/Modules  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
find_package(libdb_cxx REQUIRED)
#mark_as_advanced(
#    LIBDB_CXX_INCLUDE_DIR
#    LIBDB_CXX_LIBRARIES
#)
if(LIBDB_CXX_INCLUDE_DIR AND LIBDB_CXX_LIBRARIES)
    message(STATUS "Found libdb libraries and include")
endif()
if(LIBDB_CXX_FOUND)
#if(libdb_cxx_FOUND)
    message(STATUS "Found libdb libraries and include")
endif()
#得出可执行文件
#add_executable(witin_excu  ${SOURCES} "src/main.cpp")
#add_executable(branch_test  ${SOURCES_BRANCH})
#link libray in static mode
#target_link_libraries(witin_excu jsoncpp)
#target_link_libraries(branch_test jsoncpp)

#target_include_directories(branch_test PRIVATE ${include})

witin_add_example(main)
witin_add_example(branch)

add_library(witin SHARED ${SOURCES})
target_link_libraries(witin jsoncpp)
#SET_TARGET_PROPERTIES(witin PROPERTIES VERSION 1.0 SOVERSION 1)

#add_subdirectory(tests)

install(TARGETS witin
		EXPORT WitinTargets
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		PUBLIC_HEADER DESTINATION include
		)
#declare version info
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
		WitinConfigVersion.cmake
		VERSION ${PACKAGE_VERSION}
		COMPATIBILITY AnyNewerVersion
		)
#write WitinTargets info to WitinLibTargets.cmake
install(EXPORT WitinTargets
		FILE WitinLibTargets.cmake
		DESTINATION lib/cmake/WintinLib
		)

configure_file(./cmake/witinConfig.cmake.in witinConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/witinConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/WitinVersion.cmake"
		DESTINATION lib/cmake/
		)

