#pragma once
/********************************************************************
//    Created:     2021/12/29  10:23:14
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuthorize_User\ModuleAuthorize_User.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuthorize_User
//    File Base:   ModuleAuthorize_User
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     用户验证
//    History:
*********************************************************************/

typedef struct
{
	TCHAR tszUserName[64];
	TCHAR tszUserPass[64];
}XENGINE_USERAUTH, * LPXENGINE_USERAUTH;

class CModuleAuthorize_User
{
public:
	CModuleAuthorize_User();
	~CModuleAuthorize_User();
public:
	BOOL ModuleAuthorize_User_Init(LPCTSTR lpszAuthFile);
	BOOL ModuleAuthorize_User_Destory();
	BOOL ModuleAuthorize_User_Exist(LPCTSTR lpszUser, LPCTSTR lpszPass);
private:
	unordered_map<tstring, XENGINE_USERAUTH> stl_MapSession;
};