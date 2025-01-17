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
		if (NULL != stl_MapIterator->second->lParam)
		{
			free(stl_MapIterator->second->lParam);
			stl_MapIterator->second->lParam = NULL;
		}
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
 参数.二：lParam
  In/Out：In
  类型：无类型指针
  可空：N
  意思：输入要设置的内容
 参数.三：nLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入自定义内容大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen)
{
    Session_IsErrorOccur = false;

    if ((NULL == lpszClientID) || (NULL == lParam))
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
    if (NULL == stl_MapIterator->second->lParam)
    {
        stl_MapIterator->second->lParam = malloc(nLen);
        if (NULL == stl_MapIterator->second->lParam)
        {
			Session_IsErrorOccur = true;
			Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_MALLOC;
			st_Locker.unlock_shared();
			return false;
        }
    }
    else
    {
        stl_MapIterator->second->lParam = realloc(stl_MapIterator->second->lParam, nLen);
		if (NULL == stl_MapIterator->second->lParam)
		{
			Session_IsErrorOccur = true;
			Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_MALLOC;
			st_Locker.unlock_shared();
			return false;
		}
    }
    stl_MapIterator->second->nCtmLen = nLen;
    memcpy(stl_MapIterator->second->lParam, lParam, nLen);
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
 参数.二：lParam
  In/Out：Out
  类型：无类型指针
  可空：N
  意思：输出获取到的内容
 参数.三：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出自定义内容大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Tunnel::ModuleSession_Tunnel_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len /* = NULL */)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientID) || (NULL == lParam))
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
    //是否设置有值
    if (NULL == stl_MapIterator->second->lParam)
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_MODULE_SESSION_TUNNEL_NOTSET;
		st_Locker.unlock_shared();
		return false;
    }
	if (NULL != pInt_Len)
	{
		*pInt_Len = stl_MapIterator->second->nCtmLen;
	}
    memcpy(lParam, stl_MapIterator->second->lParam, stl_MapIterator->second->nCtmLen);
	st_Locker.unlock_shared();

	return true;
}
/********************************************************************
函数名称：ModuleSession_Tunnel_GetList
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
bool CModuleSession_Tunnel::ModuleSession_Tunnel_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();

	*pInt_Count = stl_MapClient.size();
	BaseLib_Memory_Malloc(xpppMem, *pInt_Count, nSize);
	unordered_map<xstring, PROXYTUNNEL_CLIENTINFO*>::const_iterator stl_MapIterator = stl_MapClient.begin();
    for (int i = 0; stl_MapIterator != stl_MapClient.end(); stl_MapIterator++, i++)
	{
		//是否设置有值
		if ((NULL != stl_MapIterator->second->lParam) && (stl_MapIterator->second->nCtmLen > 0))
		{
			memcpy((*xpppMem)[i], stl_MapIterator->second->lParam, stl_MapIterator->second->nCtmLen);
		}
        else
        {
            memset((*xpppMem)[i], '\0', nSize);
        }
	}
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