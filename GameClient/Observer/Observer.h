#pragma once
#ifndef _INLUDE_OBSERVER_H_
#define _INLUDE_OBSERVER_H_
#include <list>

class IObserver
{
public://观察者发现变化，通知销毁
     virtual void UnInit() = 0;
};

class CObserverManager
{
public:
	CObserverManager(){}
public://增加被通知者
     void  Attach(IObserver *pObserver)
	 {
		 m_ObserverList.push_back(pObserver);
	 }//移除被通知者
     void Detach(IObserver *pObserver)
	 {
		 m_ObserverList.remove(pObserver);
	 }
      void Notify()//挨个通知
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