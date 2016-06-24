#pragma once
#ifndef _INCLUDE_COMMON_H_
#define _INCLUDE_COMMON_H_
#include "Observer.h"
#include <Windows.h>
#include <list>

#define DEF_GAME_PLAT_SERVER_HTONS			9999
#define DEF_GAME_PLAT_SERVER_IP				"192.168.1.66"
//���ֺ궨��
#define DEF_PASSWORD_LEN	 (50)			//���볤��
#define DEF_GAME_NAME_LEN	 (50)			//��Ϸ���ֳ���
#define DEF_GAME_PATH_LEN    (256)			//��Ϸ��װ·������
#define DEF_GAME_DESCRIPT_LEN (100)			//��Ϸ��������
///////////////////////////////////////////////////
#define WM_ONE (WM_USER + 2)

///////////////////////////////////////////////////
typedef enum ENUM_GAME_STATE
{
	enum_game_status_unexist,				//����û����Ϸ
	enum_game_status_download,				//������Ҫ����
	enum_game_status_opening,				//������Ϸ�Ѿ���
	enum_game_status_closing,				//������Ϸ�Ѿ��ر�
	enum_game_status_ready_open,			//������Ϸ��Ҫ��
	enum_game_status_ready_close,			//������Ϸ��Ҫ�ر�
	enum_game_status_ready_delete			//������Ϸ��Ҫɾ��
}ENUM_GAME_STATUS;

typedef struct MyGameMessage
{
	MyGameMessage():m_iGameNameLen(0),m_iGamePathLen(0),m_iGameDescriptLen(0),m_eGameState(enum_game_status_unexist)
	{
		memset(m_szGameName,0,sizeof(m_szGameName));
		memset(m_szGamePath,0,sizeof(m_szGamePath));
		memset(m_szGameDescript,0,sizeof(m_szGameDescript));
	}
	long m_lGameID;
	ENUM_GAME_STATUS m_eGameState;
	int m_iGameNameLen;
	int m_iGamePathLen;
	int m_iGameDescriptLen;
	char m_szGameName[DEF_GAME_NAME_LEN];
	
	char m_szGamePath[DEF_GAME_PATH_LEN];
	char m_szGameDescript[DEF_GAME_DESCRIPT_LEN];
}GameMsg;
typedef struct MyUserMessage
{
	MyUserMessage():m_i64UserID(0),m_lUserAccount(0)
	{
	}
	INT64 m_i64UserID;
	long m_lUserAccount;
}MyUserMsg;


//�����״̬����ö��
enum ENUM_TASK_STATE
{
	enum_task_init, 
	enum_task_wait, 
	enum_task_response, 
	enum_task_complete
};

///////////////////////////////////////////////
//����ָ��ö������
//�������ͣ�2������  long  bool
//        3������   long long bool
//        4������   long char long bool


//Э��  �±�  ��Ӧ��  ����ָ��  ����
//  ����ָ��  ��ö��  ����
typedef enum
{
	//enum_param_add_student_two,
	//enum_param_add_class_two,
	//enum_param_add_coach_two,

}ENUM_PFUN_TYPE;
//����Rs����


#endif