// SetTimerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Dragon.h"
#include "SetTimerDlg.h"
#include "afxdialogex.h"


// CSetTimerDlg 对话框

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


// CSetTimerDlg 消息处理程序


void CSetTimerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1, m_oTime);
	EndDialog(IDC_BUTTON1);
}
