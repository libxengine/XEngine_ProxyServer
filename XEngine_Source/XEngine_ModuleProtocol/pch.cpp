#include "pch.h"
#include "ModuleProtocol_Packet/ModuleProtocol_Packet.h"
#include "ModuleProtocol_Parse/ModuleProtocol_Parse.h"
/********************************************************************
//    Created:     2022/06/08  13:15:13
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol\pch.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleProtocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
BOOL Protocol_IsErrorOccur = FALSE;
DWORD Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleProtocol_Packet m_ProtocolPacket;
CModuleProtocol_Parse m_ProtocolParse;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleProtocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Protocol_dwErrorCode;
}
/************************************************************************/
/*                        封包导出函数                                  */
/************************************************************************/
extern "C" BOOL ModuleProtocol_Packet_ForwardList(TCHAR * ptszMsgBuffer, int* pInt_Len, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, TCHAR * **ppptszListAddr, int nCount)
{
	return m_ProtocolPacket.ModuleProtocol_Packet_ForwardList(ptszMsgBuffer, pInt_Len, pSt_ProtocolHdr, ppptszListAddr, nCount);
}
/************************************************************************/
/*                        解析导出函数                                  */
/************************************************************************/
extern "C" BOOL ModuleProtocol_Parse_ForwardBind(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszSrcAddr, TCHAR * ptszDstAddr)
{
	return m_ProtocolParse.ModuleProtocol_Parse_ForwardBind(lpszMsgBuffer, nMsgLen, ptszSrcAddr, ptszDstAddr);
}