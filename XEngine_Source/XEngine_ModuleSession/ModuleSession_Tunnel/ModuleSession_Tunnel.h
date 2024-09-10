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
    int nHdrLen;                                     //HTTP头大小
    int nPosLen;                                     //HTTP当前大小
    int nCtmLen;                                     //自定义内容大小
    list<tstring>* pStl_ListField;                   //HTTP字段
    XPVOID lParam;
}PROXYTUNNEL_CLIENTINFO,*LPPROXYTUNNEL_CLIENTINFO;


class CModuleSession_Tunnel
{
public:
    CModuleSession_Tunnel();
    ~CModuleSession_Tunnel();
public:
    bool ModuleSession_Tunnel_Create(LPCXSTR lpszClientID);
    bool ModuleSession_Tunnel_Delete(LPCXSTR lpszClientID);
    bool ModuleSession_Tunnel_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen);
    bool ModuleSession_Tunnel_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len = NULL);
    bool ModuleSession_Tunnel_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize);
    bool ModuleSession_Tunnel_Packet(LPCXSTR lpszClientID, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen);
    bool ModuleSession_Tunnel_List(XCHAR*** ppptszClientList, int* pInt_ListCount);
private:
	shared_mutex st_Locker;
private:
    unordered_map<tstring,PROXYTUNNEL_CLIENTINFO *> stl_MapClient;
};
