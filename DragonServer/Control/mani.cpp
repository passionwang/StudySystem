#include "Kernel.h"
#include <iostream>

using namespace std;

int main()
{
	//��ȡ����
	CControl<CKernel>* pControl = CControl<CKernel>::GetInstance();

	//��ʼ������
	pControl->OpenControl();

	//�ȴ�����
	while (pControl->m_bQuit == FALSE)
	{
		::Sleep(10);
	}
	pControl->CloseControl();
	CControl<CKernel>::DestroyInstance();
	return 0;
}