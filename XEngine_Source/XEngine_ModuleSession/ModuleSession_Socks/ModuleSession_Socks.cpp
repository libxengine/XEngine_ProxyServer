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

	_tcsxcpy(pSt_SocksClinet->tszClientAddr, lpszClientID);
	pSt_SocksClinet->enProxyStatus = ENUM_PROXY_SESSION_SOCKS_STATUS_CREATE;
	
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
		if (NULL != stl_MapIterator->second->lParam)
		{
			free(stl_MapIterator->second->lParam);
			stl_MapIterator->second->lParam = NULL;
		}
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
 参数.二：lParam
  In/Out：In
  类型：无类型指针
  可空：N
  意思：输入设置的内容
 参数.三：nLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要设置内容大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientID) || (NULL == lParam))
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
	//内存是否已经申请
	if (NULL == stl_MapIterator->second->lParam)
	{
		stl_MapIterator->second->lParam = malloc(nLen);
		if (NULL == stl_MapIterator->second->lParam)
		{
			Session_IsErrorOccur = true;
			Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_MALLOC;
			st_Locker.unlock();
			return false;
		}
	}
	else
	{
		stl_MapIterator->second->lParam = realloc(stl_MapIterator->second->lParam, nLen);
		if (NULL == stl_MapIterator->second->lParam)
		{
			Session_IsErrorOccur = true;
			Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_MALLOC;
			st_Locker.unlock();
			return false;
		}
	}
	stl_MapIterator->second->nCTMLen = nLen;
	memcpy(stl_MapIterator->second->lParam, lParam, nLen);
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
 参数.二：lParam
  In/Out：Out
  类型：无类型指针
  可空：N
  意思：输出获取到的内容
 参数.三：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出内容大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len /* = NULL */)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientID) || (NULL == lParam))
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
	if (NULL == stl_MapIterator->second->lParam)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_SOCKS_NOTSET;
		st_Locker.unlock_shared();
		return false;
	}
	memcpy(lParam, stl_MapIterator->second->lParam, stl_MapIterator->second->nCTMLen);
	if (NULL != pInt_Len)
	{
		*pInt_Len = stl_MapIterator->second->nCTMLen;
	}
	
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Socks_GetList
函数功能：获取所有自定义数据
 参数.一：xpppMem
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出获取到的列表
 参数.二：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
 参数.三：nSize
  In/Out：Out
  类型：整数型
  可空：N
  意思：输入每个成员的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Socks::ModuleSession_Socks_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();

	*pInt_Count = stl_MapClients.size();
	BaseLib_Memory_Malloc(xpppMem, *pInt_Count, nSize);

	unordered_map<xstring, RFCPROTOCOL_SOCKS5CLIENT*>::const_iterator stl_MapIterator = stl_MapClients.begin();
	for (int i = 0; stl_MapIterator != stl_MapClients.end(); stl_MapIterator++, i++)
	{
		//是否设置有值
		if ((NULL != stl_MapIterator->second->lParam) && (stl_MapIterator->second->nCTMLen > 0))
		{
			memcpy((*xpppMem)[i], stl_MapIterator->second->lParam, stl_MapIterator->second->nCTMLen);
		}
	}
	st_Locker.unlock_shared();

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
bool CModuleSession_Socks::ModuleSession_Socks_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS* penSocks)
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
bool CModuleSession_Socks::ModuleSession_Socks_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS enStatus)
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