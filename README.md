[中文](README.md) ||  [English](README.en.md)  
# XEngine_ProxyServer
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
只要仓库不是在暂停状态,那么就会有人一直维护和开发,请放心使用  

## 介绍
c c++ SOCKS5代理服务 HTTP Tunnel隧道代理服务 tcp转发服务  
c c++ Socks5 Proxy Service,HTTP Tunnel Proxy Service,tcp forward service  

## 软件架构
此软件基于XEngine开发.采用C/C++作为开发语言  
此软件是一款标准的代理服务器,支持Socks5和HTTP的隧道代理  
## 软件特性
此软件支持以下特性  
1. SOCKS5代理  
2. Tunnel代理 
3. 数据转发服务 
4. 权限验证  
5. 操作日志
6. 加密通信(planning)  
7. 负载均衡(planning)  
8. 自定义证书代理协议(planning)  

## 安装教程

#### 版本需求
支持WINDOWS 7SP1和LINUX(UBUNT20.04,CENTOS8)以上系统  
XEngine版本需要V7.27或者以上版本  

#### Windows
使用VS打开并且编译
你需要按照下面的方式配置环境,不然你可能需要自己在项目中设置库目录

#### Linux
Linux使用Makefile编译  
UBUNTU20.04 x64或者CENTOS8 x64均可  

##### 编译命令
在XEngine_Source目录下执行命令
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

#### XEngine环境
安装XEngine可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine  

#### 使用说明

1.  切换到MASTER分支
2.  下载代码
3.  编译
4.  安装
5.  运行
6.  运行演示代码

## 目录结构
- XEngine_Docment  文档目录结构  
- XEngine_Release  安装目录结构  
- XEngine_Source   源代目录结构  
- XEngine_Apps     演示客户端目录  

## 其他问题  
你可以参考docment目录下的文档.里面包含了API协议和服务说明.

## 测试服务器
地址:app.xyry.org 或者 159.75.200.173  
端口:Socks5 5400,HTTP Tunnel 5401  

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request  

## 关注我们
如果你觉得这个软件对你有帮助,请你给我们一个START吧

## 提交问题

如果你有问题,可以在issues中提交