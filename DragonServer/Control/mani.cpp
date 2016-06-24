#include "Kernel.h"
#include <iostream>

using namespace std;

int main()
{
	//获取单利
	CControl<CKernel>* pControl = CControl<CKernel>::GetInstance();

	//初始化单利
	pControl->OpenControl();

	//等待销毁
	while (pControl->m_bQuit == FALSE)
	{
		::Sleep(10);
	}
	pControl->CloseControl();
	CControl<CKernel>::DestroyInstance();
	return 0;
}