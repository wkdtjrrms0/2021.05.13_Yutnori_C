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
	int IsTurn;
	void opPiece(int m_opRadio, int opYutNum);
	void PaintPiece(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int y1, int y2, int y3, int y4, int y5, int y6, int y7, int y8);
	int x1, x2, x3, x4, x5, x6, x7, x8, y1, y2, y3, y4, y5, y6, y7, y8; //말의 위치정보를 저장할 변수

	BOOL Catch(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int y1, int y2, int y3, int y4, int y5, int y6, int y7, int y8);
	BOOL CatchMe(int cx1, int cx2, int cx3, int cx4, int cx5, int cx6, int cx7, int cx8, int cy1, int cy2, int cy3, int cy4, int cy5, int cy6, int cy7, int cy8);
};
