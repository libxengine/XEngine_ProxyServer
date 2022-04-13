# XEngine_ProxyServer
we have a development and a master branch. If you want to use it, please use the code under the master branch

#### Description
c c++ SOCKS5代理服务 HTTP Tunnel隧道代理服务  
c c++ Socks5 Proxy Service HTTP Tunnel Proxy Service  

#### Software Architecture
the software developed based on xengine,using c/c++ development  
This software is a standard proxy server that supports Socks5 and HTTP tunnel proxy  

## Software features
this software support following features  
1. SOCKS5 proxy  
2. Tunnel proxy  
3. support Permission Validation  
4. operator log  
5. encrypt Communication(planning)  
6. load balanc(planning)  
7. custom cert proxy protocol(planning)  

## install

#### Requirements  
support system above windows 7sp1 and linux(ubuntu20.04,centos8)  
XEngine need V7.27 or above  

#### Windows
use vs open and complie  
You need to configure the environment in the following way, otherwise you may need to set the library directory in the project yourself   

#### Linux
Linux use Makefile to complie  
supproted ubuntu x64 or centos x64     

##### XEngine Install
you can install xengine env to your system by shell   
like this:sudo XEngine_RunEnv.sh -i 3  
##### complie
execute command in XEngine_Source path
make complie  
make FLAGS=InstallAll install  
make FLAGS=CleanAll clear  

#### XEngine Env
install XEngine can be refer to xengine Readme docment  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine  

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
Address:app.xyry.org or 159.75.200.173  
Port:Socks5 5400,HTTP Tunnel 5401  

## Participate in contribution

1. Fork this code
2. Create new Feat_xxx branch
3. Submit the code
4. New Pull Request

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...