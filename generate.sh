# !/bin/bash

path_to_save_files="Service"

tool_wsdl2h="gsoap/bin/wsdl2h"
tool_soapcpp2="gsoap/bin/soapcpp2"

wsdl2h_op="-c++11"
soapcpp2_op="-c++11 -j -S -x"

wsdl_list=(
https://schemas.xmlsoap.org/ws/2005/04/discovery/ws-discovery.wsdl
https://www.onvif.org/ver10/device/wsdl/devicemgmt.wsdl
https://www.onvif.org/ver10/events/wsdl/event.wsdl
https://www.onvif.org/ver10/media/wsdl/media.wsdl
https://www.onvif.org/ver20/ptz/wsdl/ptz.wsdl
https://www.onvif.org/ver10/recording.wsdl
https://www.onvif.org/ver10/search.wsdl
https://www.onvif.org/ver10/replay.wsdl
https://www.onvif.org/ver20/media/wsdl/media.wsdl
https://www.onvif.org/ver20/analytics/wsdl/analytics.wsdl
https://www.onvif.org/ver20/imaging/wsdl/imaging.wsdl
https://www.onvif.org/ver10/deviceio.wsdl
)

#for wsdl in "${wsdl_list[@]}"; do
#	service=$(basename ${wsdl} .wsdl)
#	if [[ ${service} == "ws-discovery" ]]; then
#		service="wsdd"
#	fi
#	if [[ ${service} == "media" && ${wsdl} == *"ver20"* ]]; then
#		service+="2"
#	fi
#	${tool_wsdl2h} -o ${service}.h ${wsdl2h_op} -q ${service} ${wsdl}
#	if [ -f ${service}.h ]; then
#		mkdir -p ${path_to_save_files}/${service}
#		rm -rf ${path_to_save_files}/${service}/*
#		${tool_soapcpp2} ${soapcpp2_op} -d ${path_to_save_files}/${service} -p ${service} ${service}.h
#		rm ${service}.h
#	fi
#done

#path_import="share/gsoap/import"
#wsdd="wsdd"
#mkdir -p ${path_to_save_files}/${wsdd}
#rm -rf ${path_to_save_files}/${wsdd}/*
#${tool_soapcpp2} ${soapcpp2_op} -a -I${path_import} -d ${path_to_save_files}/${wsdd} ${path_import}/${wsdd}.h

${tool_wsdl2h} -o onvif.h ${wsdl2h_op} ${wsdl_list[@]}
if [ -f onvif.h ]; then
	mkdir -p ${path_to_save_files}
	rm -rf ${path_to_save_files}/*
	${tool_soapcpp2} ${soapcpp2_op} -d ${path_to_save_files} onvif.h
	rm onvif.h
fi
