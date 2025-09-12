# !/bin/bash

gSOAP_path=`pwd`
toolkit_path=${gSOAP_path}/ToolKit

cd Resource
rm -rf gsoap-2.8
unzip gsoap_2.8.139.zip

#编译出服务器上运行的wsdl2h和soapcpp2
cd gsoap-2.8
rm -rf ${toolkit_path}/*
./configure --prefix=${toolkit_path} #prefix指定输出目录
make
make install

#工具编译成功后，修改两个文件
sed -i '578a tr2		= "http://www.onvif.org/ver20/media/wsdl"' ${toolkit_path}/share/gsoap/WS/typemap.dat #插入media2命名空间定义
sed -i '279c int SOAP_ENV__Fault_wsa5' ${toolkit_path}/share/gsoap/import/wsa5.h #修改与wsa.h的命名冲突

cd ${gSOAP_path}

generated_path=${gSOAP_path}/Generated
path_to_save_files="${generated_path}/Server"
tool_wsdl2h="${toolkit_path}/bin/wsdl2h"
tool_soapcpp2="${toolkit_path}/bin/soapcpp2"
import_path="${toolkit_path}/share/gsoap"
wsdl2h_op="-c++11"
soapcpp2_op="-c++11 -j -S -x -I ${import_path}"

#Profile G、M、S、T
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

#生成服务端代码
rm -f onvif.h
${tool_wsdl2h} -o onvif.h ${wsdl2h_op} ${wsdl_list[@]}
if [ -f onvif.h ]; then
	sed -i '125a #import "wsse.h"' onvif.h #添加WS Security支持
	mkdir -p ${path_to_save_files}
	rm -rf ${path_to_save_files}/*
	${tool_soapcpp2} ${soapcpp2_op} -d ${path_to_save_files} onvif.h
	#rm onvif.h
else
	echo "wsdl2h error"
	exit 1
fi
#屏蔽wsa5宏定义，防止编译错误
sed -i '19c // #define SOAP_WSA_2005' ${path_to_save_files}/soapStub.h

#生成客户端代码，WSDiscovery服务复用客户端的网络发送代码
path_to_save_client="${generated_path}/Client"
mkdir -p ${path_to_save_client}
rm -rf ${path_to_save_client}/*
${tool_soapcpp2} -c++11 -C -x -L -I${import_path}/import -pwsdd -d ${path_to_save_client} ${import_path}/import/wsdd.h