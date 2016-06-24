#pragma once

#ifndef __INCLUDE_LOCK_QUEUE_H__
#define __INCLUDE_LOCK_QUEUE_H__
#include "Lock.h"

template<typename T, const long MAX_LEN = 10000>
class CLockQueue
{
public:
	CLockQueue() : m_lQueueLen(0), m_lRPos(0), m_lWPos(0), m_pQueue(NULL), m_bInit(FALSE)
	{
	
	}
	BOOL InitQueue(long lQueueLen)
	{
		UnInitQueue();
		if (lQueueLen > MAX_LEN)
		{
			return FALSE;
		}
		m_pQueue = new T*[lQueueLen + 1];
		m_lQueueLen = lQueueLen + 1;
		//��ʼ��
		for (long i = 0; i < m_lQueueLen; i++)
		{
			m_pQueue[i] = NULL;
		}
		
		m_bInit = TRUE;
		return TRUE;
	}
	void UnInitQueue()
	{
		//��֪ͨʹ�øö��е��߳�ֹͣʹ��
		m_bInit = FALSE;

		//�������ٶ��е�ͬʱ�������߳��Ѿ�������m_bInit���жϣ����ڶ�д���У����ԣ�һ��Ҫ�ȴ���Щ�߳���ɱ��ζ�д����
		::Sleep(5);

		if (m_pQueue)
		{
			//�ͷŶ����е�����
			for (long i = 0; i < m_lQueueLen; i++)
			{
				if (m_pQueue[i])
				{
					delete m_pQueue[i];
					m_pQueue[i] = NULL;
				}
			}
			delete m_pQueue;
			m_pQueue = NULL;
		}
		m_lRPos = m_lWPos;
		m_lQueueLen = 0;
	}
	void Clear()
	{
		if (m_bInit == FALSE)
		{
			return;
		}
		m_oLock.Lock();
		//�ͷŶ����е�����
		for (long i = 0; i < m_lQueueLen; i++)
		{
			if (m_pQueue[i])
			{
				delete m_pQueue[i];
				m_pQueue[i] = NULL;
			}
		}	
		m_oLock.UnLock();
	}
	BOOL PushQueue(T* pNode)
	{
		if (m_bInit == FALSE)
		{
			return FALSE;
		}
		//��ȡ��һ��λ��
		CAutoLock autoLock(m_oLock);
		long lNext = (m_lWPos + 1) % m_lQueueLen;
		if (lNext == m_lRPos)
		{
			return FALSE;
		}
		else
		{
			//�ȷź��ƶ�
			m_pQueue[m_lWPos] = pNode;
			m_lWPos = lNext;
		}
		
		return TRUE;
	}
	BOOL PopQueue(T*& pNode)
	{
		if (m_bInit == FALSE)
		{
			return FALSE;
		}
		CAutoLock autoLock(m_oLock);
		if (m_lWPos == m_lRPos)
		{
			return FALSE;
		}
		else
		{
			pNode = m_pQueue[m_lRPos];
			m_pQueue[m_lRPos] = NULL;
			m_lRPos = (m_lRPos + 1) % m_lQueueLen;
		}
		return TRUE;
	}

private:
	long m_lRPos;
	long m_lWPos;
	long m_lQueueLen;
	T** m_pQueue;
	BOOL m_bInit;
	CLock m_oLock;
};
//CCircleQueue<Object> queue;
//Object* p = new ;
////init
//queue.Push(p);
//m_pQueue[i] = p;

#endif //__INCLUDE_LOCK_QUEUE_H__