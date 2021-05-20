#pragma once


// CYutnoriStart 대화 상자

class CYutnoriStart : public CDialogEx
{
	DECLARE_DYNAMIC(CYutnoriStart)

public:
	CYutnoriStart(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CYutnoriStart();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YUTNORI_START };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
};
