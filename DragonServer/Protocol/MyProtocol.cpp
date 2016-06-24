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








long STRU_LOGIN_RQ::MIN_LEN = 12;
STRU_LOGIN_RQ::STRU_LOGIN_RQ() : STRU_PROTOCOL_BASE(DEF_LOGIN_RQ)
{
	m_i64UserID = 0;
	m_wPasswordLen= 0;
}
long STRU_LOGIN_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_LOGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//������Ч����
	*(WORD*)szBuf = m_wPasswordLen;
	szBuf += sizeof(m_wPasswordLen);

	//����ʣ�೤��
	lBufLen -= STRU_LOGIN_RQ::MIN_LEN;

	//����
	if (lBufLen < m_wPasswordLen)
	{
		return 0;
	}
	memcpy(szBuf, m_szPassword, m_wPasswordLen);
	szBuf += m_wPasswordLen;

	return (STRU_LOGIN_RQ::MIN_LEN + m_wPasswordLen);
}
BOOL STRU_LOGIN_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_LOGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//���볤��
	m_wPasswordLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wPasswordLen);
	lBufLen -= STRU_LOGIN_RQ::MIN_LEN;

	//���������Ч����
	if (lBufLen < m_wPasswordLen)
	{
		return FALSE;
	}
	memcpy(m_szPassword, szBuf, m_wPasswordLen);
	szBuf += m_wPasswordLen;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
long STRU_LOGIN_RS::MIN_LEN = 20;
STRU_LOGIN_RS::STRU_LOGIN_RS() : STRU_PROTOCOL_BASE(DEF_LOGIN_RS)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_wResult = enum_res_fail;
}
long STRU_LOGIN_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_LOGIN_RS::MIN_LEN > lBufLen)
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

	//���
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	//���
	lBufLen -= STRU_LOGIN_RS::MIN_LEN;

	return (STRU_LOGIN_RS::MIN_LEN);
}
BOOL STRU_LOGIN_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_LOGIN_RS::MIN_LEN > lBufLen)
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

	//���
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	//���
	lBufLen -= STRU_LOGIN_RS::MIN_LEN;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
long STRU_GET_APP_LIST_RQ::MIN_LEN = 26;
STRU_GET_APP_LIST_RQ::STRU_GET_APP_LIST_RQ() : STRU_PROTOCOL_BASE(DEF_GET_APP_LIST_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_i64AppKey = 0;
}
long STRU_GET_APP_LIST_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_GET_APP_LIST_RQ::MIN_LEN > lBufLen)
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

	//�ļ�key
	*(INT64*)szBuf = m_i64AppKey;
	szBuf += sizeof(m_i64AppKey);

	return (STRU_GET_APP_LIST_RQ::MIN_LEN);
}
BOOL STRU_GET_APP_LIST_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_GET_APP_LIST_RQ::MIN_LEN > lBufLen)
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

	//�ļ�key
	m_i64AppKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64AppKey);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
long STRU_GET_APP_LIST_RS::MIN_LEN = 12;
STRU_GET_APP_LIST_RS::STRU_GET_APP_LIST_RS() : STRU_PROTOCOL_BASE(DEF_GET_APP_LIST_RS)
{
	m_i64UserID = 0;
	m_wAppItemCount = 0;
}
long STRU_GET_APP_LIST_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_GET_APP_LIST_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//��Ч���������
	*(WORD*)szBuf = m_wAppItemCount;
	szBuf += sizeof(m_wAppItemCount);

	//���
	lBufLen -= STRU_GET_APP_LIST_RS::MIN_LEN;

	//���л�app��Ϣ����
	long lItemLen = 0;
	for (long i = 0; i < m_wAppItemCount; ++i)
	{
		lItemLen += m_oAppList[i].Serialize(szBuf, lBufLen);	
	}

	return (STRU_GET_APP_LIST_RS::MIN_LEN+lItemLen);
}
BOOL STRU_GET_APP_LIST_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_GET_APP_LIST_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//��Ч���������
	m_wAppItemCount = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppItemCount);

	//���
	lBufLen -= STRU_GET_APP_LIST_RS::MIN_LEN;

	for (long i = 0; i < m_wAppItemCount; ++i)
	{
		m_oAppList[i].UnSerialize(szBuf, lBufLen);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
long STRU_APP_INFO::MIN_LEN = 32;
STRU_APP_INFO::STRU_APP_INFO()
{
	m_i64UserID = 0;
	m_i64AppKey = 0;
	m_dwSharedTime= 0;
	m_dwDownTimes=0;
	m_wAppDesLen= 0;
	m_wAppIconPathLen=0;
	m_wAppNameLen = 0;
	m_wAppPathLen = 0;
}
long STRU_APP_INFO::Serialize(char*& szBuf, long &lBufLen)
{
	//��鳤��
	if (STRU_APP_INFO::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//�û�id
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//�û���֤��
	*(INT64*)szBuf = m_i64AppKey;
	szBuf += sizeof(m_i64AppKey);

	//����ʱ��
	*(DWORD*)szBuf = m_dwSharedTime;
	szBuf += sizeof(m_dwSharedTime);

	//����ʱ��
	*(DWORD*)szBuf = m_dwDownTimes;
	szBuf += sizeof(m_dwDownTimes);

	//��������
	*(WORD*)szBuf = m_wAppDesLen;
	szBuf += sizeof(m_wAppDesLen);

	//ͼ�����Ƴ���
	*(WORD*)szBuf = m_wAppIconPathLen;
	szBuf += sizeof(m_wAppIconPathLen);

	//APP���Ƴ���
	*(WORD*)szBuf = m_wAppNameLen;
	szBuf += sizeof(m_wAppNameLen);

	//APP���Ƴ���
	*(WORD*)szBuf = m_wAppPathLen;
	szBuf += sizeof(m_wAppPathLen);

	memcpy(szBuf, m_szAppDescript, m_wAppDesLen);
	szBuf += m_wAppDesLen;
	memcpy(szBuf, m_szIconPath, m_wAppIconPathLen);
	szBuf += m_wAppIconPathLen;
	memcpy(szBuf, m_szAppName, m_wAppNameLen);
	szBuf += m_wAppNameLen;
	memcpy(szBuf, m_szAppPath, m_wAppPathLen);
	szBuf += m_wAppPathLen;

	return (STRU_APP_INFO::MIN_LEN + m_wAppDesLen + m_wAppIconPathLen + m_wAppNameLen + m_wAppPathLen);
}
BOOL STRU_APP_INFO::UnSerialize(const char*& szBuf, long &lBufLen)
{
	//��鳤��
	if (STRU_APP_INFO::MIN_LEN > lBufLen)
	{
		return FALSE;
	}
	//�û�id
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//�û���֤��
	m_i64AppKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64AppKey);

	//����ʱ��
	m_dwSharedTime = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwSharedTime);

	//����ʱ��
	m_dwDownTimes = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwDownTimes);

	//��������
	m_wAppDesLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppDesLen);

	//ͼ�����Ƴ���
	m_wAppIconPathLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppIconPathLen);

	//APP���Ƴ���
	m_wAppNameLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppNameLen);

	//APP���Ƴ���
	m_wAppPathLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppPathLen);

	memcpy(m_szAppDescript, szBuf, m_wAppDesLen);
	szBuf += m_wAppDesLen;
	memcpy(m_szIconPath, szBuf, m_wAppIconPathLen);
	szBuf += m_wAppIconPathLen;
	memcpy(m_szAppName, szBuf, m_wAppNameLen);
	szBuf += m_wAppNameLen;
	memcpy(m_szAppPath, szBuf, m_wAppPathLen);
	szBuf += m_wAppPathLen;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
long STRU_INSTALL_RQ::MIN_LEN = 26;
STRU_INSTALL_RQ::STRU_INSTALL_RQ() : STRU_PROTOCOL_BASE(DEF_INSTALL_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_i64AppKey = 0;
}
long STRU_INSTALL_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_INSTALL_RQ::MIN_LEN > lBufLen)
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

	//�ļ�key
	*(INT64*)szBuf = m_i64AppKey;
	szBuf += sizeof(m_i64AppKey);

	return (STRU_INSTALL_RQ::MIN_LEN);
}
BOOL STRU_INSTALL_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_INSTALL_RQ::MIN_LEN > lBufLen)
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

	//�ļ�key
	m_i64AppKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64AppKey);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
long STRU_UNINSTALL_RQ::MIN_LEN = 26;
STRU_UNINSTALL_RQ::STRU_UNINSTALL_RQ() : STRU_PROTOCOL_BASE(DEF_UNINSTALL_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_i64AppKey = 0;
}
long STRU_UNINSTALL_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_UNINSTALL_RQ::MIN_LEN > lBufLen)
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

	//�ļ�key
	*(INT64*)szBuf = m_i64AppKey;
	szBuf += sizeof(m_i64AppKey);

	return (STRU_UNINSTALL_RQ::MIN_LEN);
}
BOOL STRU_UNINSTALL_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_UNINSTALL_RQ::MIN_LEN > lBufLen)
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

	//�ļ�key
	m_i64AppKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64AppKey);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
long STRU_LOGOUT_RQ::MIN_LEN = 18;
STRU_LOGOUT_RQ::STRU_LOGOUT_RQ() : STRU_PROTOCOL_BASE(DEF_LOGOUT_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
}
long STRU_LOGOUT_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_LOGOUT_RQ::MIN_LEN > lBufLen)
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

	return (STRU_LOGOUT_RQ::MIN_LEN);
}
BOOL STRU_LOGOUT_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_LOGOUT_RQ::MIN_LEN > lBufLen)
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

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//�ϴ��ļ�
long STRU_FILE_HEAD_UPLOAD::MIN_LEN = 18;
STRU_FILE_HEAD_UPLOAD::STRU_FILE_HEAD_UPLOAD() : STRU_PROTOCOL_BASE(DEF_UPLOAD_RQ)
{
	m_i64UserID = 0;
}
long STRU_FILE_HEAD_UPLOAD::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_HEAD_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	*(DWORD*)szBuf = m_dwFileLength;
	szBuf += sizeof(m_dwFileLength);


	*(WORD*)szBuf = m_wVersion;
	szBuf += sizeof(m_wVersion);

	*(WORD*)szBuf = m_wAppNameLen;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(szBuf, m_szAppName, m_wAppNameLen);

	return (STRU_FILE_HEAD_UPLOAD::MIN_LEN + m_wAppNameLen);
}
BOOL STRU_FILE_HEAD_UPLOAD::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_HEAD_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	m_dwFileLength = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwFileLength);


	m_wVersion = *(WORD*)szBuf;
	szBuf += sizeof(m_wVersion);

	m_wAppNameLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(m_szAppName, szBuf, m_wAppNameLen);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
long STRU_FILE_TRANS_COMPLETE_UPLOAD::MIN_LEN = 22;
STRU_FILE_TRANS_COMPLETE_UPLOAD::STRU_FILE_TRANS_COMPLETE_UPLOAD() : STRU_PROTOCOL_BASE(DEF_UPLOAD_RS)
{
	m_i64UserID = 0;
	m_i64FileKey = 0;
	m_wVersion=0;
	m_wAppNameLen=0;
}
long STRU_FILE_TRANS_COMPLETE_UPLOAD::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_TRANS_COMPLETE_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	*(INT64*)szBuf = m_i64FileKey;
	szBuf += sizeof(m_i64FileKey);


	*(WORD*)szBuf = m_wVersion;
	szBuf += sizeof(m_wVersion);

	*(WORD*)szBuf = m_wAppNameLen;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(szBuf, m_szAppName, m_wAppNameLen);

	return (STRU_FILE_TRANS_COMPLETE_UPLOAD::MIN_LEN + m_wAppNameLen);
}
BOOL STRU_FILE_TRANS_COMPLETE_UPLOAD::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_TRANS_COMPLETE_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	m_i64FileKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64FileKey);


	m_wVersion = *(WORD*)szBuf;
	szBuf += sizeof(m_wVersion);

	m_wAppNameLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(m_szAppName, szBuf, m_wAppNameLen);

	return TRUE;
}
//�����ļ�
long STRU_FILE_HEAD_DOWNLOAD::MIN_LEN = 18;
STRU_FILE_HEAD_DOWNLOAD::STRU_FILE_HEAD_DOWNLOAD() : STRU_PROTOCOL_BASE(DEF_DOWNLOAD_RQ)
{
	m_i64UserID = 0;
}
long STRU_FILE_HEAD_DOWNLOAD::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_HEAD_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	*(DWORD*)szBuf = m_dwFileLength;
	szBuf += sizeof(m_dwFileLength);


	*(WORD*)szBuf = m_wVersion;
	szBuf += sizeof(m_wVersion);

	*(WORD*)szBuf = m_wAppNameLen;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(szBuf, m_szAppName, m_wAppNameLen);

	return (STRU_FILE_HEAD_DOWNLOAD::MIN_LEN + m_wAppNameLen);
}
BOOL STRU_FILE_HEAD_DOWNLOAD::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_HEAD_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	m_dwFileLength = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwFileLength);


	m_wVersion = *(WORD*)szBuf;
	szBuf += sizeof(m_wVersion);

	m_wAppNameLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(m_szAppName, szBuf, m_wAppNameLen);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
long STRU_FILE_TRANS_COMPLETE_DOWNLOAD::MIN_LEN = 22;
STRU_FILE_TRANS_COMPLETE_DOWNLOAD::STRU_FILE_TRANS_COMPLETE_DOWNLOAD() : STRU_PROTOCOL_BASE(DEF_DOWNLOAD_RS)
{
	m_i64UserID = 0;
	m_i64FileKey = 0;
	m_wVersion=0;
	m_wAppNameLen=0;
}
long STRU_FILE_TRANS_COMPLETE_DOWNLOAD::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_TRANS_COMPLETE_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	*(INT64*)szBuf = m_i64FileKey;
	szBuf += sizeof(m_i64FileKey);


	*(WORD*)szBuf = m_wVersion;
	szBuf += sizeof(m_wVersion);

	*(WORD*)szBuf = m_wAppNameLen;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(szBuf, m_szAppName, m_wAppNameLen);

	return (STRU_FILE_TRANS_COMPLETE_DOWNLOAD::MIN_LEN + m_wAppNameLen);
}
BOOL STRU_FILE_TRANS_COMPLETE_DOWNLOAD::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_FILE_TRANS_COMPLETE_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	m_i64FileKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64FileKey);


	m_wVersion = *(WORD*)szBuf;
	szBuf += sizeof(m_wVersion);

	m_wAppNameLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppNameLen);

	memcpy(m_szAppName, szBuf, m_wAppNameLen);

	return TRUE;
}




long STRU_PLAY_STRANGER_RQ::MIN_LEN = 22;
STRU_PLAY_STRANGER_RQ::STRU_PLAY_STRANGER_RQ() : STRU_PROTOCOL_BASE(DEF_PLAY_STRANGER_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
}
long STRU_PLAY_STRANGER_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_PLAY_STRANGER_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//Э������
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//�û�ID
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//�û�ID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	return (STRU_PLAY_STRANGER_RQ::MIN_LEN);
}
BOOL STRU_PLAY_STRANGER_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_LOGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//�����л�������
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//�û�ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//�û�ID
	m_i64VerifyCode = *(INT64*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);

	//���볤��
	m_iGameID = *(WORD*)szBuf;
	szBuf += sizeof(m_iGameID);
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
long STRU_PLAY_STRANGER_RS::MIN_LEN = 32;
STRU_PLAY_STRANGER_RS::STRU_PLAY_STRANGER_RS() : STRU_PROTOCOL_BASE(DEF_PLAY_STRANGER_RS)
{
	m_i64UserID = 0;
	m_i64StrangerID = 0;					//ƥ��ɹ��Ķ���ID
	m_iGameID = 0;
	m_iIndex = 0;
	m_bIsFirst = FALSE;
	m_wResult = enum_re_fail;	
}
long STRU_PLAY_STRANGER_RS::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_PLAY_STRANGER_RS::MIN_LEN > lBufLen)
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
	*(INT64*)szBuf = m_i64StrangerID;
	szBuf += sizeof(m_i64StrangerID);

	//�û�ID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//�û�ID
	*(int*)szBuf = m_iIndex;
	szBuf += sizeof(m_iIndex);

	//�û�ID
	*(BOOL*)szBuf = m_bIsFirst;
	szBuf += sizeof(m_bIsFirst);

	//���
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	//���
	lBufLen -= STRU_PLAY_STRANGER_RS::MIN_LEN;

	return (STRU_PLAY_STRANGER_RS::MIN_LEN);
}
BOOL STRU_PLAY_STRANGER_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_PLAY_STRANGER_RS::MIN_LEN > lBufLen)
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
	m_i64StrangerID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64StrangerID);

	//���볤��
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//���볤��
	m_iIndex = *(int*)szBuf;
	szBuf += sizeof(m_iIndex);

	//���볤��
	m_bIsFirst = *(BOOL*)szBuf;
	szBuf += sizeof(m_bIsFirst);

	//���
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	//���
	lBufLen -= STRU_PLAY_STRANGER_RS::MIN_LEN;

	return TRUE;
}

//������Ϸ
long STRU_DOWNLOAD_GAME_RQ::MIN_LEN = 22;
STRU_DOWNLOAD_GAME_RQ::STRU_DOWNLOAD_GAME_RQ() : STRU_PROTOCOL_BASE(DEF_DOWNLOAD_GAME_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
}
long STRU_DOWNLOAD_GAME_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_DOWNLOAD_GAME_RQ::MIN_LEN > lBufLen)
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

	return (STRU_DOWNLOAD_GAME_RQ::MIN_LEN);
}
BOOL STRU_DOWNLOAD_GAME_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_DOWNLOAD_GAME_RQ::MIN_LEN > lBufLen)
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
	
	//��ϷID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	return TRUE;
}



//tcp���ͣ���������������
//DEF_CONNECT_MESSAGE_RQ
long STRU_CONNECT_MESSAGE_RQ::MIN_LEN = 18;
STRU_CONNECT_MESSAGE_RQ::STRU_CONNECT_MESSAGE_RQ() : STRU_PROTOCOL_BASE(DEF_CONNECT_MESSAGE_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
}
long STRU_CONNECT_MESSAGE_RQ::Serialize(char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_CONNECT_MESSAGE_RQ::MIN_LEN > lBufLen)
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

	//���
	lBufLen -= STRU_CONNECT_MESSAGE_RQ::MIN_LEN;

	return (STRU_CONNECT_MESSAGE_RQ::MIN_LEN);
}
BOOL STRU_CONNECT_MESSAGE_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//��鳤��
	if (STRU_CONNECT_MESSAGE_RQ::MIN_LEN > lBufLen)
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

	//���
	lBufLen -= STRU_LOGIN_RS::MIN_LEN;

	return TRUE;
}

