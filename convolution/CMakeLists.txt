CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(convolution)

#need to include FindOPENCL.cmake to
SET( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)

message("CMake module path: ${CMAKE_MODULE_PATH}\n")

FIND_PACKAGE (OPENCL)
FIND_PACKAGE (JPEG)
FIND_PACKAGE (OpenCV REQUIRED)

SET( COMMON_PATH ${CMAKE_SOURCE_DIR}/../common)

INCLUDE_DIRECTORIES( 
    ${CMAKE_SOURCE_DIR}/../common
)

message("include directories: \n")
get_directory_property(includes INCLUDE_DIRECTORIES)
message("${includes}\n")

#provide the source path so we can open our .cl file at runtime
#ADD_DEFINITIONS(-DCL_SOURCE_DIR="${part1_SOURCE_DIR}")
#build in debug mode
ADD_DEFINITIONS(-g )

# #the source files we want to compile into the library
set (LIBCXXFILES
    ${COMMON_PATH}/utils.cpp
    ${COMMON_PATH}/utils.hpp
    ${COMMON_PATH}/jpeg_loader.cpp
    ${COMMON_PATH}/jpeg_loader.hpp)

#set a CMake variable to name the library
SET(cllib convolution)
ADD_LIBRARY(${cllib} ${LIBCXXFILES})


#create the executable
SET(EXEC a.out) 
ADD_EXECUTABLE(${EXEC} main.cpp)

# utils.hpp/cpp rely on the OPENCVLIBRARIES 
TARGET_LINK_LIBRARIES (
   ${cllib}
   ${OpenCV_LIBRARIES}
)

TARGET_LINK_LIBRARIES (${EXEC}
   ${OPENCL_LIBRARIES}
   ${JPEG_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${cllib}
)
