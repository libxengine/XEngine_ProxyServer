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
BOOL XEngine_Configure_Parament(int argc, char** argv, XENGINE_SERVICECONFIG* pSt_Configure)
{
	LPCTSTR lpszConfigFile = _T("./XEngine_Config/XEngine_Config.json");

	if (!ModuleConfigure_Json_File(lpszConfigFile, pSt_Configure))
	{
		printf("解析配置文件失败,ModuleConfigure_Json_File:%lX\n", ModuleConfigure_GetLastError());
		return FALSE;
	}

	for (int i = 0; i < argc; i++)
	{
		if ((0 == _tcscmp("-h", argv[i])) || (0 == _tcscmp("-H", argv[i])))
		{
			XEngine_Configure_Help();
			return FALSE;
		}
		else if (0 == _tcscmp("-TP", argv[i]))
		{
			pSt_Configure->nSocksPort = _ttoi(argv[i + 1]);
		}
		else if (0 == _tcscmp("-HP", argv[i]))
		{
			pSt_Configure->nTunnelPort = _ttoi(argv[i + 1]);
		}
		else if (0 == _tcscmp("-d", argv[i]))
		{
			pSt_Configure->bDeamon = _ttoi(argv[i + 1]);
		}
		else if (0 == _tcscmp("-v", argv[i]))
		{
			printf("XEngine版本:%s,服务版本;%s", XENGINE_VERSION_STR, st_ServiceConfig.st_XVer.pStl_ListVer->front().c_str());
			return FALSE;
		}
	}

	return TRUE;
}

void XEngine_Configure_Help()
{
	printf(_T("--------------------------启动参数帮助开始--------------------------\n"));
	printf(_T("网络消息队列服务启动参数：程序 参数 参数值，参数是区分大小写的。如果不指定将会加载默认的ini配置文件里面的参数\n"));
	printf(_T("-h or -H：启动参数帮助提示信息\n"));
	printf(_T("-TP：设置消息队列TCP服务端口号\n"));
	printf(_T("-HP：设置消息队列HTTP服务端口号\n"));
	printf(_T("-d：1 启用守护进程，2不启用\n"));
	printf(_T("--------------------------启动参数帮助结束--------------------------\n"));
}
