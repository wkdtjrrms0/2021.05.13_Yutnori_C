// CYutnoriStart.cpp: 구현 파일
//

#include "pch.h"
#include "Yutnori_C.h"
#include "CYutnoriStart.h"
#include "afxdialogex.h"
#include "CNaverOAuthDlg.h"
#include <atlimage.h>
#include "CBoardDlg.h"
#include "CClientSocket.h"
#include "Yutnori_CDlg.h"
#include "CYutnoriBoard.h"

// CYutnoriStart 대화 상자

IMPLEMENT_DYNAMIC(CYutnoriStart, CDialogEx)

CYutnoriStart::CYutnoriStart(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YUTNORI_START, pParent)
{

	SelectOAuth = 0;
}

CYutnoriStart::~CYutnoriStart()
{
}

void CYutnoriStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CYutnoriStart, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CYutnoriStart::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CYutnoriStart::OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CYutnoriStart::OnBnClickedButton3)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CYutnoriStart 메시지 처리기

CNaverOAuthDlg dlgtt;
void CYutnoriStart::OnClickedButton1()
{
	if (dlgtt.IsLogin == 0 || dlgtt.IsLogin == -1) { //처음(0)이나 로그아웃(-1)했을때 실행
		dlgtt.DoModal();
		if (dlgtt.IsLogin == 1) { //로그인 성공시
			SetDlgItemText(IDC_BUTTON1, _T("로그아웃"));
			SetDlgItemText(IDC_STATIC, _T(dlgtt.m_NaverNickName + "님 \n 반갑습니다."));
			SelectOAuth = 1;
		}
	} // 로그인
	else if(dlgtt.IsLogin == 1){ //로그인 되어있을 때 실행 
		dlgtt.DoModal();
		SetDlgItemText(IDC_BUTTON1, _T("로그인"));
		SetDlgItemText(IDC_STATIC, _T("게임이용을 위해 로그인을 해주시기 바랍니다."));
		SelectOAuth = -1;
	} // 로그아웃
}


void CYutnoriStart::OnClickedButton2()
{
	if (SelectOAuth == 1) {
		AfxMessageBox("준비중입니다.");
	}
	else {
		AfxMessageBox("로그인을 해주시기 바랍니다.");
	}
	// 랭킹 조회
}


void CYutnoriStart::OnBnClickedButton3()
{
	if (SelectOAuth == 1) {
		CYutnoriBoard dlggame;
		dlggame.DoModal();
	}
	else {
		CYutnoriCDlg dlggame; // 테스트 종료시 비활성화
		//dlggame.m_nickname = dlg.m_NaverNickName; //테스트 종료시 상위조건문으로 옮김
		dlggame.DoModal(); // 테스트 종료시 비활성화
		//AfxMessageBox("로그인을 해주시기 바랍니다.");  테스트 종료시 활성화
	}
	// 게임 시작
}


void CYutnoriStart::OnPaint()
{
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
		CBitmap bmpBack; bmpBack.LoadBitmapA(IDB_BITMAP1);
		BITMAP bitmap; bmpBack.GetBitmap(&bitmap);
		int nWidth = bitmap.bmWidth;
		int nHeight = bitmap.bmHeight;
		CDC memDC; memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmpBack);
		pDC->BitBlt(10, 10, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
		memDC.DeleteDC();
		bmpBack.DeleteObject();
		CDialog::OnPaint();
	}
}
