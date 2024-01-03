#include "pch.h"
#include "ModuleSession_Forward.h"
/********************************************************************
//    Created:     2022/06/08  09:58:49
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward\ModuleSession_Forward.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward
//    File Base:   ModuleSession_Forward
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话转发协议
//    History:
*********************************************************************/
CModuleSession_Forward::CModuleSession_Forward()
{

}
CModuleSession_Forward::~CModuleSession_Forward()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_Forward_Insert
函数功能：插入一条记录到会话中
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要插入的客户端
 参数.二：pSt_UserAuth
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要保存的客户端附加数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Forward::ModuleSession_Forward_Insert(LPCXSTR lpszAddr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_PARAMENT;
		return false;
	}
	SESSION_FORWARD st_Forward;
	memset(&st_Forward, '\0', sizeof(SESSION_FORWARD));

	_tcsxcpy(st_Forward.tszSrcAddr, lpszAddr);
	memcpy(&st_Forward.st_UserAuth, pSt_UserAuth, sizeof(XENGINE_PROTOCOL_USERAUTH));

	st_Locker.lock();
	unordered_map<tstring, SESSION_FORWARD>::const_iterator stl_MapIterator = stl_MapSession.find(lpszAddr);
	if (stl_MapIterator != stl_MapSession.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_EXIST;
		st_Locker.unlock();
		return false;
	}
	stl_MapSession.insert(make_pair(lpszAddr, st_Forward));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Forward_List
函数功能：获取列表
 参数.一：ppptszListAddr
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出地址列表
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
bool CModuleSession_Forward::ModuleSession_Forward_List(SESSION_FORWARD*** pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr /* = NULL */)
{
	Session_IsErrorOccur = false;

	if (NULL == pInt_Count)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_PARAMENT;
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
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ListUser, *pInt_Count, sizeof(SESSION_FORWARD));
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
函数名称：ModuleSession_Forward_Bind
函数功能：绑定转发需求
 参数.一：lpszSrcAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入绑定的原始地址
 参数.二：lpszDstAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输出绑定的目标地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Forward::ModuleSession_Forward_Bind(LPCXSTR lpszSrcAddr, LPCXSTR lpszDstAddr)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszSrcAddr) || (NULL == lpszDstAddr))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	//查找
	auto stl_MapSrcIterator = stl_MapSession.find(lpszSrcAddr);
	auto stl_MapDstIterator = stl_MapSession.find(lpszDstAddr);
	if (stl_MapSrcIterator == stl_MapSession.end() || stl_MapDstIterator == stl_MapSession.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	//如果设置过,不允许在设置
	if (stl_MapSrcIterator->second.bForward || stl_MapDstIterator->second.bForward)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_BIND;
		st_Locker.unlock_shared();
		return false;
	}
	//需要设置两方的转发内容
	stl_MapSrcIterator->second.bForward = true;
	_tcsxcpy(stl_MapSrcIterator->second.tszDstAddr, lpszDstAddr);

	stl_MapDstIterator->second.bForward = true;
	_tcsxcpy(stl_MapDstIterator->second.tszDstAddr, lpszSrcAddr);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Forward_Delete
函数功能：删除用户
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要删除的客户端
 参数.二：ptszDstAddr
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出解绑的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Forward::ModuleSession_Forward_Delete(LPCXSTR lpszAddr, XCHAR* ptszDstAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_PARAMENT;
		return false;
	}
	st_Locker.lock();
	//查找
	auto stl_MapSrcIterator = stl_MapSession.find(lpszAddr);
	if (stl_MapSrcIterator == stl_MapSession.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_NOTFOUND;
		st_Locker.unlock();
		return false;
	}
	//如果有转发,需要清理对方的转发设置
	if (stl_MapSrcIterator->second.bForward)
	{
		if (NULL != ptszDstAddr)
		{
			_tcsxcpy(ptszDstAddr, stl_MapSrcIterator->second.tszDstAddr);
		}
		
		auto stl_MapDstIterator = stl_MapSession.find(stl_MapSrcIterator->second.tszSrcAddr);
		if (stl_MapDstIterator == stl_MapSession.end())
		{
			stl_MapDstIterator->second.bForward = false;
			memset(stl_MapDstIterator->second.tszDstAddr, '\0', sizeof(stl_MapDstIterator->second.tszDstAddr));
		}
	}
	stl_MapSession.erase(stl_MapSrcIterator);
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Forward_Get
函数功能：获取转发用户给
 参数.一：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：ptszDstAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出对端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Forward::ModuleSession_Forward_Get(LPCXSTR lpszAddr, XCHAR* ptszDstAddr)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszAddr) || (NULL == ptszDstAddr))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	//查找
	auto stl_MapIterator = stl_MapSession.find(lpszAddr);
	if (stl_MapIterator == stl_MapSession.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	//如果有转发,需要清理对方的转发设置
	if (!stl_MapIterator->second.bForward)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_FORWARD_NOTFORWARD;
		st_Locker.unlock_shared();
		return false;
	}
	_tcsxcpy(ptszDstAddr, stl_MapIterator->second.tszDstAddr);
	st_Locker.unlock_shared();
	return true;
}