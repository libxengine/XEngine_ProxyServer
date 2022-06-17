RELEASE = 0
UNICODE = 0
#编译的选项参数
FLAGS = RELEASE=$(RELEASE)
#要编译的模块
THIRDPART_MODULE_JSONCPP = ./XEngine_ThirdPart/jsoncpp

MODULE_CONFIGURE_PATH = ./XEngine_ModuleConfigure
MODULE_AUTHORIZE_PATH = ./XEngine_ModuleAuthorize
MODULE_PROTOCOL_PATH = ./XEngine_ModuleProtocol
MODULE_SESSION_PATH = ./XEngine_ModuleSession
APP_SERVICE_PATH = ./XEngine_ServiceApp

XENGINE_MODULES = libjsoncpp.so libXEngine_ModuleConfigure.so libXEngine_ModuleAuthorize.so libXEngine_ModuleProtocol.so libXEngine_ModuleSession.so XEngine_ServiceApp.exe 

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)

libjsoncpp.so:
	make -C $(THIRDPART_MODULE_JSONCPP) $(FLAGS)
	
libXEngine_ModuleConfigure.so:
	make -C $(MODULE_CONFIGURE_PATH) $(FLAGS)
libXEngine_ModuleAuthorize.so:
	make -C $(MODULE_AUTHORIZE_PATH) $(FLAGS)
libXEngine_ModuleProtocol.so:
	make -C $(MODULE_PROTOCOL_PATH) $(FLAGS)
libXEngine_ModuleSession.so:
	make -C $(MODULE_SESSION_PATH) $(FLAGS)	
	
XEngine_ServiceApp.exe:
	make -C $(APP_SERVICE_PATH) $(FLAGS)


SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 
 
SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB) 
