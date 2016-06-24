
// GameClientDlg.h : 头文件
//

#pragma once

#include "Kernel.h"
#include "LoginDlg.h"
#include "afxcmn.h"
// CGameClientDlg 对话框
class CGameClientDlg : public CDialogEx ,public IUIToKernel
{
// 构造
public:
	CGameClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GAMECLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public://Kernel通知UI处理
	virtual bool NotifyLogin();
	virtual bool NotifyGetGameList(STRU_GET_APP_LIST_RS oDataAppListRs);
	virtual bool NotifyLoginRs(INT64 m_i64UserID,bool bRes);
	virtual bool NotifyRegisterRs(long lUserID,bool bRes);
	virtual bool NotifyGetGameListRs(bool bRes);

// 实现
protected:
	HICON m_hIcon;
	CKernel oKernel;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_GameList;
	afx_msg void OnBnClickedJoingame();
public:
	
	afx_msg void OnBnClickedDowngame();
};
