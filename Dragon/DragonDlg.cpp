
// DragonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int iBL=50;//棋盘左侧边距
const int iBT=50;//棋盘上侧边距
const int iBS=50;//棋盘棋位单位

const int InitChessBoard[10][10] =
{
	{ 0,-1, 0,-1, 0,-1, 0,-1, 0,-1},
	{-1, 0,-1, 0,-1, 0,-1, 0,-1, 0},
	{ 0,-1, 0,-1, 0,-1, 0,-1, 0,-1},
	{-1, 0,-1, 0,-1, 0,-1, 0,-1, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}
};
BOOL CDragonDlg::IsGameOver(int nType)
{
	int mBoad[10][10];
	memcpy(mBoad, m_ChessBoard, sizeof(m_ChessBoard));
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			switch(mBoad[i][j])
			{
			case 4:
				mBoad[i][j] = -1;
				break;
			case 6:
				mBoad[i][j] = 1;
				break;
			case -4:
				mBoad[i][j] = -2;
				break;
			case -6:
				mBoad[i][j] = 2;
				break;

			}
		}
	}
	
	if(0 == m_oMG.CreatePossibleMove(mBoad,1,1) && -1 == nType) return TRUE;
	if (0 == m_oMG.CreatePossibleMove(mBoad,1,-1) && 1 == nType) return TRUE;
	return FALSE;
//	m_oTcpNet.CloseTcpNet();
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDragonDlg 对话框



CDragonDlg::CDragonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDragonDlg::IDD, pParent)
{
	m_bIsWriteGo = FALSE;
	m_bIsBegin = FALSE;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			m_ChessBoard[i][j] = InitChessBoard[i][j];
			m_TranslateBoard[i][j] = InitChessBoard[i][j];
		}
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDragonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_Output);
	DDX_Control(pDX, IDC_EDIT1, m_oThinkTime);
}

BEGIN_MESSAGE_MAP(CDragonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CDragonDlg 消息处理程序
BOOL CDragonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Chessman.Create(IDB_CHESS, 50, 50, RGB(80,80,80)); 

	BITMAP BitMap;
	m_BoardBmp.LoadBitmap(IDB_BOARD);
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth =  BitMap.bmWidth; 
	m_nBoardHeight = BitMap.bmHeight;
	m_BoardBmp.DeleteObject();

	m_bGameOver = FALSE;//this code does not contents in books.

	m_oKernel.InitKernel(this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
//预留kernel通知接口
void CDragonDlg::NoticeUIBegin()
{
	m_bIsBegin = TRUE;
	CString sNodeCount;
	if(TRUE == m_oKernel.m_bIsFirst)
	{
		m_bIsWriteGo = FALSE;
		sNodeCount.Format(L" 您是先手！！！");
	}
	else
	{
		m_bIsWriteGo = TRUE;
		sNodeCount.Format(L" 您是后手！！！");
	}
	m_Output.SetWindowText(sNodeCount);
	m_iShowTime = m_oKernel.m_iTime;
	SetTimer(1,1000,NULL);
	CString sThinkTime;
	sThinkTime.Format(L"    %d s",m_iShowTime);
	m_oThinkTime.SetWindowText(sThinkTime);
}
int CDragonDlg::NoticeUISetTimer()
{
	int nTime;
	//获取时间
	CSetTimerDlg oSetTimerdlg;
	if (oSetTimerdlg.DoModal() == IDC_BUTTON1)
	{
		//拿到窗口设置时间
		nTime = _ttoi(oSetTimerdlg.m_oTime.GetBuffer());
	}
	else
	{
		PostMessage(WM_CLOSE);
		return FALSE;
	}
	return nTime;
}
void CDragonDlg::NoticeUIEnd()
{

}
void CDragonDlg::NoticeUIFailedNet()
{
	MessageBox(L"Connect Net Failed,Please Check Your Net....");
}
void CDragonDlg::NoticeUIiMove(int nWays,int nWhich)
{
	iMove(nWays,nWhich);
}


void CDragonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
void DrawGo(CDC* pDC)
{
	CBrush jBrush;
	jBrush.CreateSolidBrush(RGB(0,255,0));  //绿
	pDC->SelectObject(&jBrush);
	pDC->Rectangle(iBL+11*iBS,iBT+iBS,iBL+12*iBS,iBT+8*iBS);
	pDC->SelectStockObject(BLACK_BRUSH);
	jBrush.DeleteObject();
	jBrush.CreateSolidBrush(RGB(0,0,255));  //绿
	pDC->SelectObject(&jBrush);
	pDC->Rectangle(iBL+12*iBS,iBT+iBS,iBL+13*iBS,iBT+8*iBS);
	pDC->SelectStockObject(BLACK_BRUSH);
	jBrush.DeleteObject();
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDragonDlg::OnPaint()
{
	CPaintDC dc(this);
	CDC MemDC;
	int i, j;
	POINT pt;
	CBitmap *pOldBmp;
	static int nTimes = 0;
	nTimes++;
	if(10 == nTimes)
	{
		nTimes = 2;
	}
//	DrawGo(&dc);
	MemDC.CreateCompatibleDC(&dc);
	m_BoardBmp.LoadBitmap(IDB_BOARD);
	pOldBmp = MemDC.SelectObject(&m_BoardBmp);
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
		{
			if (m_ChessBoard[i][j] == 0)
				continue;
			pt.x = j*GRILLEHEIGHT ;
			pt.y = i*GRILLEWIDTH ;
			switch(m_ChessBoard[i][j])
			{
			case WHITE:
				m_Chessman.Draw(&MemDC, 0, pt, ILD_TRANSPARENT);
				break;
			case BLACK: 
				m_Chessman.Draw(&MemDC, 1, pt, ILD_TRANSPARENT);
				break;
			case -2: 
				m_Chessman.Draw(&MemDC, 2, pt, ILD_TRANSPARENT);
				break;
			case 2: 
				m_Chessman.Draw(&MemDC, 3, pt, ILD_TRANSPARENT);	
				break;
			case 4: 
				m_Chessman.Draw(&MemDC, 4, pt, ILD_TRANSPARENT);	
				break;
			case 6: 
				m_Chessman.Draw(&MemDC, 5, pt, ILD_TRANSPARENT);
				break;
			case -4: 
				m_Chessman.Draw(&MemDC, 6, pt, ILD_TRANSPARENT);	
				break;
			case -6: 
				m_Chessman.Draw(&MemDC, 7, pt, ILD_TRANSPARENT);
				break;
			case -5: 
				m_Chessman.Draw(&MemDC, 8, pt, ILD_TRANSPARENT);
				break;
			case 5: 
				m_Chessman.Draw(&MemDC, 9, pt, ILD_TRANSPARENT);
				break;
			}
		}
		dc.BitBlt(BORDERWIDTH, BORDERHEIGHT, m_nBoardWidth, m_nBoardHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(&pOldBmp);
		MemDC.DeleteDC();
		m_BoardBmp.DeleteObject();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDragonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CMyChessView message handlers
int iInArea(int x,int y,int x1,int y1,int x2,int y2)
{
	if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
		return 1;
	else
		return 0;
}
//extern int m_count;//记录估值结点数;
void CDragonDlg::iMove(int nWays,int nWhich)          
{
	if (FALSE == m_bIsWriteGo)//判断白棋行棋
		return;
	int nNum = -1;
	int ncount = m_oMG.CreatePossibleMove(m_TranslateBoard,0,1) ;
	//行棋
	nNum = nWhich;
	if(-1 != nNum)
	{
		m_oMM.MakeMove1(&m_oMG.m_nMoveList[0][nNum],1,m_TranslateBoard);
		TranslateToChessBoard();
	}
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
	m_bIsWriteGo = FALSE;
	//对方走法已经传过来，开始计时
	KillTimer(2);
	m_iShowTime = m_oKernel.m_iTime;
	SetTimer(1,1000,NULL);

	if (IsGameOver(-1))
	{
		m_bGameOver = TRUE;
		//向服务器发送结束信息
		//m_oKernel.SendToServerEnd(FALSE);
		MessageBox(L"Game Over! You Lost!...");
	}
}

//鼠标控制黑棋行进
void CDragonDlg::Move(int iX,int iY)     
{
	if (TRUE == m_bIsWriteGo/* || FALSE == m_bIsBegin*/)//判断黑棋行棋
		return;
	int nCountMove;
	int nSendData = -1;
	m_iM = (iY-iBT)/iBS;  //将座标转换成数组下标
	m_iN = (iX-iBL)/iBS;
	switch(m_ChessBoard[m_iM][m_iN])
	{
	case BLACK://找到当前被点击的点的走法，
	case 2*BLACK:
		m_m = m_iM;
		m_n = m_iN;
		m_oMM.UnMakeMove(m_ChessBoard);
		nCountMove = m_oMG.CreatePossibleMove(m_ChessBoard,0,1);
		m_nPosMove = 0;
		for(int i=0;i<nCountMove;i++)
		{
			if(m_iM == m_oMG.m_nMoveList[0][i].From.y && m_iN == m_oMG.m_nMoveList[0][i].From.x)
			{
				m_nSurePos[m_nPosMove] = i;
				m_nPosMove++;
			}
		}//并显示出来
		for(int i=0;i<m_nPosMove;i++)
		{   
			//1.显示
			m_oMM.MakeMoveQi(&m_oMG.m_nMoveList[0][m_nSurePos[i]],m_ChessBoard);
			//2.记录最后点坐标
			m_MoveTPos[i] = m_oMG.m_nMoveList[0][m_nSurePos[i]].From;
		}
		break;
	case 5:
		m_oMM.UnMakeMove(m_ChessBoard);
		nCountMove = m_oMG.CreatePossibleMove(m_ChessBoard,0,1);
		for(int i=0;i<m_nPosMove;i++)
		{   
			//1.核对坐标
			if(m_iM == m_MoveTPos[i].y && m_iN == m_MoveTPos[i].x)
			{
				//2.按照找到坐标行棋
				m_oMM.MakeMoveQi(&m_oMG.m_nMoveList[0][m_nSurePos[i]],m_ChessBoard);
				//3.按照走的就真走了
				m_oMM.MakeMoveSure(m_m,m_n,m_oMG.m_nMoveList[0][m_nSurePos[i]].m_Type,m_ChessBoard);
				//4.成王判断
				m_oMM.BecomeKing(m_ChessBoard);
				TranslateToTranslateBoard();
				//5.记录发送数据
				nSendData = m_nSurePos[i];
			}
		}
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		//结构体准备
		m_oKernel.SendToServerGameData(nCountMove,nSendData);
		//规定时间内完成，停止计时
		KillTimer(1);
		m_iShowTime = m_oKernel.m_iTime;
		SetTimer(2,1000,NULL);

		m_iShowTime = m_oKernel.m_iTime;
		CString sThinkTime;
		sThinkTime.Format(L" %d s",m_iShowTime);
		m_oThinkTime.SetWindowText(sThinkTime);

		m_bIsWriteGo = TRUE;
		
		if (IsGameOver(1))
		{
			m_bGameOver = TRUE;
			//向服务器发送结束信息
			//m_oKernel.SendToServerEnd(TRUE);
			MessageBox(L"Game Over! You Win!...");
		}
		break;
	}
}
void CDragonDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnLButtonDown(nFlags, point);
	int iX,iY;
	iX= point.x;
	iY= point.y;
//	if(iInArea(iX,iY,iBL+11*iBS,iBT+iBS,iBL+12*iBS,iBT+8*iBS)) //如果机器行棋（行棋范围）
//	{
//		iMove();
//	}
	if(iInArea(iX,iY,iBL,iBT,iBL+iBS*10,iBT+iBS*10)) //棋盘范围
	{
		Move(iX,iY);
	}
	if(iInArea(iX,iY,iBL+12*iBS,iBT+iBS,iBL+13*iBS,iBT+8*iBS)) //棋盘范围
	{
	}
	InvalidateRect(NULL,FALSE);
	UpdateWindow();
}
void CDragonDlg::TranslateToChessBoard()
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			if(6 == m_TranslateBoard[9-i][9-j])
			{
				m_ChessBoard[i][j] = 4;
				continue;
			}
			if(-6 == m_TranslateBoard[9-i][9-j])
			{
				m_ChessBoard[i][j] = -4;
				continue;
			}
			m_ChessBoard[i][j] = -1 * m_TranslateBoard[9-i][9-j];
		}
	}
}
void CDragonDlg::TranslateToTranslateBoard()
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			m_TranslateBoard[i][j] = -1 * m_ChessBoard[9-i][9-j];
		}
	}
}
void CDragonDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString sThinkTime;
	switch(nIDEvent)
	{
	case 1: 
		m_iShowTime--;
		if(0 == m_iShowTime)
		{
			KillTimer(1);
			m_iShowTime = m_oKernel.m_iTime;
			sThinkTime.Format(L"No Time!");
			m_oThinkTime.SetWindowText(sThinkTime);

			m_bGameOver = TRUE;
			//向服务器发送结束信息
			//m_oKernel.SendToServerEnd(FALSE);
			MessageBox(L"Game Over! You Lost!...");
		}
		else
		{
			sThinkTime.Format(L"    %d s",m_iShowTime);
			m_oThinkTime.SetWindowText(sThinkTime);
		}
		break;
	case 2:
		m_iShowTime--;
		if(0 == m_iShowTime)
		{
			KillTimer(2);
			m_iShowTime = m_oKernel.m_iTime;
			m_bGameOver = TRUE;
			//向服务器发送结束信息
			//m_oKernel.SendToServerEnd(TRUE);
			MessageBox(L"Game Over! You Win!...");
		}
		else
		{
			sThinkTime.Format(L"    %d s",m_iShowTime);
			m_oThinkTime.SetWindowText(sThinkTime);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
