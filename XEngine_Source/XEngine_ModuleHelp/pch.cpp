#include "pch.h"
#include "ModuleHelp_APIHelp/ModuleHelp_APIHelp.h"
/********************************************************************
//    Created:     2025/05/14  15:06:43
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp\pch.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool APIHelp_IsErrorOccur = false;
XLONG APIHelp_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleHelp_APIHelp m_APIHelp;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleHelp_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return APIHelp_dwErrorCode;
}
/************************************************************************/
/*                        帮助函数                                      */
/************************************************************************/
extern "C" bool ModuleHelp_APIHelp_RuleIPSelect(SESSION_IPCONUT*** pppSt_IPCount, int nIPCount)
{
	return m_APIHelp.ModuleHelp_APIHelp_RuleIPSelect(pppSt_IPCount, nIPCount);
}
extern "C" bool ModuleHelp_APIHelp_QSort(XPVOID lPBase, int nNumber, int nSize)
{
	return m_APIHelp.ModuleHelp_APIHelp_QSort(lPBase, nNumber, nSize);
}