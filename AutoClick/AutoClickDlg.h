
// AutoClickDlg.h: 头文件
//

#pragma once


// CAutoClickDlg 对话框
class CAutoClickDlg : public CDialogEx
{
// 构造
public:
	CAutoClickDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTOCLICK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBegin();
private:
	long m_xPos, m_yPos;
	bool m_bTimer;
	const unsigned int timer_id = 1001;
	const unsigned int start_hot_key = 1002;
	const unsigned int stop_hot_key = 1003;
	int screen_width;
	int screen_height;
	int test_count;
	bool kill_timer();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
