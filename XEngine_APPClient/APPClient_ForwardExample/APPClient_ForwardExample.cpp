#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_ProxyProtocol")
#pragma comment(lib,"../../XEngine_Source//Debug/jsoncpp")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"

//需要优先配置XEngine
//WINDOWS支持VS2022 x86 debug 编译调试
//linux使用下面的命令编译
//g++ -std=c++17 -Wall -g APPClient_ForwardExample.cpp -o APPClient_ForwardExample.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_ProxyProtocol -ljsoncpp -Wl,-rpath=../../XEngine_Source/XEngine_ThirdPart/jsoncpp,--disable-new-dtags

int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XSOCKET m_Socket;
	LPCXSTR lpszServiceAddr = _X("127.0.0.1");
	if (!XClient_TCPSelect_Create(&m_Socket, lpszServiceAddr, 5402))
	{
		printf("连接失败！错误:%lX\n", XClient_GetLastError());
		return 0;
	}
	printf("连接成功！\n");

	int nMsgLen = 0;
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOL_USERAUTH st_UserAuth;

	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
	//登录
	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	st_ProtocolHdr.byIsReply = true;
	st_ProtocolHdr.byVersion = 0;
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTH);
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LOGREQ;

	_tcsxcpy(st_UserAuth.tszUserName, "test");

	if (!XClient_TCPSelect_SendMsg(m_Socket, (LPCXSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR)))
	{
		printf("发送失败！\n");
		return 0;
	}
	if (!XClient_TCPSelect_SendMsg(m_Socket, (LPCXSTR)&st_UserAuth, sizeof(XENGINE_PROTOCOL_USERAUTH)))
	{
		printf("发送失败！\n");
		return 0;
	}

	nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	if (!XClient_TCPSelect_RecvPkt(m_Socket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
	{
		printf("接受数据失败！\n");
		return 0;
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	//列举
	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	st_ProtocolHdr.byIsReply = true;
	st_ProtocolHdr.byVersion = 0;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_USER_FORWARD;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LISTREQ;
	if (!XClient_TCPSelect_SendMsg(m_Socket, (LPCXSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR)))
	{
		printf("发送失败！\n");
		return 0;
	}
	nMsgLen = 0;
	if (!XClient_TCPSelect_RecvPkt(m_Socket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
	{
		printf("接受数据失败！\n");
		return 0;
	}
	printf("%s\n", ptszMsgBuffer);

	Json::Value st_JsonAddr;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonAddr, &st_JsonError))
	{
		return false;
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	if (st_JsonAddr["Count"].asInt() > 0)
	{
		//请求绑定
		Json::Value st_JsonRoot;
		Json::Value st_JsonArray = st_JsonAddr["Array"];
		st_JsonRoot["tszDstAddr"] = st_JsonArray[0]["tszSrcAddr"].asCString();

		st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
		st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
		st_ProtocolHdr.byIsReply = true;
		st_ProtocolHdr.byVersion = 0;
		st_ProtocolHdr.unPacketSize = st_JsonRoot.toStyledString().length();
		st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_USER_FORWARD;
		st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_BINDREQ;
		if (!XClient_TCPSelect_SendMsg(m_Socket, (LPCXSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR)))
		{
			printf("发送失败！\n");
			return 0;
		}
		if (!XClient_TCPSelect_SendMsg(m_Socket, st_JsonRoot.toStyledString().c_str(), st_ProtocolHdr.unPacketSize))
		{
			printf("发送失败！\n");
			return 0;
		}
		nMsgLen = 2048;
		if (!XClient_TCPSelect_RecvPkt(m_Socket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			printf("接受数据失败！\n");
			return 0;
		}
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		//成功后发送数据
		for (int i = 0; i < 10; i++)
		{
			XClient_TCPSelect_SendMsg(m_Socket, "hello", 5);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("send 5 str\n");
		}
	}
	else
	{
		bool bGet = false;
		while (1)
		{
			if (bGet)
			{
				//收到转发请求
				nMsgLen = 2048;
				XCHAR tszMsgBuffer[2048];

				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				if (XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
				{
					printf("%s\n", tszMsgBuffer);
				}
			}
			else
			{
				if (XClient_TCPSelect_RecvPkt(m_Socket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
				{
					//收到转发请求
					if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_BINDREQ == st_ProtocolHdr.unOperatorCode)
					{
						bGet = true;
						printf("get forward\n");
					}
					BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
				}
			}
		}		
	}

	std::this_thread::sleep_for(std::chrono::seconds(20000));
	XClient_TCPSelect_Close(m_Socket);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}