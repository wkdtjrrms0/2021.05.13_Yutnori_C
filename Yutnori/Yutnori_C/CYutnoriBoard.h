#pragma once
#include "CClientSocket.h"
#include "afxwin.h"
#include "afxdhtml.h"
#include<ctime>

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
	afx_msg void OnPaint();
	void ConnectChat();
	virtual BOOL OnInitDialog();
	CString m_strMessage;
	CClientSocket* m_pClientSocket; //CClient소켓 개체의 주소를 저장할 멤버변수
	virtual BOOL DestroyWindow();
	CEdit m_ctrlEdit;
	CString m_nickname;
	virtual void OnOK();
	afx_msg void OnBnClickedButton4();
	CString m_strAddress;
	int m_nPort;
	int SelectOAuth; //로그인, 로그아웃 상태를 저장하는 변수합니다. 
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CEdit m_ctrlEditN;
	afx_msg void OnBnClickedCancel();
	int m_Radio;
	int IsThrow;
	int YutNum;
	int goalm; //나의 성공말갯수
	int goaly; //상대의 성공말갯수
	int IsGamebutton;
};
