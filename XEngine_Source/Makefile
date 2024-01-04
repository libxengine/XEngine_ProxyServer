RELEASE = 0
UNICODE = 0
PLATFORM=linux
FLAGS= 
#要编译的模块
THIRDPART_MODULE_JSONCPP = ./XEngine_ThirdPart/jsoncpp
MODULE_CONFIGURE_PATH = ./XEngine_ModuleConfigure
MODULE_PROTOCOL_PATH = ./XEngine_ModuleProtocol
MODULE_SESSION_PATH = ./XEngine_ModuleSession
APP_SERVICE_PATH = ./XEngine_ServiceApp

XENGINE_MODULES = libjsoncpp.so libXEngine_ModuleConfigure.so libXEngine_ModuleProtocol.so libXEngine_ModuleSession.so XEngine_ServiceApp.exe 

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)

libjsoncpp.so:
	make -C $(THIRDPART_MODULE_JSONCPP) PLATFORM=$(PLATFORM) $(FLAGS)
	
libXEngine_ModuleConfigure.so:
	make -C $(MODULE_CONFIGURE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_ModuleProtocol.so:
	make -C $(MODULE_PROTOCOL_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_ModuleSession.so:
	make -C $(MODULE_SESSION_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
	
XEngine_ServiceApp.exe:
	make -C $(APP_SERVICE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
