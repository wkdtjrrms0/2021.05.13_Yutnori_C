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
	goalm = 0;
	goaly = 0;
	IsGamebutton = 0;
	IsTurn = 0;
	x1 = FIRSTX; x2 = FIRSTX; x3 = FIRSTX; x4 = FIRSTX; x5 = FIRSTX; x6 = FIRSTX; x7 = FIRSTX; x8 = FIRSTX;
	y1 = FIRSTY; y2 = FIRSTY; y3 = FIRSTY; y4 = FIRSTY; y5 = FIRSTY; y6 = FIRSTY; y7 = FIRSTY; y8 = FIRSTY;
}

/*소멸자*/
CYutnoriBoard::~CYutnoriBoard(){}

void CYutnoriBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strMessage);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
	DDX_Control(pDX, IDC_EDIT5, m_ctrlEditN);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
}


BEGIN_MESSAGE_MAP(CYutnoriBoard, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON4, &CYutnoriBoard::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CYutnoriBoard::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CYutnoriBoard::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CYutnoriBoard::OnBnClickedButton3)
	ON_BN_CLICKED(IDCANCEL, &CYutnoriBoard::OnBnClickedCancel)
END_MESSAGE_MAP()

CNaverOAuthDlg dlg;

/*윷판을 그립니다.*/
void CYutnoriBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.


	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}

	else
	{
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
void CYutnoriBoard::ConnectChat()
{
	m_pClientSocket = new CClientSocket;
	m_pClientSocket->Create(); //(2)소켓만듬
	this->UpdateData(TRUE);
	CString str; str.Format(_T("[Status] 서버와 연결합니다....\r\n"));
	m_ctrlEdit.ReplaceSel(str);
	bool success = false;
	success = m_pClientSocket->Connect(m_strAddress, m_nPort);  //(3)연결시도
	if (!success) m_ctrlEdit.ReplaceSel(_T("[error] 서버와 연결하지 못했습니다.\r\n"));
}


BOOL CYutnoriBoard::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

/*연결이 끝나면 소켓을 끊습니다.*/
BOOL CYutnoriBoard::DestroyWindow()
{
	m_pClientSocket->ShutDown();
	m_pClientSocket->Close();
	delete m_pClientSocket;
	return CDialogEx::DestroyWindow();
}

/*채팅전송버튼입니다.*/
void CYutnoriBoard::OnOK()
{
	if ((SelectOAuth == 1) && (IsGamebutton == 1)) {
		this->UpdateData(TRUE);
		m_strMessage.Append(_T("\r\n"));
		m_pClientSocket->Send("[Chat]" + m_nickname + ": " + m_strMessage, m_nickname.GetLength() + 8 +m_strMessage.GetLength());  //(4)데이터 보냄.
		m_strMessage = _T("");
		this->UpdateData(FALSE);
	}
	else {AfxMessageBox("로그인과 게임준비를 해주시기 바랍니다.");}
}

/*로그인버튼입니다, 로그인 성공시 소켓연결 함수 실행*/
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
	} // 로그인
	else if (dlg.IsLogin == 1) { //로그인 되어있을 때 실행
		if (IsGamebutton == 0) {
			dlg.DoModal();
			SelectOAuth = -1;
			SetDlgItemText(IDC_BUTTON4, _T("로그인"));
			SetDlgItemText(IDC_STATIC, _T("게임이용을 위해 로그인을 해주시기 바랍니다."));
		}
		else if (goalm == 4 || goaly == 4) {
			dlg.DoModal();
			SelectOAuth = -1;
			m_nickname = _T("");
			m_ctrlEditN.SetSel(0, -1);
			m_ctrlEditN.Clear();
			m_ctrlEditN.ReplaceSel(m_nickname);
			SetDlgItemText(IDC_BUTTON4, _T("로그인"));
			SetDlgItemText(IDC_STATIC, _T("게임이용을 위해 로그인을 해주시기 바랍니다."));
		}
		else {
			m_ctrlEdit.ReplaceSel(_T("게임이용중에는 로그아웃 할 수 없습니다. 기권 후 눌러주세요.\r\n"));
		}
	} // 로그아웃
}

/*게임준비버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton1()
{
	if (SelectOAuth == 1) {
		if (IsGamebutton != 1) {
			ConnectChat();
			SetDlgItemText(IDC_BUTTON1, _T("기권하기"));
			IsGamebutton = 1;
		}
		else{
			goaly = 4; 			
			m_ctrlEdit.ReplaceSel(_T("연결 종료\r\n"));
			m_pClientSocket->ShutDown();
			m_pClientSocket->Close();
			delete m_pClientSocket;
			SetDlgItemText(IDC_BUTTON1, _T("게임준비"));
			IsGamebutton = 0;
		}
	}
	else { AfxMessageBox("로그인을 해주시기 바랍니다."); }
}

CPiece dlg0, dlg1, dlg2, dlg3; //내 말
CPiece dlg4, dlg5, dlg6, dlg7; //상대방 말
/*옮기기버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton2()
{
	if (IsGamebutton == 1) {
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
					AfxMessageBox("말을 옮겼습니다."); //그림출력 테스트용
				};

				x1 = dlg0.x; y1 = dlg0.y;
				x2 = dlg1.x; y2 = dlg1.y;
				x3 = dlg2.x; y3 = dlg2.y;
				x4 = dlg3.x; y4 = dlg3.y;
				if (Catch(x1, x2, x3, x4, x5, x6, x7, x8, y1, y2, y3, y4, y5, y6, y7, y8) == TRUE) { //내가 상대말을 잡는경우
					m_ctrlEdit.ReplaceSel(_T("축하합니다. 상대말을 잡았습니다.\r\n"));
					dlg0.x = x1; dlg0.y = y1;
					dlg1.x = x2; dlg1.y = y2;
					dlg2.x = x3; dlg2.y = y3;
					dlg3.x = x4; dlg3.y = y4;
					dlg4.x = x5; dlg4.y = y5;
					dlg5.x = x6; dlg5.y = y6;
					dlg6.x = x7; dlg6.y = y7;
					dlg7.x = x8; dlg7.y = y8;
				}
				PaintPiece(x1, x2, x3, x4, x5, x6, x7, x8, y1, y2, y3, y4, y5, y6, y7, y8);


				if (x1 == -1 && y1 == -1) { if (x2 == -1 && y2 == -1) { if (x3 == -1 && y3 == -1) { if (x4 == -1 && y4 == -1) { goalm = 4; } } } } //4개 다 들어갔을시


			}
			else {
				m_ctrlEdit.ReplaceSel(_T("윷을 던지고 옮겨주세요.\r\n"));
			}
		}
		else { m_ctrlEdit.ReplaceSel(_T("상대방이 던질때까지 기다려주세요.\r\n")); }
	} else { 
		AfxMessageBox("로그인 후 게임준비버튼을 눌러주시기 바랍니다.");
	 }
}

/*상대방말 움직이는 함수*/
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

	if ((m_opRadio == 0) || (m_opRadio == 1) || (m_opRadio == 2) || (m_opRadio == 3)) { //말이 선택되었을때 실질적인 윷정보를 보냄
		m_ctrlEdit.ReplaceSel(_T("상대방이 말을 옮겼습니다.\r\n"));


		AfxMessageBox("상대방이 말을 옮겼습니다."); //그림출력 테스트용
	};

	x5 = dlg4.x; y5 = dlg4.y;
	x6 = dlg5.x; y6 = dlg5.y;
	x7 = dlg6.x; y7 = dlg6.y;
	x8 = dlg7.x; y8 = dlg7.y;
	if (CatchMe(x5, x6, x7, x8, x1, x2, x3, x4, y5, y6, y7, y8, y1, y2, y3, y4) == TRUE) { //상대가 내 말을 잡는경우
		m_ctrlEdit.ReplaceSel(_T("아쉽내요. 상대말에게 잡혔습니다.\r\n"));
		dlg0.x = x1; dlg0.y = y1;
		dlg1.x = x2; dlg1.y = y2;
		dlg2.x = x3; dlg2.y = y3;
		dlg3.x = x4; dlg3.y = y4;
		dlg4.x = x5; dlg4.y = y5;
		dlg5.x = x6; dlg5.y = y6;
		dlg6.x = x7; dlg6.y = y7;
		dlg7.x = x8; dlg7.y = y8;

	}
	PaintPiece(x1, x2, x3, x4, x5, x6, x7, x8, y1, y2, y3, y4, y5, y6, y7, y8);

	if (x5 == -1 && y5 == -1) { if (x6 == -1 && y6 == -1) { if (x7 == -1 && y7 == -1) { if (x8 == -1 && y8 == -1) { goalm = 4; } } } } //4개 다 들어갔을시


}













/*던지기버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton3()
{
	if (IsGamebutton == 1) {
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
				IsThrow++;
			}
		}
		else { m_ctrlEdit.ReplaceSel(_T("상대방이 던질때까지 기다려주세요.\r\n")); }
	}
	else { AfxMessageBox("로그인 후 게임준비버튼을 눌러주시기 바랍니다."); }
}

void CYutnoriBoard::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pClientSocket->ShutDown();
	m_pClientSocket->Close();
	delete m_pClientSocket;
	CDialogEx::OnCancel();
}



/*좌표 색칠하는 함수*/
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
	dc.MoveTo(x1,y1-6); dc.LineTo(x1,y1+6); //1번말 표기
	dc.MoveTo(x2 - 4, y2 - 6); dc.LineTo(x2 - 4, y2 + 6); dc.MoveTo(x2 + 4, y2 - 6); dc.LineTo(x2 + 4, y2 + 6);//2번말 표기
	dc.MoveTo(x3, y3 - 6); dc.LineTo(x3, y3 + 6); dc.MoveTo(x3 - 5, y3 - 6); dc.LineTo(x3 - 5, y3 + 6); dc.MoveTo(x3 + 5, y3 - 6); dc.LineTo(x3 + 5, y3 + 6); //3번말 표기
	dc.MoveTo(x4-6, y4 - 6); dc.LineTo(x4-6, y4 + 6); dc.MoveTo(x4 - 2, y4 - 6); dc.LineTo(x4 - 2, y4 + 6); //4번말 표기
	dc.MoveTo(x4 + 2, y4 - 6); dc.LineTo(x4 + 2, y4 + 6); dc.MoveTo(x4 + 6, y4 - 6); dc.LineTo(x4 + 6, y4 + 6); //4번말 표기
	dc.MoveTo(x5, y5 - 6); dc.LineTo(x5, y5 + 6); //상대방 1번말 표기
	dc.MoveTo(x6 - 4, y6 - 6); dc.LineTo(x6 - 4, y6 + 6); dc.MoveTo(x6 + 4, y6 - 6); dc.LineTo(x6 + 4, y6 + 6);//상대방 2번말 표기
	dc.MoveTo(x7, y7 - 6); dc.LineTo(x7, y7 + 6); dc.MoveTo(x7 - 5, y7 - 6); dc.LineTo(x7 - 5, y7 + 6); dc.MoveTo(x7 + 5, y7 - 6); dc.LineTo(x7 + 5, y7 + 6); //상대방 3번말 표기
	dc.MoveTo(x8 - 6, y8 - 6); dc.LineTo(x8 - 6, y8 + 6); dc.MoveTo(x8 - 2, y8 - 6); dc.LineTo(x8 - 2, y8 + 6); //상대방 4번말 표기
	dc.MoveTo(x8 + 2, y8 - 6); dc.LineTo(x8 + 2, y8 + 6); dc.MoveTo(x8 + 6, y8 - 6); dc.LineTo(x8 + 6, y8 + 6); //상대방 4번말 표기
	dc.SelectObject(oldpen);
}

/*상대방 말을 잡는 함수*/
BOOL CYutnoriBoard::Catch(int cx1, int cx2, int cx3, int cx4, int cx5, int cx6, int cx7, int cx8, int cy1, int cy2, int cy3, int cy4, int cy5, int cy6, int cy7, int cy8)
{
	if ((cx1 == cx5) && (cy1 == cy5) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x5 = FIRSTX; y5 = FIRSTY; return TRUE;}
	else if ((cx1 == cx6) && (cy1 == cy6) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x6 = FIRSTX; y6 = FIRSTY; return TRUE;}
	else if ((cx1 == cx7) && (cy1 == cy7) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x7 = FIRSTX; y7 = FIRSTY; return TRUE;}
	else if ((cx1 == cx8) && (cy1 == cy8) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x8 = FIRSTX; y8 = FIRSTY; return TRUE;}
	else if ((cx2 == cx5) && (cy2 == cy5) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x5 = FIRSTX; y5 = FIRSTY; return TRUE;}
	else if ((cx2 == cx6) && (cy2 == cy6) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x6 = FIRSTX; y6 = FIRSTY; return TRUE;}
	else if ((cx2 == cx7) && (cy2 == cy7) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x7 = FIRSTX; y7 = FIRSTY; return TRUE;}
	else if ((cx2 == cx8) && (cy2 == cy8) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x8 = FIRSTX; y8 = FIRSTY; return TRUE;}
	else if ((cx3 == cx5) && (cy3 == cy5) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x5 = FIRSTX; y5 = FIRSTY; return TRUE;}
	else if ((cx3 == cx6) && (cy3 == cy6) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x6 = FIRSTX; y6 = FIRSTY; return TRUE;}
	else if ((cx3 == cx7) && (cy3 == cy7) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x7 = FIRSTX; y7 = FIRSTY; return TRUE;}
	else if ((cx3 == cx8) && (cy3 == cy8) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x8 = FIRSTX; y8 = FIRSTY; return TRUE;}
	else if ((cx4 == cx5) && (cy4 == cy5) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x5 = FIRSTX; y5 = FIRSTY; return TRUE;}
	else if ((cx4 == cx6) && (cy4 == cy6) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x6 = FIRSTX; y6 = FIRSTY; return TRUE;}
	else if ((cx4 == cx7) && (cy4 == cy7) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x7 = FIRSTX; y7 = FIRSTY; return TRUE;}
	else if ((cx4 == cx8) && (cy4 == cy8) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x8 = FIRSTX; y8 = FIRSTY; return TRUE;}
	return FALSE;
}

/*내 말이 잡히는 함수*/
BOOL CYutnoriBoard::CatchMe(int cx1, int cx2, int cx3, int cx4, int cx5, int cx6, int cx7, int cx8, int cy1, int cy2, int cy3, int cy4, int cy5, int cy6, int cy7, int cy8)
{
	if ((cx1 == cx5) && (cy1 == cy5) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x1 = FIRSTX; y1 = FIRSTY; return TRUE; }
	else if ((cx1 == cx6) && (cy1 == cy6) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x2 = FIRSTX; y2 = FIRSTY; return TRUE; }
	else if ((cx1 == cx7) && (cy1 == cy7) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x3 = FIRSTX; y3 = FIRSTY; return TRUE; }
	else if ((cx1 == cx8) && (cy1 == cy8) && (cx1 != FIRSTX) && (cy1 != FIRSTY)) { x4 = FIRSTX; y4 = FIRSTY; return TRUE; }
	else if ((cx2 == cx5) && (cy2 == cy5) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x1 = FIRSTX; y1 = FIRSTY; return TRUE; }
	else if ((cx2 == cx6) && (cy2 == cy6) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x2 = FIRSTX; y2 = FIRSTY; return TRUE; }
	else if ((cx2 == cx7) && (cy2 == cy7) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x3 = FIRSTX; y3 = FIRSTY; return TRUE; }
	else if ((cx2 == cx8) && (cy2 == cy8) && (cx2 != FIRSTX) && (cy2 != FIRSTY)) { x4 = FIRSTX; y4 = FIRSTY; return TRUE; }
	else if ((cx3 == cx5) && (cy3 == cy5) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x1 = FIRSTX; y1 = FIRSTY; return TRUE; }
	else if ((cx3 == cx6) && (cy3 == cy6) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x2 = FIRSTX; y2 = FIRSTY; return TRUE; }
	else if ((cx3 == cx7) && (cy3 == cy7) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x3 = FIRSTX; y3 = FIRSTY; return TRUE; }
	else if ((cx3 == cx8) && (cy3 == cy8) && (cx3 != FIRSTX) && (cy3 != FIRSTY)) { x4 = FIRSTX; y4 = FIRSTY; return TRUE; }
	else if ((cx4 == cx5) && (cy4 == cy5) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x1 = FIRSTX; y1 = FIRSTY; return TRUE; }
	else if ((cx4 == cx6) && (cy4 == cy6) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x2 = FIRSTX; y2 = FIRSTY; return TRUE; }
	else if ((cx4 == cx7) && (cy4 == cy7) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x3 = FIRSTX; y3 = FIRSTY; return TRUE; }
	else if ((cx4 == cx8) && (cy4 == cy8) && (cx4 != FIRSTX) && (cy4 != FIRSTY)) { x4 = FIRSTX; y4 = FIRSTY; return TRUE; }
	return FALSE;
}
