#pragma once
/********************************************************************
//    Created:     2025/05/14  15:11:29
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_APIHelp\ModuleHelp_APIHelp.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_APIHelp
//    File Base:   ModuleHelp_APIHelp
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     帮助函数
//    History:
*********************************************************************/

class CModuleHelp_APIHelp
{
public:
	CModuleHelp_APIHelp();
	~CModuleHelp_APIHelp();
public:
	bool ModuleHelp_APIHelp_RuleIPSelect(SESSION_IPCONUT*** pppSt_IPCount, int nIPCount);
	bool ModuleHelp_APIHelp_QSort(XPVOID lPBase, int nNumber, int nSize);
private:
	static int ModuleHelp_APIHelp_QSortCmp(const void* lParam, const void* WParam);
};	