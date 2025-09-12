cmake_minimum_required(VERSION 3.10)
project(onvif)

include_directories(.)
include_directories(gSOAP/ToolKit/include)
include_directories(gSOAP/ToolKit/share/gsoap/plugin)
include_directories(gSOAP/Generated/Server)

#gSOAP engine
file(GLOB Src_gSOAP
    gSOAP/Resource/gsoap-2.8/gsoap/dom.cpp
    gSOAP/Resource/gsoap-2.8/gsoap/stdsoap2.cpp
)

#wsdd plugin
file(GLOB Src_wsdd
    gSOAP/ToolKit/share/gsoap/plugin/wsddapi.c
    gSOAP/ToolKit/share/gsoap/plugin/wsaapi.c
    gSOAP/Generated/Client/wsddClient.cpp
)

#http digest authentication plugin
file(GLOB Src_httpda
    gSOAP/ToolKit/share/gsoap/plugin/httpda.c
    gSOAP/ToolKit/share/gsoap/plugin/smdevp.c
    gSOAP/ToolKit/share/gsoap/plugin/threads.c
)

#wsse plugin
file(GLOB Src_wsse
    gSOAP/ToolKit/share/gsoap/plugin/wsseapi.c
    gSOAP/ToolKit/share/gsoap/plugin/mecevp.c
    gSOAP/ToolKit/share/gsoap/custom/struct_timeval.c
)

#gSOAP生成的Server代码
file(GLOB_RECURSE Src_Server gSOAP/Generated/Server/*.cpp)

#自定义代码
aux_source_directory(Service Src_Service)
aux_source_directory(Device Src_Device)
aux_source_directory(. Src_Onvif)

#包含了C++头文件，需要设置语言类型
set_source_files_properties(${Src_wsdd} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${Src_httpda} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${Src_wsse} PROPERTIES LANGUAGE CXX)

#-DWITH_DEFAULT_VIRTUAL：默认开启gSOAP的虚拟继承，否则需要手动添加
#-DWITH_OPENSSL：开启gSOAP的SSL支持
#-DWITH_DOM：开启gSOAP的DOM支持
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -DWITH_DEFAULT_VIRTUAL \
    -DWITH_OPENSSL \
    -DWITH_DOM \
    -DWITH_NO_C_LOCALE \
    -ffunction-sections -fdata-sections"
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -DWITH_DEFAULT_VIRTUAL \
    -DWITH_OPENSSL \
    -DWITH_DOM \
    -DWITH_NO_C_LOCALE \
    -std=c++11 \
    -ffunction-sections -fdata-sections"
)

#自定义代码添加调试信息以及版本信息
set_source_files_properties(${Src_Service} ${Src_Device} ${Src_Onvif} PROPERTIES COMPILE_FLAGS
    "-DONVIF_CORE_SPECIFICATION_VERSION=\\\"25.06\\\" \
    -DMEDIA_SERVICE_SPECIFICATION_VERSION=\\\"24.12\\\" \
    -DMEDIA2_SERVICE_SPECIFICATION_VERSION=\\\"25.06\\\" \
    -DPTZ_SERVICE_SPECIFICATION_VERSION=\\\"23.06\\\" \
    -g \
    -Wall \
    -fsanitize=address -fno-omit-frame-pointer"
)

add_library(onvif STATIC
    ${Src_gSOAP}
    ${Src_wsdd}
    ${Src_httpda}
    ${Src_wsse}
    ${Src_Server}
    ${Src_Service}
    ${Src_Device}
    ${Src_Onvif}
)
