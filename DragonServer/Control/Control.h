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

	//��װ����̨ʱ�䴦����
	static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);

	//�ṩ��main���õĽӿ�
	BOOL OpenControl();
	void CloseControl();

	//�ṩ������ȥʵ����Դ��ʼ������Դ���ٵķ���
	BOOL Init()
	{
		return TRUE;
	}
	void UnInit()
	{
	
	}

public:
	//����̨�¼�֪ͨ���߳��˳��ı��
	static BOOL m_bQuit;

private:
	static CControl* m_pSingleton;
};

//��̬��Ա��ʼ��
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
//��װ����̨ʱ�䴦����
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

//�ṩ��main���õĽӿ�
template<typename T>
BOOL CControl<T>::OpenControl()
{
	//���ÿ���̨�¼�
	::SetConsoleCtrlHandler(CControl<T>::HandlerRoutine, TRUE);

	//��ʼ�����������
	((T*)CControl<T>::m_pSingleton)->Init();

	return TRUE;
}

template<typename T>
void CControl<T>::CloseControl()
{
	//����ʼ�����������
	((T*)CControl<T>::m_pSingleton)->UnInit();
}

#endif //__INCLUDE_CONTROL_H__