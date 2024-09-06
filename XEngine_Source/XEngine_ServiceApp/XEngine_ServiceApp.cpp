#include "XEngine_Hdr.h"

bool bIsRun = false;
XHANDLE xhLog = NULL;

XHANDLE xhSocksSocket = NULL;
XHANDLE xhSocksHeart = NULL;
XHANDLE xhSocksClient = NULL;

XHANDLE xhTunnelSocket = NULL;
XHANDLE xhTunnelHeart = NULL;
XHANDLE xhTunnelClient = NULL;

XHANDLE xhForwardSocket = NULL;
XHANDLE xhForwardHeart = NULL;
XHANDLE xhForwardPacket = NULL;
XHANDLE xhForwardPool = NULL;
//配置文件
XENGINE_SERVICECONFIG st_ServiceConfig;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("服务器退出..."));
		bIsRun = false;
		//销毁Socks资源
		NetCore_TCPXCore_DestroyEx(xhSocksSocket);
		SocketOpt_HeartBeat_DestoryEx(xhSocksHeart);
		XClient_TCPSelect_StopEx(xhSocksClient);
		//销毁Tunnel资源
		NetCore_TCPXCore_DestroyEx(xhTunnelSocket);
		SocketOpt_HeartBeat_DestoryEx(xhTunnelHeart);
		XClient_TCPSelect_StopEx(xhTunnelClient);
		//销毁Forward资源
		HelpComponents_Datas_Destory(xhForwardPacket);
		NetCore_TCPXCore_DestroyEx(xhForwardSocket);
		SocketOpt_HeartBeat_DestoryEx(xhForwardHeart);
		ManagePool_Thread_NQDestroy(xhForwardPool);
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
	bIsRun = true;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVICECONFIG));
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
	st_XLogConfig.XLog_MaxBackupFile = st_ServiceConfig.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_ServiceConfig.st_XLog.nMaxSize;
	_tcsxcpy(st_XLogConfig.tszFileName, st_ServiceConfig.st_XLog.tszLogFile);
	xhLog = HelpComponents_XLog_Init(st_ServiceConfig.st_XLog.nLogLeave, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务中,启动日志失败,错误：%lX", XLog_GetLastError());
		goto XENGINE_SERVICEAPP_EXIT;
	}
	//设置日志打印级别
	HelpComponents_XLog_SetLogPriority(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化日志系统成功"));

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化信号量处理程序成功"));
	//启动Socks服务相关代码
	if (st_ServiceConfig.nSocksPort > 0)
	{
		//启动心跳
		if (st_ServiceConfig.st_XTime.nSocksTimeOut > 0)
		{
			xhSocksHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nSocksTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_SocksHeart);
			if (NULL == xhSocksHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化Socks心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化Socks心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nSocksTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,Socks心跳服务被设置为不启用"));
		}
		//启动网络
		xhSocksSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nSocksPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhSocksSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动Socks网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动Socks网络服务器成功,Socks端口:%d,网络IO线程个数:%d"), st_ServiceConfig.nSocksPort, st_ServiceConfig.st_XMax.nIOThread);
		//绑定网络事件
		NetCore_TCPXCore_RegisterCallBackEx(xhSocksSocket, Network_Callback_SocksLogin, Network_Callback_SocksRecv, Network_Callback_SocksLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,注册Socks网络事件成功"));
		//客户端
		xhSocksClient = XClient_TCPSelect_StartEx(XEngine_Socks_CBRecv);
		if (NULL == xhSocksClient)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动Socks客户端服务失败,错误：%lX"), XClient_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动Socks客户端服务成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,Socks消息服务没有被启用"));
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
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化Tunnel心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化Tunnel心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nTunnelTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,Tunnel心跳服务被设置为不启用"));
		}
		//网络
		xhTunnelSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nTunnelPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhTunnelSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动Tunnel网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动Tunnel网络服务器成功,Tunnel端口:%d,IO:%d"), st_ServiceConfig.nTunnelPort, st_ServiceConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhTunnelSocket, Network_Callback_TunnelLogin, Network_Callback_TunnelRecv, Network_Callback_TunnelLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,注册Tunnel网络事件成功"));
		//客户端
		xhTunnelClient = XClient_TCPSelect_StartEx(XEngine_Tunnel_CBRecv);
		if (NULL == xhTunnelClient)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动Tunnel客户端服务失败,错误：%lX"), XClient_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动Tunnel客户端服务成功"));
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,Tunnel消息服务没有被启用"));
	}
	//启动转发协议服务
	if (st_ServiceConfig.nForwardPort > 0)
	{
		//组包器
		xhForwardPacket = HelpComponents_Datas_Init(st_ServiceConfig.st_XMax.nMaxQueue, st_ServiceConfig.st_XMax.nForwardThread);
		if (NULL == xhForwardPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("初始化Forward组包器失败,错误：%lX"), Packets_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动Forward组包器成功"));
		//启动心跳
		if (st_ServiceConfig.st_XTime.nForwardTimeOut > 0)
		{
			xhForwardHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nForwardTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_ForwardHeart);
			if (NULL == xhForwardHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化Forward心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化Forward心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nForwardTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,Forward心跳服务被设置为不启用"));
		}
		//网络
		xhForwardSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nForwardPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhForwardSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动Forward网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动Forward网络服务器成功,Forward端口:%d,IO:%d"), st_ServiceConfig.nForwardPort, st_ServiceConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhForwardSocket, Network_Callback_ForwardLogin, Network_Callback_ForwardRecv, Network_Callback_ForwardLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,注册Forward网络事件成功"));
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
		xhForwardPool = ManagePool_Thread_NQCreate(&ppSt_ListParam, st_ServiceConfig.st_XMax.nForwardThread);
		if (NULL == xhForwardPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动Forward线程池服务失败,错误：%lX"), ManagePool_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动Forward线程池服务成功,启动个数:%d"), st_ServiceConfig.st_XMax.nForwardThread);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,Forward服务没有被启用"));
	}
	//发送信息报告
	if (st_ServiceConfig.st_XReport.bEnable)
	{
		if (InfoReport_APIMachine_Send(st_ServiceConfig.st_XReport.tszAPIUrl, st_ServiceConfig.st_XReport.tszServiceName))
		{
			__int64x nTimeNumber = 0;
			InfoReport_APIMachine_GetTime(st_ServiceConfig.st_XReport.tszAPIUrl, st_ServiceConfig.st_XReport.tszServiceName, &nTimeNumber);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中，启动信息报告给API服务器:%s 成功,报告次数:%lld"), st_ServiceConfig.st_XReport.tszAPIUrl, nTimeNumber);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中，启动信息报告给API服务器:%s 失败，错误：%lX"), st_ServiceConfig.st_XReport.tszAPIUrl, InfoReport_GetLastError());
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中，信息报告给API服务器没有启用"));
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("所有服务成功启动,服务运行中,XEngine版本:%s%s,服务版本:%s,发行次数:%d。。。"), BaseLib_OperatorVer_XNumberStr(), BaseLib_OperatorVer_XTypeStr(), st_ServiceConfig.st_XVer.pStl_ListVer->front().c_str(), st_ServiceConfig.st_XVer.pStl_ListVer->size());

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
XENGINE_SERVICEAPP_EXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("有服务启动失败,服务器退出..."));
		bIsRun = false;
		//销毁Socks资源
		NetCore_TCPXCore_DestroyEx(xhSocksSocket);
		SocketOpt_HeartBeat_DestoryEx(xhSocksHeart);
		XClient_TCPSelect_StopEx(xhSocksClient);
		//销毁Tunnel资源
		NetCore_TCPXCore_DestroyEx(xhTunnelSocket);
		SocketOpt_HeartBeat_DestoryEx(xhTunnelHeart);
		XClient_TCPSelect_StopEx(xhTunnelClient);
		//销毁Forward资源
		HelpComponents_Datas_Destory(xhForwardPacket);
		NetCore_TCPXCore_DestroyEx(xhForwardSocket);
		SocketOpt_HeartBeat_DestoryEx(xhForwardHeart);
		ManagePool_Thread_NQDestroy(xhForwardPool);
		//销毁日志资源
		HelpComponents_XLog_Destroy(xhLog);
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
