#pragma once
/********************************************************************
//    Created:     2021/12/03  14:16:27
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_UserProtocol.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source
//    File Base:   XEngine_UserProtocol
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     �û��Զ���Э��ͷ
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                          ��Э�鶨��;unOperatorType
//////////////////////////////////////////////////////////////////////////
typedef enum 
{
	//�û���Ϣ
	ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_MSG = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_USER + 1
}ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_CUSTOM;
//////////////////////////////////////////////////////////////////////////
//                          ��Э�鶨��:unOperatorCode
//////////////////////////////////////////////////////////////////////////
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_MSG_TEXTREQ 0x0001   //������Ϣ
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_MSG_TEXTREP 0x0002   //�ظ���Ϣ
///////////////////////////////////////////////////////////////////////////
//                          ���������ݽṹ
///////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
typedef struct  
{
	TCHAR tszIPAddr[128];
	ENUM_RFCCOMPONENTS_PROXY_STATUS enStatus;
	SOCKET hSocket;
	BOOL bConnect;
}PROXYPROTOCOL_CLIENTINFO;
#pragma pack(pop)