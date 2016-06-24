// GameClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "afxdialogex.h"
#include <afxcmn.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CGameClientDlg �Ի���



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


// CGameClientDlg ��Ϣ�������

BOOL CGameClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	oKernel.InitKernel(this);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	//���µ�����������
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGameClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGameClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGameClientDlg::OnBnClickedJoingame()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = -1;
	nIndex = m_GameList.GetSelectionMark();
	oKernel.NoticeKernelSendPlayStranger(nIndex);
}


void CGameClientDlg::OnBnClickedDowngame()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = -1;
	nIndex = m_GameList.GetSelectionMark();
	oKernel.NoticeKernelSendDownLoadGame(nIndex);
}
