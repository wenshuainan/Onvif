cmake_minimum_required(VERSION 3.10)
project(OnvifTest)

include_directories(..)
link_directories(../Build)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -ffunction-sections -fdata-sections -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -ffunction-sections -fdata-sections -fsanitize=address -fno-omit-frame-pointer")

add_executable(OnvifTest test.cpp)

set_target_properties(OnvifTest PROPERTIES LINK_FLAGS "-Wl,--gc-sections -fsanitize=address")

target_link_libraries(OnvifTest onvif crypto ssl)