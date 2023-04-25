#pragma once
/********************************************************************
//    Created:     2022/06/08  13:54:47
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet\ModuleProtocol_Packet.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet
//    File Base:   ModuleProtocol_Packet
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     封装协议
//    History:
*********************************************************************/

class CModuleProtocol_Packet
{
public:
	CModuleProtocol_Packet();
	~CModuleProtocol_Packet();
public:
	bool ModuleProtocol_Packet_ForwardList(XCHAR* ptszMsgBuffer, int* pInt_Len, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XCHAR*** ppptszListAddr, int nCount);
private:
};