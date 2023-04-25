#include "pch.h"
#include "ModuleProtocol_Parse.h"
/********************************************************************
//    Created:     2022/06/08  13:56:46
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse\ModuleProtocol_Parse.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse
//    File Base:   ModuleProtocol_Parse
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     解析协议类
//    History:
*********************************************************************/
CModuleProtocol_Parse::CModuleProtocol_Parse()
{

}
CModuleProtocol_Parse::~CModuleProtocol_Parse()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
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
bool CModuleProtocol_Parse::ModuleProtocol_Parse_ForwardBind(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszSrcAddr, XCHAR* ptszDstAddr)
{
	Protocol_IsErrorOccur = false;

	if (NULL == lpszMsgBuffer)
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		Protocol_IsErrorOccur = true;
		Protocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_JSON;
		return false;
	}
	//_tcsxcpy(ptszSrcAddr, st_JsonRoot["tszSrcAddr"].asCString());  暂时不支持
	_tcsxcpy(ptszDstAddr, st_JsonRoot["tszDstAddr"].asCString());

	return true;
}