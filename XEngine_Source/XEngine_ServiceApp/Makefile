CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L ../XEngine_ModuleConfigure
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lHelpComponents_XLog -lHelpComponents_Packets -lRfcComponents_HttpServer -lXEngine_ModuleConfigure
LIBEX = -static-libgcc -ldl -lrt -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = XEngine_Configure.o XEngine_Network.o XEngine_TCPTask.o XEngine_HTTPTask.o XEngine_ServiceApp.o

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
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

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_ServiceApp $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
XEngine_Configure.o:./XEngine_Configure.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_Configure.cpp
XEngine_Network.o:./XEngine_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_Network.cpp 
XEngine_TCPTask.o:./XEngine_TCPTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_TCPTask.cpp
XEngine_HTTPTask.o:./XEngine_HTTPTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_HTTPTask.cpp
		
XEngine_ServiceApp.o:./XEngine_ServiceApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_ServiceApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_ServiceApp
	cp -f ./XEngine_ServiceApp ../../XEngine_Release/XEngine_ServiceApp

CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_ServiceApp     
	       
