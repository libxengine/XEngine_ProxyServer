#include "pch.h"
#include "ModuleConfigure_Json.h"
/********************************************************************
//    Created:     2021/12/02  16:14:11
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfigure_Json\ModuleConfigure_Json.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfigure_Json
//    File Base:   ModuleConfigure_Json
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JSON配置读写实现
//    History:
*********************************************************************/
CModuleConfigure_Json::CModuleConfigure_Json()
{

}
CModuleConfigure_Json::~CModuleConfigure_Json()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
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
bool CModuleConfigure_Json::ModuleConfigure_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig)
{
	Config_IsErrorOccur = false;

	if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//读取配置文件所有内容到缓冲区
	FILE* pSt_File = _xtfopen(lpszConfigFile, _X("rb"));
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_OPENFILE;
		return false;
	}
	int nCount = 0;
	XCHAR tszMsgBuffer[4096];
	while (1)
	{
		int nRet = fread(tszMsgBuffer + nCount, 1, 2048, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nCount, &st_JsonRoot, &st_JsonError))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARSE;
		return false;
	}
	_tcsxcpy(pSt_ServerConfig->tszIPAddr, st_JsonRoot["tszIPAddr"].asCString());
	pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asInt();
	pSt_ServerConfig->nSocksPort = st_JsonRoot["nSocksPort"].asInt();
	pSt_ServerConfig->nTunnelPort = st_JsonRoot["nTunnelPort"].asInt();
	pSt_ServerConfig->nForwardPort = st_JsonRoot["nForwardPort"].asInt();

	if (st_JsonRoot["XMax"].empty() || (4 != st_JsonRoot["XMax"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XMAX;
		return false;
	}
	Json::Value st_JsonXMax = st_JsonRoot["XMax"];
	pSt_ServerConfig->st_XMax.nMaxClient = st_JsonXMax["nMaxClient"].asInt();
	pSt_ServerConfig->st_XMax.nMaxQueue = st_JsonXMax["nMaxQueue"].asInt();
	pSt_ServerConfig->st_XMax.nIOThread = st_JsonXMax["nIOThread"].asInt();
	pSt_ServerConfig->st_XMax.nForwardThread = st_JsonXMax["nForwardThread"].asInt();

	if (st_JsonRoot["XTime"].empty() || (4 != st_JsonRoot["XTime"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XTIME;
		return false;
	}
	Json::Value st_JsonXTime = st_JsonRoot["XTime"];
	pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
	pSt_ServerConfig->st_XTime.nSocksTimeOut = st_JsonXTime["nSocksTimeOut"].asInt();
	pSt_ServerConfig->st_XTime.nTunnelTimeOut = st_JsonXTime["nTunnelTimeOut"].asInt();
	pSt_ServerConfig->st_XTime.nForwardTimeOut = st_JsonXTime["nForwardTimeOut"].asInt();

	if (st_JsonRoot["XLog"].empty() || (4 != st_JsonRoot["XLog"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XLOG;
		return false;
	}
	Json::Value st_JsonXLog = st_JsonRoot["XLog"];
	pSt_ServerConfig->st_XLog.nMaxSize = st_JsonXLog["MaxSize"].asInt();
	pSt_ServerConfig->st_XLog.nMaxCount = st_JsonXLog["MaxCount"].asInt();
	pSt_ServerConfig->st_XLog.nLogLeave = st_JsonXLog["LogLeave"].asInt();
	_tcsxcpy(pSt_ServerConfig->st_XLog.tszLogFile, st_JsonXLog["tszLogFile"].asCString());

	if (st_JsonRoot["XAuth"].empty() || (2 != st_JsonRoot["XAuth"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XAUTH;
		return false;
	}
	Json::Value st_JsonXAuth = st_JsonRoot["XAuth"];
	pSt_ServerConfig->st_XAuth.bAuth = st_JsonXAuth["bAuth"].asBool();
	_tcsxcpy(pSt_ServerConfig->st_XAuth.tszAuthUrl, st_JsonXAuth["tszAuthUrl"].asCString());

	if (st_JsonRoot["XVer"].empty())
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XVER;
		return false;
	}
	pSt_ServerConfig->st_XVer.pStl_ListVer = new list<string>;
	Json::Value st_JsonXVer = st_JsonRoot["XVer"];
	for (unsigned int i = 0; i < st_JsonXVer.size(); i++)
	{
		pSt_ServerConfig->st_XVer.pStl_ListVer->push_back(st_JsonXVer[i].asCString());
	}
	return true;
}