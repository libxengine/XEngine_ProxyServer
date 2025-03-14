#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2021/12/02  16:34:09
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_Configure.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_Configure
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     配置文件读写代码
//    History:
*********************************************************************/
bool XEngine_Configure_Parament(int argc, char** argv, XENGINE_SERVICECONFIG* pSt_Configure)
{
	LPCXSTR lpszConfigFile = _X("./XEngine_Config/XEngine_Config.json");
	LPCXSTR lpszConfigVersion = _X("./XEngine_Config/XEngine_Version.json");

	if (!ModuleConfigure_Json_File(lpszConfigFile, pSt_Configure))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_File:%lX\n", ModuleConfigure_GetLastError());
		return false;
	}
	if (!ModuleConfigure_Json_Version(lpszConfigVersion, pSt_Configure))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_Version:%lX\n", ModuleConfigure_GetLastError());
		return false;
	}

	for (int i = 0; i < argc; i++)
	{
		if ((0 == _tcsxcmp("-h", argv[i])) || (0 == _tcsxcmp("-H", argv[i])))
		{
			XEngine_Configure_Help();
			return false;
		}
		else if (0 == _tcsxcmp("-d", argv[i]))
		{
			pSt_Configure->bDeamon = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-ps", argv[i]))
		{
			st_ServiceConfig.nSocksPort = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-pt", argv[i]))
		{
			st_ServiceConfig.nTunnelPort = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-pf", argv[i]))
		{
			st_ServiceConfig.nForwardPort = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-pp", argv[i]))
		{
			st_ServiceConfig.nProxyPort = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-ll", argv[i]))
		{
			pSt_Configure->st_XLog.nLogLeave = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-lt", argv[i]))
		{
			pSt_Configure->st_XLog.nLogType = _ttxoi(argv[++i]);
		}
		else if (0 == _tcsxcmp("-v", argv[i]))
		{
			string m_StrVersion = st_ServiceConfig.st_XVer.pStl_ListVer->front();
			printf("Version：%s\n", m_StrVersion.c_str());
			return false;
		}
		else if (0 == _tcsxcmp("-t", argv[i]))
		{
			bIsTest = true;
		}
	}

	return true;
}

void XEngine_Configure_Help()
{
	printf(_X("--------------------------启动参数帮助开始--------------------------\n"));
	printf(_X("网络消息队列服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的ini配置文件里面的参数\n"));
	printf(_X("-h or -H：启动参数帮助提示信息\n"));
	printf(_X("-d：1 启用守护进程，2不启用\n"));
	printf(_X("--------------------------启动参数帮助结束--------------------------\n"));
}
