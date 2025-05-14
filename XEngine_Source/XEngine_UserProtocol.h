#pragma once
/********************************************************************
//    Created:     2021/12/03  14:16:27
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_UserProtocol.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source
//    File Base:   XEngine_UserProtocol
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     用户自定义协议头
//    History:
*********************************************************************/
///////////////////////////////////////////////////////////////////////////
//                          导出的协议定义
///////////////////////////////////////////////////////////////////////////
typedef enum en_XEngine_XUser_Protocol
{
	ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_USER_FORWARD = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_USER + 1
}ENUM_XNETENGINE_XUSER_PROTOCOL;
//转发协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LOGREQ 0xE1000        //登录请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LOGREP 0xE1001        
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LISTREQ 0xE1002       //列表请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LISTREP 0xE1003 
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_NAMEDREQ 0xE1004      //命名请求
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_NAMEDREP 0xE1005
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_ANONYREQ 0xE1006      //匿名转发
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_ANONYREP 0xE1007
///////////////////////////////////////////////////////////////////////////
//                          导出的错误信息
///////////////////////////////////////////////////////////////////////////
#define ERROR_XENGINE_PROXY_PROTOCOL_FORMAT 0x01                           //协议格式内容错误
#define ERROR_XENGINE_PROXY_PROTOCOL_SERVER 0x02                           //服务器内部处理错误
#define ERROR_XENGINE_PROXY_PROTOCOL_DOMAIN 0x03                           //域名转IP错误
#define ERROR_XENGINE_PROXY_PROTOCOL_NOTLOGIN 0x04                         //未登录
#define ERROR_XENGINE_PROXY_PROTOCOL_NOTFOUND 0x05                         //未找到
#define ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT 0x06                       //不支持
///////////////////////////////////////////////////////////////////////////
//                          导出的数据结构
///////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)

#pragma pack(pop)