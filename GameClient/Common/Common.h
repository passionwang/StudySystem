#pragma once
#ifndef _INCLUDE_COMMON_H_
#define _INCLUDE_COMMON_H_
#include "Observer.h"
#include <Windows.h>
#include <list>

#define DEF_GAME_PLAT_SERVER_HTONS			9999
#define DEF_GAME_PLAT_SERVER_IP				"192.168.1.66"
//各种宏定义
#define DEF_PASSWORD_LEN	 (50)			//密码长度
#define DEF_GAME_NAME_LEN	 (50)			//游戏名字长度
#define DEF_GAME_PATH_LEN    (256)			//游戏安装路径长度
#define DEF_GAME_DESCRIPT_LEN (100)			//游戏描述长度
///////////////////////////////////////////////////
#define WM_ONE (WM_USER + 2)

///////////////////////////////////////////////////
typedef enum ENUM_GAME_STATE
{
	enum_game_status_unexist,				//本地没有游戏
	enum_game_status_download,				//本地需要下载
	enum_game_status_opening,				//本地游戏已经打开
	enum_game_status_closing,				//本地游戏已经关闭
	enum_game_status_ready_open,			//本地游戏需要打开
	enum_game_status_ready_close,			//本地游戏需要关闭
	enum_game_status_ready_delete			//本地游戏需要删除
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


//任务的状态机制枚举
enum ENUM_TASK_STATE
{
	enum_task_init, 
	enum_task_wait, 
	enum_task_response, 
	enum_task_complete
};

///////////////////////////////////////////////
//函数指针枚举类型
//参数类型：2个参数  long  bool
//        3个参数   long long bool
//        4个参数   long char long bool


//协议  下标  对应的  函数指针  数组
//  函数指针  的枚举  类型
typedef enum
{
	//enum_param_add_student_two,
	//enum_param_add_class_two,
	//enum_param_add_coach_two,

}ENUM_PFUN_TYPE;
//返回Rs请求


#endif