cmake_minimum_required(VERSION 3.10)
project(onvif)

include_directories(./gsoap/include)
include_directories(.)

file(GLOB Src_gSOAP resource/gsoap-2.8/gsoap/dom.cpp resource/gsoap-2.8/gsoap/stdsoap2.cpp)
file(GLOB_RECURSE Src_Service Service/*.cpp)
aux_source_directory(Device Src_Device)
aux_source_directory(. Src_Onvif)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT -DWITH_DEFAULT_VIRTUAL -Wall -ffunction-sections -fdata-sections -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_REENTRANT -DWITH_DEFAULT_VIRTUAL -Wall -ffunction-sections -fdata-sections -fsanitize=address -fno-omit-frame-pointer")

add_library(onvif STATIC ${Src_gSOAP} ${Src_Service} ${Src_Device} ${Src_Onvif})