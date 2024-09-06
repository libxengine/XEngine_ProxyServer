#include "pch.h"
#include "ModuleSession_Forward/ModuleSession_Forward.h"
#include "ModuleSession_Socks/ModuleSession_Socks.h"
#include "ModuleSession_Tunnel/ModuleSession_Tunnel.h"
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
CModuleSession_Socks m_Socks;
CModuleSession_Tunnel m_Tunnel;
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
extern "C" bool ModuleSession_Forward_List(SESSION_FORWARD * **pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr)
{
	return m_Forward.ModuleSession_Forward_List(pppSt_ListUser, pInt_Count, lpszAddr);
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
/************************************************************************/
/*                     SOCK代理服务器导出函数                           */
/************************************************************************/
extern "C" bool ModuleSession_Socks_Create(LPCXSTR lpszClientID)
{
	return m_Socks.ModuleSession_Socks_Create(lpszClientID);
}
extern "C" bool ModuleSession_Socks_Delete(LPCXSTR lpszClientID)
{
	return m_Socks.ModuleSession_Socks_Delete(lpszClientID);
}
extern "C" bool ModuleSession_Socks_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen)
{
	return m_Socks.ModuleSession_Socks_SetInfo(lpszClientID, lParam, nLen);
}
extern "C" bool ModuleSession_Socks_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len)
{
	return m_Socks.ModuleSession_Socks_GetInfo(lpszClientID, lParam, pInt_Len);
}
extern "C" bool ModuleSession_Socks_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize)
{
	return m_Socks.ModuleSession_Socks_GetList(xpppMem, pInt_Count, nSize);
}
extern "C" bool ModuleSession_Socks_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS* penSocks)
{
	return m_Socks.ModuleSession_Socks_GetStatus(lpszClientID, penSocks);
}
extern "C" bool ModuleSession_Socks_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS enStatus)
{
	return m_Socks.ModuleSession_Socks_SetStatus(lpszClientID, enStatus);
}
extern "C" bool ModuleSession_Socks_List(XCHAR*** ppptszClientList, int* pInt_ListCount)
{
	return m_Socks.ModuleSession_Socks_List(ppptszClientList, pInt_ListCount);
}
/************************************************************************/
/*                     隧道代理服务                                     */
/************************************************************************/
extern "C" bool ModuleSession_Tunnel_Create(LPCXSTR lpszClientID)
{
	return m_Tunnel.ModuleSession_Tunnel_Create(lpszClientID);
}
extern "C" bool ModuleSession_Tunnel_Delete(LPCXSTR lpszClientID)
{
	return m_Tunnel.ModuleSession_Tunnel_Delete(lpszClientID);
}
extern "C" bool ModuleSession_Tunnel_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen)
{
	return m_Tunnel.ModuleSession_Tunnel_SetInfo(lpszClientID, lParam, nLen);
}
extern "C" bool ModuleSession_Tunnel_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len)
{
	return m_Tunnel.ModuleSession_Tunnel_GetInfo(lpszClientID, lParam, pInt_Len);
}
extern "C" bool ModuleSession_Tunnel_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize)
{
	return m_Tunnel.ModuleSession_Tunnel_GetList(xpppMem, pInt_Count, nSize);
}
extern "C" bool ModuleSession_Tunnel_Packet(LPCXSTR lpszClientID, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	return m_Tunnel.ModuleSession_Tunnel_Packet(lpszClientID, lpszMsgBuffer, nMsgLen, ptszMSGBuffer, pInt_MSGLen);
}
extern "C" bool ModuleSession_Tunnel_List(XCHAR*** ppptszClientList, int* pInt_ListCount)
{
	return m_Tunnel.ModuleSession_Tunnel_List(ppptszClientList, pInt_ListCount);
}