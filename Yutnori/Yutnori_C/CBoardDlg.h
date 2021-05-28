#pragma once


// CBoardDlg 대화 상자

class CBoardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBoardDlg)

public:
	CBoardDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBoardDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	int m_nRand1;
	int m_nRand2;
	int m_nRand3;
	int m_nRand4;
	int m_nMove;
	afx_msg HCURSOR OnQueryDragIcon();
protected:
	HICON m_hIcon;
};
