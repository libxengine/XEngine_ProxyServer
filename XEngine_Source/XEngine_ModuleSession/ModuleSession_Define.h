﻿#pragma once
/********************************************************************
//    Created:     2022/06/08  10:10:08
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Define.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession
//    File Base:   ModuleSession_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义
//    History:
*********************************************************************/
///当前客户端状态
typedef enum 
{
	ENUM_PROXY_SESSION_SOCKS_STATUS_CREATE = 0,                  //创建已完毕
	ENUM_PROXY_SESSION_SOCKS_STATUS_AUTH = 1,                    //请求验证协议协商
	ENUM_PROXY_SESSION_SOCKS_STATUS_USER = 2,                    //用户验证请求
	ENUM_PROXY_SESSION_SOCKS_STATUS_FORWARD = 3                  //数据转发请求
}ENUM_PROXY_SESSION_SOCKS_STATUS;
typedef struct
{
	XENGINE_PROTOCOL_USERAUTH st_UserAuth;
	XCHAR tszSrcAddr[128];
	XCHAR tszDstAddr[128];
	bool bForward;
}SESSION_FORWARD, * LPSESSION_FORWARD;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleSession_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        转发导出函数                                  */
/************************************************************************/
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
extern "C" bool ModuleSession_Forward_Insert(LPCXSTR lpszAddr, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth);
/********************************************************************
函数名称：ModuleSession_Forward_List
函数功能：获取列表
 参数.一：pppSt_ListUser
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
extern "C" bool ModuleSession_Forward_List(SESSION_FORWARD * **pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr = NULL);
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
extern "C" bool ModuleSession_Forward_Bind(LPCXSTR lpszSrcAddr, LPCXSTR lpszDstAddr);
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
extern "C" bool ModuleSession_Forward_Delete(LPCXSTR lpszAddr, XCHAR * ptszDstAddr = NULL);
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
  可空：Y
  意思：输出对端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_Forward_Get(LPCXSTR lpszAddr, XCHAR* ptszDstAddr = NULL);

/************************************************************************/
/*                     SOCK代理服务器导出函数                           */
/************************************************************************/
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
extern "C" bool ModuleSession_Socks_Create(LPCXSTR lpszClientID);
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
extern "C" bool ModuleSession_Socks_Delete(LPCXSTR lpszClientID);
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
extern "C" bool ModuleSession_Socks_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen);
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
extern "C" bool ModuleSession_Socks_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len = NULL);
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
extern "C" bool ModuleSession_Socks_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize);
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
extern "C" bool ModuleSession_Socks_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS* penSocks);
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
extern "C" bool ModuleSession_Socks_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS enStatus);
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
extern "C" bool ModuleSession_Socks_List(XCHAR*** ppptszClientList, int* pInt_ListCount);
/************************************************************************/
/*                     隧道代理服务                                     */
/************************************************************************/
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
extern "C" bool ModuleSession_Tunnel_Create(LPCXSTR lpszClientID);
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
extern "C" bool ModuleSession_Tunnel_Delete(LPCXSTR lpszClientID);
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
extern "C" bool ModuleSession_Tunnel_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen);
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
extern "C" bool ModuleSession_Tunnel_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len = NULL);
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
extern "C" bool ModuleSession_Tunnel_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize);
/********************************************************************
函数名称：ModuleSession_Tunnel_Packet
函数功能：解析客户端的隧道代理协议
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
extern "C" bool ModuleSession_Tunnel_Packet(LPCXSTR lpszClientID, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen);
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
extern "C" bool ModuleSession_Tunnel_List(XCHAR*** ppptszClientList, int* pInt_ListCount);