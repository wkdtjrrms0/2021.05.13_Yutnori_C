#pragma once
#include "CClientSocket.h"
#include "afxwin.h"
#include "afxdhtml.h"
#include<ctime>

class CYutnoriBoard : public CDialogEx
{
	DECLARE_DYNAMIC(CYutnoriBoard)
public:
	CYutnoriBoard(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CYutnoriBoard();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YUTNORI_Board };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	int m_nPort; //포트번호
	int SelectOAuth; //로그인, 로그아웃 상태를 저장하는 변수합니다. 
	int m_Radio; //선택한 말
	int IsThrow; //던지기 전에는 0, 던진 후에는 증가
	int YutNum; // 1~5의 랜덤한 숫자
	int IsGamebutton; //게임버튼을 눌렀는지 체크
	int IsTurn; //나의 턴인가 체크
	int IsGameStart; //게임버튼이 동작되도록 눌렸는가 체크
	int ConnectError; //연결에러체크
	CString m_strMessage; //채팅메시지
	CString m_nickname; //닉네임
	CString m_nid; //네이버고유ID
	CString m_strAddress; //접속하고자하는 IP주소
	CStatic m_Text; //상태문자
	CEdit m_ctrlEdit; //채팅창
	CClientSocket* m_pClientSocket; //CClient소켓 개체의 주소를 저장할 멤버변수
	BOOL Catch(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int y1, int y2, int y3, int y4, int y5, int y6, int y7, int y8); //내가 상대를 잡았을때
	BOOL CatchMe(int cx1, int cx2, int cx3, int cx4, int cx5, int cx6, int cx7, int cx8, int cy1, int cy2, int cy3, int cy4, int cy5, int cy6, int cy7, int cy8); //상대가 나를 잡았을때
	virtual BOOL DestroyWindow();
	virtual void OnOK(); //채팅전송
	afx_msg void OnPaint(); //윷판그림
	afx_msg void OnBnClickedButton1(); //게임준비버튼
	afx_msg void OnBnClickedButton2(); //옮기기버튼
	afx_msg void OnBnClickedButton3(); //던지기버튼
	afx_msg void OnBnClickedButton4(); //로그인버튼 + 기권버튼
	afx_msg void OnBnClickedCancel(); //나가기버튼
	void Connect(); //서버연결
	void opPiece(int m_opRadio, int opYutNum); //상대말 움직임
	void PaintPiece(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int y1, int y2, int y3, int y4, int y5, int y6, int y7, int y8); //해당좌표그림
	void victoryCheck(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy); //승리여부체크
	void InitPiece(); //승리 or 패배시 이어서 게임할 수 있도록 초기화
	afx_msg void OnBnClickedButton5();
};
