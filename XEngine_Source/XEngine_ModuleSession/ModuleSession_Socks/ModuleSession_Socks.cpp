#include "pch.h"
#include "ModuleSession_Socks.h"
/********************************************************************
//    Created:     2024/09/06  13:48:58
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Socks\ModuleSession_Socks.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Socks
//    File Base:   ModuleSession_Socks
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     代理服务器SOCK5
//    History:
*********************************************************************/
CModuleSession_Socks::CModuleSession_Socks()
{
}
CModuleSession_Socks::~CModuleSession_Socks()
{
}
//////////////////////////////////////////////////////////////////////////
//                 公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_Socks_Create
函数功能：创建一个客户端
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
bool CModuleSession_Socks::ModuleSession_Socks_Create(LPCXSTR lpszClientID)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientID)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	RFCPROTOCOL_SOCKS5CLIENT* pSt_SocksClinet = new RFCPROTOCOL_SOCKS5CLIENT;
	if (NULL == pSt_SocksClinet)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_MALLOC;
		return false;
	}
	memset(pSt_SocksClinet, '\0', sizeof(RFCPROTOCOL_SOCKS5CLIENT));

	pSt_SocksClinet->enProxyStatus = ENUM_PROXY_SESSION_CLIENT_CREATE;
	
	st_Locker.lock();
	stl_MapClients.insert(make_pair(lpszClientID, pSt_SocksClinet));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_Delete
函数功能：删除客户端
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
bool CModuleSession_Socks::ModuleSession_Socks_Delete(LPCXSTR lpszClientID)
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	unordered_map<xstring, RFCPROTOCOL_SOCKS5CLIENT*>::const_iterator stl_MapIterator = stl_MapClients.find(lpszClientID);
	if (stl_MapIterator != stl_MapClients.end())
	{
		stl_MapClients.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_SetInfo
函数功能：设置自定义信息
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入客户端网络句柄
 参数.三：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：绑定的客户端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_SetInfo(LPCXSTR lpszClientID, XNETHANDLE xhClient, LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientID))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	st_Locker.lock();
	unordered_map<xstring, RFCPROTOCOL_SOCKS5CLIENT*>::const_iterator stl_MapIterator = stl_MapClients.find(lpszClientID);
	if (stl_MapIterator == stl_MapClients.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_NOTFOUND;
		st_Locker.unlock();
		return false;
	}
	stl_MapIterator->second->xhClient = xhClient;
	_tcsxcpy(stl_MapIterator->second->tszClientAddr, lpszClientAddr);
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_GetInfo
函数功能：获取自定义信息
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pxhClient
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出客户端网络句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_GetInfo(LPCXSTR lpszClientID, XNETHANDLE *pxhClient)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientID))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<xstring, RFCPROTOCOL_SOCKS5CLIENT*>::const_iterator stl_MapIterator = stl_MapClients.find(lpszClientID);
	if (stl_MapIterator == stl_MapClients.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pxhClient = stl_MapIterator->second->xhClient;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_GetHandleForAddr
函数功能：通过客户端地址获取句柄
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取的客户端地址
 参数.二：pxhClient
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_GetHandleForAddr(LPCXSTR lpszClientAddr, XNETHANDLE* pxhClient)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientAddr))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	bool bFound = false;

	st_Locker.lock_shared();
	for (auto stl_MapIterator = stl_MapClients.begin(); stl_MapIterator != stl_MapClients.end(); stl_MapIterator++)
	{
		if (0 == _tcsxnicmp(lpszClientAddr, stl_MapIterator->second->tszClientAddr, _tcsxlen(lpszClientAddr)))
		{
			bFound = true;
			*pxhClient = stl_MapIterator->second->xhClient;
			break;
		}
	}
	st_Locker.unlock_shared();

	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_NOTFOUND;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_GetAddrForHandle
函数功能：通过客户端句柄来获得对应的IP地址
 参数.一：pxhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入句柄
 参数.二：lpszClientAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取的客户端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_GetAddrForHandle(XNETHANDLE xhClient, XCHAR* ptszClientAddr)
{
	Session_IsErrorOccur = false;

	if ((NULL == ptszClientAddr))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	bool bFound = false;

	st_Locker.lock_shared();
	for (auto stl_MapIterator = stl_MapClients.begin(); stl_MapIterator != stl_MapClients.end(); stl_MapIterator++)
	{
		if (xhClient == stl_MapIterator->second->xhClient)
		{
			_tcsxcpy(ptszClientAddr, stl_MapIterator->first.c_str());
			bFound = true;
			break;
		}
	}
	st_Locker.unlock_shared();

	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_NOTFOUND;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_GetStatus
函数功能：获取客户端状态
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端ID
 参数.二：penSocks
  In/Out：Out
  类型：枚举类型
  可空：N
  意思：输出当前客户端状态
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS* penSocks)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientID) || (NULL == penSocks))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<xstring, RFCPROTOCOL_SOCKS5CLIENT*>::const_iterator stl_MapIterator = stl_MapClients.find(lpszClientID);
	if (stl_MapIterator == stl_MapClients.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*penSocks = stl_MapIterator->second->enProxyStatus;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_SetStatus
函数功能：设置客户端状态
 参数.一：lpszClientID
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端ID
 参数.二：enStatus
  In/Out：In
  类型：枚举类型
  可空：N
  意思：输入客户端状态
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS enStatus)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientID)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<xstring, RFCPROTOCOL_SOCKS5CLIENT*>::const_iterator stl_MapIterator = stl_MapClients.find(lpszClientID);
	if (stl_MapIterator == stl_MapClients.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	stl_MapIterator->second->enProxyStatus = enStatus;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_List
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
bool CModuleSession_Socks::ModuleSession_Socks_List(XCHAR*** ppptszClientList, int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	if (NULL == pInt_ListCount)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();

	*pInt_ListCount = stl_MapClients.size();
	if (NULL != ppptszClientList)
	{
		BaseLib_Memory_Malloc((XPPPMEM)ppptszClientList, stl_MapClients.size(), 128);
		auto stl_MapIterator = stl_MapClients.begin();
		for (int i = 0; stl_MapIterator != stl_MapClients.end(); stl_MapIterator++)
		{
			_tcsxcpy((*ppptszClientList)[i], stl_MapIterator->first.c_str());
		}
	}
	st_Locker.unlock_shared();

	return true;
}