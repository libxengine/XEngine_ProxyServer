#pragma once
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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_Forward_Insert(LPCXSTR lpszAddr);
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
extern "C" bool ModuleSession_Forward_List(XCHAR*** ppptszListAddr, int* pInt_Count, LPCXSTR lpszAddr = NULL);
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
  可空：N
  意思：输出解绑的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_Forward_Delete(LPCXSTR lpszAddr, XCHAR * ptszDstAddr);
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
extern "C" bool ModuleSession_Forward_Get(LPCXSTR lpszAddr, XCHAR* ptszDstAddr);