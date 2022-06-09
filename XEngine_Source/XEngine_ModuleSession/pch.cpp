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
BOOL Session_IsErrorOccur = FALSE;
DWORD Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleSession_Forward m_Forward;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleSession_GetLastError(int* pInt_SysError)
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
extern "C" BOOL ModuleSession_Forward_Insert(LPCTSTR lpszAddr)
{
	return m_Forward.ModuleSession_Forward_Insert(lpszAddr);
}
extern "C" BOOL ModuleSession_Forward_List(TCHAR * **ppptszListAddr, int* pInt_Count, LPCTSTR lpszAddr)
{
	return m_Forward.ModuleSession_Forward_List(ppptszListAddr, pInt_Count, lpszAddr);
}
extern "C" BOOL ModuleSession_Forward_Bind(LPCTSTR lpszSrcAddr, LPCTSTR lpszDstAddr)
{
	return m_Forward.ModuleSession_Forward_Bind(lpszSrcAddr, lpszDstAddr);
}
extern "C" BOOL ModuleSession_Forward_Delete(LPCTSTR lpszAddr, TCHAR * ptszDstAddr)
{
	return m_Forward.ModuleSession_Forward_Delete(lpszAddr, ptszDstAddr);
}
extern "C" BOOL ModuleSession_Forward_Get(LPCTSTR lpszAddr, TCHAR * ptszDstAddr)
{
	return m_Forward.ModuleSession_Forward_Get(lpszAddr, ptszDstAddr);
}