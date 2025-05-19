#include "pch.h"
#include "ModuleSession_ProxyRule.h"
/********************************************************************
//    Created:     2025/05/15  11:35:12
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy\ModuleSession_ProxyRule.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy
//    File Base:   ModuleSession_ProxyRule
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     负载转发规则处理
//    History:
*********************************************************************/
CModuleSession_ProxyRule::CModuleSession_ProxyRule()
{

}
CModuleSession_ProxyRule::~CModuleSession_ProxyRule()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_ProxyRule_Insert
函数功能：插入一个后台服务
 参数.一：lpszIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要插入的后台服务
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_ProxyRule::ModuleSession_ProxyRule_Insert(LPCXSTR lpszIPAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszIPAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	std::list<SESSION_PROXYRULE> stl_ListRule;

	st_Locker.lock();
	stl_MapProxyRule.insert(make_pair(lpszIPAddr, stl_ListRule));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_ProxyRule_Delete
函数功能：删除一个后台服务地址
 参数.一：lpszIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的后台服务地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_ProxyRule::ModuleSession_ProxyRule_Delete(LPCXSTR lpszIPAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszIPAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	st_Locker.lock();
	auto stl_MapIterator = stl_MapProxyRule.find(lpszIPAddr);
	if (stl_MapIterator != stl_MapProxyRule.end())
	{
		stl_MapProxyRule.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_ProxyRule_Set
函数功能：设置后台服务信息
 参数.一：lpszIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的地址
 参数.二：lpszUseAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入使用的客户端地址
 参数.三：bAdd
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：添加还是删除
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_ProxyRule::ModuleSession_ProxyRule_Set(LPCXSTR lpszIPAddr, LPCXSTR lpszUseAddr, bool bAdd /* = true */)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszIPAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	st_Locker.lock();
	auto stl_MapIterator = stl_MapProxyRule.find(lpszIPAddr);
	if (stl_MapIterator == stl_MapProxyRule.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_NOTFOUND;
		st_Locker.unlock();
		return false;
	}
	//增加还是删除
	if (bAdd)
	{
		SESSION_PROXYRULE st_ProxyRule = {};
		_tcsxcpy(st_ProxyRule.tszIPAddr, lpszUseAddr);
		stl_MapIterator->second.push_back(st_ProxyRule);
	}
	else
	{
		for (auto stl_ListIterator = stl_MapIterator->second.begin(); stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
		{
			if (0 == _tcsxnicmp(lpszUseAddr, stl_ListIterator->tszIPAddr, _tcsxlen(lpszUseAddr)))
			{
				stl_MapIterator->second.erase(stl_ListIterator);
				break;
			}
		}
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_ProxyRule_GetCount
函数功能：获取统计信息
 参数.一：lpszIPAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的地址
 参数.二：pInt_Count
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_ProxyRule::ModuleSession_ProxyRule_GetCount(LPCXSTR lpszIPAddr, int* pInt_Count)
{
	Session_IsErrorOccur = false;

	if (NULL == pInt_Count)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapProxyRule.find(lpszIPAddr);
	if (stl_MapIterator == stl_MapProxyRule.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pInt_Count = stl_MapIterator->second.size();
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_ProxyRule_GetList
函数功能：获取所有列表
 参数.一：pppSt_IPCount
  In/Out：In
  类型：三级指针
  可空：N
  意思：输出获取到的列表信息
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
bool CModuleSession_ProxyRule::ModuleSession_ProxyRule_GetList(SESSION_IPCONUT*** pppSt_IPCount, int* pInt_Count)
{
	Session_IsErrorOccur = false;

	if (NULL == pppSt_IPCount || NULL == pInt_Count)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_PROXY_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	*pInt_Count = stl_MapProxyRule.size();
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_IPCount, *pInt_Count, sizeof(SESSION_IPCONUT));
	//遍历
	auto stl_MapIterator = stl_MapProxyRule.begin();
	for (int i = 0; stl_MapIterator != stl_MapProxyRule.end(); stl_MapIterator++, i++)
	{
		(*pppSt_IPCount)[i]->nIPCount = stl_MapIterator->second.size();
		_tcsxcpy((*pppSt_IPCount)[i]->tszIPAddr, stl_MapIterator->first.c_str());
	}
	st_Locker.unlock_shared();
	return true;
}