#pragma once
#ifndef _INLUDE_OBSERVER_H_
#define _INLUDE_OBSERVER_H_
#include <list>

class IObserver
{
public://�۲��߷��ֱ仯��֪ͨ����
     virtual void UnInit() = 0;
};

class CObserverManager
{
public:
	CObserverManager(){}
public://���ӱ�֪ͨ��
     void  Attach(IObserver *pObserver)
	 {
		 m_ObserverList.push_back(pObserver);
	 }//�Ƴ���֪ͨ��
     void Detach(IObserver *pObserver)
	 {
		 m_ObserverList.remove(pObserver);
	 }
      void Notify()//����֪ͨ
	 {
		 std::list<IObserver *>::iterator it = m_ObserverList.begin();
		 while (it != m_ObserverList.end())
		 {
			 (*it)->UnInit();
			  ++it;
		 }
		 m_ObserverList.clear();
	 }
public:
	 std::list<IObserver *> m_ObserverList;
};

#endif