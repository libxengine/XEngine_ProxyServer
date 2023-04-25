#include "pch.h"
#include "ModuleAuthorize_User/ModuleAuthorize_User.h"
/********************************************************************
//    Created:     2021/12/29  10:35:16
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\pch.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Authorize_IsErrorOccur = false;
DWORD Authorize_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleAuthorize_User m_User;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleAuthorize_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Authorize_dwErrorCode;
}
/************************************************************************/
/*                        验证导出函数                                  */
/************************************************************************/
extern "C" bool ModuleAuthorize_User_Init(LPCXSTR lpszAuthFile)
{
	return m_User.ModuleAuthorize_User_Init(lpszAuthFile);
}
extern "C" bool ModuleAuthorize_User_Destory()
{
	return m_User.ModuleAuthorize_User_Destory();
}
extern "C" bool ModuleAuthorize_User_Exist(LPCXSTR lpszUser, LPCXSTR lpszPass)
{
	return m_User.ModuleAuthorize_User_Exist(lpszUser, lpszPass);
}