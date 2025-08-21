#pragma once
/********************************************************************
//    Created:     2024/09/06  16:29:02
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Tunnel\ModuleSession_Tunnel.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Tunnel
//    File Base:   ModuleSession_Tunnel
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     隧道代理服务端
//    History:
*********************************************************************/
typedef struct
{
    XCHAR tszMsgBuffer[4096];                        //只需要取得头
    XCHAR tszClientAddr[128];
	XNETHANDLE xhClient;                             //客户端句柄
    int nHdrLen;                                     //HTTP头大小
    int nPosLen;                                     //HTTP当前大小

    ENUM_PROXY_SESSION_CLIENT_STATUS enClientStatus;
    list<xstring>* pStl_ListField;                   //HTTP字段
}PROXYTUNNEL_CLIENTINFO,*LPPROXYTUNNEL_CLIENTINFO;


class CModuleSession_Tunnel
{
public:
    CModuleSession_Tunnel();
    ~CModuleSession_Tunnel();
public:
    bool ModuleSession_Tunnel_Create(LPCXSTR lpszClientID);
    bool ModuleSession_Tunnel_Delete(LPCXSTR lpszClientID);
    bool ModuleSession_Tunnel_SetInfo(LPCXSTR lpszClientID, XNETHANDLE xhClient, LPCXSTR lpszClientAddr);
    bool ModuleSession_Tunnel_GetInfo(LPCXSTR lpszClientID, XNETHANDLE *pxhClient);
    bool ModuleSession_Tunnel_GetAddrForHandle(XNETHANDLE xhClient, XCHAR* ptszClientAddr);
	bool ModuleSession_Tunnel_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS* penStatus);
	bool ModuleSession_Tunnel_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS enStatus);
    bool ModuleSession_Tunnel_Packet(LPCXSTR lpszClientID, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen);
    bool ModuleSession_Tunnel_List(XCHAR*** ppptszClientList, int* pInt_ListCount);
private:
	shared_mutex st_Locker;
private:
    unordered_map<xstring,PROXYTUNNEL_CLIENTINFO *> stl_MapClient;
};
