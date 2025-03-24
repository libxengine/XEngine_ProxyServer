#pragma once
/********************************************************************
//    Created:     2021/12/02  16:09:03
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfig_Define.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure
//    File Base:   ModuleConfig_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置模块导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的数据结构
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XEngine_ServiceConfig
{
	XCHAR tszIPAddr[128];                     //本机IP地址,根据需要配置
	bool bDeamon;                             //是否以守护进程启动,LINUX有效
	int nSocksPort;                           //Socks端口,<=0不启用
	int nTunnelPort;                          //Tunnel服务端口
	int nForwardPort;                         //数据转发端口
	int nProxyPort;                           //全代理转发
	struct
	{
		int nMaxClient;                       //最大客户端个数
		int nMaxQueue;                        //最大队列个数
		int nIOThread;                        //网络IO线程数
		int nForwardThread;                   //转发服务
	}st_XMax;
	struct
	{
		int nTimeCheck;                       //检测次数
		int nSocksTimeout;                    //TCP超时时间
		int nTunnelTimeout;                   //HTTP超时时间
		int nForwardTimeout;                  //转发超时时间
		int nProxyTimeout;                    //代理超时时间
	}st_XTime;                                //次数*时间=超时
	struct
	{
		XCHAR tszLogFile[MAX_PATH];           //日志保存路径
		int nMaxSize;                         //最大日志大小
		int nMaxCount;                        //最大日志个数
		int nLogLeave;                        //日志等级
		int nLogType;                         //日志类型
	}st_XLog;
	struct
	{
		bool bEnable;
		XCHAR tszAPIUrl[MAX_PATH];
		XCHAR tszServiceName[128];
	}st_XReport;
	struct
	{
		bool bEnable;
		XCHAR tszIPAddr[128];
		std::list<xstring>* pStl_ListIPAddr;
	}st_XProxy;
	struct  
	{
		list<string>* pStl_ListVer;
	}st_XVer;
}XENGINE_SERVICECONFIG;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleConfigure_GetLastError(int* pInt_ErrorCode = NULL);
/************************************************************************/
/*                        文件配置读取                                  */
/************************************************************************/
/********************************************************************
函数名称：ModuleConfigure_Json_File
函数功能：读取JSON配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出服务配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleConfigure_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig);
/********************************************************************
函数名称：ModuleConfigure_Json_Version
函数功能：读取版本配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleConfigure_Json_Version(LPCXSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig);