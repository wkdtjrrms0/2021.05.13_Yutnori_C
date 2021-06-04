// CYutnoriBoard.cpp: 구현 파일//

#include "pch.h"
#include "framework.h"
#include "Yutnori_C.h"
#include "CYutnoriBoard.h"
#include "afxdialogex.h"
#include "CNaverOAuthDlg.h"
#include <atlimage.h>
#include "CPiece.h"

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

/*서버와 연결하는 함수입니다.*/
void CYutnoriBoard::ConnectChat()
{
	// TODO: 여기에 구현 코드 추가.
	m_pClientSocket = new CClientSocket;
	m_pClientSocket->Create();
	this->UpdateData(TRUE);
	CString str; str.Format(_T("[나] 서버(%s:%d)와 연결합니다....\r\n"), m_strAddress, m_nPort);
	m_ctrlEdit.ReplaceSel(str);
	bool success = false;
	success = m_pClientSocket->Connect(m_strAddress, m_nPort);
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
	if (SelectOAuth == 1) {
		this->UpdateData(TRUE);
		m_strMessage.Append(_T("\r\n"));
		m_pClientSocket->Send(m_nickname + ": " + m_strMessage, m_nickname.GetLength() + 2 +m_strMessage.GetLength());
		m_strMessage = _T("");
		this->UpdateData(FALSE);
	}
	else {AfxMessageBox("로그인을 해주시기 바랍니다.");}
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
			AfxMessageBox("게임이용중에는 로그아웃 할 수 없습니다. 기권 후 눌러주세요.");
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

/*옮기기버튼입니다.*/

CPiece dlg0, dlg1, dlg2, dlg3;

void CYutnoriBoard::OnBnClickedButton2()
{
	if (IsGamebutton == 1) {
		if (IsThrow == 1) { //윷던지기 눌렀을때 실행
			Invalidate();
			UpdateData(true);
			int x1, x2, x3, x4;
			int y1, y2, y3, y4;

			switch (m_Radio){
			
			case 0: dlg0.Move(YutNum); 
				x1 = dlg0.x; y1 = dlg0.y;
				AfxMessageBox("말을 옮겼습니다.");
				IsThrow = 0; break;
			case 1: dlg1.Move(YutNum); 
				x2 = dlg1.x; y2 = dlg1.y;
				AfxMessageBox("말을 옮겼습니다.");
				IsThrow = 0; break;
			case 2: dlg2.Move(YutNum);
				x3 = dlg2.x; y3 = dlg2.y;
				AfxMessageBox("말을 옮겼습니다.");
				IsThrow = 0; break;
			case 3: dlg3.Move(YutNum);		
				x4 = dlg3.x; y4 = dlg3.y;
				AfxMessageBox("말을 옮겼습니다.");
				IsThrow = 0; break;
			
			default: AfxMessageBox("옮길 말을 선택해주세요."); break;}

			x1 = dlg0.x; y1 = dlg0.y;
			x2 = dlg1.x; y2 = dlg1.y;
			x3 = dlg2.x; y3 = dlg2.y;
			x4 = dlg3.x; y4 = dlg3.y;

			CClientDC dc(this);
			CBrush blue(RGB(0, 100, 255));
			CBrush* old_blue = dc.SelectObject(&blue);
			dc.Ellipse(x1 - 15, y1 - 15, x1 + 15, y1 + 15);
			dc.Ellipse(x2 - 15, y2 - 15, x2 + 15, y2 + 15);
			dc.Ellipse(x3 - 15, y3 - 15, x3 + 15, y3 + 15);
			dc.Ellipse(x4 - 15, y4 - 15, x4 + 15, y4 + 15);
			dc.SelectObject(old_blue);

			if (x1 == -1 && y1 == -1) { if (x2 == -1 && y2 == -1) { if (x3 == -1 && y3 == -1) {if (x4 == -1 && y4 == -1) { goalm = 4; }}}} //4개 다 들어갔을시
			

		} else {
			AfxMessageBox("윷을 던지고 옮겨주세요.");
		}
	} else { 
		AfxMessageBox("로그인 후 게임준비버튼을 눌러주시기 바랍니다.");
	 }
}

/*던지기버튼입니다.*/
void CYutnoriBoard::OnBnClickedButton3()
{
	if (IsGamebutton == 1) {
		if (IsThrow >= 2) { AfxMessageBox("옮긴 후 던져주세요."); IsThrow--; }
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
