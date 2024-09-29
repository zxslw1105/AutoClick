
// AutoClickDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AutoClick.h"
#include "AutoClickDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAutoClickDlg 对话框



CAutoClickDlg::CAutoClickDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AUTOCLICK_DIALOG, pParent)
	, m_xPos(0)
	, m_yPos(0)
	, m_bTimer(false)
	, screen_width(0)
	, screen_height(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoClickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoClickDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CAutoClickDlg::OnBnClickedBtnBegin)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CAutoClickDlg 消息处理程序

BOOL CAutoClickDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//注册ALT+K开始快捷键
	if (!RegisterHotKey(m_hWnd, start_hot_key, MOD_ALT, 0x4B))
	{
		SetDlgItemText(IDC_S_START_TIP, _T("开始快捷键注册失败"));
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAutoClickDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoClickDlg::OnPaint()
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
HCURSOR CAutoClickDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAutoClickDlg::OnBnClickedBtnBegin()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bTimer)
	{
		kill_timer();
	}
	else
	{
		//获取窗口右上角位置
		CRect rect;
		GetWindowRect(&rect);
		m_xPos = rect.right;
		m_yPos = rect.top;

		screen_width = GetSystemMetrics(SM_CXSCREEN);
		screen_height = GetSystemMetrics(SM_CYSCREEN);

		if (SetTimer(timer_id, 120, nullptr) == timer_id)
		{
			m_bTimer = true;
			//注册ALT+J结束快捷键
			if (!RegisterHotKey(m_hWnd, start_hot_key, MOD_ALT, 0x4A))
			{
				SetDlgItemText(IDC_S_START_TIP, _T("结束快捷键注册失败"));
			}
		}
	}

	SetDlgItemText(IDC_BTN_BEGIN, m_bTimer ? _T("结束") : _T("开始"));

	test_count = 0;
}

bool CAutoClickDlg::kill_timer()
{
	bool result = false;

	if (m_bTimer)
	{
		if (KillTimer(timer_id))
		{
			m_bTimer = false;
			result = true;
		}
	}

	return result;
}

void CAutoClickDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	DWORD x_pos = m_xPos * 65535 / screen_width + 10;
	DWORD y_pos = m_yPos * 65535 / screen_height;
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x_pos, y_pos, 0, GetMessageExtraInfo());
	//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x_pos, y_pos, 0, GetMessageExtraInfo());
	mouse_event(MOUSEEVENTF_LEFTDOWN , x_pos, y_pos, 0, GetMessageExtraInfo());
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTUP, x_pos, y_pos, 0, GetMessageExtraInfo());

	CDialogEx::OnTimer(nIDEvent);
}


void CAutoClickDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_bTimer)
	{
		UnregisterHotKey(m_hWnd, stop_hot_key);
	}
	UnregisterHotKey(m_hWnd, start_hot_key);

	kill_timer();
}


void CAutoClickDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nHotKeyId == stop_hot_key)
	{
		UnregisterHotKey(m_hWnd, stop_hot_key);
		kill_timer();
		SetDlgItemText(IDC_BTN_BEGIN, _T("开始"));
	}
	else if(nHotKeyId == start_hot_key)
	{
		OnBnClickedBtnBegin();
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
