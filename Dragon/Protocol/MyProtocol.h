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
//#define m_iGameID    1234       //端口号
//回复成功失败
enum ENUM_RQ_RESULT
{
	enum_res_success, 
	enum_res_unexist,
	enum_res_password_error, 
	enum_res_fail
};

///////////////////////////////////////////////////////////////////////////
//定义协议类型
#define DEF_PROTOCOL_BASE				(10010)
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
//下线
#define DEF_LOGOUT_RQ					(DEF_PROTOCOL_BASE + 13)

#define DEF_PROTOCOL_COUNT				(100)

///////////////////////////////////////////////////////////////////////////

//每次发送数据的最大长度（保证不分包）
#define DEF_MAX_BUF						(1200)

//信息的最大长度
#define DEF_COMMON_MAX_LEN				(50)

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

//对战平台打开游戏后，需要给游戏发送 过去 自己的ID，游戏的ID，对手的ID  使用什么方式？？
//写配置文件，一个写完另一个读取
//游戏结束后如何处理，是直接关闭游戏


////发送己放游戏数据
//#define DEF_SEND_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 23)
//#define DEF_SEND_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 24)
////接收对方游戏数据
//#define DEF_RECV_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 25)
//#define DEF_RECV_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 26)
#endif