#pragma once

#ifndef __INCLUDE_THREAD_POOL_H__
#define __INCLUDE_THREAD_POOL_H__

#include "LockQueue.h"

//定义基类任务接口
class ITask
{
public:
	virtual ~ITask(){}
	virtual bool RunTask() = 0;
};

class CThreadPool
{
public:
	CThreadPool();
	~CThreadPool();

public:
	//对外方法
	//创建线程池
	bool CreatePool(const long lMinThreadCount, const long lMaxThreadCount, const long lMaxTaskCount);

	//投递任务
	bool PushTask(ITask* pTask);

	//销毁线程池
	void DestroyPool();

public:
	static UINT WINAPI ThreadProc(LPVOID lpParam);
	//调用普通线程函数（pThis的成员函数）
	void ThreadPool();

private:
	//任务队列
	CLockQueue<ITask> m_oTaskQueue;
	//最小线程数量
	long m_lMinThreadCount;
	//最大线程数量
	long m_lMaxThreadCount;
	//运行线程的数量
	long m_lRunThreadCount;
	//已经创建的线程数量
	long m_lCreateThreadCount;
	//用来通知线程运行的信号量
	HANDLE m_hSemaphore;
	//退出通知事件
	HANDLE m_hQuitEvent;
};

#endif //__INCLUDE_THREAD_POOL_H__