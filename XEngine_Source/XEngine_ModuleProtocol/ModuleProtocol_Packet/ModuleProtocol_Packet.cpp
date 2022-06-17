#include "pch.h"
#include "ModuleProtocol_Packet.h"
/********************************************************************
//    Created:     2022/06/08  13:55:09
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet\ModuleProtocol_Packet.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet
//    File Base:   ModuleProtocol_Packet
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     封装协议
//    History:
*********************************************************************/
CModuleProtocol_Packet::CModuleProtocol_Packet()
{

}
CModuleProtocol_Packet::~CModuleProtocol_Packet()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
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
BOOL CModuleProtocol_Packet::ModuleProtocol_Packet_ForwardList(TCHAR* ptszMsgBuffer, int* pInt_Len, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, TCHAR*** ppptszListAddr, int nCount)
{
	Protocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_Len))
	{
		Protocol_IsErrorOccur = TRUE;
		Protocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	for (int i = 0; i < nCount; i++)
	{
		st_JsonArray[i] = (*ppptszListAddr)[i];
	}
	st_JsonRoot["Count"] = nCount;
	st_JsonRoot["Array"] = st_JsonArray;

	pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();

	*pInt_Len = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
	memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), pSt_ProtocolHdr->unPacketSize);
	return TRUE;
}