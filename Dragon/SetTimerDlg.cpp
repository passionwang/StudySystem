// SetTimerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dragon.h"
#include "SetTimerDlg.h"
#include "afxdialogex.h"


// CSetTimerDlg �Ի���

IMPLEMENT_DYNAMIC(CSetTimerDlg, CDialog)

CSetTimerDlg::CSetTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTimerDlg::IDD, pParent)
{

}

CSetTimerDlg::~CSetTimerDlg()
{
}

void CSetTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetTimerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetTimerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSetTimerDlg ��Ϣ�������


void CSetTimerDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT1, m_oTime);
	EndDialog(IDC_BUTTON1);
}
