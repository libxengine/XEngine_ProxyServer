#pragma once
/********************************************************************
//    Created:     2021/12/02  16:09:03
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfig_Define.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure
//    File Base:   ModuleConfig_Define
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     ����ģ�鵼������
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        ���������ݽṹ
//////////////////////////////////////////////////////////////////////////
typedef struct tag_XEngine_ServiceConfig
{
	TCHAR tszIPAddr[128];                     //����IP��ַ,������Ҫ����
	BOOL bDeamon;                             //�Ƿ����ػ���������,LINUX��Ч
	int nSocksPort;                           //Socks�˿�,<=0������
	int nTunnelPort;                          //Tunnel����˿�
	struct
	{
		int nMaxClient;                       //���ͻ��˸���
		int nMaxQueue;                        //�����и���
		int nIOThread;                        //����IO�߳���
	}st_XMax;
	struct
	{
		int nTimeCheck;                       //������
		int nSocksTimeOut;                    //TCP��ʱʱ��
		int nTunnelTimeOut;                   //HTTP��ʱʱ��
	}st_XTime;                                //����*ʱ��=��ʱ
	struct
	{
		TCHAR tszLogFile[MAX_PATH];           //��־����·��
		int nMaxSize;                         //�����־��С
		int nMaxCount;                        //�����־����
		int nLogLeave;                        //��־�ȼ�
	}st_XLog;
	struct  
	{
		int bAuth;                            //�Ƿ�������֤,����0����
		TCHAR tszAuthFile[MAX_PATH];          //��֤��ַ
	}st_XAuth;
	struct  
	{
		list<string>* pStl_ListVer;
	}st_XVer;
}XENGINE_SERVICECONFIG;
//////////////////////////////////////////////////////////////////////////
//                        ������������
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleConfigure_GetLastError(int* pInt_ErrorCode = NULL);
/************************************************************************/
/*                        �ļ����ö�ȡ                                  */
/************************************************************************/
/********************************************************************
�������ƣ�ModuleConfigure_Json_File
�������ܣ���ȡJSON�����ļ�
 ����.һ��lpszConfigFile
  In/Out��In
  ���ͣ������ַ�ָ��
  �ɿգ�N
  ��˼������Ҫ��ȡ�������ļ�
 ����.����pSt_ServerConfig
  In/Out��Out
  ���ͣ����ݽṹָ��
  �ɿգ�N
  ��˼���������������Ϣ
����ֵ
  ���ͣ��߼���
  ��˼���Ƿ�ɹ�
��ע��
*********************************************************************/
extern "C" BOOL ModuleConfigure_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig);