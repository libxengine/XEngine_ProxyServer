#include "pch.h"
#include "ModuleAuthorize_User.h"
/********************************************************************
//    Created:     2021/12/29  10:24:25
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuthorize_User\ModuleAuthorize_User.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuthorize_User
//    File Base:   ModuleAuthorize_User
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     用户验证
//    History:
*********************************************************************/
CModuleAuthorize_User::CModuleAuthorize_User()
{

}
CModuleAuthorize_User::~CModuleAuthorize_User()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleAuthorize_User_Init
函数功能：初始化授权会话管理器
 参数.一：lpszAuthFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入授权文件地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleAuthorize_User::ModuleAuthorize_User_Init(LPCTSTR lpszAuthFile)
{
	Authorize_IsErrorOccur = FALSE;

	FILE* pSt_File = _tfopen(lpszAuthFile, _T("rb"));
	if (NULL == pSt_File)
	{
		Authorize_IsErrorOccur = TRUE;
		Authorize_dwErrorCode = ERROR_MODULE_AUTHORIZE_USER_OPENFILE;
		return FALSE;
	}
	int nCount = 0;
	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	while (TRUE)
	{
		int nRet = fread(tszMsgBuffer + nCount, 1, 1024, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);

	LPCTSTR lpszLineStr = _T("\r\n");
	TCHAR* ptszTokStr = _tcstok(tszMsgBuffer, lpszLineStr);
	while (1)
	{
		if (NULL == ptszTokStr)
		{
			break;
		}
		XENGINE_USERAUTH st_UserInfo;
		memset(&st_UserInfo, '\0', sizeof(XENGINE_USERAUTH));

		_stscanf(ptszTokStr, _T("%s %s"), st_UserInfo.tszUserName, st_UserInfo.tszUserPass);
		stl_MapSession.insert(make_pair(st_UserInfo.tszUserName, st_UserInfo));

		ptszTokStr = _tcstok(NULL, lpszLineStr);
	}
	return TRUE;
}
/************************************************************************
函数名称：ModuleAuthorize_User_Destory
函数功能：销毁授权管理器
返回值
  类型：逻辑型
  意思：是否销毁成功
备注：
************************************************************************/
BOOL CModuleAuthorize_User::ModuleAuthorize_User_Destory()
{
	Authorize_IsErrorOccur = FALSE;

	stl_MapSession.clear();
	return TRUE;
}
/********************************************************************
函数名称：ModuleAuthorize_User_Exist
函数功能：是否验证通过
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleAuthorize_User::ModuleAuthorize_User_Exist(LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	Authorize_IsErrorOccur = FALSE;

	unordered_map<string, XENGINE_USERAUTH>::const_iterator stl_MapIterator = stl_MapSession.find(lpszUser);
	if (stl_MapIterator == stl_MapSession.end())
	{
		Authorize_IsErrorOccur = TRUE;
		Authorize_dwErrorCode = ERROR_MODULE_AUTHORIZE_USER_NOTFOUND;
		return FALSE;
	}

	if (_tcslen(lpszPass) != _tcslen(stl_MapIterator->second.tszUserPass))
	{
		Authorize_IsErrorOccur = TRUE;
		Authorize_dwErrorCode = ERROR_MODULE_AUTHORIZE_USER_PASSWORD;
		return FALSE;
	}
	if (0 != _tcsncmp(lpszPass, stl_MapIterator->second.tszUserPass, _tcslen(lpszPass)))
	{
		Authorize_IsErrorOccur = TRUE;
		Authorize_dwErrorCode = ERROR_MODULE_AUTHORIZE_USER_PASSWORD;
		return FALSE;
	}
	return TRUE;
}