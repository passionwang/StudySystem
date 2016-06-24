#include "stdafx.h"
#include <stdlib.h>
#include "TcpNet.h"



#define ServerPorts  12345

CTcpNet::CTcpNet()
{
	m_bRun = FALSE;
	m_bConnect = FALSE;
}
CTcpNet::~CTcpNet()
{

}
BOOL CTcpNet::UnInitNet()
{
	return TRUE;
}

long CTcpNet::SendData(const char* pData, long lDataLen)
{
	return ::send(m_socket_client,pData,lDataLen,0);
}
////DWORD WINAPI CTcpNet::ThreadConnectProc(LPVOID lpParameter)
//{
//	CTcpNet* pThis = (CTcpNet*)lpParameter;
//	pThis->ThreadConnectProc();
//	return 1L;
//}
void CTcpNet::ThreadRecvProc()
{
	m_bRun = TRUE;
	char *szRecvBuf = new char[2*1024*1024];
	while(TRUE == m_bRun)
	{
		long lRecvLen;
		lRecvLen = ::recv(m_socket_client, szRecvBuf, 2*1024*1024, 0);
		m_pKernel->OnRecvData(szRecvBuf, lRecvLen,enum_recv_tcp);
	}
	delete[] szRecvBuf;
}
DWORD WINAPI CTcpNet::ThreadRecvProc(LPVOID lpParameter)
{
	CTcpNet* pThis = (CTcpNet*)lpParameter;
	pThis->ThreadRecvProc();
	return 1L;
}
//void CTcpNet::ThreadConnectProc()
//{
//	sockaddr_in  sock_struct;
//	sock_struct.sin_family  = AF_INET;
//	sock_struct.sin_addr.S_un .S_addr  = ServerIP;
//	sock_struct.sin_port = htons(ServerPorts);
//
//	if(SOCKET_ERROR == connect(m_socket_client,(const sockaddr*)&sock_struct,sizeof(sock_struct)))
//	{
//		if(m_socket_client)
//		{
//			closesocket(m_socket_client);
//			m_socket_client = NULL;
//		}
//		WSACleanup(); 
//	}
//	//	m_bConnect = TRUE;
//}
BOOL CTcpNet::InitNet(INetToKernel* pKernel)
{
	m_pKernel = pKernel;
	//1.���꣬���ؿ�
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	//�ߵ�λת����2.2
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) 
	{    
        printf("WSAStartup failed with error: %d\n", err);
        return FALSE;
    }
	//�˶����뵽�Ŀ�
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) 
	{
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return FALSE;
    }
    else
        printf("The Winsock 2.2 dll was found okay\n");
	//2.���꣬socket
	m_socket_client = socket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP );
	if(INVALID_SOCKET == m_socket_client)
	{
		WSACleanup(); 
		return FALSE;
	} 
	//���ü����׽���Ϊ������
	/*int nValue;
	u_long flag = 100;
	nValue = ::ioctlsocket(m_socket_client, FIONBIO, &flag);*/
	//3.��Ӷ�곤��bind
	sockaddr_in  sock_struct;
	sock_struct.sin_family  = AF_INET;
	sock_struct.sin_addr.S_un .S_addr  = Server_IP_TCP;
	sock_struct.sin_port = Server_TCP_PORT;
	
	//::bind(m_socket_client,(sockaddr*)&sock_struct,sizeof(sock_struct));
	

	/*sock_struct;
	sock_struct.sin_family  = AF_INET;
	sock_struct.sin_addr.S_un .S_addr  = GetHostIP();
	sock_struct.sin_port = htons(12345);*/

	if(SOCKET_ERROR == connect(m_socket_client,(const sockaddr*)&sock_struct,sizeof(sock_struct)))
	{
		if(m_socket_client)
		{
			closesocket(m_socket_client);
			m_socket_client = NULL;
		}
		WSACleanup(); 
		return FALSE;
	}
	//����һ�����ӷ������߳�
	/*HANDLE m_hThreadConnect = CreateThread(NULL, 0,&CTcpNet::ThreadConnectProc, (LPVOID*)this, 0, NULL);
	if(NULL != m_hThreadConnect)
	{
		CloseHandle(m_hThreadConnect);
		m_hThreadConnect = NULL;
	}*/

	//����һ����������
	HANDLE m_hThreadRecv = CreateThread(NULL, 0,&CTcpNet::ThreadRecvProc, (LPVOID*)this, 0, NULL);
	if(NULL != m_hThreadRecv)
	{
		CloseHandle(m_hThreadRecv);
		m_hThreadRecv = NULL;
	}
	return TRUE;
}
void CTcpNet::CloseTcpNet()
{
	m_bRun = FALSE;
	::Sleep(10);
	//�ر�Socket
	closesocket(m_socket_client);
	//�ص꣬ж�ؿ⣬
	WSACleanup();
}
//��ȡ����������Ч��ַ
long CTcpNet::GetHostIP()
{
	long lValidIP = inet_addr("127.1.1.1");

	//��ȡ��������
	char szHostName[100] = "";
	::gethostname(szHostName, sizeof(szHostName) - 1);

	//��ȡ��ַ�б�
	struct hostent* pHostAddr = ::gethostbyname(szHostName);
	
	//������ЧIP
	if (pHostAddr != NULL)
	{
		if (pHostAddr->h_addr_list[0] != NULL && pHostAddr->h_length == 4)
		{
			lValidIP = *(long*)pHostAddr->h_addr_list[0];
		}
	}

	return lValidIP;
}