CC = g++ -Wall -std=c++17
PLATFORM = linux
PLATVER = 
PLATDIR = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_NetHelp  \
         -L ../XEngine_ModuleConfigure -L ../XEngine_ModuleSession -L ../XEngine_ModuleProtocol  \
		 -L ../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_OPenSsl -lXEngine_ManagePool -lXEngine_NetXApi -lHelpComponents_XLog -lHelpComponents_Packets -lRfcComponents_ProxyProtocol -lXClient_Socket -lNetHelp_APIHelp \
      -lXEngine_ModuleConfigure -lXEngine_ModuleSession -lXEngine_ModuleProtocol  \
	  -ljsoncpp
LIBEX = 
OBJECTS = XEngine_Configure.o XEngine_Network.o XEngine_SocksTask.o XEngine_TunnelTask.o XEngine_ForwardTask.o XEngine_ServiceApp.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
		PLATDIR = XEngine_Linux/Centos
	else
		PLATVER = -D __UBUNTU__
		PLATDIR = XEngine_Linux/Ubuntu
	endif
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_ProxyServiceApp $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)
  
XEngine_Configure.o:./XEngine_Configure.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_Configure.cpp
XEngine_Network.o:./XEngine_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_Network.cpp 
XEngine_SocksTask.o:./XEngine_SocksTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_SocksTask.cpp
XEngine_TunnelTask.o:./XEngine_TunnelTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_TunnelTask.cpp
XEngine_ForwardTask.o:./XEngine_ForwardTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_ForwardTask.cpp

XEngine_ServiceApp.o:./XEngine_ServiceApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_ServiceApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_ProxyServiceApp
	cp -f ./XEngine_ProxyServiceApp ../../XEngine_Release/XEngine_ProxyServiceApp

CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_ProxyServiceApp
	       
