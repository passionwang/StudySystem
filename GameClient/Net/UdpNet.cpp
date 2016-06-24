#include "stdafx.h"
#include <stdlib.h>
#include "UdpNet.h"


CUdpNet::CUdpNet()
{
	m_bRun = FALSE;
}
CUdpNet::~CUdpNet()
{
	CloseUdpNet();
}
BOOL CUdpNet::InitNet(INetToKernel* pKernel)
{
	//����ص�ָ��
	m_pKernel = pKernel;
	//�򿪿�
	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	::WSAStartup(wVersion, &wsaData);

	//У���
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		return FALSE;
	}

	//��������SOCKET
	m_socket_client = ::socket(AF_INET, SOCK_DGRAM, 0);
	
	//��ʼ���ͻ����Լ���������ݵĵ�ַ

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = Server_UDP_PORT;
	addr.sin_addr.S_un.S_addr = Server_IP_UDP;

	//��
	//::bind(m_socket_client, (sockaddr*)&addr, sizeof(addr));



	//����һ���������ӵ��߳�
	HANDLE m_hThreadRecv = CreateThread(NULL, 0,&CUdpNet::ThreadRecvProc, (LPVOID*)this, 0, NULL);
	if(NULL != m_hThreadRecv)
	{
		CloseHandle(m_hThreadRecv);
		m_hThreadRecv = NULL;
	}
	
	return TRUE;
}
long CUdpNet::SendData(const char* pData, long lDataLen)
{
	long lLength;
	//����
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = Server_UDP_PORT;
	addr.sin_addr.S_un.S_addr = Server_IP_UDP;
	lLength = sendto(m_socket_client, pData, lDataLen, 0, (sockaddr*)&addr, sizeof(addr));
	return lLength;
}

void CUdpNet::ThreadRecvProc()
{
	m_bRun = TRUE;
	while(TRUE == m_bRun)
	{
		char szRecvBuf[1024];
		long lRecvLen;
		lRecvLen = ::recvfrom(m_socket_client, szRecvBuf, 1024, 0,NULL,NULL);
		m_pKernel->OnRecvData(szRecvBuf,lRecvLen,enum_recv_udp);
	}
}
DWORD WINAPI CUdpNet::ThreadRecvProc(LPVOID lpParameter)
{
	CUdpNet* pThis = (CUdpNet*)lpParameter;
	pThis->ThreadRecvProc();
	return 1L;
}

void CUdpNet::CloseUdpNet()
{
	m_bRun = FALSE;
	::Sleep(10);
	//�ر�Socket
	closesocket(m_socket_client);
	//�ص꣬ж�ؿ⣬
	WSACleanup();
}

BOOL CUdpNet::UnInitNet()
{

	return TRUE;
}
//��ȡ����������Ч��ַ
long CUdpNet::GetHostIP()
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