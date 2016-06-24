#pragma once


// CSetTimerDlg 对话框

class CSetTimerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetTimerDlg)

public:
	CSetTimerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetTimerDlg();

// 对话框数据
	enum { IDD = IDD_SETTIMERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_oTime;
};
