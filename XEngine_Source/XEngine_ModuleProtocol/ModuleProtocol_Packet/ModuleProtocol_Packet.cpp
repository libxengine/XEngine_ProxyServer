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
函数名称：ModuleProtocol_Packet_Comm
函数功能：打包一个通用回复消息
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出打好包的JSON数据信息
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出打包大小
 参数.三：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入返回的错误码
 参数.四：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入错误信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_Comm(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, int nCode /* = 0 */, LPCXSTR lpszMSGBuffer /* = _X("success") */)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMSGBuffer) || (NULL == pInt_MSGLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::StreamWriterBuilder st_JsonWBuilder;
	//JSON赋值
	st_JsonRoot["code"] = nCode;
	st_JsonRoot["msg"] = lpszMSGBuffer;

	*pInt_MSGLen = (int)Json::writeString(st_JsonWBuilder, st_JsonRoot).length();
	memcpy(ptszMSGBuffer, Json::writeString(st_JsonWBuilder, st_JsonRoot).c_str(), *pInt_MSGLen);
	return true;
}
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
 参数.三：pppSt_ListUser
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的列表
 参数.四：nCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
 参数.五：pSt_ProtocolHdr
  In/Out：In
  类型：协议头
  可空：N
  意思：输入请求的头
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_ForwardList(XCHAR* ptszMsgBuffer, int* pInt_Len, SESSION_FORWARD*** pppSt_ListUser, int nCount, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_Len))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	for (int i = 0; i < nCount; i++)
	{
		Json::Value st_JsonObject;
		Json::Value st_JsonSub;
		st_JsonObject["bForward"] = (*pppSt_ListUser)[i]->bForward;
		st_JsonObject["tszSrcAddr"] = (*pppSt_ListUser)[i]->tszSrcAddr;
		st_JsonObject["tszDstAddr"] = (*pppSt_ListUser)[i]->tszDstAddr;

		st_JsonSub["tszUserName"] = (*pppSt_ListUser)[i]->st_UserAuth.tszUserName;
		st_JsonSub["tszUserPass"] = (*pppSt_ListUser)[i]->st_UserAuth.tszUserPass;
		st_JsonSub["tszDCode"] = (*pppSt_ListUser)[i]->st_UserAuth.tszDCode;
		st_JsonSub["enClientType"] = (*pppSt_ListUser)[i]->st_UserAuth.enClientType;
		st_JsonSub["enDeviceType"] = (*pppSt_ListUser)[i]->st_UserAuth.enDeviceType;

		st_JsonObject["st_UserAuth"] = st_JsonSub;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";
	st_JsonRoot["Count"] = nCount;
	st_JsonRoot["Array"] = st_JsonArray;

	if (NULL == pSt_ProtocolHdr)
	{
		*pInt_Len = st_JsonRoot.toStyledString().length();
		memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_Len);
	}
	else
	{
		pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();

		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), pSt_ProtocolHdr->unPacketSize);
		*pInt_Len = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
	}
	
	return true;
}
/********************************************************************
函数名称：ModuleProtocol_Packet_GetProxyRuleList
函数功能：代理转发规则列表信息
 参数.一：ptszMSGBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出封装好的包
 参数.二：pInt_MSGLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出封装大小
 参数.三：pppSt_IPCount
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入要处理的列表
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_GetProxyRuleList(XCHAR* ptszMSGBuffer, int* pInt_MSGLen, SESSION_IPCONUT*** pppSt_IPCount, int nListCount)
{
	Protocol_IsErrorOccur = false;

	if ((NULL == ptszMSGBuffer) || (NULL == pInt_MSGLen))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;
	Json::StreamWriterBuilder st_JsonWBuilder;

	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonObject;
		Json::Value st_JsonSub;
		st_JsonObject["nIPCount"] = (*pppSt_IPCount)[i]->nIPCount;
		st_JsonObject["tszIPAddr"] = (*pppSt_IPCount)[i]->tszIPAddr;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";
	st_JsonRoot["Count"] = nListCount;
	st_JsonRoot["Array"] = st_JsonArray;

	st_JsonWBuilder["emitUTF8"] = true;
	*pInt_MSGLen = (int)Json::writeString(st_JsonWBuilder, st_JsonRoot).length();
	memcpy(ptszMSGBuffer, Json::writeString(st_JsonWBuilder, st_JsonRoot).c_str(), *pInt_MSGLen);
	return true;
}