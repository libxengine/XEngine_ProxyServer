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
3. TCP数据转发服务
4. 权限验证
5. 操作日志
6. 加密通信(planning)
7. 负载均衡(planning)
8. 自定义证书代理协议(planning)
9. 多级代理(planning)

## 安装教程

#### XEngine环境
必须安装XEngine,版本需要V8.15或者以上版本,安装XEngine可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine

###### 快速部署
git clone https://gitee.com/xyry/libxengine.git 或者 git clone https://github.com/libxengine/xengine.git  
window执行XEngine_WINEnv.bat 脚本.  
Linux执行:sudo ./XEngine_LINEnv.sh -i 3  
macos执行:./XEngine_LINEnv.sh -i 3  

#### Windows
使用VS打开并且编译,支持WINDOWS 7SP1以上系统  
直接运行即可,使用XEngine_AuthorizeApp

#### Linux
Linux使用Makefile编译,UBUNTU22.04 x64或者RockyLinux9 x64  
在控制台运行

##### 编译命令
在XEngine_Source目录下执行命令
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

#### Macos
使用makefile编译,控制台运行,需要mac 13以及以上版本  
在控制台运行

##### 编译命令
参考Linux

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
地址:app.xyry.org  
端口:Socks5 5400,HTTP Tunnel 5401,Forward 5402

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request  

## 开发计划
透传代理  
支持加密代理  
转发代理绑定解绑支持附加用户信息  
转发代理绑定支持拒绝  
支持UDP转发代理  
性能优化  

## 关注我们
如果你觉得这个软件对你有帮助,请你给我们一个START吧

## 提交问题

如果你有问题,可以在issues中提交