#include "pch.h"
#include "ModuleSession_Proxy.h"
/********************************************************************
//    Created:     2025/01/20  11:13:37
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy\ModuleSession_Proxy.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy
//    File Base:   ModuleSession_Proxy
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     全代理转发
//    History:
*********************************************************************/
CModuleSession_Proxy::CModuleSession_Proxy()
{

}
CModuleSession_Proxy::~CModuleSession_Proxy()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_Proxy_Insert
函数功能：插入一条记录到会话中
 参数.一：lpszSrcIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要插入的源客户端
 参数.二：lpszDstIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要插入的目标客户端
 参数.三：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入客户端句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Proxy::ModuleSession_Proxy_Insert(LPCXSTR lpszSrcIPAddr, LPCXSTR lpszDstIPAddr, XNETHANDLE xhClient)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszSrcIPAddr || NULL == lpszDstIPAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	SESSION_FORWARD st_ProxyClient = {};

	st_ProxyClient.xhClient = xhClient;
	_tcsxcpy(st_ProxyClient.tszSrcAddr, lpszSrcIPAddr);
	_tcsxcpy(st_ProxyClient.tszDstAddr, lpszDstIPAddr);

	st_Locker.lock();
	unordered_map<xstring, SESSION_FORWARD>::const_iterator stl_MapIterator = stl_MapSession.find(lpszSrcIPAddr);
	if (stl_MapIterator != stl_MapSession.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_EXIST;
		st_Locker.unlock();
		return false;
	}
	stl_MapSession.insert(make_pair(lpszSrcIPAddr, st_ProxyClient));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Proxy_GetForAddr
函数功能：通过地址获取客户端信息
 参数.一：lpszIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取的客户端
 参数.二：pSt_ProxyInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Proxy::ModuleSession_Proxy_GetForAddr(LPCXSTR lpszIPAddr, SESSION_FORWARD* pSt_ProxyInfo)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszIPAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapSession.find(lpszIPAddr);
	if (stl_MapIterator == stl_MapSession.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pSt_ProxyInfo = stl_MapIterator->second;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Proxy_GetForToken
函数功能：通过TOKEN获取客户端信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要获取的客户端
 参数.二：pSt_ProxyInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Proxy::ModuleSession_Proxy_GetForToken(XNETHANDLE xhToken, SESSION_FORWARD* pSt_ProxyInfo)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	bool bFound = false;
	auto stl_MapIterator = stl_MapSession.begin();
	for (; stl_MapIterator != stl_MapSession.end(); stl_MapIterator++)
	{
		if (xhToken == stl_MapIterator->second.xhClient)
		{
			bFound = true;
			*pSt_ProxyInfo = stl_MapIterator->second;
			break;
		}
	}
	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Proxy_List
函数功能：获取列表
 参数.一：pppSt_ListUser
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出列表
 参数.二：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
 参数.三：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入忽略地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Proxy::ModuleSession_Proxy_List(SESSION_FORWARD*** pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr /* = NULL */)
{
	Session_IsErrorOccur = false;

	if (NULL == pInt_Count)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();

	if (NULL == lpszAddr)
	{
		*pInt_Count = stl_MapSession.size(); 
	}
	else
	{
		*pInt_Count = stl_MapSession.size() - 1;   //减去自己
	}
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_ListUser, *pInt_Count, sizeof(SESSION_FORWARD));
	//遍历
	auto stl_MapIterator = stl_MapSession.begin();
	for (int i = 0; stl_MapIterator != stl_MapSession.end(); stl_MapIterator++, i++)
	{
		if (NULL != lpszAddr)
		{
			if (0 == _tcsxncmp(lpszAddr, stl_MapIterator->first.c_str(), _tcsxlen(lpszAddr)))
			{
				continue;
			}
		}
		*(*pppSt_ListUser)[i] = stl_MapIterator->second;
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Proxy_Delete
函数功能：删除用户
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要删除的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Proxy::ModuleSession_Proxy_Delete(LPCXSTR lpszIPAddr)
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	auto stl_MapSrcIterator = stl_MapSession.find(lpszIPAddr);
	if (stl_MapSrcIterator != stl_MapSession.end())
	{
		stl_MapSession.erase(stl_MapSrcIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Proxy_GetIPCount
函数功能：获取IP目标转发地址的统计
 参数.一：pppSt_IPCount
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出IP目标转发地址列表信息
 参数.二：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Proxy::ModuleSession_Proxy_GetIPCount(SESSION_IPCONUT ***pppSt_IPCount, int* pInt_Count)
{
	Session_IsErrorOccur = false;

	if (NULL == pppSt_IPCount || NULL == pInt_Count)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	std::list<SESSION_IPCONUT> stl_ListIPCount;
	st_Locker.lock_shared();
	//遍历
	for (auto stl_MapIterator = stl_MapSession.begin(); stl_MapIterator != stl_MapSession.end(); stl_MapIterator++)
	{
		//遍历已有的列表
		bool bFound = false;
		for (auto stl_ListIterator = stl_ListIPCount.begin(); stl_ListIterator != stl_ListIPCount.end(); stl_ListIterator++)
		{
			if (0 == _tcsxnicmp(stl_ListIterator->tszIPAddr, stl_MapIterator->second.tszDstAddr, _tcsxlen(stl_ListIterator->tszIPAddr)))
			{
				bFound = true;
				stl_ListIterator->nIPCount++;
				break;
			}
		}
		//是否找到
		if (!bFound)
		{
			//没有找到加入新的列表
			SESSION_IPCONUT st_IPCount = {};
			st_IPCount.nIPCount = 1;
			_tcsxcpy(st_IPCount.tszIPAddr, stl_MapIterator->second.tszDstAddr);
			stl_ListIPCount.push_back(st_IPCount);
		}
	}
	st_Locker.unlock_shared();

	*pInt_Count = stl_ListIPCount.size();
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_IPCount, *pInt_Count, sizeof(SESSION_IPCONUT));

	auto stl_ListIterator = stl_ListIPCount.begin();
	for (int i = 0; stl_ListIterator != stl_ListIPCount.end(); stl_ListIterator++)
	{
		*(*pppSt_IPCount)[i] = *stl_ListIterator;
	}
	return true;
}