#include <process.h>
#include "ThreadPool.h"

CThreadPool::CThreadPool() : 
m_lMinThreadCount(0), 
m_lMaxThreadCount(0), 
m_lRunThreadCount(0), 
m_lCreateThreadCount(0), 
m_hSemaphore(NULL), 
m_hQuitEvent(NULL)
{
	
}

CThreadPool::~CThreadPool()
{
	DestroyPool();
}

//�����̳߳�
bool CThreadPool::CreatePool(const long lMinThreadCount, const long lMaxThreadCount, const long lMaxTaskCount)
{
	//������
	if (lMinThreadCount > lMaxThreadCount || lMaxTaskCount <= 0)
	{
		return false;
	}

	//��ֹ�ظ���ʼ��
	DestroyPool();

	//�����˳��¼�����
	m_hQuitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hQuitEvent)
	{
		return false;
	}

	//�����ź���
	m_hSemaphore = ::CreateSemaphore(NULL, lMinThreadCount, lMaxThreadCount, NULL);
	if (NULL == m_hSemaphore)
	{
		return false;
	}

	//��ʼ���������
	if (false == m_oTaskQueue.InitQueue(lMaxTaskCount))
	{
		return false;
	}

	//�����߳�
	for (long i = 0; i < lMinThreadCount; ++i)
	{
		::InterlockedIncrement(&m_lCreateThreadCount);
		//�����µ��߳�
		HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadProc, (LPVOID)this, 0, NULL);
		if (NULL == hThread)
		{
			::InterlockedDecrement(&m_lCreateThreadCount);
			return false;
		}
		::CloseHandle(hThread);
	}

	//��������
	m_lMinThreadCount = lMinThreadCount;
	m_lMaxThreadCount = lMaxThreadCount;

	return true;
}

//Ͷ������
bool CThreadPool::PushTask(ITask* pTask)
{
	//������
	if (NULL == pTask)
	{
		return false;
	}

	//Ͷ�ݵ����������
	if (false == m_oTaskQueue.PushQueue(pTask))
	{
		return false;
	}

	//��������߳�����С���Ѿ������̵߳�����
	if (m_lRunThreadCount < m_lCreateThreadCount)
	{
		//�ͷ�һ���ź�������һ���Ѿ��������ǹ�����߳̿�ʼ����
		::ReleaseSemaphore(m_hSemaphore, 1, NULL);
	}
	//��������߳��������ڵ�����С��������ô�̲߳���
	else if (m_lRunThreadCount >= m_lCreateThreadCount)
	{
		//����߳������ﵽ�����ֵ��û�а취��...�����򴴽��µ��߳�
		if (m_lCreateThreadCount < m_lMaxThreadCount)
		{
			//���Ӵ����߳�����
			::InterlockedIncrement(&m_lCreateThreadCount);
			//�����µ��߳�
			HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadProc, (LPVOID)this, 0, NULL);
			if (NULL == hThread)
			{
				::InterlockedDecrement(&m_lCreateThreadCount);
				return false;
			}
			::CloseHandle(hThread);

			//�ͷ�һ���ź��������µ��߳̿�ʼ����
			::ReleaseSemaphore(m_hSemaphore, 1, NULL);
		}
	}

	return true;
}

//�����̳߳�
void CThreadPool::DestroyPool()
{
	//�����¼�Ϊ���źţ�֪ͨ�߳��˳�
	if (m_hQuitEvent)
	{
		::SetEvent(m_hQuitEvent);
	}

	//֪ͨ���еĹ����߳��˳�
	long iCount = m_lCreateThreadCount;
	for (long i = 0; i < iCount; i++)
	{
		::ReleaseSemaphore(m_hSemaphore, 1, NULL);
	}

	//���߳��˳�
	while (m_lCreateThreadCount)
	{
		::Sleep(10);
	}

	//�ر��¼����ź���
	if (m_hQuitEvent)
	{
		::CloseHandle(m_hQuitEvent);
		m_hQuitEvent = NULL;
	}
	if (m_hSemaphore)
	{
		::CloseHandle(m_hSemaphore);
		m_hSemaphore = NULL;
	}

	//�����������
	m_oTaskQueue.UnInitQueue();

	//�������
	m_lMinThreadCount = 0;
	m_lMaxThreadCount = 0;
}

UINT WINAPI CThreadPool::ThreadProc(LPVOID lpParam)
{
	//�����̲߳���
	CThreadPool* pThis = (CThreadPool*)lpParam;

	//������ͨ�̺߳�����pThis�ĳ�Ա������
	pThis->ThreadPool();

	return 1L;
}

//������ͨ�̺߳�����pThis�ĳ�Ա������
void CThreadPool::ThreadPool()
{
	//ѭ����������(����ȴ��˳�֪ͨ��ʱ��˵������Ҫ�˳�����������)
	while (WAIT_TIMEOUT == WaitForSingleObject(m_hQuitEvent, 0))
	{
		//�ȴ��������ź�
		::WaitForSingleObject(m_hSemaphore, INFINITE);

		//�л��߳����Ϊ�����߳�
		::InterlockedIncrement(&m_lRunThreadCount);

		//ѭ����������
		ITask* pTask = NULL;
		while (TRUE == m_oTaskQueue.PopQueue(pTask))
		{
			pTask->RunTask();
			delete pTask;
		}

		//�л����Ϊ�ǹ����߳�
		::InterlockedDecrement(&m_lRunThreadCount);
	}

	//���ٴ����߳�����
	::InterlockedDecrement(&m_lCreateThreadCount);
}