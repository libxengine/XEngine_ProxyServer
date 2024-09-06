#pragma once
/********************************************************************
//    Created:     2021/12/29  10:29:54
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuth_Error.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize
//    File Base:   ModuleAuth_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出错误
//    History:
*********************************************************************/
/************************************************************************/
/*                     转发代理服务器错误                                 */
/************************************************************************/
#define ERROR_MODULE_SESSION_FORWARD_PARAMENT 0xC0001                 //参数错误
#define ERROR_MODULE_SESSION_FORWARD_NOTFOUND 0xC0002                 //没有找到
#define ERROR_MODULE_SESSION_FORWARD_BIND 0xC0003                     //已经绑定
#define ERROR_MODULE_SESSION_FORWARD_NOTFORWARD 0xC0004               //没有转发
#define ERROR_MODULE_SESSION_FORWARD_EXIST 0xC0005                    //已经存在
/************************************************************************/
/*                     SOCKS5服务器错误                                 */
/************************************************************************/
#define ERROR_MODULE_SESSION_SOCKS_PARAMENT 0xC1001        //参数错误
#define ERROR_MODULE_SESSION_SOCKS_MALLOC 0xC1002          //申请内存失败
#define ERROR_MODULE_SESSION_SOCKS_NOTFOUND 0xC1011        //没有找到
#define ERROR_MODULE_SESSION_SOCKS_VERSION 0xC1022           //协议错误
#define ERROR_MODULE_SESSION_SOCKS_NOTSET 0xC1063            //没有设置客户端数据
