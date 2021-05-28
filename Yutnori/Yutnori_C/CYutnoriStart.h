#pragma once
#include "afxdhtml.h"

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
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CStatic m_Myicon;
	int SelectOAuth; //로그인, 로그아웃 상태를 저장하는 변수합니다. 
	afx_msg void OnPaint();
};
