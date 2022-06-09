#pragma once
/********************************************************************
//    Created:     2022/06/08  13:56:05
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse\ModuleProtocol_Parse.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse
//    File Base:   ModuleProtocol_Parse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     解析协议类
//    History:
*********************************************************************/

class CModuleProtocol_Parse
{
public:
	CModuleProtocol_Parse();
	~CModuleProtocol_Parse();
public:
	BOOL ModuleProtocol_Parse_ForwardBind(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszSrcAddr, TCHAR* ptszDstAddr);
private:
};