cmake_minimum_required (VERSION 2.8)

project (ahocorasick)
set (ahocorasick_VERSION_MAJOR 0)
set (ahocorasick_VERSION_MINOR 1)

# Choose Release or Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    #set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_C_FLAGS_RELEASE "-Wall -std=c99 -O2")
set(CMAKE_C_FLAGS_DEBUG "-Wall -std=c99 -g")

# Set source and header files
file(GLOB SOURCES "src/*.c")
include_directories(include)

add_executable(ahocorasick ${SOURCES})
