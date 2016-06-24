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

//创建线程池
bool CThreadPool::CreatePool(const long lMinThreadCount, const long lMaxThreadCount, const long lMaxTaskCount)
{
	//检查参数
	if (lMinThreadCount > lMaxThreadCount || lMaxTaskCount <= 0)
	{
		return false;
	}

	//防止重复初始化
	DestroyPool();

	//创建退出事件对象
	m_hQuitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hQuitEvent)
	{
		return false;
	}

	//创建信号量
	m_hSemaphore = ::CreateSemaphore(NULL, lMinThreadCount, lMaxThreadCount, NULL);
	if (NULL == m_hSemaphore)
	{
		return false;
	}

	//初始化任务队列
	if (false == m_oTaskQueue.InitQueue(lMaxTaskCount))
	{
		return false;
	}

	//创建线程
	for (long i = 0; i < lMinThreadCount; ++i)
	{
		::InterlockedIncrement(&m_lCreateThreadCount);
		//创建新的线程
		HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadProc, (LPVOID)this, 0, NULL);
		if (NULL == hThread)
		{
			::InterlockedDecrement(&m_lCreateThreadCount);
			return false;
		}
		::CloseHandle(hThread);
	}

	//保存数据
	m_lMinThreadCount = lMinThreadCount;
	m_lMaxThreadCount = lMaxThreadCount;

	return true;
}

//投递任务
bool CThreadPool::PushTask(ITask* pTask)
{
	//检查参数
	if (NULL == pTask)
	{
		return false;
	}

	//投递到任务队列中
	if (false == m_oTaskQueue.PushQueue(pTask))
	{
		return false;
	}

	//如果运行线程数量小于已经创建线程的数量
	if (m_lRunThreadCount < m_lCreateThreadCount)
	{
		//释放一个信号量，让一个已经创建但是挂起的线程开始工作
		::ReleaseSemaphore(m_hSemaphore, 1, NULL);
	}
	//如果运行线程数量大于等于最小数量，那么线程不足
	else if (m_lRunThreadCount >= m_lCreateThreadCount)
	{
		//如果线程数量达到了最大值，没有办法了...，否则创建新的线程
		if (m_lCreateThreadCount < m_lMaxThreadCount)
		{
			//增加创建线程数量
			::InterlockedIncrement(&m_lCreateThreadCount);
			//创建新的线程
			HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadProc, (LPVOID)this, 0, NULL);
			if (NULL == hThread)
			{
				::InterlockedDecrement(&m_lCreateThreadCount);
				return false;
			}
			::CloseHandle(hThread);

			//释放一个信号量，让新的线程开始工作
			::ReleaseSemaphore(m_hSemaphore, 1, NULL);
		}
	}

	return true;
}

//销毁线程池
void CThreadPool::DestroyPool()
{
	//设置事件为有信号，通知线程退出
	if (m_hQuitEvent)
	{
		::SetEvent(m_hQuitEvent);
	}

	//通知所有的挂起线程退出
	long iCount = m_lCreateThreadCount;
	for (long i = 0; i < iCount; i++)
	{
		::ReleaseSemaphore(m_hSemaphore, 1, NULL);
	}

	//等线程退出
	while (m_lCreateThreadCount)
	{
		::Sleep(10);
	}

	//关闭事件和信号量
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

	//销毁任务队列
	m_oTaskQueue.UnInitQueue();

	//清空数据
	m_lMinThreadCount = 0;
	m_lMaxThreadCount = 0;
}

UINT WINAPI CThreadPool::ThreadProc(LPVOID lpParam)
{
	//解析线程参数
	CThreadPool* pThis = (CThreadPool*)lpParam;

	//调用普通线程函数（pThis的成员函数）
	pThis->ThreadPool();

	return 1L;
}

//调用普通线程函数（pThis的成员函数）
void CThreadPool::ThreadPool()
{
	//循环处理任务(如果等待退出通知超时，说明不需要退出，继续工作)
	while (WAIT_TIMEOUT == WaitForSingleObject(m_hQuitEvent, 0))
	{
		//等待工作的信号
		::WaitForSingleObject(m_hSemaphore, INFINITE);

		//切换线程身份为工作线程
		::InterlockedIncrement(&m_lRunThreadCount);

		//循环处理任务
		ITask* pTask = NULL;
		while (TRUE == m_oTaskQueue.PopQueue(pTask))
		{
			pTask->RunTask();
			delete pTask;
		}

		//切换身份为非工作线程
		::InterlockedDecrement(&m_lRunThreadCount);
	}

	//减少创建线程数量
	::InterlockedDecrement(&m_lCreateThreadCount);
}