#pragma once


// CSetTimerDlg �Ի���

class CSetTimerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetTimerDlg)

public:
	CSetTimerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetTimerDlg();

// �Ի�������
	enum { IDD = IDD_SETTIMERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_oTime;
};
