
// GameClientDlg.h : ͷ�ļ�
//

#pragma once

#include "Kernel.h"
#include "LoginDlg.h"
#include "afxcmn.h"
// CGameClientDlg �Ի���
class CGameClientDlg : public CDialogEx ,public IUIToKernel
{
// ����
public:
	CGameClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GAMECLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public://Kernel֪ͨUI����
	virtual bool NotifyLogin();
	virtual bool NotifyGetGameList(STRU_GET_APP_LIST_RS oDataAppListRs);
	virtual bool NotifyLoginRs(INT64 m_i64UserID,bool bRes);
	virtual bool NotifyRegisterRs(long lUserID,bool bRes);
	virtual bool NotifyGetGameListRs(bool bRes);

// ʵ��
protected:
	HICON m_hIcon;
	CKernel oKernel;
	// ���ɵ���Ϣӳ�亯��
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
