#pragma once
/********************************************************************
//    Created:     2025/05/14  14:29:52
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Define.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp
//    File Base:   ModuleHelp_Define
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     导出定义
//    History:
*********************************************************************/          
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleHelp_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        帮助函数                                      */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_APIHelp_QSort
函数功能：快速排序算法
 参数.一：lPBase
  In/Out：In/Out
  类型：五类型指针
  可空：N
  意思：输入：要排序的结构,输出：排序好的
 参数.二：nNumber
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要排序的个数
 参数.三：nSize
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要排序的结构大小
返回值
  类型：逻辑型
  意思：是否排序成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_APIHelp_QSort(XPVOID lPBase, int nNumber, int nSize);