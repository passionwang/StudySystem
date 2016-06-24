#pragma once

#ifndef __INCLUDE_CONTROL_H__
#define __INCLUDE_CONTROL_H__
#include <Windows.h>

template<typename T>
class CControl
{
protected:
	CControl();
	~CControl();
	CControl(CControl& control);
	CControl& operator = (CControl& control);
	
public:
	static CControl* GetInstance();
	static void DestroyInstance();

	//封装控制台时间处理函数
	static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);

	//提供给main调用的接口
	BOOL OpenControl();
	void CloseControl();

	//提供派生类去实现资源初始化和资源销毁的方法
	BOOL Init()
	{
		return TRUE;
	}
	void UnInit()
	{
	
	}

public:
	//控制台事件通知主线程退出的标记
	static BOOL m_bQuit;

private:
	static CControl* m_pSingleton;
};

//静态成员初始化
template<typename T>
CControl<T>* CControl<T>::m_pSingleton = NULL;

template<typename T>
BOOL CControl<T>::m_bQuit = FALSE;

template<typename T>
CControl<T>::CControl()
{

}

template<typename T>
CControl<T>::~CControl()
{

}

template<typename T>
CControl<T>::CControl(CControl<T>& control)
{

}

template<typename T>
CControl<T>& CControl<T>::operator = (CControl<T>& control)
{
	return *CControl<T>::m_pSingleton;
}

//////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
CControl<T>* CControl<T>::GetInstance()
{
	if (CControl<T>::m_pSingleton == NULL)
	{
		CControl<T>::m_pSingleton = new T;
	}

	return CControl<T>::m_pSingleton;
}

template<typename T>
void CControl<T>::DestroyInstance()
{
	if (CControl<T>::m_pSingleton != NULL)
	{
		delete (T*)CControl<T>::m_pSingleton;
		CControl<T>::m_pSingleton = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
//封装控制台时间处理函数
template<typename T>
BOOL WINAPI CControl<T>::HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		{
			CControl<T>::m_bQuit = TRUE;
			while(1)
			{
				::Sleep(10);
			}
		}
		break;
	}
	return TRUE;
}

//提供给main调用的接口
template<typename T>
BOOL CControl<T>::OpenControl()
{
	//设置控制台事件
	::SetConsoleCtrlHandler(CControl<T>::HandlerRoutine, TRUE);

	//初始化派生类对象
	((T*)CControl<T>::m_pSingleton)->Init();

	return TRUE;
}

template<typename T>
void CControl<T>::CloseControl()
{
	//反初始化派生类对象
	((T*)CControl<T>::m_pSingleton)->UnInit();
}

#endif //__INCLUDE_CONTROL_H__