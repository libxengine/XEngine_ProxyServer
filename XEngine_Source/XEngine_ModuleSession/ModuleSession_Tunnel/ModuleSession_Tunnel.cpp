#include "pch.h"
#include "ModuleSession_Tunnel.h"
/********************************************************************
//    Created:     2024/09/06  16:29:12
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Tunnel\ModuleSession_Tunnel.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Tunnel
//    File Base:   ModuleSession_Tunnel
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     隧道代理服务端
//    History:
*********************************************************************/
CModuleSession_Tunnel::CModuleSession_Tunnel()
{
}
CModuleSession_Tunnel::~CModuleSession_Tunnel()
{
}
//////////////////////////////////////////////////////////////////////////
//                 公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_Tunnel_Create
函数功能：创建一个隧道代理客户端
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_Create(LPCXSTR lpszClientID)
{
    Session_IsErrorOccur = false;

    if (NULL == lpszClientID)
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_PARAMENT;
        return false;
    }

    PROXYTUNNEL_CLIENTINFO* pSt_TunnelInfo = new PROXYTUNNEL_CLIENTINFO;
    if (NULL == pSt_TunnelInfo)
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_MALLOC;
		return false;
    }
    memset(pSt_TunnelInfo, '\0', sizeof(PROXYTUNNEL_CLIENTINFO));

    pSt_TunnelInfo->pStl_ListField = new list<xstring>;
    if (NULL == pSt_TunnelInfo->pStl_ListField)
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_MALLOC;
		return false;
    }
    _tcsxcpy(pSt_TunnelInfo->tszClientAddr, lpszClientID);

    st_Locker.lock();
    stl_MapClient.insert(make_pair(lpszClientID, pSt_TunnelInfo));
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_Delete
函数功能：删除一个指定的客户端
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_Delete(LPCXSTR lpszClientID)
{
    Session_IsErrorOccur = false;

    st_Locker.lock();
    unordered_map<xstring, PROXYTUNNEL_CLIENTINFO*>::const_iterator stl_MapIterator = stl_MapClient.find(lpszClientID);
    if (stl_MapIterator != stl_MapClient.end())
    {
        if (NULL != stl_MapIterator->second->pStl_ListField)
        {
            delete stl_MapIterator->second->pStl_ListField;
            stl_MapIterator->second->pStl_ListField = NULL;
        }
        stl_MapClient.erase(stl_MapIterator);
    }
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_SetInfo
函数功能：设置客户端信息
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要设置的客户端句柄
 参数.三：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入绑定的句柄客户端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_SetInfo(LPCXSTR lpszClientID, XNETHANDLE xhClient, LPCXSTR lpszClientAddr)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientID) || (NULL == lpszClientAddr))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_PARAMENT;
        return false;
	}
	st_Locker.lock_shared();
	unordered_map<xstring, PROXYTUNNEL_CLIENTINFO*>::const_iterator stl_MapIterator = stl_MapClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
	}
	stl_MapIterator->second->xhClient = xhClient;
	_tcsxcpy(stl_MapIterator->second->tszClientAddr, lpszClientAddr);
	st_Locker.unlock_shared();
    return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_GetInfo
函数功能：获取客户端信息
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pxhClient
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出获取到的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_GetInfo(LPCXSTR lpszClientID, XNETHANDLE *pxhClient)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientID) || (NULL == pxhClient))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<xstring, PROXYTUNNEL_CLIENTINFO*>::const_iterator stl_MapIterator = stl_MapClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pxhClient = stl_MapIterator->second->xhClient;
	st_Locker.unlock_shared();

	return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_GetAddrForHandle
函数功能：通过句柄获取ID
 参数.一：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要获取的客户端句柄
 参数.二：ptszClientAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取的客户端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_GetAddrForHandle(XNETHANDLE xhClient, XCHAR* ptszClientAddr)
{
	Session_IsErrorOccur = false;

	bool bFound = false;

	st_Locker.lock_shared();
	unordered_map<xstring, PROXYTUNNEL_CLIENTINFO*>::const_iterator stl_MapIterator = stl_MapClient.begin();
	for (; stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		if (xhClient == stl_MapIterator->second->xhClient)
		{
			bFound = true;
			_tcsxcpy(ptszClientAddr, stl_MapIterator->first.c_str());
			break;
		}
	}
	st_Locker.unlock_shared();

	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_NOTFOUND;
		return false;
	}

	return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_SetStatus
函数功能：设置客户端状态
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：enStatus
  In/Out：In
  类型：枚举型
  可空：N
  意思：输入设置的状态
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS enStatus)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	stl_MapIterator->second->enClientStatus = enStatus;
	st_Locker.unlock_shared();

	return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_GetStatus
函数功能：获取客户端状态
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：penStatus
  In/Out：Out
  类型：枚举型
  可空：N
  意思：输出客户端的状态
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS* penStatus)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClient.find(lpszClientID);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*penStatus = stl_MapIterator->second->enClientStatus;
	st_Locker.unlock_shared();

	return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_Packet
函数功能：输入打包协议数据
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端ID
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入内容大小
 参数.四：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打包的内容
 参数.五：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_Packet(LPCXSTR lpszClientID, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientID) || (NULL == lpszMsgBuffer))
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_PARAMENT;
		return false;
    }
    st_Locker.lock_shared();
    unordered_map<xstring, PROXYTUNNEL_CLIENTINFO*>::const_iterator stl_MapIterator = stl_MapClient.find(lpszClientID);
    if (stl_MapIterator == stl_MapClient.end())
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_NOTFOUND;
        st_Locker.unlock_shared();
        return false;
    }
    memcpy(stl_MapIterator->second->tszMsgBuffer + stl_MapIterator->second->nPosLen, lpszMsgBuffer, nMsgLen);
    stl_MapIterator->second->nPosLen += nMsgLen;
    //头是否足够
    if (!Algorithm_String_XFastMatch(stl_MapIterator->second->tszMsgBuffer, _X("\r\n\r\n"), &stl_MapIterator->second->nHdrLen, stl_MapIterator->second->nPosLen))
    {
        Session_IsErrorOccur = true;
        Session_dwErrorCode = Algorithm_GetLastError();
        st_Locker.unlock_shared();
        return false;
    }
    *pInt_MSGLen = stl_MapIterator->second->nHdrLen;
    memcpy(ptszMSGBuffer, stl_MapIterator->second->tszMsgBuffer, stl_MapIterator->second->nHdrLen);
    
    stl_MapIterator->second->nHdrLen = 0;
    memset(stl_MapIterator->second->tszMsgBuffer, '\0', sizeof(stl_MapIterator->second->tszMsgBuffer));
    st_Locker.unlock_shared();
    return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_List
函数功能：获取客户端列表
 参数.一：ppptszClientList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出客户端列表,可以为NULL
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_List(XCHAR*** ppptszClientList, int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	*pInt_ListCount = stl_MapClient.size();
	if (NULL != ppptszClientList)
	{
		BaseLib_Memory_Malloc((XPPPMEM)ppptszClientList, stl_MapClient.size(), 128);
		auto stl_MapIterator = stl_MapClient.begin();
		for (int i = 0; stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
		{
			_tcsxcpy((*ppptszClientList)[i], stl_MapIterator->first.c_str());
		}
	}
	st_Locker.unlock_shared();

	return true;
}