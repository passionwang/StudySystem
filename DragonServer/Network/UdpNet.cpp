//UDP本地IP 端口8888
#include <process.h>
#include "UdpNet.h"

#pragma warning(disable:4554 4244)
CUdpNet::CUdpNet() : m_sock(NULL), m_lThreadCount(0), m_bRun(FALSE), m_pKernel(NULL)
{

}
CUdpNet::~CUdpNet()
{

}

/////////////////////////////////////////////////////////////////////////////////
BOOL CUdpNet::InitNet(IKernel* pKernel)
{
	UnInitNet();
	InnerInitNet();
	m_pKernel = pKernel;
	m_bRun = TRUE;
	//创建一个收数据的线程
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, RecvThread, this, 0, NULL);
	if (hThread == NULL)
	{
		return FALSE;
	}
	::CloseHandle(hThread);
	return TRUE;
}

void CUdpNet::UnInitNet()
{
	//通知线程退出
	m_bRun = FALSE;

	//等待所有线程退出
	while (m_lThreadCount > 0)
	{
		::Sleep(1);
	}

	//关闭连接卸载库
	if (m_sock != NULL)
	{
		::closesocket(m_sock);
		m_sock = NULL;

		//销毁连接信息
		std::map<INT64, STRU_SESSION*>::iterator it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			delete (STRU_SESSION*)it->second;
			++it;
		}
		m_mapSession.clear();

		//关闭哭
		::WSACleanup();
	}
}

//单播和广播
long CUdpNet::SendData(STRU_SESSION* pSession, const char* pData, long lDataLen)
{
	//初始化地址
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = (WORD)(pSession->m_sock);
	addr.sin_addr.S_un.S_addr = (DWORD)(pSession->m_sock >> 32);

	return ::sendto(m_sock, pData, lDataLen, 0, (sockaddr*)&addr, sizeof(addr));
}

/////////////////////////////////////////////////////////////////////////////////
//初始化基本的网络(内部初始化网络)
BOOL CUdpNet::InnerInitNet()
{
	//打开库
	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	::WSAStartup(wVersion, &wsaData);

	//校验库
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		return FALSE;
	}

	//创建监听SOCKET
	m_sock = ::socket(AF_INET, SOCK_DGRAM, 0);

	//初始化地址
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = LOCAL_UDP_PORT;
	addr.sin_addr.S_un.S_addr = LOCAL_IP_UDP;

	//绑定
	::bind(m_sock, (sockaddr*)&addr, sizeof(addr));

	//设置SOCKET支持广播
	BOOL flag = TRUE;
	::setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char*)&flag, sizeof(flag));

	return TRUE;
}

//获取本地主机有效地址
long CUdpNet::GetHostIP()
{
		long lValidIP = inet_addr("127.1.1.1");

	//获取主机名称
	char szHostName[100] = "";
	::gethostname(szHostName, sizeof(szHostName) - 1);

	//获取地址列表
	struct hostent* pHostAddr = ::gethostbyname(szHostName);
	
	//返回有效IP
	if (pHostAddr != NULL)
	{
		if (pHostAddr->h_addr_list[0] != NULL && pHostAddr->h_length == 4)
		{
			lValidIP = *(long*)pHostAddr->h_addr_list[0];
		}
	}

	return lValidIP;
}

unsigned int WINAPI CUdpNet::RecvThread(void* pParam)
{
	//转换参数
	CUdpNet* pThis = (CUdpNet*)pParam;

	//调用普通线程函数
	pThis->RecvThread();

	return 1L;
}

void CUdpNet::RecvThread()
{
	//增加线程数量
	::InterlockedIncrement(&(m_lThreadCount));

	//循环收数据
	char szRecvBuf[DEF_MAX_CACHE];
	int iRet = -1;
	timeval val;
	val.tv_sec = 0;
	val.tv_usec = 10;

	while (m_bRun)
	{
		fd_set readSet;

		//清空
		FD_ZERO(&readSet);

		//填充待检测的SOCKET
		FD_SET(m_sock, &readSet);

		//调用select
		iRet = ::select(0, &readSet, NULL, NULL, &val);
		if (iRet == 0)
		{
			continue;
		}
		else if (iRet == SOCKET_ERROR)
		{
			break;
		}

		//接收数据(检查原始的客户端集合谁还在返回的readSet中，谁就需要收数据)
		SOCKADDR_IN clientAddr;
		int lClientAddrLen = sizeof(clientAddr);

		if (FD_ISSET(m_sock, &readSet) > 0)
		{
			iRet = ::recvfrom(m_sock, szRecvBuf, sizeof(szRecvBuf), 0, (sockaddr*)&clientAddr,
				&lClientAddrLen);
			if (iRet > 0)
			{
				//合成对端信息
				INT64 i64Key = (((INT64)clientAddr.sin_addr.S_un.S_addr) << 32) + clientAddr.sin_port;
				//查找连接信息
				std::map<INT64, STRU_SESSION*>::iterator it = m_mapSession.find(i64Key);
				STRU_SESSION* pSession = NULL;
				if (it != m_mapSession.end())
				{
					pSession = it->second;
				}
				else
				{
					pSession = new STRU_SESSION;
					pSession->m_sock = i64Key;
					m_mapSession[i64Key] = pSession;
				}

				//通知上层的调用这收到数据
				if (m_pKernel)
				{
					m_pKernel->OnRecvData(pSession, szRecvBuf, iRet, enum_recv_udp);
				}
			}
		}
	}

	//减少线程数量
	::InterlockedDecrement(&(m_lThreadCount));
}