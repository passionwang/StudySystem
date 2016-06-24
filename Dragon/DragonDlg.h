
// DragonDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MakeMove.h"
#include "TcpNet.h"
#include "SetTimerDlg.h"
#include "Kernel.h"
// CDragonDlg �Ի���
class CDragonDlg : public CDialogEx , public IKernelToUI
{
// ����
public:
	CDragonDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DRAGON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	virtual void NoticeUIBegin();
	virtual int NoticeUISetTimer();
	virtual void NoticeUIiMove(int nWays,int nWhich);
	virtual void NoticeUIEnd();
	virtual void NoticeUIFailedNet();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL IsGameOver(int nType);
	void iMove(int nWays,int nWhich);
	void Move(int iX,int iY); 
	void TranslateToChessBoard();
	void TranslateToTranslateBoard();
private:
	CTcpNet m_oTcpNet;
	CKernel m_oKernel;
	BOOL m_bGameOver;
	POINT m_ptMoveChess;
	CBitmap m_BoardBmp;
	CImageList m_Chessman;
	int m_nBoardWidth; 
	int m_nBoardHeight;
	
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	BOOL m_bIsWriteGo;
	BOOL m_bIsBegin;
	CEdit m_Output;
	CMoveGenerator m_oMG;
	int m_ChessBoard[10][10];
	int m_TranslateBoard[10][10];
	int m_iM;
	int m_iN;
	int m_m,m_n;
	CHESSPOS m_MoveTPos[100];
	int m_nSurePos[20];
	int m_nPosMove;
	CMakeMove m_oMM;
	
	CEdit m_oThinkTime;
	int m_iShowTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
