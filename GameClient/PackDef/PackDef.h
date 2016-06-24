#pragma once

#ifndef __INCLUDE_PROTOCOL_H__
#define __INCLUDE_PROTOCOL_H__
#include <Windows.h>

enum ENUM_RS_RESULT
{
	enum_rs_success, 
	enum_rs_fail, 
	enum_rs_exist,
	enum_rs_unexist,
};

//回复成功失败
enum ENUM_RQ_RESULT
{
	enum_res_success, 
	enum_res_fail,
	enum_res_unexist,
	enum_res_password_error
};
enum ENUM_RS_PLAYSTRANGER
{
	enum_re_success, 
	enum_re_fail,
	enum_re_justwait 
	
};
///////////////////////////////////////////////////////////////////////////

//最大密码长度
#define DEF_MAX_PASSWORD_LEN			(16)

//每次发送数据的最大长度（保证不分包）
#define DEF_MAX_BUF						(1400)

//信息的最大长度
#define DEF_COMMON_MAX_LEN				(50)

//一次获取GAME列表的长度
#define DEF_GET_GAME_LIST_COUNT			(10)

//文件名称的最大长度
#define DEF_FILE_NAME_MAX_LEN			(20)

//一次获取好友列表的长度
#define DEF_FRIEND_LIST_COUNT			(20)

///////////////////////////////////////////////////////////////////////////
//定义协议类型
#define DEF_PROTOCOL_BASE				(10010)
//登录
#define DEF_LOGIN_RQ					(DEF_PROTOCOL_BASE + 1)
#define DEF_LOGIN_RS					(DEF_PROTOCOL_BASE + 2)
//上传文件
#define DEF_UPLOAD_RQ					(DEF_PROTOCOL_BASE + 3)
#define DEF_UPLOAD_RS					(DEF_PROTOCOL_BASE + 4)
//下载文件
#define DEF_DOWNLOAD_RQ					(DEF_PROTOCOL_BASE + 5)
#define DEF_DOWNLOAD_RS					(DEF_PROTOCOL_BASE + 6)
//获取游戏列表
#define DEF_GET_GAME_LIST_RQ			(DEF_PROTOCOL_BASE + 7)
#define DEF_GET_GAME_LIST_RS			(DEF_PROTOCOL_BASE + 8)
//安装
#define DEF_INSTALL_RQ					(DEF_PROTOCOL_BASE + 9)
#define DEF_INSTALL_RS					(DEF_PROTOCOL_BASE + 10)
//卸载
#define DEF_UNINSTALL_RQ				(DEF_PROTOCOL_BASE + 11)
#define DEF_UNINSTALL_RS				(DEF_PROTOCOL_BASE + 12)
//连接信息-通知服务器保存SOCKET
#define DEF_CONNECT_MESSAGE_RQ          (DEF_PROTOCOL_BASE + 13)
//心跳包
#define DEF_HEARTBEAT_RQ				(DEF_PROTOCOL_BASE + 14)
#define DEF_HEARTBEAT_RS				(DEF_PROTOCOL_BASE + 15) //服务端有没有必要回复？？
//注册
#define DEF_REGISTER_RQ					(DEF_PROTOCOL_BASE + 30)
#define DEF_REGISTER_RS					(DEF_PROTOCOL_BASE + 31)

//下载游戏
#define DEF_DOWNLOAD_GAME_RQ			(DEF_PROTOCOL_BASE + 32)
#define DEF_DOWNLOAD_GAME_RS			(DEF_PROTOCOL_BASE + 33)
//陌生人匹配游戏
#define DEF_PLAY_STRANGER_RQ			(DEF_PROTOCOL_BASE + 34)
#define DEF_PLAY_STRANGER_RS			(DEF_PROTOCOL_BASE + 35)
//获取好友列表
#define DEF_GET_FRIEND_LIST_RQ			(DEF_PROTOCOL_BASE + 36)
#define DEF_GET_FRIEND_LIST_RS			(DEF_PROTOCOL_BASE + 37)
//邀请好友游戏
#define DEF_INVITE_FRIEND_RQ			(DEF_PROTOCOL_BASE + 38)
#define DEF_INVITE_FRIEND_RS			(DEF_PROTOCOL_BASE + 39)
//被好友邀请游戏
#define DEF_BE_INVITE_FRIEND_RQ			(DEF_PROTOCOL_BASE + 40)
#define DEF_BE_INVITE_FRIEND_RS			(DEF_PROTOCOL_BASE + 41)
//加好友
#define DEF_ADD_FRIEND_RQ				(DEF_PROTOCOL_BASE + 42)
#define DEF_ADD_FRIEND_RS				(DEF_PROTOCOL_BASE + 43)
//删除好友
#define DEF_DELETE_FRIEND_RQ			(DEF_PROTOCOL_BASE + 44)
#define DEF_DELETE_FRIEND_RS			(DEF_PROTOCOL_BASE + 45)

//////////？？？？？？？？？？？？？？？？？？？需要查找玩家吗？？？？
//#define DEF_FIND_PLAYER_RQ				(DEF_PROTOCOL_BASE +  XXX )
//#define DEF_FIND_PLAYER_RS				(DEF_PROTOCOL_BASE +  XXX )

//下线
#define DEF_LOGOUT_RQ					(DEF_PROTOCOL_BASE + 27)

#define DEF_PROTOCOL_COUNT				(100)

///////////////////////////////////////////////////////////////////////////
struct STRU_PROTOCOL_BASE
{
	STRU_PROTOCOL_BASE(WORD wType) : m_wProtocolType(wType)
	{
	
	}
	virtual long Serialize(char szBuf[], long lBufLen) = 0;
	virtual BOOL UnSerialize(const char szBuf[], long lBufLen) = 0;
protected:
	WORD m_wProtocolType;						//每种新协议结构的类型字段
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
struct STRU_LOGIN_RQ : public STRU_PROTOCOL_BASE
{
	STRU_LOGIN_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	WORD m_wPasswordLen;
	char m_szPassword[DEF_MAX_PASSWORD_LEN];
};

struct STRU_LOGIN_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_LOGIN_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;						//由服务器根据用户ID+登陆时间生成，下发给客户端，用来作为以后客户端与服务器交互的身份验证标示
	WORD m_wResult;								//参照enum ENUM_RQ_RESULT
};

//////////////////////////////////////////////////////////////////////////////////////
//DEF_HEARTBEAT_RQ
struct STRU_HEARTBEAT_RQ : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_HEARTBEAT_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
};

struct STRU_HEARTBEAT_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_HEARTBEAT_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	WORD m_wResult;
};
//////////////////////////////////////////////////////////////////////////////////////
//DEF_REGISTER_RQ
struct STRU_REGISTER_RQ : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_REGISTER_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	WORD m_wPasswordLen;
	char m_szPassword[DEF_MAX_PASSWORD_LEN];
};

struct STRU_REGISTER_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_REGISTER_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	WORD m_wResult;
};
//////////////////////////////////////////////////////////////////////////////////////
struct STRU_GET_APP_LIST_RQ : public STRU_PROTOCOL_BASE
{
	STRU_GET_APP_LIST_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;			
	INT64 m_i64AppKey;				//通过app+version生成的key
};

//app信息结构
struct STRU_APP_INFO
{
	STRU_APP_INFO();
	long Serialize(char*& szBuf, long &lBufLen);
	BOOL UnSerialize(const char*& szBuf, long &lBufLen);
	static long MIN_LEN;

public:
	INT64 m_i64UserID;				//APP的贡献者
	INT64 m_i64AppKey;				//通过app+version生成的key
	DWORD m_dwSharedTime;			//上传的时间
	DWORD m_dwDownTimes;			//下载次数
	WORD m_wAppDesLen;				
	WORD m_wAppIconPathLen;			
	WORD m_wAppNameLen;
	WORD m_wAppPathLen;
	char m_szAppDescript[DEF_COMMON_MAX_LEN];
	char m_szIconPath[DEF_COMMON_MAX_LEN];
	char m_szAppName[DEF_COMMON_MAX_LEN];
	char m_szAppPath[DEF_COMMON_MAX_LEN];
};

struct STRU_GET_APP_LIST_RS : public STRU_PROTOCOL_BASE
{
	STRU_GET_APP_LIST_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	WORD m_wAppItemCount;
	STRU_APP_INFO m_oAppList[DEF_GET_GAME_LIST_COUNT];
};

//////////////////////////////////////////////////////////////////////////////////////
//DEF_DOWNLOAD_GAME_RQ
struct STRU_DOWNLOAD_GAME_RQ : public STRU_PROTOCOL_BASE
{
	STRU_DOWNLOAD_GAME_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;	
	int m_iGameID;	
};

struct STRU_DOWNLOAD_GAME_RS : public STRU_PROTOCOL_BASE
{
	STRU_DOWNLOAD_GAME_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	long m_lGameID;
	WORD m_wResult;					//enum EUNM_RQ_RESULT
};
//////////////////////////////////////////////////////////////////////////////////////
//DEF_PLAY_STRANGER_RQ
struct STRU_PLAY_STRANGER_RQ : public STRU_PROTOCOL_BASE
{
	STRU_PLAY_STRANGER_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
};

struct STRU_PLAY_STRANGER_RS : public STRU_PROTOCOL_BASE
{
	STRU_PLAY_STRANGER_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64StrangerID;					//匹配成功的对手ID
	int m_iGameID;
	int m_iIndex;
	BOOL m_bIsFirst;
	WORD m_wResult;	
};
//////////////////////////////////////////////////////////////////////////////////////
//DEF_GET_FRIEND_LIST_RQ
struct STRU_GET_FRIEND_LIST_RQ : public STRU_PROTOCOL_BASE
{
	STRU_GET_FRIEND_LIST_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
};

struct STRU_USER_INFO
{
	STRU_USER_INFO();
	long Serialize(char*& szBuf, long &lBufLen);
	BOOL UnSerialize(const char*& szBuf, long &lBufLen);
	static long MIN_LEN;

public:
	INT64 m_i64UserID;				//
	//返回的一定是在线的，本地维护一个好友列表文件
};
struct STRU_GET_FRIEND_LIST_RS : public STRU_PROTOCOL_BASE
{
	STRU_GET_FRIEND_LIST_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	WORD m_wFriendItemCount;
	STRU_USER_INFO m_oFriendList[DEF_FRIEND_LIST_COUNT];
};
//////////////////////////////////////////////////////////////////////////////////////
//DEF_INVITE_FRIEND_RQ
struct STRU_INVITE_FRIEND_RQ : public STRU_PROTOCOL_BASE
{
	STRU_INVITE_FRIEND_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64FriendID;	
	long m_lGameID;
};
//------？？？？？？？？？？？需要对邀请者和被邀请者分别发送不一样的回复包
struct STRU_INVITE_FRIEND_RS : public STRU_PROTOCOL_BASE
{
	STRU_INVITE_FRIEND_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64FriendID;
	long m_lGameID;
	WORD m_wResult;	
};
//////////////////////////////////////////////////////////////////////////////////////
//DEF_BE_INVITE_FRIEND_RQ
struct STRU_BE_INVITE_FRIEND_RQ : public STRU_PROTOCOL_BASE
{
	STRU_BE_INVITE_FRIEND_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64FriendID;
	long m_lGameID;
};
//------？？？？？？？？？？？需要对邀请者和被邀请者分别发送不一样的回复包
struct STRU_BE_INVITE_FRIEND_RS : public STRU_PROTOCOL_BASE
{
	STRU_BE_INVITE_FRIEND_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64FriendID;
	long m_lGameID;
	WORD m_wResult;	
};

//////////////////////////////////////////////////////////////////////////////////////
//DEF_ADD_FRIEND_RQ
struct STRU_ADD_FRIEND_RQ : public STRU_PROTOCOL_BASE
{
	STRU_ADD_FRIEND_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64PlayerID;
};
//------？？？？？？？？？？？需要对加好友的人 和 被加好友的人  分别发送不一样的回复包
struct STRU_ADD_FRIEND_RS : public STRU_PROTOCOL_BASE
{
	STRU_ADD_FRIEND_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64PlayerID;
	WORD m_wResult;
};
//////////////////////////////////////////////////////////////////////////////////////
//DEF_DELETE_FRIEND_RQ
struct STRU_DELETE_FRIEND_RQ : public STRU_PROTOCOL_BASE
{
	STRU_DELETE_FRIEND_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64FriendID;
};
//------？？？？？？？？？？？需要对删除好友的人 和 被删除好友的人  分别发送不一样的回复包
struct STRU_DELETE_FRIEND_RS : public STRU_PROTOCOL_BASE
{
	STRU_DELETE_FRIEND_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64FriendID;
	WORD m_wResult;
};


//对战平台打开游戏后，需要给游戏发送 过去 自己的ID，游戏的ID，对手的ID  使用什么方式？？
//写配置文件，一个写完另一个读取
//游戏结束后如何处理，是直接关闭游戏


////发送己放游戏数据
//#define DEF_SEND_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 23)
//#define DEF_SEND_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 24)
////接收对方游戏数据
//#define DEF_RECV_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 25)
//#define DEF_RECV_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 26)

//////////////////////////////////////////////////////////////////////////////////////
struct STRU_LOGOUT_RQ : public STRU_PROTOCOL_BASE
{
	STRU_LOGOUT_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
};

//上传文件
struct STRU_FILE_HEAD_UPLOAD : public STRU_PROTOCOL_BASE
{
	STRU_FILE_HEAD_UPLOAD();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	DWORD m_dwFileLength;
	WORD m_wVersion;
	WORD m_wAppNameLen;
	char m_szAppName[DEF_FILE_NAME_MAX_LEN];
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//传送完成
struct STRU_FILE_TRANS_COMPLETE_UPLOAD : public STRU_PROTOCOL_BASE
{
	STRU_FILE_TRANS_COMPLETE_UPLOAD();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;

public:
	INT64 m_i64UserID;
	INT64 m_i64FileKey;
	WORD m_wVersion;
	WORD m_wAppNameLen;
	char m_szAppName[DEF_FILE_NAME_MAX_LEN];
};

//下载文件
struct STRU_FILE_HEAD_DOWNLOAD : public STRU_PROTOCOL_BASE
{
	STRU_FILE_HEAD_DOWNLOAD();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	DWORD m_dwFileLength;
	WORD m_wVersion;
	WORD m_wAppNameLen;
	char m_szAppName[DEF_FILE_NAME_MAX_LEN];
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//下载文件传送完成
struct STRU_FILE_TRANS_COMPLETE_DOWNLOAD : public STRU_PROTOCOL_BASE
{
	STRU_FILE_TRANS_COMPLETE_DOWNLOAD();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;

public:
	INT64 m_i64UserID;
	INT64 m_i64FileKey;
	WORD m_wVersion;
	WORD m_wAppNameLen;
	char m_szAppName[DEF_FILE_NAME_MAX_LEN];
};

//tcp发送，给服务器保存用
//DEF_CONNECT_MESSAGE_RQ
struct STRU_CONNECT_MESSAGE_RQ : public STRU_PROTOCOL_BASE
{
	STRU_CONNECT_MESSAGE_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;

public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
};
#endif