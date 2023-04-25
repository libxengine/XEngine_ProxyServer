#pragma once
/********************************************************************
//    Created:     2022/06/08  11:28:19
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Define.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol
//    File Base:   ModuleProtocol_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleProtocol_GetLastError(int* pInt_SysError = NULL);
/************************************************************************/
/*                        封包导出函数                                  */
/************************************************************************/
/********************************************************************
函数名称：ModuleProtocol_Packet_ForwardList
函数功能：转发协议封装请求
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出封装好的包
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出封装大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：协议头
  可空：N
  意思：输入请求的头
 参数.四：ppptszListAddr
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的列表
 参数.五：nCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleProtocol_Packet_ForwardList(XCHAR* ptszMsgBuffer, int* pInt_Len, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XCHAR*** ppptszListAddr, int nCount);
/************************************************************************/
/*                        解析导出函数                                  */
/************************************************************************/
/********************************************************************
函数名称：ModuleProtocol_Packet_ForwardList
函数功能：转发协议封装请求
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入解析的大小
 参数.三：ptszSrcAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析的原始地址
 参数.四：ptszDstAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析的目的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleProtocol_Parse_ForwardBind(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszSrcAddr, XCHAR * ptszDstAddr);