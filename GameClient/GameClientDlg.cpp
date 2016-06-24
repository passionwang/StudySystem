// GameClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "afxdialogex.h"
#include <afxcmn.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CGameClientDlg 对话框



CGameClientDlg::CGameClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CGameClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_GameList);
}

BEGIN_MESSAGE_MAP(CGameClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_JOINGAME, &CGameClientDlg::OnBnClickedJoingame)
	ON_BN_CLICKED(IDC_DOWNGAME, &CGameClientDlg::OnBnClickedDowngame)
END_MESSAGE_MAP()


// CGameClientDlg 消息处理程序

BOOL CGameClientDlg::OnInitDialog()
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

	ShowWindow(SW_MINIMIZE);


	// TODO: 在此添加额外的初始化代码
	oKernel.InitKernel(this);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



bool CGameClientDlg::NotifyLogin()
{
	CLoginDlg oLogin;
	if(IDC_LOGIN == oLogin.DoModal())
	{
		INT64 i64UserID = _ttoi64(oLogin.m_strUser.GetBuffer());
		WORD wPasswordLen = oLogin.m_strPassword.GetLength();
		char szbuf[DEF_MAX_PASSWORD_LEN];
		memset(szbuf, 0, DEF_MAX_PASSWORD_LEN);
		WideCharToMultiByte(CP_OEMCP, 0, oLogin.m_strPassword.GetBuffer(), -1, szbuf, wPasswordLen, NULL, FALSE);  
		oKernel.NoticeKernelSendLogin(i64UserID,wPasswordLen,szbuf);
	}
	return true;
}
bool CGameClientDlg::NotifyGetGameList(STRU_GET_APP_LIST_RS oDataAppListRs)
{
	//更新到主窗口上面
	TCHAR str[1024] = {0};
	CImageList imageList;
	//ILC_COLOR

	imageList.Create(24, 24, ILC_COLOR32, 0, 4);
	for(int i=0;i<oDataAppListRs.m_wAppItemCount;i++)
	{
		oDataAppListRs.m_oAppList[i].m_szIconPath[oDataAppListRs.m_oAppList[i].m_wAppIconPathLen] = '\0';
		MultiByteToWideChar(CP_ACP, 0, oDataAppListRs.m_oAppList[i].m_szIconPath, -1, str, 1024);
		//HANDLE hhh =LoadImage(GetModuleHandle(NULL),str,IMAGE_ICON ,0,0,LR_LOADFROMFILE  );
		
		 CBitmap pbmImage;
		 BOOL bT = pbmImage.LoadBitmap(IDB_BITMAP1);
		 int nNum = GetLastError();
		 int iNum = imageList.Add(&pbmImage,RGB(0, 0, 0));
	}
	/*HANDLE hhh =LoadImage(GetModuleHandle(NULL),L"D:/app1.ico",IMAGE_ICON ,0,0,LR_LOADFROMFILE  );
	int iNum = imageList.Add((HICON)hhh);*/
/**/
	//iNum = imageList.Add(AfxGetApp()->LoadIcon(IDI_GAME2));
	m_GameList.SetImageList(&imageList, LVSIL_SMALL); 
	for(int i=0;i<oDataAppListRs.m_wAppItemCount;i++)
	{
		oDataAppListRs.m_oAppList[i].m_szAppName[oDataAppListRs.m_oAppList[i].m_wAppNameLen] = '\0';
		MultiByteToWideChar(CP_ACP, 0, oDataAppListRs.m_oAppList[i].m_szAppName, -1, str, 1024);
		m_GameList.InsertItem(i, str, i);
	}
	//oDataAppListRs.m_oAppList[0].m_szAppName[oDataAppListRs.m_oAppList[0].m_wAppNameLen] = '\0';
	
	
	/*MultiByteToWideChar(CP_ACP, 0, oDataAppListRs.m_oAppList[0].m_szAppName, -1, str, 1024);
	m_GameList.InsertItem(1, str, 0);
	oDataAppListRs.m_oAppList[1].m_szAppName[oDataAppListRs.m_oAppList[1].m_wAppNameLen] = '\0';
	
	MultiByteToWideChar(CP_ACP, 0, oDataAppListRs.m_oAppList[1].m_szAppName, -1, str, 1024);
	m_GameList.InsertItem(1, str, 1);*/
	return true;
}
bool CGameClientDlg::NotifyLoginRs(INT64 m_i64UserID,bool bRes)
{

	return true;
}
bool CGameClientDlg::NotifyRegisterRs(long lUserID,bool bRes)
{

	return true;
}
bool CGameClientDlg::NotifyGetGameListRs(bool bRes)
{

	return true;
}
void CGameClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGameClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGameClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGameClientDlg::OnBnClickedJoingame()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = -1;
	nIndex = m_GameList.GetSelectionMark();
	oKernel.NoticeKernelSendPlayStranger(nIndex);
}


void CGameClientDlg::OnBnClickedDowngame()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = -1;
	nIndex = m_GameList.GetSelectionMark();
	oKernel.NoticeKernelSendDownLoadGame(nIndex);
}
