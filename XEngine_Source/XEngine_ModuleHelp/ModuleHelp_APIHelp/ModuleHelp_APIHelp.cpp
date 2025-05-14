#include "pch.h"
#include "ModuleHelp_APIHelp.h"
/********************************************************************
//    Created:     2025/05/14  15:11:37
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_APIHelp\ModuleHelp_APIHelp.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_APIHelp
//    File Base:   ModuleHelp_APIHelp
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     帮助函数
//    History:
*********************************************************************/
CModuleHelp_APIHelp::CModuleHelp_APIHelp()
{

}
CModuleHelp_APIHelp::~CModuleHelp_APIHelp()
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
bool CModuleHelp_APIHelp::ModuleHelp_APIHelp_RuleIPSelect(SESSION_IPCONUT*** pppSt_IPCount, int nIPCount)
{
	APIHelp_IsErrorOccur = false;

	ModuleHelp_APIHelp_QSort(pppSt_IPCount, nIPCount, sizeof(SESSION_IPCONUT));
	for (int i = 0; i < nIPCount; i++)
	{

	}
	return true;
}
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
bool CModuleHelp_APIHelp::ModuleHelp_APIHelp_QSort(XPVOID lPBase, int nNumber, int nSize)
{
	APIHelp_IsErrorOccur = false;

    qsort(lPBase, nNumber, nSize, ModuleHelp_APIHelp_QSortCmp);

    return true;
}
//////////////////////////////////////////////////////////////////////////
//                     保护函数
//////////////////////////////////////////////////////////////////////////
int CModuleHelp_APIHelp::ModuleHelp_APIHelp_QSortCmp(const void* lParam, const void* WParam)
{
	int nAValue = *(const int*)lParam;
	int nBValue = *(const int*)WParam;

	return nAValue - nBValue;
}