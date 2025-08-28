#pragma once
/********************************************************************
//    Created:     2021/12/02  16:39:22
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_Network.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_Network
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO相关代码
//    History:
*********************************************************************/
//SOCKS相关
bool XCALLBACK Network_Callback_SocksLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户连接
void XCALLBACK Network_Callback_SocksRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam); //接受到数据
void XCALLBACK Network_Callback_SocksLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户离开
void XCALLBACK Network_Callback_SocksHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);                      //心跳超时
//Tunnel相关
bool XCALLBACK Network_Callback_TunnelLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_TunnelRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_TunnelLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_TunnelHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
//Forward相关
bool XCALLBACK Network_Callback_ForwardLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_ForwardRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_ForwardLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_ForwardHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
//Forward相关
bool XCALLBACK Network_Callback_ProxyLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_ProxyRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_ProxyLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_ProxyHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
//关闭与发送
void XEngine_Network_Close(LPCXSTR lpszClientAddr, int nIPProto, int nCloseType);
bool XEngine_Network_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nIPProto);