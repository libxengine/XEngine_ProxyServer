#pragma once
//加载系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <locale.h>
#include <thread>
#include <list>
#include <string>
#include <functional>
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#include <minidumpapiset.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
using namespace std;
//加载XEngine头文件
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_NetHelp/XSocket_Define.h>
#include <XEngine_Include/XEngine_NetHelp/XSocket_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
//加载项目相关头文件
#include "../XEngine_UserProtocol.h"
#include "../XEngine_Depend/XEngine_Module/XEngine_InfoReport/InfoReport_Define.h"
#include "../XEngine_Depend/XEngine_Module/XEngine_InfoReport/InfoReport_Error.h"
#include "../XEngine_ModuleConfigure/ModuleConfig_Define.h"
#include "../XEngine_ModuleConfigure/ModuleConfig_Error.h"
#include "../XEngine_ModuleSession/ModuleSession_Define.h"
#include "../XEngine_ModuleSession/ModuleSession_Error.h"
#include "../XEngine_ModuleProtocol/ModuleProtocol_Define.h"
#include "../XEngine_ModuleProtocol/ModuleProtocol_Error.h"
#include "../XEngine_ModuleHelp/ModuleHelp_Define.h"
#include "../XEngine_ModuleHelp/ModuleHelp_Error.h"
//加载自己的头文件
#include "XEngine_Configure.h"
#include "XEngine_Network.h"
#include "XEngine_SocksTask.h"
#include "XEngine_TunnelTask.h"
#include "XEngine_ForwardTask.h"
#include "XEngine_ProxyTask.h"
/********************************************************************
//    Created:     2021/12/02  16:34:41
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_Hdr.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_Hdr
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
extern bool bIsRun;
extern bool bIsTest;
extern XHANDLE xhLog;
//Socks服务器
extern XHANDLE xhSocksSocket;
extern XHANDLE xhSocksHeart;
extern XHANDLE xhSocksClient;
//Tunnel服务器
extern XHANDLE xhTunnelSocket;
extern XHANDLE xhTunnelHeart;
extern XHANDLE xhTunnelClient;
//Forward服务器
extern XHANDLE xhForwardSocket;
extern XHANDLE xhForwardHeart;
extern XHANDLE xhForwardPacket;
extern XHANDLE xhForwardPool;
extern XHANDLE xhForwardClient;
//代理转发服务器
extern XHANDLE xhProxySocket;
extern XHANDLE xhProxyHeart;
extern XHANDLE xhProxyClient;
//配置文件
extern XENGINE_SERVICECONFIG st_ServiceConfig;

//网络类型定义
#define XENGINE_CLIENT_NETTYPE_SOCKS 1
#define XENGINE_CLIENT_NETTYPE_TUNNEL 2
#define XENGINE_CLIENT_NETTYPE_FORWARD 3
#define XENGINE_CLIENT_NETTYPE_PROXY 4
//关闭模式
#define XENGINE_CLIENT_CLOSE_NETWORK 1
#define XENGINE_CLIENT_CLOSE_HEARTBEAT 2
#define XENGINE_CLIENT_CLOSE_SERVICE 3

typedef struct
{
	XCHAR tszIPAddr[128];
	ENUM_PROXY_SESSION_SOCKS_STATUS enStatus;
	XNETHANDLE xhClient;
}PROXYPROTOCOL_CLIENTINFO;

//连接库
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIAddr.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_XSocket.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_ProxyProtocol.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Dbghelp.lib")
#ifdef _DEBUG
#ifdef _M_X64
#pragma comment(lib,"../x64/Debug/XEngine_InfoReport.lib")
#pragma comment(lib,"../x64/Debug/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../x64/Debug/XEngine_ModuleSession.lib")
#pragma comment(lib,"../x64/Debug/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../x64/Debug/XEngine_ModuleHelp.lib")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Debug/XEngine_InfoReport.lib")
#pragma comment(lib,"../ARM64/Debug/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../ARM64/Debug/XEngine_ModuleSession.lib")
#pragma comment(lib,"../ARM64/Debug/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../ARM64/Debug/XEngine_ModuleHelp.lib")
#elif _M_IX86
#pragma comment(lib,"../Debug/XEngine_InfoReport.lib")
#pragma comment(lib,"../Debug/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../Debug/XEngine_ModuleSession.lib")
#pragma comment(lib,"../Debug/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../Debug/XEngine_ModuleHelp.lib")
#endif
#else
#ifdef _M_X64
#pragma comment(lib,"../x64/Release/XEngine_InfoReport.lib")
#pragma comment(lib,"../x64/Release/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../x64/Release/XEngine_ModuleSession.lib")
#pragma comment(lib,"../x64/Release/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../x64/Release/XEngine_ModuleHelp.lib")
#elif _M_ARM64
#pragma comment(lib,"../ARM64/Release/XEngine_InfoReport.lib")
#pragma comment(lib,"../ARM64/Release/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../ARM64/Release/XEngine_ModuleSession.lib")
#pragma comment(lib,"../ARM64/Release/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../ARM64/Release/XEngine_ModuleHelp.lib")
#elif _M_IX86
#pragma comment(lib,"../Release/XEngine_InfoReport.lib")
#pragma comment(lib,"../Release/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../Release/XEngine_ModuleSession.lib")
#pragma comment(lib,"../Release/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../Release/XEngine_ModuleHelp.lib")
#endif
#endif
#endif