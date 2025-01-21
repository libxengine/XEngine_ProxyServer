CC = g++ -Wall -std=c++20 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ -I ../XEngine_ThirdPart/jsoncpp
LOADSO = 
LIB = -lXEngine_BaseLib -lXEngine_Algorithm
LIBEX = 
OBJECTS = ModuleSession_Forward.o ModuleSession_Socks.o ModuleSession_Tunnel.o ModuleSession_Proxy.o pch.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g 
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg 
	endif
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
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	FILEEXT = dylib
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif


all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_ModuleSession.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

ModuleSession_Forward.o:./ModuleSession_Forward/ModuleSession_Forward.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleSession_Forward/ModuleSession_Forward.cpp 
ModuleSession_Socks.o:./ModuleSession_Socks/ModuleSession_Socks.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleSession_Socks/ModuleSession_Socks.cpp
ModuleSession_Tunnel.o:./ModuleSession_Tunnel/ModuleSession_Tunnel.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleSession_Tunnel/ModuleSession_Tunnel.cpp 	
ModuleSession_Proxy.o:./ModuleSession_Proxy/ModuleSession_Proxy.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleSession_Proxy/ModuleSession_Proxy.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModuleSession.$(FILEEXT)
	cp ./libXEngine_ModuleSession.$(FILEEXT) ../../XEngine_Release/libXEngine_ModuleSession.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
