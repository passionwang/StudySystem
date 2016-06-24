#pragma once

#ifndef __INCLUDE_THREAD_POOL_H__
#define __INCLUDE_THREAD_POOL_H__

#include "LockQueue.h"

//�����������ӿ�
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
	//���ⷽ��
	//�����̳߳�
	bool CreatePool(const long lMinThreadCount, const long lMaxThreadCount, const long lMaxTaskCount);

	//Ͷ������
	bool PushTask(ITask* pTask);

	//�����̳߳�
	void DestroyPool();

public:
	static UINT WINAPI ThreadProc(LPVOID lpParam);
	//������ͨ�̺߳�����pThis�ĳ�Ա������
	void ThreadPool();

private:
	//�������
	CLockQueue<ITask> m_oTaskQueue;
	//��С�߳�����
	long m_lMinThreadCount;
	//����߳�����
	long m_lMaxThreadCount;
	//�����̵߳�����
	long m_lRunThreadCount;
	//�Ѿ��������߳�����
	long m_lCreateThreadCount;
	//����֪ͨ�߳����е��ź���
	HANDLE m_hSemaphore;
	//�˳�֪ͨ�¼�
	HANDLE m_hQuitEvent;
};

#endif //__INCLUDE_THREAD_POOL_H__