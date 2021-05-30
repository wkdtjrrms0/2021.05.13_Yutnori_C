#pragma once


// CYutnoriBoard 대화 상자

class CYutnoriBoard : public CDialogEx
{
	DECLARE_DYNAMIC(CYutnoriBoard)

public:
	CYutnoriBoard(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CYutnoriBoard();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YUTNORI_Board };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRadio2();
};
