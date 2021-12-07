#include "XEngine_Hdr.h"

BOOL bIsRun = FALSE;
XLOG xhLog = NULL;

XNETHANDLE xhSocksSocket = 0;
XNETHANDLE xhSocksHeart = 0;

XNETHANDLE xhTunnelSocket = 0;
XNETHANDLE xhTunnelHeart = 0;
//配置文件
XENGINE_SERVICECONFIG st_ServiceConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("服务器退出..."));
		bIsRun = FALSE;
		//销毁TCP资源
		NetCore_TCPXCore_DestroyEx(xhSocksSocket);
		SocketOpt_HeartBeat_DestoryEx(xhSocksHeart);
		//销毁HTTP资源
		NetCore_TCPXCore_DestroyEx(xhTunnelSocket);
		SocketOpt_HeartBeat_DestoryEx(xhTunnelHeart);
		//销毁日志资源
		HelpComponents_XLog_Destroy(xhLog);
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	exit(0);
}
//LINUX守护进程
static int ServiceApp_Deamon()
{
#ifndef _WINDOWS
	pid_t nPID = 0;
	int nStatus = 0;
	nPID = fork();
	if (nPID > 0)
	{
		exit(0);
	}

	close(2);
	while (1)
	{
		nPID = fork();
		if (nPID < 0)
		{
			exit(1);
		}
		if (nPID == 0)
		{
			return 0;
		}
		waitpid(nPID, &nStatus, 0);
		sleep(1);
	}
#endif
	return 0;
}

int main(int argc, char** argv)
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	bIsRun = TRUE;
	LPCTSTR lpszLogFile = _T("./XEngine_Log/XEngine_MQServiceApp.Log");
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVICECONFIG));

	st_XLogConfig.XLog_MaxBackupFile = 10;
	st_XLogConfig.XLog_MaxSize = 1024000;
	_tcscpy(st_XLogConfig.tszFileName, lpszLogFile);

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	//初始化参数
	if (!XEngine_Configure_Parament(argc, argv, &st_ServiceConfig))
	{
		return -1;
	}
	//判断是否以守护进程启动
	if (st_ServiceConfig.bDeamon)
	{
		ServiceApp_Deamon();
	}
	//初始日志
	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_STD | HELPCOMPONENTS_XLOG_OUTTYPE_FILE, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务中,启动日志失败,错误：%lX", XLog_GetLastError());
		goto XENGINE_SERVICEAPP_EXIT;
	}
	//设置日志打印级别
	HelpComponents_XLog_SetLogPriority(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化日志系统成功"));
	//启动TCP服务相关代码
	if (st_ServiceConfig.nSocksPort > 0)
	{
		//启动心跳
		if (st_ServiceConfig.st_XTime.nSocksTimeOut > 0)
		{
			if (!SocketOpt_HeartBeat_InitEx(&xhSocksHeart, st_ServiceConfig.st_XTime.nSocksTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_SocksHeart))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化TCP心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化TCP心跳服务成功,句柄:%llu,时间:%d,次数:%d"), xhSocksHeart, st_ServiceConfig.st_XTime.nSocksTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,TCP心跳服务被设置为不启用"));
		}
		//启动网络
		if (!NetCore_TCPXCore_StartEx(&xhSocksSocket, st_ServiceConfig.nSocksPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动TCP网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动TCP网络服务器成功,TCP端口:%d,网络IO线程个数:%d"), st_ServiceConfig.nSocksPort, st_ServiceConfig.st_XMax.nIOThread);
		//绑定网络事件
		NetCore_TCPXCore_RegisterCallBackEx(xhSocksSocket, Network_Callback_SocksLogin, Network_Callback_SocksRecv, Network_Callback_SocksLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册TCP网络事件成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,TCP消息服务没有被启用"));
	}
	//启动HTTP服务相关代码
	if (st_ServiceConfig.nTunnelPort > 0)
	{
		//启动心跳
		if (st_ServiceConfig.st_XTime.nTunnelTimeOut > 0)
		{
			if (!SocketOpt_HeartBeat_InitEx(&xhTunnelHeart, st_ServiceConfig.st_XTime.nTunnelTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_TunnelHeart))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化HTTP心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化HTTP心跳服务成功,句柄:%llu,时间:%d,次数:%d"), xhTunnelHeart, st_ServiceConfig.st_XTime.nTunnelTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,HTTP心跳服务被设置为不启用"));
		}
		//网络
		if (!NetCore_TCPXCore_StartEx(&xhTunnelSocket, st_ServiceConfig.nTunnelPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动HTTP网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动HTTP网络服务器成功,HTTP端口:%d,IO:%d"), st_ServiceConfig.nTunnelPort, st_ServiceConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhTunnelSocket, Network_Callback_TunnelLogin, Network_Callback_TunnelRecv, Network_Callback_TunnelLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册HTTP网络事件成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,HTTP消息服务没有被启用"));
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动,服务运行中。。。"));
	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
XENGINE_SERVICEAPP_EXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("有服务启动失败,服务器退出..."));
		bIsRun = FALSE;
		//销毁TCP资源
		NetCore_TCPXCore_DestroyEx(xhSocksSocket);
		SocketOpt_HeartBeat_DestoryEx(xhSocksHeart);
		//销毁HTTP资源
		NetCore_TCPXCore_DestroyEx(xhTunnelSocket);
		SocketOpt_HeartBeat_DestoryEx(xhTunnelHeart);
		//销毁日志资源
		HelpComponents_XLog_Destroy(xhLog);
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
