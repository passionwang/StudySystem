#include "stdafx.h"
#include "MyProtocol.h"
////////////////////////////////////////////////////////////////////
//1.//1.DEF_HEARTBEAT_RQ
long STRU_HEARTBEAT_RQ::MIN_LEN = 24;
STRU_HEARTBEAT_RQ::STRU_HEARTBEAT_RQ() : STRU_PROTOCOL_BASE(DEF_HEARTBEAT_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
}
long STRU_HEARTBEAT_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_HEARTBEAT_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	return (STRU_HEARTBEAT_RQ::MIN_LEN);
}
BOOL STRU_HEARTBEAT_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_HEARTBEAT_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	return TRUE;
}
//////////////////////////////////////
long STRU_HEARTBEAT_RS::MIN_LEN = 28;
STRU_HEARTBEAT_RS::STRU_HEARTBEAT_RS() : STRU_PROTOCOL_BASE(DEF_HEARTBEAT_RS)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_wResult = 0;
}
long STRU_HEARTBEAT_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_HEARTBEAT_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//���
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	return (STRU_HEARTBEAT_RS::MIN_LEN);
}
BOOL STRU_HEARTBEAT_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_HEARTBEAT_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//���
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////
//2.DEF_NOTICE_BEGIN_RQ
long STRU_NOTICE_BEGIN_RQ::MIN_LEN = 32;
STRU_NOTICE_BEGIN_RQ::STRU_NOTICE_BEGIN_RQ() : STRU_PROTOCOL_BASE(DEF_NOTICE_BEGIN_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_iTime = 30;
	m_bIsFirst = FALSE;
	
}
long STRU_NOTICE_BEGIN_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_BEGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//ʱ��
	*(int*)szBuf = m_iTime;
	szBuf += sizeof(m_iTime);
	
	//�Ƿ�����
	*(BOOL*)szBuf = m_bIsFirst;
	szBuf += sizeof(m_bIsFirst);

	

	return (STRU_NOTICE_BEGIN_RQ::MIN_LEN);
}
BOOL STRU_NOTICE_BEGIN_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_BEGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//ʱ��
	m_iTime = *(int*)szBuf;
	szBuf += sizeof(m_iTime);

	//�Ƿ�����
	m_bIsFirst = *(BOOL*)szBuf;
	szBuf += sizeof(m_bIsFirst);

	

	return TRUE;
}

//////////////////////////////////////
long STRU_NOTICE_BEGIN_RS::MIN_LEN = 36;
STRU_NOTICE_BEGIN_RS::STRU_NOTICE_BEGIN_RS() : STRU_PROTOCOL_BASE(DEF_NOTICE_BEGIN_RS)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_iTime = 0;
	m_wResult = 0;
	m_iIndex = 0;
}
long STRU_NOTICE_BEGIN_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_BEGIN_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//����ʱ��
	*(int*)szBuf = m_iTime;
	szBuf += sizeof(m_iTime);

	//���
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	//�Ƿ�����
	*(int*)szBuf = m_iIndex;
	szBuf += sizeof(m_iIndex);

	return (STRU_NOTICE_BEGIN_RS::MIN_LEN);
}
BOOL STRU_NOTICE_BEGIN_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_BEGIN_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//����ʱ��
	m_iTime = *(int*)szBuf;
	szBuf += sizeof(m_iTime);

	//���
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	//ʱ��
	m_iIndex = *(int*)szBuf;
	szBuf += sizeof(m_iIndex);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////
//3.DEF_NOTICE_END_RQ
long STRU_NOTICE_END_RQ::MIN_LEN = 28;
STRU_NOTICE_END_RQ::STRU_NOTICE_END_RQ() : STRU_PROTOCOL_BASE(DEF_NOTICE_END_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_bIsWin = FALSE;
}
long STRU_NOTICE_END_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_END_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);
	
	//�Ƿ�����
	*(BOOL*)szBuf = m_bIsWin;
	szBuf += sizeof(m_bIsWin);

	return (STRU_NOTICE_END_RQ::MIN_LEN);
}
BOOL STRU_NOTICE_END_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_END_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//�Ƿ�����
	m_bIsWin = *(BOOL*)szBuf;
	szBuf += sizeof(m_bIsWin);

	return TRUE;
}
//////////////////////////////////////
long STRU_NOTICE_END_RS::MIN_LEN = 28;
STRU_NOTICE_END_RS::STRU_NOTICE_END_RS() : STRU_PROTOCOL_BASE(DEF_NOTICE_END_RS)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_wResult = 0;
}
long STRU_NOTICE_END_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_END_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//���
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	return (STRU_NOTICE_END_RS::MIN_LEN);
}
BOOL STRU_NOTICE_END_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_NOTICE_END_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//���
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////
//4.DEF_SEND_GAME_DATA_RQ
long STRU_GAMEDATA::MIN_LEN = 8;
STRU_GAMEDATA::STRU_GAMEDATA()
{
	m_iWayCounts = 0;
	m_iWhichWay = 0;
}
long STRU_GAMEDATA::Serialize(char*& szBuf, long &lBufLen)
{
	//��鳤��
	if (STRU_GAMEDATA::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//�߷���������
	*(int*)szBuf = m_iWayCounts;
	szBuf += sizeof(m_iWayCounts);

	//��i���߷�
	*(int*)szBuf = m_iWhichWay;
	szBuf += sizeof(m_iWhichWay);

	return STRU_GAMEDATA::MIN_LEN;
}
BOOL STRU_GAMEDATA::UnSerialize(const char*& szBuf, long &lBufLen)
{
	//��鳤��
	if (STRU_GAMEDATA::MIN_LEN > lBufLen)
	{
		return FALSE;
	}
	//�߷���������
	m_iWayCounts = *(int*)szBuf;
	szBuf += sizeof(m_iWayCounts);

	//��i���߷�
	m_iWhichWay = *(int*)szBuf;
	szBuf += sizeof(m_iWhichWay);

	return TRUE;
}

long STRU_SEND_GAME_DATA_RQ::MIN_LEN = 28;
STRU_SEND_GAME_DATA_RQ::STRU_SEND_GAME_DATA_RQ() : STRU_PROTOCOL_BASE(DEF_SEND_GAME_DATA_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_iIndex = 0;
}
long STRU_SEND_GAME_DATA_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_SEND_GAME_DATA_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//��ϷID
	*(int*)szBuf = m_iIndex;
	szBuf += sizeof(m_iIndex);

	//���
	lBufLen -= STRU_SEND_GAME_DATA_RQ::MIN_LEN;

	long lItemLen = m_sGameData.Serialize(szBuf, lBufLen);

	return (STRU_SEND_GAME_DATA_RQ::MIN_LEN + lItemLen);
}
BOOL STRU_SEND_GAME_DATA_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_SEND_GAME_DATA_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	m_iIndex = *(int*)szBuf;
	szBuf += sizeof(m_iIndex);

	//���
	lBufLen -= STRU_SEND_GAME_DATA_RQ::MIN_LEN;

	m_sGameData.UnSerialize(szBuf, lBufLen);
	
	return TRUE;
}
//////////////////////////////////////
long STRU_SEND_GAME_DATA_RS::MIN_LEN = 28;
STRU_SEND_GAME_DATA_RS::STRU_SEND_GAME_DATA_RS() : STRU_PROTOCOL_BASE(DEF_SEND_GAME_DATA_RS)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_wResult = 0;
}
long STRU_SEND_GAME_DATA_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_SEND_GAME_DATA_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//���
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	return (STRU_SEND_GAME_DATA_RS::MIN_LEN);
}
BOOL STRU_SEND_GAME_DATA_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_SEND_GAME_DATA_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//���
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////
//5.DEF_RECV_GAME_DATA_RQ
long STRU_RECV_GAME_DATA_RQ::MIN_LEN = 4;
STRU_RECV_GAME_DATA_RQ::STRU_RECV_GAME_DATA_RQ() : STRU_PROTOCOL_BASE(DEF_RECV_GAME_DATA_RQ)
{
}
long STRU_RECV_GAME_DATA_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_RECV_GAME_DATA_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//���
	lBufLen -= STRU_RECV_GAME_DATA_RQ::MIN_LEN;

	long lItemLen = m_sGameData.Serialize(szBuf, lBufLen);

	return (STRU_RECV_GAME_DATA_RQ::MIN_LEN + lItemLen);
}
BOOL STRU_RECV_GAME_DATA_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_RECV_GAME_DATA_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//���
	lBufLen -= STRU_RECV_GAME_DATA_RQ::MIN_LEN;

	m_sGameData.UnSerialize(szBuf, lBufLen);
	
	return TRUE;
}
//////////////////////////////////////
long STRU_RECV_GAME_DATA_RS::MIN_LEN = 28;
STRU_RECV_GAME_DATA_RS::STRU_RECV_GAME_DATA_RS() : STRU_PROTOCOL_BASE(DEF_RECV_GAME_DATA_RS)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_wResult = 0;
}
long STRU_RECV_GAME_DATA_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_RECV_GAME_DATA_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//��ϷID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//���
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	return (STRU_RECV_GAME_DATA_RS::MIN_LEN);
}
BOOL STRU_RECV_GAME_DATA_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_RECV_GAME_DATA_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(WORD*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//���
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//6.������Ϸ
long STRU_READY_PLAY_RQ::MIN_LEN = 30;
STRU_READY_PLAY_RQ::STRU_READY_PLAY_RQ() : STRU_PROTOCOL_BASE(DEF_READY_PLAY_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
	m_bIsFirst = FALSE;
	m_iIndex = 0;
}
long STRU_READY_PLAY_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_READY_PLAY_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//������Ч����
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//�Ƿ�����
	*(BOOL*)szBuf = m_bIsFirst;
	szBuf += sizeof(m_bIsFirst);

	//�Ƿ�����
	*(int*)szBuf = m_iIndex;
	szBuf += sizeof(m_iIndex);

	return (STRU_READY_PLAY_RQ::MIN_LEN);
}
BOOL STRU_READY_PLAY_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_READY_PLAY_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��֤��
	m_i64VerifyCode = *(INT64*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);

	//���볤��
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//�Ƿ�����
	m_bIsFirst = *(BOOL*)szBuf;
	szBuf += sizeof(m_bIsFirst);

	//�Ƿ�����
	m_iIndex = *(int*)szBuf;
	szBuf += sizeof(m_iIndex);

	return TRUE;
}
