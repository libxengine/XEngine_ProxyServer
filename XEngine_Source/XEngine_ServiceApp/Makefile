CC = g++ -Wall -std=c++20
PLATVER = 
LOADHDR = -I ./ -I ../XEngine_Depend/XEngine_Module/jsoncpp
LOADSO = -L ../XEngine_ModuleConfigure -L ../XEngine_ModuleSession -L ../XEngine_ModuleProtocol -L ../XEngine_ModuleHelp \
		 -L ../XEngine_Depend/XEngine_Module/jsoncpp -L ../XEngine_Depend/XEngine_Module/XEngine_InfoReport
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_Cryption -lXEngine_ManagePool -lXClient_Socket -lXClient_APIHelp -lHelpComponents_XLog -lHelpComponents_Packets -lRfcComponents_ProxyProtocol -lNetHelp_APIHelp -lNetHelp_APIAddr -lNetHelp_XSocket \
      -lXEngine_ModuleConfigure -lXEngine_ModuleSession -lXEngine_ModuleProtocol -lXEngine_ModuleHelp \
	  -ljsoncpp -lXEngine_InfoReport
LIBEX = 
OBJECTS = XEngine_Configure.o XEngine_Network.o XEngine_SocksTask.o XEngine_TunnelTask.o XEngine_ForwardTask.o XEngine_ProxyTask.o XEngine_ServiceApp.o

ifeq ($(RELEASE),1)
FLAGS = -c -D _RELEASE
DEBUG = -O2
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g -D _DEBUG
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg -D _DEBUG
	endif
endif

ifeq ($(UNICODE),1)
override UNICODE := -D __UNICODE__
else
override UNICODE := -D __ANSI__
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
	else
		PLATVER = -D __UBUNTU__
	endif
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
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
XEngine_ProxyTask.o:./XEngine_ProxyTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_ProxyTask.cpp

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
	       
