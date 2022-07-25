#include "XEngine_Hdr.h"

BOOL bIsRun = FALSE;
XLOG xhLog = NULL;

XHANDLE xhSocksSocket = NULL;
XHANDLE xhSocksHeart = NULL;

XHANDLE xhTunnelSocket = NULL;
XHANDLE xhTunnelHeart = NULL;

XHANDLE xhForwardSocket = NULL;
XHANDLE xhForwardHeart = NULL;
XHANDLE xhForwardPacket = NULL;
XNETHANDLE xhForwardPool = 0;
//配置文件
XENGINE_SERVICECONFIG st_ServiceConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("服务器退出..."));
		bIsRun = FALSE;
		//销毁Socks资源
		NetCore_TCPXCore_DestroyEx(xhSocksSocket);
		SocketOpt_HeartBeat_DestoryEx(xhSocksHeart);
		//销毁Tunnel资源
		NetCore_TCPXCore_DestroyEx(xhTunnelSocket);
		SocketOpt_HeartBeat_DestoryEx(xhTunnelHeart);
		//销毁Forward资源
		HelpComponents_Packets_Destory(xhForwardPacket);
		NetCore_TCPXCore_DestroyEx(xhForwardSocket);
		SocketOpt_HeartBeat_DestoryEx(xhForwardHeart);
		ManagePool_Thread_NQDestroy(xhForwardPool);
		//销毁日志资源
		HelpComponents_XLog_Destroy(xhLog);
		ModuleAuthorize_User_Destory();
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	exit(0);
}
//LINUX守护进程
static int ServiceApp_Deamon()
{
#ifndef _MSC_BUILD
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
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

#if XENGINE_VERSION_BIT < 7270001001
	printf("版本号过低,无法继续,按任意键退出!\n");
	getchar();
	return 0;
#endif
	bIsRun = TRUE;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVICECONFIG));

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
	st_XLogConfig.XLog_MaxBackupFile = 10;
	st_XLogConfig.XLog_MaxSize = 1024000;
	_tcscpy(st_XLogConfig.tszFileName, st_ServiceConfig.st_XLog.tszLogFile);
	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_STD | HELPCOMPONENTS_XLOG_OUTTYPE_FILE, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务中,启动日志失败,错误：%lX", XLog_GetLastError());
		goto XENGINE_SERVICEAPP_EXIT;
	}
	//设置日志打印级别
	HelpComponents_XLog_SetLogPriority(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化日志系统成功"));

	if (st_ServiceConfig.st_XAuth.bAuth >0)
	{
		if (!ModuleAuthorize_User_Init(st_ServiceConfig.st_XAuth.tszAuthFile))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启用用户验证服务失败,错误：%lX"), ModuleAuthorize_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启用用户验证服务,用户列表地址:%s"), st_ServiceConfig.st_XAuth.tszAuthFile);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,没有启用用户验证服务"));
	}
	//启动Socks服务相关代码
	if (st_ServiceConfig.nSocksPort > 0)
	{
		//启动心跳
		if (st_ServiceConfig.st_XTime.nSocksTimeOut > 0)
		{
			xhSocksHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nSocksTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_SocksHeart);
			if (NULL == xhSocksHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化Socks心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化Socks心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nSocksTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,Socks心跳服务被设置为不启用"));
		}
		//启动网络
		xhSocksSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nSocksPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhSocksSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动Socks网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动Socks网络服务器成功,Socks端口:%d,网络IO线程个数:%d"), st_ServiceConfig.nSocksPort, st_ServiceConfig.st_XMax.nIOThread);
		//绑定网络事件
		NetCore_TCPXCore_RegisterCallBackEx(xhSocksSocket, Network_Callback_SocksLogin, Network_Callback_SocksRecv, Network_Callback_SocksLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册Socks网络事件成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,Socks消息服务没有被启用"));
	}
	//启动Tunnel服务相关代码
	if (st_ServiceConfig.nTunnelPort > 0)
	{
		//启动心跳
		if (st_ServiceConfig.st_XTime.nTunnelTimeOut > 0)
		{
			xhTunnelHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nTunnelTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_TunnelHeart);
			if (NULL == xhTunnelHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化Tunnel心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化Tunnel心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nTunnelTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,Tunnel心跳服务被设置为不启用"));
		}
		//网络
		xhTunnelSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nTunnelPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhTunnelSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动Tunnel网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动Tunnel网络服务器成功,Tunnel端口:%d,IO:%d"), st_ServiceConfig.nTunnelPort, st_ServiceConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhTunnelSocket, Network_Callback_TunnelLogin, Network_Callback_TunnelRecv, Network_Callback_TunnelLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册Tunnel网络事件成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,Tunnel消息服务没有被启用"));
	}
	//启动转发协议服务
	if (st_ServiceConfig.nForwardPort > 0)
	{
		//组包器
		xhForwardPacket = HelpComponents_Datas_Init(st_ServiceConfig.st_XMax.nMaxQueue, st_ServiceConfig.st_XMax.nForwardThread);
		if (NULL == xhForwardPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("初始化Forward组包器失败，错误：%lX"), Packets_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动Forward组包器成功"));
		//启动心跳
		if (st_ServiceConfig.st_XTime.nForwardTimeOut > 0)
		{
			xhForwardHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nForwardTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_ForwardHeart);
			if (NULL == xhForwardHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化Forward心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化Forward心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nForwardTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,Forward心跳服务被设置为不启用"));
		}
		//网络
		xhForwardSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nForwardPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhForwardSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动Forward网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动Forward网络服务器成功,Forward端口:%d,IO:%d"), st_ServiceConfig.nForwardPort, st_ServiceConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhForwardSocket, Network_Callback_ForwardLogin, Network_Callback_ForwardRecv, Network_Callback_ForwardLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册Forward网络事件成功"));
		//任务池
		THREADPOOL_PARAMENT** ppSt_ListParam;
		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListParam, st_ServiceConfig.st_XMax.nForwardThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceConfig.st_XMax.nForwardThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListParam[i]->lParam = pInt_Pos;
			ppSt_ListParam[i]->fpCall_ThreadsTask = XEngine_Forward_Thread;
		}
		if (!ManagePool_Thread_NQCreate(&xhForwardPool, &ppSt_ListParam, st_ServiceConfig.st_XMax.nForwardThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动Forward线程池服务失败，错误：%lX"), ManagePool_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动Forward线程池服务成功,启动个数:%d"), st_ServiceConfig.st_XMax.nForwardThread);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,Forward服务没有被启用"));
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动,服务运行中,XEngine版本:%s,服务版本;%s,发行次数:%d。。。"), BaseLib_OperatorVer_XGetStr(), st_ServiceConfig.st_XVer.pStl_ListVer->front().c_str(), st_ServiceConfig.st_XVer.pStl_ListVer->size());

	while (TRUE)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
XENGINE_SERVICEAPP_EXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("有服务启动失败,服务器退出..."));
		bIsRun = FALSE;
		//销毁Socks资源
		NetCore_TCPXCore_DestroyEx(xhSocksSocket);
		SocketOpt_HeartBeat_DestoryEx(xhSocksHeart);
		//销毁Tunnel资源
		NetCore_TCPXCore_DestroyEx(xhTunnelSocket);
		SocketOpt_HeartBeat_DestoryEx(xhTunnelHeart);
		//销毁Forward资源
		HelpComponents_Packets_Destory(xhForwardPacket);
		NetCore_TCPXCore_DestroyEx(xhForwardSocket);
		SocketOpt_HeartBeat_DestoryEx(xhForwardHeart);
		ManagePool_Thread_NQDestroy(xhForwardPool);
		//销毁日志资源
		HelpComponents_XLog_Destroy(xhLog);
		ModuleAuthorize_User_Destory();
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
