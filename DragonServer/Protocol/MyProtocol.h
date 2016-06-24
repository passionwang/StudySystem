#pragma once
//1.什么是协议：通信双方都知道并且都遵守的一套规则（每一条规则为一对结构）
//2.为了解决双方收到数据为字节流（位流），无法知道对方发送的信息的具体含义是什么：在每个协议结构的相同位置
//固定的位置存放能够标示收到数据类型的字段，从而收到数据的一方可以根据这个类型字段确定收到数据的具体
//含义（是哪一种结构）
//3.为了防止结构中类型字段被赋值错误导致通信失败，将协议类型字段放入公共基类中进行保护继承
//4.协议中出现的字符数组性成员，必须有对应的长度字段，从而每次发送只需要发送有效的长度，不需要发送
//数组的全部长度，减少发送数据的量
//5.由于两端对齐方式不同，导致收发数据错位：保留每端默认的对齐方式，从而使每端I/O的效率仍然很高，然后采用
//将需要发送的有效数据按照固定的顺序和真实的大小填充到一个连续的buf中进行序列化发送的方式，保证两端数据的
//一致性协调性。
#ifndef __INCLUDE_PROTOCOL_H__
#define __INCLUDE_PROTOCOL_H__
#include <Windows.h>
//最大密码长度
#define DEF_MAX_PASSWORD_LEN			(16)
//信息的最大长度
#define DEF_COMMON_MAX_LEN				(50)
//一次获取APP列表的长度
#define DEF_GET_APP_LIST_COUNT			(5)
//文件名称的最大长度
#define DEF_FILE_NAME_MAX_LEN			(20)
//每次发送数据的最大长度（保证不分包）
#define DEF_MAX_BUF						(1200)
//信息的最大长度
#define DEF_COMMON_MAX_LEN				(50)
//一次获取GAME列表的长度
#define DEF_GET_GAME_LIST_COUNT			(10)
//一次获取好友列表的长度
#define DEF_FRIEND_LIST_COUNT			(20)
//#define m_iGameID    1234       //端口号
//回复成功失败
enum ENUM_RQ_RESULT
{
	enum_res_success, 
	enum_res_unexist,
	enum_res_password_error, 
	enum_res_fail
};
enum ENUM_RS_PLAYSTRANGER
{
	enum_re_success, 
	enum_re_fail,
	enum_re_justwait 
	
};
///////////////////////////////////////////////////////////////////////////
//定义协议类型
#define DEF_PROTOCOL_BASE				(10010)

#define DEF_LOGIN_RQ					(DEF_PROTOCOL_BASE + 1)
#define DEF_LOGIN_RS					(DEF_PROTOCOL_BASE + 2)

#define DEF_UPLOAD_RQ					(DEF_PROTOCOL_BASE + 3)
#define DEF_UPLOAD_RS					(DEF_PROTOCOL_BASE + 4)

#define DEF_DOWNLOAD_RQ					(DEF_PROTOCOL_BASE + 5)
#define DEF_DOWNLOAD_RS					(DEF_PROTOCOL_BASE + 6)

#define DEF_GET_APP_LIST_RQ				(DEF_PROTOCOL_BASE + 7)
#define DEF_GET_APP_LIST_RS				(DEF_PROTOCOL_BASE + 8)

#define DEF_INSTALL_RQ					(DEF_PROTOCOL_BASE + 9)
#define DEF_INSTALL_RS					(DEF_PROTOCOL_BASE + 10)

#define DEF_UNINSTALL_RQ				(DEF_PROTOCOL_BASE + 11)
#define DEF_UNINSTALL_RS				(DEF_PROTOCOL_BASE + 12)

#define DEF_CONNECT_MESSAGE_RQ          (DEF_PROTOCOL_BASE + 13)
//1.心跳包
#define DEF_HEARTBEAT_RQ				(DEF_PROTOCOL_BASE + 14)
#define DEF_HEARTBEAT_RS				(DEF_PROTOCOL_BASE + 15) //服务端有没有必要回复？？
//2.通知游戏开始
#define DEF_NOTICE_BEGIN_RQ				(DEF_PROTOCOL_BASE + 16)
#define DEF_NOTICE_BEGIN_RS             (DEF_PROTOCOL_BASE + 17)
//3.通知游戏结束
#define DEF_NOTICE_END_RQ               (DEF_PROTOCOL_BASE + 18)
#define DEF_NOTICE_END_RS				(DEF_PROTOCOL_BASE + 19)
//4.发送游戏数据
#define DEF_SEND_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 20)
#define DEF_SEND_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 21)
//5.接收游戏数据
#define DEF_RECV_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 22)
#define DEF_RECV_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 23)
//6.请求游戏
#define DEF_READY_PLAY_RQ		     	(DEF_PROTOCOL_BASE + 24)
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

#define DEF_LOGOUT_RQ					(DEF_PROTOCOL_BASE + 13)



//下线

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
//1.DEF_HEARTBEAT_RQ
struct STRU_HEARTBEAT_RQ : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_HEARTBEAT_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
};

struct STRU_HEARTBEAT_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_HEARTBEAT_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	WORD m_wResult;
};
//2.DEF_NOTICE_BEGIN_RQ
struct STRU_NOTICE_BEGIN_RQ : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_NOTICE_BEGIN_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	int m_iTime;
	BOOL m_bIsFirst;
	
};

struct STRU_NOTICE_BEGIN_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_NOTICE_BEGIN_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	int m_iTime;
	WORD m_wResult;
	int m_iIndex;
};
//3.DEF_NOTICE_END_RQ
struct STRU_NOTICE_END_RQ : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_NOTICE_END_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	BOOL m_bIsWin;
};

struct STRU_NOTICE_END_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_NOTICE_END_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	WORD m_wResult;
};
//4.DEF_SEND_GAME_DATA_RQ
struct STRU_GAMEDATA
{
	STRU_GAMEDATA();
	long Serialize(char*& szBuf, long &lBufLen);
	BOOL UnSerialize(const char*& szBuf, long &lBufLen);
	static long MIN_LEN;
public:
	int m_iWayCounts;
	int m_iWhichWay;
};
struct STRU_SEND_GAME_DATA_RQ : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_SEND_GAME_DATA_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	int m_iIndex;
	STRU_GAMEDATA m_sGameData;
};

struct STRU_SEND_GAME_DATA_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_SEND_GAME_DATA_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	WORD m_wResult;
};
//5.DEF_RECV_GAME_DATA_RQ
struct STRU_RECV_GAME_DATA_RQ : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_RECV_GAME_DATA_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	STRU_GAMEDATA m_sGameData;
};

struct STRU_RECV_GAME_DATA_RS : public STRU_PROTOCOL_BASE
{
	static long MIN_LEN;
	STRU_RECV_GAME_DATA_RS();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;
	int m_iGameID;
	WORD m_wResult;
};

//6.请求游戏
struct STRU_READY_PLAY_RQ : public STRU_PROTOCOL_BASE
{
	STRU_READY_PLAY_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;		
	int m_iGameID;
	BOOL m_bIsFirst;
	int m_iIndex;
};

















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
	STRU_APP_INFO m_oAppList[DEF_GET_APP_LIST_COUNT];
};

//////////////////////////////////////////////////////////////////////////////////
struct STRU_INSTALL_RQ : public STRU_PROTOCOL_BASE
{
	STRU_INSTALL_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;						
	INT64 m_i64AppKey;				//通过app+version生成的key
};

//////////////////////////////////////////////////////////////////////////////////
struct STRU_UNINSTALL_RQ : public STRU_PROTOCOL_BASE
{
	STRU_UNINSTALL_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;						
	INT64 m_i64AppKey;				//通过app+version生成的key
};

//////////////////////////////////////////////////////////////////////////////////
struct STRU_LOGOUT_RQ : public STRU_PROTOCOL_BASE
{
	STRU_LOGOUT_RQ();
	long Serialize(char szBuf[], long lBufLen);
	BOOL UnSerialize(const char szBuf[], long lBufLen);
	static long MIN_LEN;
public:
	INT64 m_i64UserID;
	INT64 m_i64VerifyCode;						
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
//对战平台打开游戏后，需要给游戏发送 过去 自己的ID，游戏的ID，对手的ID  使用什么方式？？
//写配置文件，一个写完另一个读取
//游戏结束后如何处理，是直接关闭游戏


////发送己放游戏数据
//#define DEF_SEND_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 23)
//#define DEF_SEND_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 24)
////接收对方游戏数据
//#define DEF_RECV_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 25)
//#define DEF_RECV_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 26)

//注册
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