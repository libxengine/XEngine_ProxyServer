[中文](README.md) ||  [English](README.en.md)  
# XEngine_ProxyServer
we have a development and a master branch. If you want to use it, please use the code under the master branch  
As long as the repository is not in suspended state, we will maintain and develop it all the time, please use it with confidence  

#### Description
c c++ SOCKS5代理服务,HTTP Tunnel隧道代理服务,tcp转发控制服务,负载均衡服务  
c c++ Socks5 Proxy Service,HTTP Tunnel Proxy Service,tcp forward control service,load balance forward service  

## Notice
the compiled release version will only be released on github.

#### Software Architecture
the software developed based on xengine,using c/c++ development  
This software is a standard proxy server that supports Socks5 and HTTP tunnel proxy  

## Software features
this software support following features  
1. SOCKS5 proxy  
2. Tunnel proxy  
3. TCP forward control
4. operator log  
5. encrypt Communication(planning)  
6. load balance
7. custom cert proxy protocol(planning)  
8. multilevel proxy(planning)

## install

#### XEngine Evn
you must install XEngine,need V9.x or above,install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/libxengine/libxengine  
GITHUB:https://github.com/libxengine/libxengine

##### fast to deployment 
git clone https://gitee.com/libxengine/libxengine.git or git clone https://github.com/libxengine/libxengine.git  
window Exection XEngine_WINEnv.bat   
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3

#### sub module
Due to the dependent sub-modules, after you checkout the warehouse, execute the following command in the warehouse directory to pull the sub-modules  
git submodule init  
git submodule update  

#### Windows
use vs open and compile,suport windows 7sp1 and above  
Just Run it

#### Linux
use makefile compile,UBUNTU22.04 x64 or RockyLinux9 x64  
Run it on the terminal

##### complie
execute command in XEngine_Source path
make complie  
make FLAGS=InstallAll install  
make FLAGS=CleanAll clear  

#### Macos
use makefile compile,mac 12 and above  
Run it on the terminal

##### complie
Refer to Linux

#### use

1.  Switch to the MASTER branch
2.  download code
3.  complie
4.  install
5.  run
6.  run example code  

## directory struct
- XEngine_Docment  docment directory  
- XEngine_Release  install directory  
- XEngine_Source   code    directory  
- XEngine_Apps     Client code directory  

## other problems   
You can refer to the document under the docment directory. It contains API protocol and service description.  

## Test Server Address
Address:app.libxengine.com  
Port:Socks5 5400,HTTP Tunnel 5401,Forward 5402

## Participate in contribution

1. Fork this code
2. Create new Feat_xxx branch
3. Submit the code
4. New Pull Request

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...