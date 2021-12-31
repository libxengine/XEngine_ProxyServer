#include "pch.h"
#include "ModuleAuthorize_User.h"
/********************************************************************
//    Created:     2021/12/29  10:24:25
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuthorize_User\ModuleAuthorize_User.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleAuthorize\ModuleAuthorize_User
//    File Base:   ModuleAuthorize_User
//    File Ext:    cpp
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     �û���֤
//    History:
*********************************************************************/
CModuleAuthorize_User::CModuleAuthorize_User()
{

}
CModuleAuthorize_User::~CModuleAuthorize_User()
{

}
//////////////////////////////////////////////////////////////////////////
//                        ���ú���
//////////////////////////////////////////////////////////////////////////
/********************************************************************
�������ƣ�ModuleAuthorize_User_Init
�������ܣ���ʼ����Ȩ�Ự������
 ����.һ��lpszAuthFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼��������Ȩ�ļ���ַ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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
�������ƣ�ModuleAuthorize_User_Destory
�������ܣ�������Ȩ������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ����ٳɹ�
��ע��
************************************************************************/
BOOL CModuleAuthorize_User::ModuleAuthorize_User_Destory()
{
	Authorize_IsErrorOccur = FALSE;

	stl_MapSession.clear();
	return TRUE;
}
/********************************************************************
�������ƣ�ModuleAuthorize_User_Exist
�������ܣ��Ƿ���֤ͨ��
 ����.һ��lpszUser
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼�������û���
 ����.����lpszPass
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼����������
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
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