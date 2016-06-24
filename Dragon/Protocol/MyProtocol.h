#pragma once
//1.ʲô��Э�飺ͨ��˫����֪�����Ҷ����ص�һ�׹���ÿһ������Ϊһ�Խṹ��
//2.Ϊ�˽��˫���յ�����Ϊ�ֽ�����λ�������޷�֪���Է����͵���Ϣ�ľ��庬����ʲô����ÿ��Э��ṹ����ͬλ��
//�̶���λ�ô���ܹ���ʾ�յ��������͵��ֶΣ��Ӷ��յ����ݵ�һ�����Ը�����������ֶ�ȷ���յ����ݵľ���
//���壨����һ�ֽṹ��
//3.Ϊ�˷�ֹ�ṹ�������ֶα���ֵ������ͨ��ʧ�ܣ���Э�������ֶη��빫�������н��б����̳�
//4.Э���г��ֵ��ַ������Գ�Ա�������ж�Ӧ�ĳ����ֶΣ��Ӷ�ÿ�η���ֻ��Ҫ������Ч�ĳ��ȣ�����Ҫ����
//�����ȫ�����ȣ����ٷ������ݵ���
//5.�������˶��뷽ʽ��ͬ�������շ����ݴ�λ������ÿ��Ĭ�ϵĶ��뷽ʽ���Ӷ�ʹÿ��I/O��Ч����Ȼ�ܸߣ�Ȼ�����
//����Ҫ���͵���Ч���ݰ��չ̶���˳�����ʵ�Ĵ�С��䵽һ��������buf�н������л����͵ķ�ʽ����֤�������ݵ�
//һ����Э���ԡ�
#ifndef __INCLUDE_PROTOCOL_H__
#define __INCLUDE_PROTOCOL_H__
#include <Windows.h>
//#define m_iGameID    1234       //�˿ں�
//�ظ��ɹ�ʧ��
enum ENUM_RQ_RESULT
{
	enum_res_success, 
	enum_res_unexist,
	enum_res_password_error, 
	enum_res_fail
};

///////////////////////////////////////////////////////////////////////////
//����Э������
#define DEF_PROTOCOL_BASE				(10010)
//1.������
#define DEF_HEARTBEAT_RQ				(DEF_PROTOCOL_BASE + 14)
#define DEF_HEARTBEAT_RS				(DEF_PROTOCOL_BASE + 15) //�������û�б�Ҫ�ظ�����
//2.֪ͨ��Ϸ��ʼ
#define DEF_NOTICE_BEGIN_RQ				(DEF_PROTOCOL_BASE + 16)
#define DEF_NOTICE_BEGIN_RS             (DEF_PROTOCOL_BASE + 17)
//3.֪ͨ��Ϸ����
#define DEF_NOTICE_END_RQ               (DEF_PROTOCOL_BASE + 18)
#define DEF_NOTICE_END_RS				(DEF_PROTOCOL_BASE + 19)
//4.������Ϸ����
#define DEF_SEND_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 20)
#define DEF_SEND_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 21)
//5.������Ϸ����
#define DEF_RECV_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 22)
#define DEF_RECV_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 23)
//6.������Ϸ
#define DEF_READY_PLAY_RQ		     	(DEF_PROTOCOL_BASE + 24)
//����
#define DEF_LOGOUT_RQ					(DEF_PROTOCOL_BASE + 13)

#define DEF_PROTOCOL_COUNT				(100)

///////////////////////////////////////////////////////////////////////////

//ÿ�η������ݵ���󳤶ȣ���֤���ְ���
#define DEF_MAX_BUF						(1200)

//��Ϣ����󳤶�
#define DEF_COMMON_MAX_LEN				(50)

struct STRU_PROTOCOL_BASE
{
	STRU_PROTOCOL_BASE(WORD wType) : m_wProtocolType(wType)
	{
	
	}
	virtual long Serialize(char szBuf[], long lBufLen) = 0;
	virtual BOOL UnSerialize(const char szBuf[], long lBufLen) = 0;
protected:
	WORD m_wProtocolType;						//ÿ����Э��ṹ�������ֶ�
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

//6.������Ϸ
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

//��սƽ̨����Ϸ����Ҫ����Ϸ���� ��ȥ �Լ���ID����Ϸ��ID�����ֵ�ID  ʹ��ʲô��ʽ����
//д�����ļ���һ��д����һ����ȡ
//��Ϸ��������δ�����ֱ�ӹر���Ϸ


////���ͼ�����Ϸ����
//#define DEF_SEND_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 23)
//#define DEF_SEND_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 24)
////���նԷ���Ϸ����
//#define DEF_RECV_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 25)
//#define DEF_RECV_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 26)
#endif