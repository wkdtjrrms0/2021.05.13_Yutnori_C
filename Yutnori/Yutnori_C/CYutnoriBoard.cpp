// CYutnoriBoard.cpp: 구현 파일//

#include "pch.h"
#include "framework.h"
#include "Yutnori_C.h"
#include "CYutnoriBoard.h"
#include "afxdialogex.h"
#include "CNaverOAuthDlg.h"
#include <atlimage.h>
#include "CPiece.h"

#define FIRSTX 275
#define FIRSTY 480

IMPLEMENT_DYNAMIC(CYutnoriBoard, CDialogEx)

/*생성자*/
CYutnoriBoard::CYutnoriBoard(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YUTNORI_Board, pParent)
	, m_strAddress(_T("127.0.0.1"))//연결할 컴퓨터 주소
	, m_nPort(7000)
	, m_strMessage(_T("메시지를 입력하세요."))
	, m_Radio(-1)
{
	m_nickname = _T("");
	SelectOAuth = 0;
	IsThrow = 0;
	YutNum = 0;
	IsGamebutton = 0;
	IsTurn = 0;
	IsGameStart = 0;
	ConnectError = 0;
}

/*소멸자*/
CYutnoriBoard::~CYutnoriBoard(){}

void CYutnoriBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strMessage);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
	DDX_Control(pDX, IDC_STATIC2, m_Text);
}

BEGIN_MESSAGE_MAP(CYutnoriBoard, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON4, &CYutnoriBoard::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CYutnoriBoard::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CYutnoriBoard::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CYutnoriBoard::OnBnClickedButton3)
	ON_BN_CLICKED(IDCANCEL, &CYutnoriBoard::OnBnClickedCancel)
END_MESSAGE_MAP()

CNaverOAuthDlg dlg; //네이버 로그인 객체 생성
CPiece dlg0, dlg1, dlg2, dlg3; //내 말객체
CPiece dlg4, dlg5, dlg6, dlg7; //상대방 말객체

/*윷판을 그립니다.*/
void CYutnoriBoard::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else { //윷판을 그림 
		CDC* pDC = GetDC();
		CClientDC dc(this);
		CBitmap bmpBack1; bmpBack1.LoadBitmapA(IDB_BITMAP2);
		BITMAP bitmap; bmpBack1.GetBitmap(&bitmap);
		int nWidth = bitmap.bmWidth;
		int nHeight = bitmap.bmHeight;
		CDC memDC; memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmpBack1);
		pDC->BitBlt(10, 10, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
		memDC.DeleteDC();
		bmpBack1.DeleteObject();
		CDialog::OnPaint();
	}
}

/*(2)소켓만듬, (3)연결시도, 서버와 연결하는 함수입니다.*/
void CYutnoriBoard::Connect()
{
	m_pClientSocket = new CClientSocket;
	m_pClientSocket->Create(); //(2)소켓만듬
	this->UpdateData(TRUE);
	CString str; str.Format(_T("[Status] 서버와 연결합니다....\r\n"));
	m_ctrlEdit.ReplaceSel(str);
	bool success = false;
	success = m_pClientSocket->Connect(m_strAddress, m_nPort);  //(3)연결시도
	if (!success) { ConnectError = -1;  m_ctrlEdit.ReplaceSel(_T("[error] 서버와 연결하지 못했습니다.\r\n")); };
}

/*[1] 로그인버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton4()
{
	if (dlg.IsLogin == 0 || dlg.IsLogin == -1) { //처음(0)이나 로그아웃(-1)했을때 실행
		dlg.DoModal();
		if (dlg.IsLogin == 1) { //로그인 성공시
			SelectOAuth = 1;
			m_nickname = dlg.m_NaverNickName;
			SetDlgItemText(IDC_BUTTON4, _T("로그아웃"));
			SetDlgItemText(IDC_STATIC, _T(m_nickname + "님 \n 반갑습니다."));
		}
	}
	else if (dlg.IsLogin == 1) { //로그인 되어있을 때 실행
		if (IsGamebutton == 0) {
			dlg.DoModal();
			SelectOAuth = -1;
			SetDlgItemText(IDC_BUTTON4, _T("로그인"));
			SetDlgItemText(IDC_STATIC, _T("게임이용을 위해 로그인을 해주시기 바랍니다."));
		}
		else {
			m_ctrlEdit.ReplaceSel(_T("게임이용중에는 로그아웃 할 수 없습니다. 기권 후 눌러주세요.\r\n"));
		}
	}
}

/*[2-1] 게임준비버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton1()
{
	if (SelectOAuth == 1) {
		if ((IsGamebutton != 1) && (ConnectError == 0)) {
			Connect();
		}
		if ((IsGamebutton != 1) && (ConnectError != -1)) {
			SetDlgItemText(IDC_STATIC2, _T("상대방을 기다리는 중입니다."));
			SetDlgItemText(IDC_BUTTON1, _T("기권하기"));
			IsGamebutton = 1;
		}
		else if((IsGamebutton == 1) && (ConnectError != -1)) {
			CString lose;
			lose.Format(_T("[Game]%s: Lose\r\n"), m_nickname);
			m_pClientSocket->Send(lose, lose.GetLength());
			m_ctrlEdit.ReplaceSel(_T("[Status] 연결 종료\r\n"));
			m_pClientSocket->ShutDown();
			m_pClientSocket->Close();
			delete m_pClientSocket;
			InitPiece();
			SetDlgItemText(IDC_STATIC2, _T("안녕하세요. 윷놀이 게임입니다."));
			SetDlgItemText(IDC_BUTTON1, _T("게임준비"));
			IsGamebutton = 0;
		}
		else { ConnectError = 0; }
	}
	else { AfxMessageBox("로그인을 해주시기 바랍니다."); }
}

/*[2-2]연결이 끝나면 소켓을 끊습니다.*/
BOOL CYutnoriBoard::DestroyWindow()
{
	if (IsGameStart == 1) {
		m_pClientSocket->ShutDown();
		m_pClientSocket->Close();
		delete m_pClientSocket;
	}
	return CDialogEx::DestroyWindow();
}

/*[3] 채팅전송버튼입니다.*/
void CYutnoriBoard::OnOK()
{
	if ((SelectOAuth == 1) && (IsGamebutton == 1) && (ConnectError != -1)) {
		this->UpdateData(TRUE);
		m_strMessage.Append(_T("\r\n"));
		m_pClientSocket->Send("[Chat]" + m_nickname + ": " + m_strMessage, m_nickname.GetLength() + 8 + m_strMessage.GetLength());  //(4)데이터 보냄.
		m_strMessage = _T("");
		this->UpdateData(FALSE);
	}
	else { AfxMessageBox("로그인과 게임준비를 해주시기 바랍니다."); }
}

/*[3] 던지기버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton3()
{
	if (IsGamebutton == 1) {
		if (IsGameStart == 1) {
			if (IsTurn != -1) {
				if (IsThrow >= 1) { m_ctrlEdit.ReplaceSel(_T("옮긴 후 던져주세요.\r\n")); }
				else {
					srand((unsigned int)time(NULL));
					YutNum = rand() % 5 + 1;
					/*도,개,걸,윷,모*/
					if (YutNum == 1) {
						CClientDC dc(this); //메모리DC를 만듬
						CDC memDC; memDC.CreateCompatibleDC(&dc); //메모리DC를 만듬
						CBitmap bmpBG; bmpBG.LoadBitmap(IDB_BITMAP3); //비트맵리소스를 읽어옴
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						BITMAP bitmap; bmpBG.GetBitmap(&bitmap); //부가정보를 가져오는 함수
						int nWidth = bitmap.bmWidth; int nHeight = bitmap.bmHeight;
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						dc.BitBlt(560, 200, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
						memDC.DeleteDC();  //메모리DC삭제
						bmpBG.DeleteObject(); //비트맵리소스 삭제
					}
					else if (YutNum == 2) {
						CClientDC dc(this); //메모리DC를 만듬
						CDC memDC; memDC.CreateCompatibleDC(&dc); //메모리DC를 만듬
						CBitmap bmpBG; bmpBG.LoadBitmap(IDB_BITMAP4); //비트맵리소스를 읽어옴
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						BITMAP bitmap; bmpBG.GetBitmap(&bitmap); //부가정보를 가져오는 함수
						int nWidth = bitmap.bmWidth; int nHeight = bitmap.bmHeight;
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						dc.BitBlt(560, 200, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
						memDC.DeleteDC();  //메모리DC삭제
						bmpBG.DeleteObject(); //비트맵리소스 삭제
					}
					else if (YutNum == 3) {
						CClientDC dc(this); //메모리DC를 만듬
						CDC memDC; memDC.CreateCompatibleDC(&dc); //메모리DC를 만듬
						CBitmap bmpBG; bmpBG.LoadBitmap(IDB_BITMAP5); //비트맵리소스를 읽어옴
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						BITMAP bitmap; bmpBG.GetBitmap(&bitmap); //부가정보를 가져오는 함수
						int nWidth = bitmap.bmWidth; int nHeight = bitmap.bmHeight;
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						dc.BitBlt(560, 200, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
						memDC.DeleteDC();  //메모리DC삭제
						bmpBG.DeleteObject(); //비트맵리소스 삭제
					}
					else if (YutNum == 4) {
						CClientDC dc(this); //메모리DC를 만듬
						CDC memDC; memDC.CreateCompatibleDC(&dc); //메모리DC를 만듬
						CBitmap bmpBG; bmpBG.LoadBitmap(IDB_BITMAP6); //비트맵리소스를 읽어옴
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						BITMAP bitmap; bmpBG.GetBitmap(&bitmap); //부가정보를 가져오는 함수
						int nWidth = bitmap.bmWidth; int nHeight = bitmap.bmHeight;
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						dc.BitBlt(560, 200, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
						memDC.DeleteDC();  //메모리DC삭제
						bmpBG.DeleteObject(); //비트맵리소스 삭제
					}
					else if (YutNum == 5) {
						CClientDC dc(this); //메모리DC를 만듬
						CDC memDC; memDC.CreateCompatibleDC(&dc); //메모리DC를 만듬
						CBitmap bmpBG; bmpBG.LoadBitmap(IDB_BITMAP7); //비트맵리소스를 읽어옴
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						BITMAP bitmap; bmpBG.GetBitmap(&bitmap); //부가정보를 가져오는 함수
						int nWidth = bitmap.bmWidth; int nHeight = bitmap.bmHeight;
						memDC.SelectObject(&bmpBG); //비트맵(bmpBG) 리소스를 선택
						dc.BitBlt(560, 200, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
						memDC.DeleteDC();  //메모리DC삭제
						bmpBG.DeleteObject(); //비트맵리소스 삭제
					}
					IsThrow++;}
			} else { m_ctrlEdit.ReplaceSel(_T("상대방이 던질때까지 기다려주세요.\r\n")); }
		} else{ m_ctrlEdit.ReplaceSel(_T("상대방이 접속할때까지 기다려주세요.\r\n")); }
	} else { AfxMessageBox("로그인 후 게임준비버튼을 눌러주시기 바랍니다."); }
}

/*[3] 옮기기버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton2()
{
	if (IsGamebutton == 1) {
		if (IsGameStart == 1) {
			if (IsTurn != -1) {
				if (IsThrow != 0) { //윷던지기 눌렀을때 실행
					Invalidate();
					UpdateData(true); //라디오버튼 체크
					CString yutinfo;
					switch (m_Radio) { //선택한 말 이동
					case 0: dlg0.Move(YutNum); break;
					case 1: dlg1.Move(YutNum); break;
					case 2: dlg2.Move(YutNum); break;
					case 3: dlg3.Move(YutNum); break;
					default: m_ctrlEdit.ReplaceSel(_T("옮길 말을 선택해주세요.\r\n")); break;
					}
					if ((m_Radio == 0) || (m_Radio == 1) || (m_Radio == 2) || (m_Radio == 3)) { //말이 선택되었을때 실질적인 윷정보를 보냄
						yutinfo.Format(_T("[Yutinfo]%s: Piece: %d, YutNum: %d\r\n"), m_nickname, m_Radio, YutNum);
						m_pClientSocket->Send(yutinfo, yutinfo.GetLength());
						m_ctrlEdit.ReplaceSel(_T("말을 옮겼습니다.\r\n"));
						IsThrow = 0;
						AfxMessageBox("말을 옮겼습니다.");
					};
					if (Catch(dlg0.x, dlg1.x, dlg2.x, dlg3.x, dlg4.x, dlg5.x, dlg6.x, dlg7.x, dlg0.y, dlg1.y, dlg2.y, dlg3.y, dlg4.y, dlg5.y, dlg6.y, dlg7.y) == TRUE) {
						m_ctrlEdit.ReplaceSel(_T("축하합니다. 상대말을 잡았습니다.\r\n"));
					} //내가 상대말을 잡았는지 체크
					PaintPiece(dlg0.x, dlg1.x, dlg2.x, dlg3.x, dlg4.x, dlg5.x, dlg6.x, dlg7.x, dlg0.y, dlg1.y, dlg2.y, dlg3.y, dlg4.y, dlg5.y, dlg6.y, dlg7.y);
					victoryCheck(dlg0.x, dlg0.y, dlg1.x, dlg1.y, dlg2.x, dlg2.y, dlg3.x, dlg3.y); //승리했는지 체크
				}
				else { m_ctrlEdit.ReplaceSel(_T("윷을 던지고 옮겨주세요.\r\n")); }
			}
			else { m_ctrlEdit.ReplaceSel(_T("상대방이 던질때까지 기다려주세요.\r\n")); }
		}
		else { m_ctrlEdit.ReplaceSel(_T("상대방이 접속할때까지 기다려주세요.\r\n")); }
	}
	else { AfxMessageBox("로그인 후 게임준비버튼을 눌러주시기 바랍니다."); }
}

/*[4] 상대방말 움직이는 함수*/
void CYutnoriBoard::opPiece(int m_opRadio, int opYutNum)
{
	Invalidate();
	UpdateData(true);
	CString yutinfo;
	switch (m_opRadio) { //선택한 말 이동
	case 0: dlg4.Move(opYutNum); break;
	case 1: dlg5.Move(opYutNum); break;
	case 2: dlg6.Move(opYutNum); break;
	case 3: dlg7.Move(opYutNum); break;
	default: m_ctrlEdit.ReplaceSel(_T("옮길 말을 선택해주세요.\r\n")); break;
	}
	if ((m_opRadio == 0) || (m_opRadio == 1) || (m_opRadio == 2) || (m_opRadio == 3)) { //말이 체크되었을때
		m_ctrlEdit.ReplaceSel(_T("상대방이 말을 옮겼습니다.\r\n"));
		AfxMessageBox("상대방이 말을 옮겼습니다.");
	};
	if (CatchMe(dlg4.x, dlg5.x, dlg6.x, dlg7.x, dlg0.x, dlg1.x, dlg2.x, dlg3.x, dlg4.y, dlg5.y, dlg6.y, dlg7.y, dlg0.y, dlg1.y, dlg2.y, dlg3.y) == TRUE) {
		m_ctrlEdit.ReplaceSel(_T("아쉽내요. 상대말에게 잡혔습니다.\r\n"));
	} //상대가 내 말을 잡았는지 체크
	PaintPiece(dlg0.x, dlg1.x, dlg2.x, dlg3.x, dlg4.x, dlg5.x, dlg6.x, dlg7.x, dlg0.y, dlg1.y, dlg2.y, dlg3.y, dlg4.y, dlg5.y, dlg6.y, dlg7.y);
}

/*[3.옮기기,4] 좌표 색칠하는 함수*/
void CYutnoriBoard::PaintPiece(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int y1, int y2, int y3, int y4, int y5, int y6, int y7, int y8)
{
	CClientDC dc(this);
	CBrush blue(RGB(0, 100, 255));
	CBrush* old_blue = dc.SelectObject(&blue);
	dc.Ellipse(x1 - 15, y1 - 15, x1 + 15, y1 + 15);
	dc.Ellipse(x2 - 15, y2 - 15, x2 + 15, y2 + 15);
	dc.Ellipse(x3 - 15, y3 - 15, x3 + 15, y3 + 15);
	dc.Ellipse(x4 - 15, y4 - 15, x4 + 15, y4 + 15);
	dc.SelectObject(old_blue);
	CBrush orange(RGB(255, 100, 0));
	CBrush* old_orange = dc.SelectObject(&orange);
	dc.Ellipse(x5 - 15, y5 - 15, x5 + 15, y5 + 15);
	dc.Ellipse(x6 - 15, y6 - 15, x6 + 15, y6 + 15);
	dc.Ellipse(x7 - 15, y7 - 15, x7 + 15, y7 + 15);
	dc.Ellipse(x8 - 15, y8 - 15, x8 + 15, y8 + 15);
	dc.SelectObject(old_orange);
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	CPen* oldpen = dc.SelectObject(&pen);
	dc.MoveTo(x1,y1-7); dc.LineTo(x1,y1+7); //1번말 표기(l)
	dc.MoveTo(x2 - 7, y2); dc.LineTo(x2 + 7, y2); //2번말 표기(ㅡ)
	dc.MoveTo(x3 - 6, y3 - 6); dc.LineTo(x3 + 6, y3 + 6); //3번말 표기(＼)
	dc.MoveTo(x4 - 6, y4 + 6); dc.LineTo(x4 + 6, y4 - 6); //4번말 표기(/)
	dc.MoveTo(x5, y5 - 7); dc.LineTo(x5, y5 + 7); //상대 1번말 표기(l)
	dc.MoveTo(x6 - 7, y6); dc.LineTo(x6 + 7, y6); //상대 2번말 표기(ㅡ)
	dc.MoveTo(x7 - 6, y7 - 6); dc.LineTo(x7 + 6, y7 + 6); //상대 3번말 표기(＼)
	dc.MoveTo(x8 - 6, y8 + 6); dc.LineTo(x8 + 6, y8 - 6); //상대 4번말 표기(/)
	dc.SelectObject(oldpen);
}

/*[3.옮기기] 상대방 말을 잡는 함수*/
BOOL CYutnoriBoard::Catch(int cx1, int cx2, int cx3, int cx4, int cx5, int cx6, int cx7, int cx8, int cy1, int cy2, int cy3, int cy4, int cy5, int cy6, int cy7, int cy8)
{
	if ((cx1 == cx5) && (cy1 == cy5) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg4.x = FIRSTX; dlg4.y = FIRSTY; dlg4.catched = 1; return TRUE; }
	else if ((cx1 == cx6) && (cy1 == cy6) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg5.x = FIRSTX; dlg5.y = FIRSTY; dlg5.catched = 1; return TRUE;}
	else if ((cx1 == cx7) && (cy1 == cy7) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg6.x = FIRSTX; dlg6.y = FIRSTY; dlg6.catched = 1; return TRUE;}
	else if ((cx1 == cx8) && (cy1 == cy8) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg7.x = FIRSTX; dlg7.y = FIRSTY; dlg7.catched = 1; return TRUE;}
	else if ((cx2 == cx5) && (cy2 == cy5) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg4.x = FIRSTX; dlg4.y = FIRSTY; dlg4.catched = 1; return TRUE;}
	else if ((cx2 == cx6) && (cy2 == cy6) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg5.x = FIRSTX; dlg5.y = FIRSTY; dlg5.catched = 1; return TRUE;}
	else if ((cx2 == cx7) && (cy2 == cy7) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg6.x = FIRSTX; dlg6.y = FIRSTY; dlg6.catched = 1; return TRUE;}
	else if ((cx2 == cx8) && (cy2 == cy8) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg7.x = FIRSTX; dlg7.y = FIRSTY; dlg7.catched = 1; return TRUE;}
	else if ((cx3 == cx5) && (cy3 == cy5) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg4.x = FIRSTX; dlg4.y = FIRSTY; dlg4.catched = 1; return TRUE;}
	else if ((cx3 == cx6) && (cy3 == cy6) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg5.x = FIRSTX; dlg5.y = FIRSTY; dlg5.catched = 1; return TRUE;}
	else if ((cx3 == cx7) && (cy3 == cy7) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg6.x = FIRSTX; dlg6.y = FIRSTY; dlg6.catched = 1; return TRUE;}
	else if ((cx3 == cx8) && (cy3 == cy8) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg7.x = FIRSTX; dlg7.y = FIRSTY; dlg7.catched = 1; return TRUE;}
	else if ((cx4 == cx5) && (cy4 == cy5) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg4.x = FIRSTX; dlg4.y = FIRSTY; dlg4.catched = 1; return TRUE;}
	else if ((cx4 == cx6) && (cy4 == cy6) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg5.x = FIRSTX; dlg5.y = FIRSTY; dlg5.catched = 1; return TRUE;}
	else if ((cx4 == cx7) && (cy4 == cy7) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg6.x = FIRSTX; dlg6.y = FIRSTY; dlg6.catched = 1; return TRUE;}
	else if ((cx4 == cx8) && (cy4 == cy8) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg7.x = FIRSTX; dlg7.y = FIRSTY; dlg7.catched = 1; return TRUE;}
	return FALSE;
}

/*[4]내 말이 잡히는 함수*/
BOOL CYutnoriBoard::CatchMe(int cx1, int cx2, int cx3, int cx4, int cx5, int cx6, int cx7, int cx8, int cy1, int cy2, int cy3, int cy4, int cy5, int cy6, int cy7, int cy8)
{
	if ((cx1 == cx5) && (cy1 == cy5) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg0.x = FIRSTX; dlg0.y = FIRSTY; dlg0.catched = 1; return TRUE; }
	else if ((cx1 == cx6) && (cy1 == cy6) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg1.x = FIRSTX; dlg1.y = FIRSTY; dlg1.catched = 1; return TRUE; }
	else if ((cx1 == cx7) && (cy1 == cy7) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg2.x = FIRSTX; dlg2.y = FIRSTY; dlg2.catched = 1; return TRUE; }
	else if ((cx1 == cx8) && (cy1 == cy8) && (cx1 != FIRSTX) && (cy1 != FIRSTY) && (cx1 != -1) && (cy1 != -1)) { dlg3.x = FIRSTX; dlg3.y = FIRSTY; dlg3.catched = 1; return TRUE; }
	else if ((cx2 == cx5) && (cy2 == cy5) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg0.x = FIRSTX; dlg0.y = FIRSTY; dlg0.catched = 1; return TRUE; }
	else if ((cx2 == cx6) && (cy2 == cy6) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg1.x = FIRSTX; dlg1.y = FIRSTY; dlg1.catched = 1; return TRUE; }
	else if ((cx2 == cx7) && (cy2 == cy7) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg2.x = FIRSTX; dlg2.y = FIRSTY; dlg2.catched = 1; return TRUE; }
	else if ((cx2 == cx8) && (cy2 == cy8) && (cx2 != FIRSTX) && (cy2 != FIRSTY) && (cx2 != -1) && (cy2 != -1)) { dlg3.x = FIRSTX; dlg3.y = FIRSTY; dlg3.catched = 1; return TRUE; }
	else if ((cx3 == cx5) && (cy3 == cy5) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg0.x = FIRSTX; dlg0.y = FIRSTY; dlg0.catched = 1; return TRUE; }
	else if ((cx3 == cx6) && (cy3 == cy6) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg1.x = FIRSTX; dlg1.y = FIRSTY; dlg1.catched = 1; return TRUE; }
	else if ((cx3 == cx7) && (cy3 == cy7) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg2.x = FIRSTX; dlg2.y = FIRSTY; dlg2.catched = 1; return TRUE; }
	else if ((cx3 == cx8) && (cy3 == cy8) && (cx3 != FIRSTX) && (cy3 != FIRSTY) && (cx3 != -1) && (cy3 != -1)) { dlg3.x = FIRSTX; dlg3.y = FIRSTY; dlg3.catched = 1; return TRUE; }
	else if ((cx4 == cx5) && (cy4 == cy5) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg0.x = FIRSTX; dlg0.y = FIRSTY; dlg0.catched = 1; return TRUE; }
	else if ((cx4 == cx6) && (cy4 == cy6) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg1.x = FIRSTX; dlg1.y = FIRSTY; dlg1.catched = 1; return TRUE; }
	else if ((cx4 == cx7) && (cy4 == cy7) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg2.x = FIRSTX; dlg2.y = FIRSTY; dlg2.catched = 1; return TRUE; }
	else if ((cx4 == cx8) && (cy4 == cy8) && (cx4 != FIRSTX) && (cy4 != FIRSTY) && (cx4 != -1) && (cy4 != -1)) { dlg3.x = FIRSTX; dlg3.y = FIRSTY; dlg3.catched = 1; return TRUE; }
	return FALSE;
}

/*[3.옮기기,4] 말이 4개 다 들어갔는지 확인해주는 함수*/
void CYutnoriBoard::victoryCheck(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy)
{
	if (ax == -1 && ay == -1) { if (bx == -1 && by == -1) { if (cx == -1 && cy == -1) { if (dx == -1 && dy == -1) {
		CString victory;
		victory.Format(_T("[Game]%s: Win\r\n"), m_nickname);
		m_pClientSocket->Send(victory, victory.GetLength());
	} } } }
}

/*[5] 나가기버튼입니다.*/
void CYutnoriBoard::OnBnClickedCancel()
{
	if (IsGameStart == 1) {
		CString lose;
		lose.Format(_T("[Game]%s: Lose\r\n"), m_nickname);
		m_pClientSocket->Send(lose, lose.GetLength());
		m_pClientSocket->ShutDown();
		m_pClientSocket->Close();
		delete m_pClientSocket;
	}
	CDialogEx::OnCancel();
}

/*[6] 게임종료 후 이전게임정보를 초기화하는 함수*/
void CYutnoriBoard::InitPiece()
{
	Invalidate();
	IsThrow = 0;
	YutNum = 0;
	IsGamebutton = 0;
	IsTurn = 0;
	IsGameStart = 0;
	SetDlgItemText(IDC_BUTTON1, _T("게임준비"));
	SetDlgItemText(IDC_STATIC2, _T("안녕하세요. 윷놀이 게임입니다."));
	dlg0.x = FIRSTX; dlg0.y = FIRSTY; dlg0.catched = 0; dlg0.count = 0;
	dlg1.x = FIRSTX; dlg1.y = FIRSTY; dlg1.catched = 0; dlg1.count = 0;
	dlg2.x = FIRSTX; dlg2.y = FIRSTY; dlg2.catched = 0; dlg2.count = 0;
	dlg3.x = FIRSTX; dlg3.y = FIRSTY; dlg3.catched = 0; dlg3.count = 0;
	dlg4.x = FIRSTX; dlg4.y = FIRSTY; dlg4.catched = 0; dlg4.count = 0;
	dlg5.x = FIRSTX; dlg5.y = FIRSTY; dlg5.catched = 0; dlg5.count = 0;
	dlg6.x = FIRSTX; dlg6.y = FIRSTY; dlg6.catched = 0; dlg6.count = 0;
	dlg7.x = FIRSTX; dlg7.y = FIRSTY; dlg7.catched = 0; dlg7.count = 0;
	AfxMessageBox("게임이 끝났습니다.");
	PaintPiece(dlg0.x, dlg1.x, dlg2.x, dlg3.x, dlg4.x, dlg5.x, dlg6.x, dlg7.x, dlg0.y, dlg1.y, dlg2.y, dlg3.y, dlg4.y, dlg5.y, dlg6.y, dlg7.y);
}