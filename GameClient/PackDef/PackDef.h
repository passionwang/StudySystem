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

//�ظ��ɹ�ʧ��
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

//������볤��
#define DEF_MAX_PASSWORD_LEN			(16)

//ÿ�η������ݵ���󳤶ȣ���֤���ְ���
#define DEF_MAX_BUF						(1400)

//��Ϣ����󳤶�
#define DEF_COMMON_MAX_LEN				(50)

//һ�λ�ȡGAME�б�ĳ���
#define DEF_GET_GAME_LIST_COUNT			(10)

//�ļ����Ƶ���󳤶�
#define DEF_FILE_NAME_MAX_LEN			(20)

//һ�λ�ȡ�����б�ĳ���
#define DEF_FRIEND_LIST_COUNT			(20)

///////////////////////////////////////////////////////////////////////////
//����Э������
#define DEF_PROTOCOL_BASE				(10010)
//��¼
#define DEF_LOGIN_RQ					(DEF_PROTOCOL_BASE + 1)
#define DEF_LOGIN_RS					(DEF_PROTOCOL_BASE + 2)
//�ϴ��ļ�
#define DEF_UPLOAD_RQ					(DEF_PROTOCOL_BASE + 3)
#define DEF_UPLOAD_RS					(DEF_PROTOCOL_BASE + 4)
//�����ļ�
#define DEF_DOWNLOAD_RQ					(DEF_PROTOCOL_BASE + 5)
#define DEF_DOWNLOAD_RS					(DEF_PROTOCOL_BASE + 6)
//��ȡ��Ϸ�б�
#define DEF_GET_GAME_LIST_RQ			(DEF_PROTOCOL_BASE + 7)
#define DEF_GET_GAME_LIST_RS			(DEF_PROTOCOL_BASE + 8)
//��װ
#define DEF_INSTALL_RQ					(DEF_PROTOCOL_BASE + 9)
#define DEF_INSTALL_RS					(DEF_PROTOCOL_BASE + 10)
//ж��
#define DEF_UNINSTALL_RQ				(DEF_PROTOCOL_BASE + 11)
#define DEF_UNINSTALL_RS				(DEF_PROTOCOL_BASE + 12)
//������Ϣ-֪ͨ����������SOCKET
#define DEF_CONNECT_MESSAGE_RQ          (DEF_PROTOCOL_BASE + 13)
//������
#define DEF_HEARTBEAT_RQ				(DEF_PROTOCOL_BASE + 14)
#define DEF_HEARTBEAT_RS				(DEF_PROTOCOL_BASE + 15) //�������û�б�Ҫ�ظ�����
//ע��
#define DEF_REGISTER_RQ					(DEF_PROTOCOL_BASE + 30)
#define DEF_REGISTER_RS					(DEF_PROTOCOL_BASE + 31)

//������Ϸ
#define DEF_DOWNLOAD_GAME_RQ			(DEF_PROTOCOL_BASE + 32)
#define DEF_DOWNLOAD_GAME_RS			(DEF_PROTOCOL_BASE + 33)
//İ����ƥ����Ϸ
#define DEF_PLAY_STRANGER_RQ			(DEF_PROTOCOL_BASE + 34)
#define DEF_PLAY_STRANGER_RS			(DEF_PROTOCOL_BASE + 35)
//��ȡ�����б�
#define DEF_GET_FRIEND_LIST_RQ			(DEF_PROTOCOL_BASE + 36)
#define DEF_GET_FRIEND_LIST_RS			(DEF_PROTOCOL_BASE + 37)
//���������Ϸ
#define DEF_INVITE_FRIEND_RQ			(DEF_PROTOCOL_BASE + 38)
#define DEF_INVITE_FRIEND_RS			(DEF_PROTOCOL_BASE + 39)
//������������Ϸ
#define DEF_BE_INVITE_FRIEND_RQ			(DEF_PROTOCOL_BASE + 40)
#define DEF_BE_INVITE_FRIEND_RS			(DEF_PROTOCOL_BASE + 41)
//�Ӻ���
#define DEF_ADD_FRIEND_RQ				(DEF_PROTOCOL_BASE + 42)
#define DEF_ADD_FRIEND_RS				(DEF_PROTOCOL_BASE + 43)
//ɾ������
#define DEF_DELETE_FRIEND_RQ			(DEF_PROTOCOL_BASE + 44)
#define DEF_DELETE_FRIEND_RS			(DEF_PROTOCOL_BASE + 45)

//////////����������������������������������������Ҫ��������𣿣�����
//#define DEF_FIND_PLAYER_RQ				(DEF_PROTOCOL_BASE +  XXX )
//#define DEF_FIND_PLAYER_RS				(DEF_PROTOCOL_BASE +  XXX )

//����
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
	WORD m_wProtocolType;						//ÿ����Э��ṹ�������ֶ�
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
	INT64 m_i64VerifyCode;						//�ɷ����������û�ID+��½ʱ�����ɣ��·����ͻ��ˣ�������Ϊ�Ժ�ͻ���������������������֤��ʾ
	WORD m_wResult;								//����enum ENUM_RQ_RESULT
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
	INT64 m_i64AppKey;				//ͨ��app+version���ɵ�key
};

//app��Ϣ�ṹ
struct STRU_APP_INFO
{
	STRU_APP_INFO();
	long Serialize(char*& szBuf, long &lBufLen);
	BOOL UnSerialize(const char*& szBuf, long &lBufLen);
	static long MIN_LEN;

public:
	INT64 m_i64UserID;				//APP�Ĺ�����
	INT64 m_i64AppKey;				//ͨ��app+version���ɵ�key
	DWORD m_dwSharedTime;			//�ϴ���ʱ��
	DWORD m_dwDownTimes;			//���ش���
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
	INT64 m_i64StrangerID;					//ƥ��ɹ��Ķ���ID
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
	//���ص�һ�������ߵģ�����ά��һ�������б��ļ�
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
//------������������������������Ҫ�������ߺͱ������߷ֱ��Ͳ�һ���Ļظ���
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
//------������������������������Ҫ�������ߺͱ������߷ֱ��Ͳ�һ���Ļظ���
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
//------������������������������Ҫ�ԼӺ��ѵ��� �� ���Ӻ��ѵ���  �ֱ��Ͳ�һ���Ļظ���
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
//------������������������������Ҫ��ɾ�����ѵ��� �� ��ɾ�����ѵ���  �ֱ��Ͳ�һ���Ļظ���
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


//��սƽ̨����Ϸ����Ҫ����Ϸ���� ��ȥ �Լ���ID����Ϸ��ID�����ֵ�ID  ʹ��ʲô��ʽ����
//д�����ļ���һ��д����һ����ȡ
//��Ϸ��������δ�����ֱ�ӹر���Ϸ


////���ͼ�����Ϸ����
//#define DEF_SEND_GAME_DATA_RQ			(DEF_PROTOCOL_BASE + 23)
//#define DEF_SEND_GAME_DATA_RS			(DEF_PROTOCOL_BASE + 24)
////���նԷ���Ϸ����
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

//�ϴ��ļ�
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
//�������
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

//�����ļ�
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
//�����ļ��������
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

//tcp���ͣ���������������
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