#include "pch.h"
#include "ModuleSession_Forward/ModuleSession_Forward.h"
/********************************************************************
//    Created:     2022/06/08  10:10:52
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\pch.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Session_IsErrorOccur = false;
XLONG Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleSession_Forward m_Forward;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleSession_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Session_dwErrorCode;
}
/************************************************************************/
/*                        转发导出函数                                  */
/************************************************************************/
extern "C" bool ModuleSession_Forward_Insert(LPCXSTR lpszAddr, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth)
{
	return m_Forward.ModuleSession_Forward_Insert(lpszAddr, pSt_UserAuth);
}
extern "C" bool ModuleSession_Forward_List(XCHAR * **ppptszListAddr, int* pInt_Count, LPCXSTR lpszAddr)
{
	return m_Forward.ModuleSession_Forward_List(ppptszListAddr, pInt_Count, lpszAddr);
}
extern "C" bool ModuleSession_Forward_Bind(LPCXSTR lpszSrcAddr, LPCXSTR lpszDstAddr)
{
	return m_Forward.ModuleSession_Forward_Bind(lpszSrcAddr, lpszDstAddr);
}
extern "C" bool ModuleSession_Forward_Delete(LPCXSTR lpszAddr, XCHAR * ptszDstAddr)
{
	return m_Forward.ModuleSession_Forward_Delete(lpszAddr, ptszDstAddr);
}
extern "C" bool ModuleSession_Forward_Get(LPCXSTR lpszAddr, XCHAR * ptszDstAddr)
{
	return m_Forward.ModuleSession_Forward_Get(lpszAddr, ptszDstAddr);
}