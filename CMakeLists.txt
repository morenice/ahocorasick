cmake_minimum_required (VERSION 3.0)

project (ahocorasick)
set (ahocorasick_VERSION_MAJOR 0)
set (ahocorasick_VERSION_MINOR 1)

# Choose Release or Debug
if(NOT CMAKE_BUILD_TYPE)
    #set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

if( CMAKE_BUILD_TYPE EQUAL Release)
    set(CMAKE_C_FLAGS_RELEASE "-Wl -O2")
endif( CMAKE_BUILD_TYPE EQUAL Release)

if( CMAKE_BUILD_TYPE EQUAL Debug)
    set(CMAKE_C_FLAGS_RELEASE "-Wl --ggdb")
endif( CMAKE_BUILD_TYPE EQUAL Debug)

# Set source and header files
file(GLOB SOURCES "src/*.c")
include_directories(include)

add_executable(ahocorasick ${SOURCES})
