#pragma once
/********************************************************************
//    Created:     2021/12/29  10:30:04
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuth_Define.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize
//    File Base:   ModuleAuth_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleAuthorize_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        验证导出函数                                  */
/************************************************************************/
/********************************************************************
函数名称：ModuleAuthorize_User_Init
函数功能：初始化授权会话管理器
 参数.一：lpszAuthFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入授权文件地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleAuthorize_User_Init(LPCXSTR lpszAuthFile);
/************************************************************************
函数名称：ModuleAuthorize_User_Destory
函数功能：销毁授权管理器
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
************************************************************************/
extern "C" bool ModuleAuthorize_User_Destory();
/********************************************************************
函数名称：ModuleAuthorize_User_Exist
函数功能：是否验证通过
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleAuthorize_User_Exist(LPCXSTR lpszUser, LPCXSTR lpszPass);