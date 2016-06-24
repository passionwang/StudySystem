#include "stdafx.h"
#include "PackDef.h"

long STRU_LOGIN_RQ::MIN_LEN = 12;
STRU_LOGIN_RQ::STRU_LOGIN_RQ() : STRU_PROTOCOL_BASE(DEF_LOGIN_RQ)
{
	m_i64UserID = 0;
	m_wPasswordLen= 0;
}
long STRU_LOGIN_RQ::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_LOGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//密码有效长度
	*(WORD*)szBuf = m_wPasswordLen;
	szBuf += sizeof(m_wPasswordLen);

	//计算剩余长度
	lBufLen -= STRU_LOGIN_RQ::MIN_LEN;

	//密码
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
	//检查长度
	if (STRU_LOGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//密码长度
	m_wPasswordLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wPasswordLen);
	lBufLen -= STRU_LOGIN_RQ::MIN_LEN;

	//检查密码有效长度
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
	//检查长度
	if (STRU_LOGIN_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//验证码
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//结果
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	//结果
	lBufLen -= STRU_LOGIN_RS::MIN_LEN;

	return (STRU_LOGIN_RS::MIN_LEN);
}
BOOL STRU_LOGIN_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_LOGIN_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//验证码
	m_i64VerifyCode = *(INT64*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);

	//结果
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	//结果
	lBufLen -= STRU_LOGIN_RS::MIN_LEN;

	return TRUE;
}









///////////////////////////////////////////////////////////////////////////////////
long STRU_LOGOUT_RQ::MIN_LEN = 10;
STRU_LOGOUT_RQ::STRU_LOGOUT_RQ() : STRU_PROTOCOL_BASE(DEF_LOGOUT_RQ)
{
	m_i64UserID = 0;
}
long STRU_LOGOUT_RQ::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_LOGOUT_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	return (STRU_LOGOUT_RQ::MIN_LEN);
}
BOOL STRU_LOGOUT_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_LOGOUT_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
long STRU_GET_APP_LIST_RQ::MIN_LEN = 26;
STRU_GET_APP_LIST_RQ::STRU_GET_APP_LIST_RQ() : STRU_PROTOCOL_BASE(DEF_GET_GAME_LIST_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_i64AppKey = 0;
}
long STRU_GET_APP_LIST_RQ::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_GET_APP_LIST_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//验证码
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//文件key
	*(INT64*)szBuf = m_i64AppKey;
	szBuf += sizeof(m_i64AppKey);

	return (STRU_GET_APP_LIST_RQ::MIN_LEN);
}
BOOL STRU_GET_APP_LIST_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_GET_APP_LIST_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//验证码
	m_i64VerifyCode = *(INT64*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);

	//文件key
	m_i64AppKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64AppKey);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
long STRU_GET_APP_LIST_RS::MIN_LEN = 12;
STRU_GET_APP_LIST_RS::STRU_GET_APP_LIST_RS() : STRU_PROTOCOL_BASE(DEF_GET_GAME_LIST_RS)
{
	m_i64UserID = 0;
	m_wAppItemCount = 0;
}
long STRU_GET_APP_LIST_RS::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_GET_APP_LIST_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//有效子项的数量
	*(WORD*)szBuf = m_wAppItemCount;
	szBuf += sizeof(m_wAppItemCount);

	//结果
	lBufLen -= STRU_GET_APP_LIST_RS::MIN_LEN;

	//序列化app信息子项
	long lItemLen = 0;
	for (long i = 0; i < m_wAppItemCount; ++i)
	{
		lItemLen += m_oAppList[i].Serialize(szBuf, lBufLen);	
	}

	return (STRU_GET_APP_LIST_RS::MIN_LEN+lItemLen);
}
BOOL STRU_GET_APP_LIST_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_GET_APP_LIST_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//有效子项的数量
	m_wAppItemCount = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppItemCount);

	//结果
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
	//检查长度
	if (STRU_APP_INFO::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//用户id
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//用户验证码
	*(INT64*)szBuf = m_i64AppKey;
	szBuf += sizeof(m_i64AppKey);

	//共享时间
	*(DWORD*)szBuf = m_dwSharedTime;
	szBuf += sizeof(m_dwSharedTime);

	//下载时间
	*(DWORD*)szBuf = m_dwDownTimes;
	szBuf += sizeof(m_dwDownTimes);

	//描述长度
	*(WORD*)szBuf = m_wAppDesLen;
	szBuf += sizeof(m_wAppDesLen);

	//图标名称长度
	*(WORD*)szBuf = m_wAppIconPathLen;
	szBuf += sizeof(m_wAppIconPathLen);

	//APP名称长度
	*(WORD*)szBuf = m_wAppNameLen;
	szBuf += sizeof(m_wAppNameLen);

	//APP名称长度
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
	//检查长度
	if (STRU_APP_INFO::MIN_LEN > lBufLen)
	{
		return FALSE;
	}
	//用户id
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//用户验证码
	m_i64AppKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64AppKey);

	//共享时间
	m_dwSharedTime = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwSharedTime);

	//下载时间
	m_dwDownTimes = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwDownTimes);

	//描述长度
	m_wAppDesLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppDesLen);

	//图标名称长度
	m_wAppIconPathLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppIconPathLen);

	//APP名称长度
	m_wAppNameLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppNameLen);

	//APP名称长度
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



long STRU_PLAY_STRANGER_RQ::MIN_LEN = 22;
STRU_PLAY_STRANGER_RQ::STRU_PLAY_STRANGER_RQ() : STRU_PROTOCOL_BASE(DEF_PLAY_STRANGER_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
}
long STRU_PLAY_STRANGER_RQ::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_PLAY_STRANGER_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//用户ID
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//用户ID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	return (STRU_PLAY_STRANGER_RQ::MIN_LEN);
}
BOOL STRU_PLAY_STRANGER_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_LOGIN_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//用户ID
	m_i64VerifyCode = *(int*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);

	//密码长度
	m_iGameID = *(WORD*)szBuf;
	szBuf += sizeof(m_iGameID);
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
long STRU_PLAY_STRANGER_RS::MIN_LEN = 32;
STRU_PLAY_STRANGER_RS::STRU_PLAY_STRANGER_RS() : STRU_PROTOCOL_BASE(DEF_PLAY_STRANGER_RS)
{
	m_i64UserID = 0;
	m_i64StrangerID = 0;					//匹配成功的对手ID
	m_iGameID = 0;
	m_iIndex = 0;
	m_bIsFirst = FALSE;
	m_wResult = enum_re_fail;	
}
long STRU_PLAY_STRANGER_RS::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_PLAY_STRANGER_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//验证码
	*(INT64*)szBuf = m_i64StrangerID;
	szBuf += sizeof(m_i64StrangerID);

	//用户ID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	//用户ID
	*(int*)szBuf = m_iIndex;
	szBuf += sizeof(m_iIndex);

	//用户ID
	*(BOOL*)szBuf = m_bIsFirst;
	szBuf += sizeof(m_bIsFirst);

	//密码长度
	m_bIsFirst = *(BOOL*)szBuf;
	szBuf += sizeof(m_bIsFirst);

	//结果
	*(WORD*)szBuf = m_wResult;
	szBuf += sizeof(m_wResult);

	//结果
	lBufLen -= STRU_PLAY_STRANGER_RS::MIN_LEN;

	return (STRU_PLAY_STRANGER_RS::MIN_LEN);
}
BOOL STRU_PLAY_STRANGER_RS::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_PLAY_STRANGER_RS::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//验证码
	m_i64StrangerID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64StrangerID);

	//密码长度
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	//密码长度
	m_iIndex = *(int*)szBuf;
	szBuf += sizeof(m_iIndex);

	//密码长度
	m_bIsFirst = *(BOOL*)szBuf;
	szBuf += sizeof(m_bIsFirst);

	//结果
	m_wResult = *(WORD*)szBuf;
	szBuf += sizeof(m_wResult);

	//结果
	lBufLen -= STRU_PLAY_STRANGER_RS::MIN_LEN;

	return TRUE;
}

//下载游戏
long STRU_DOWNLOAD_GAME_RQ::MIN_LEN = 22;
STRU_DOWNLOAD_GAME_RQ::STRU_DOWNLOAD_GAME_RQ() : STRU_PROTOCOL_BASE(DEF_DOWNLOAD_GAME_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
	m_iGameID = 0;
}
long STRU_DOWNLOAD_GAME_RQ::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_DOWNLOAD_GAME_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//验证码
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//游戏ID
	*(int*)szBuf = m_iGameID;
	szBuf += sizeof(m_iGameID);

	return (STRU_DOWNLOAD_GAME_RQ::MIN_LEN);
}
BOOL STRU_DOWNLOAD_GAME_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_DOWNLOAD_GAME_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//验证码
	m_i64VerifyCode = *(INT64*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);
	
	//游戏ID
	m_iGameID = *(int*)szBuf;
	szBuf += sizeof(m_iGameID);

	return TRUE;
}

//上传文件
long STRU_FILE_HEAD_UPLOAD::MIN_LEN = 18;
STRU_FILE_HEAD_UPLOAD::STRU_FILE_HEAD_UPLOAD() : STRU_PROTOCOL_BASE(DEF_UPLOAD_RQ)
{
	m_i64UserID = 0;
}
long STRU_FILE_HEAD_UPLOAD::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_FILE_HEAD_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
	//检查长度
	if (STRU_FILE_HEAD_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
	//检查长度
	if (STRU_FILE_TRANS_COMPLETE_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
	//检查长度
	if (STRU_FILE_TRANS_COMPLETE_UPLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
//下载文件
long STRU_FILE_HEAD_DOWNLOAD::MIN_LEN = 18;
STRU_FILE_HEAD_DOWNLOAD::STRU_FILE_HEAD_DOWNLOAD() : STRU_PROTOCOL_BASE(DEF_DOWNLOAD_RQ)
{
	m_i64UserID = 0;
}
long STRU_FILE_HEAD_DOWNLOAD::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_FILE_HEAD_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
	//检查长度
	if (STRU_FILE_HEAD_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
	//检查长度
	if (STRU_FILE_TRANS_COMPLETE_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
	//检查长度
	if (STRU_FILE_TRANS_COMPLETE_DOWNLOAD::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
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
//tcp发送，给服务器保存用
//DEF_CONNECT_MESSAGE_RQ
long STRU_CONNECT_MESSAGE_RQ::MIN_LEN = 18;
STRU_CONNECT_MESSAGE_RQ::STRU_CONNECT_MESSAGE_RQ() : STRU_PROTOCOL_BASE(DEF_CONNECT_MESSAGE_RQ)
{
	m_i64UserID = 0;
	m_i64VerifyCode = 0;
}
long STRU_CONNECT_MESSAGE_RQ::Serialize(char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_CONNECT_MESSAGE_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}
	//协议类型
	*(WORD*)szBuf = m_wProtocolType;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	*(INT64*)szBuf = m_i64UserID;
	szBuf += sizeof(m_i64UserID);

	//验证码
	*(INT64*)szBuf = m_i64VerifyCode;
	szBuf += sizeof(m_i64VerifyCode);

	//结果
	lBufLen -= STRU_CONNECT_MESSAGE_RQ::MIN_LEN;

	return (STRU_CONNECT_MESSAGE_RQ::MIN_LEN);
}
BOOL STRU_CONNECT_MESSAGE_RQ::UnSerialize(const char szBuf[], long lBufLen)
{
	//检查长度
	if (STRU_CONNECT_MESSAGE_RQ::MIN_LEN > lBufLen)
	{
		return 0;
	}

	//反序列化包类型
	m_wProtocolType = *(WORD*)szBuf;
	szBuf += sizeof(m_wProtocolType);

	//用户ID
	m_i64UserID = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserID);

	//验证码
	m_i64VerifyCode = *(INT64*)szBuf;
	szBuf += sizeof(m_i64VerifyCode);

	//结果
	lBufLen -= STRU_LOGIN_RS::MIN_LEN;

	return TRUE;
}

