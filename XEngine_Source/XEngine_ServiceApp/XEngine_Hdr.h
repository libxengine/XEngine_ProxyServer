#pragma once
//加载系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <thread>
#include <list>
#include <string>
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
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
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Define.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
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
#include "../XEngine_ModuleConfigure/ModuleConfig_Define.h"
#include "../XEngine_ModuleConfigure/ModuleConfig_Error.h"
#include "../XEngine_ModuleAuthorize/ModuleAuth_Define.h"
#include "../XEngine_ModuleAuthorize/ModuleAuth_Error.h"
//加载自己的头文件
#include "XEngine_Configure.h"
#include "XEngine_Network.h"
#include "XEngine_SocksTask.h"
#include "XEngine_TunnelTask.h"
#include "XEngine_Forward.h"
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
extern BOOL bIsRun;
extern XLOG xhLog;
//Socks服务器
extern XNETHANDLE xhSocksSocket;
extern XNETHANDLE xhSocksHeart;
//Tunnel服务器
extern XNETHANDLE xhTunnelSocket;
extern XNETHANDLE xhTunnelHeart;
//Forward服务器
extern XNETHANDLE xhForwardSocket;
extern XNETHANDLE xhForwardHeart;
extern XNETHANDLE xhForwardPool;
extern XHANDLE xhForwardPacket;
//配置文件
extern XENGINE_SERVICECONFIG st_ServiceConfig;

//网络类型定义
#define XENGINE_CLIENT_NETTYPE_SOCKS 1
#define XENGINE_CLIENT_NETTYPE_TUNNEL 2
#define XENGINE_CLIENT_NETTYPE_FORWARD 3
//关闭模式
#define XENGINE_CLIENT_CLOSE_NETWORK 1
#define XENGINE_CLIENT_CLOSE_HEARTBEAT 2
#define XENGINE_CLIENT_CLOSE_SERVICE 3

//连接库
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../x64/Release/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../x64/Release/XEngine_ModuleAuthorize.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"../Debug/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../Debug/XEngine_ModuleAuthorize.lib")
#else
#pragma comment(lib,"../Release/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../Release/XEngine_ModuleAuthorize.lib")
#endif
#endif
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl.lib")
#pragma comment(lib,"XEngine_Core/XEngine_NetXApi.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_ProxyProtocol.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"Ws2_32.lib")
#endif