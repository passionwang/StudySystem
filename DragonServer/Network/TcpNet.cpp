//Tcp本地IP   端口12345
#include <process.h>
#include "TcpNet.h"
#pragma warning(disable:4554 4244)

CTcpNet::CTcpNet() : m_sockListen(NULL), m_lThreadCount(0), m_bRun(FALSE)
{
}

CTcpNet::~CTcpNet()
{
}

//////////////////////////////////////////////////////////////////////////////
//实现基类接口提供外部调用
BOOL CTcpNet::InitNet(IKernel* pKernel)
{
	//初始化基本的网络(内部初始化网络)
	InnerInitNet();

	//保存回调对象
	m_pKernel = pKernel;
	m_bRun = TRUE;

	//创建一个接受连接的线程
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, AcceptThread, this, 0, NULL);
	if (hThread == NULL)
	{
		return FALSE;
	}
	::CloseHandle(hThread);
	//创建一个收数据的线程
	hThread = (HANDLE)::_beginthreadex(NULL, 0, RecvThread, this, 0, NULL);
	if (hThread == NULL)
	{
		return FALSE;
	}
	::CloseHandle(hThread);

	return TRUE;
}

void CTcpNet::UnInitNet()
{
	//通知线程退出
	m_bRun = FALSE;

	//等待所有线程退出
	while (m_lThreadCount > 0)
	{
		::Sleep(1);
	}

	//关闭连接卸载库
	if (m_sockListen != NULL)
	{
		::closesocket(m_sockListen);
		m_sockListen = NULL;

		//关闭客户端
		std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			delete (STRU_SESSION*)(it->second);
			++it;
		}
		m_mapSession.clear();

		//关闭哭
		::WSACleanup();
	}
}

long CTcpNet::SendData(STRU_SESSION* pSession, const char* pData, long lDataLen)
{
	return ::send(pSession->m_sock, pData, lDataLen, 0);
}

//////////////////////////////////////////////////////////////////////////////
//处理线程函数（接受连接和收数据）
unsigned int WINAPI CTcpNet::AcceptThread(void* pParam)
{
	//转换参数
	CTcpNet* pThis = (CTcpNet*)pParam;

	//调用普通线程函数
	pThis->AcceptThread();
	
	return 1L;
}

void CTcpNet::AcceptThread()
{
	//增加线程数量
	::InterlockedIncrement(&(m_lThreadCount));

	//设置监听套接字为非阻塞
	u_long flag = 100;
	::ioctlsocket(m_sockListen, FIONBIO, &flag);

	//循环接受连接
	SOCKET client;
	int iErrorCode = 0;
	while (m_bRun)
	{
		client = ::accept(m_sockListen, NULL, NULL);
		if (SOCKET_ERROR == client)
		{
			//获取错误码
			iErrorCode = ::WSAGetLastError();
			if (iErrorCode == WSAEWOULDBLOCK)
			{
				::Sleep(1);
				continue;
			}
			else
			{
				break;
			}
		}

		//成功接受一个连接
		//(1)创建客户端连接信息结构
		STRU_SESSION* pSession = new STRU_TCP_SESSION;

		//(2)填充结构
		pSession->m_sock = client;

		//(3)保存连接信息
		m_oLock.Lock();
		(m_mapSession)[client] = pSession;
		m_oLock.UnLock();
	}

	//减少线程数量
	::InterlockedDecrement(&(m_lThreadCount));
}

unsigned int WINAPI CTcpNet::RecvThread(void* pParam)
{
	//转换参数
	CTcpNet* pThis = (CTcpNet*)pParam;

	//调用普通线程函数
	pThis->RecvThread();

	return 1L;
}

void CTcpNet::RecvThread()
{
	//增加线程数量
	::InterlockedIncrement(&(m_lThreadCount));

	//循环收数据
	char *szRecvBuf = new char[1024*1024];
	int iRet = -1;
	timeval val;
	val.tv_sec = 0;
	val.tv_usec = 10;

	while (m_bRun)
	{
		fd_set readSet;

		//清空
		FD_ZERO(&readSet);
		//填充客户端集合
		std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			FD_SET((SOCKET)(it->first), &readSet);
			++it;
		}

		//调用select
		iRet = ::select(0, &readSet, NULL, NULL, &val);
		if (iRet == 0)
		{
			continue;
		}
		else if (iRet == SOCKET_ERROR)
		{
			continue;
		}

		//接收数据(检查原始的客户端集合谁还在返回的readSet中，谁就需要收数据)
		it = m_mapSession.begin();
		while (it != m_mapSession.end())
		{
			if (FD_ISSET(it->first, &readSet) > 0)
			{
				//recv
				iRet = ::recv(it->first, szRecvBuf, 1024*1024, 0);
				if (iRet == 0 || iRet == SOCKET_ERROR)
				{
					//清楚无效的连接
					RemoveSession(it->first);
				}
				else
				{
					//通知上层的调用这收到数据
					if (m_pKernel)
					{
						m_pKernel->OnRecvData(it->second, szRecvBuf, iRet, enum_recv_tcp);
					}
				}
			}
			it++;
		}
	}

	//减少线程数量
	::InterlockedDecrement(&(m_lThreadCount));
}

//初始化基本的网络(内部初始化网络)
BOOL CTcpNet::InnerInitNet()
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
	m_sockListen = ::socket(AF_INET, SOCK_STREAM, 0);

	//初始化地址
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = LOCAL_TCP_PORT;
	addr.sin_addr.S_un.S_addr = LOCAL_IP_TCP;

	//绑定
	::bind(m_sockListen, (sockaddr*)&addr, sizeof(addr));

	//监听
	::listen(m_sockListen, 5);

	return TRUE;
}

//获取本地主机有效地址
long CTcpNet::GetHostIP()
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

//删除指定的连接信息
BOOL CTcpNet::RemoveSession(SOCKET sock)
{
	CAutoLock autoLock(m_oLock);
	//m_oLock.Lock();
	std::map<SOCKET, STRU_SESSION*>::iterator it = m_mapSession.find(sock);
	if (it == m_mapSession.end())
	{
		//m_oLock.UnLock();
		return FALSE;
	}
	else
	{
		::closesocket(it->first);
		delete (STRU_SESSION*)(it->second);
		m_mapSession.erase(it);
	}
	//m_oLock.UnLock();
	return TRUE;
}