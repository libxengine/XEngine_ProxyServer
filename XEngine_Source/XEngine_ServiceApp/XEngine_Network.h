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
BOOL __stdcall Network_Callback_SocksLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);                                  //用户连接
void __stdcall Network_Callback_SocksRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam); //接受到数据
void __stdcall Network_Callback_SocksLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);                                  //用户离开
void __stdcall Network_Callback_SocksHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);                      //心跳超时
//Tunnel相关
BOOL __stdcall Network_Callback_TunnelLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall Network_Callback_TunnelRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void __stdcall Network_Callback_TunnelLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void __stdcall Network_Callback_TunnelHeart(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);
//关闭与发送
void XEngine_Network_Close(LPCTSTR lpszClientAddr, int nIPProto, BOOL bHeart);
BOOL XEngine_Network_Send(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nIPProto);