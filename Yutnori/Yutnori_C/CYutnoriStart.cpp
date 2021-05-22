// CYutnoriStart.cpp: 구현 파일
//

#include "pch.h"
#include "Yutnori_C.h"
#include "CYutnoriStart.h"
#include "afxdialogex.h"
#include "CNaverOAuthDlg.h"
#include <atlimage.h>

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
END_MESSAGE_MAP()


// CYutnoriStart 메시지 처리기

CNaverOAuthDlg dlg;
void CYutnoriStart::OnClickedButton1()
{
	if (dlg.IsLogin == 0 || dlg.IsLogin == -1) { //처음(0)이나 로그아웃(-1)했을때 실행
		dlg.DoModal();
		if (dlg.IsLogin == 1) { //로그인 성공시
			SetDlgItemText(IDC_BUTTON1, _T("로그아웃"));
			SelectOAuth = 1;
		}
	} // 로그인
	else if(dlg.IsLogin == 1){ //로그인 되어있을 때 실행 
		dlg.DoModal();
		SetDlgItemText(IDC_BUTTON1, _T("로그인"));

		SelectOAuth = -1;
	} // 로그아웃
}


void CYutnoriStart::OnClickedButton2()
{
	AfxMessageBox("준비중입니다.");
	// 랭킹 조회
}


void CYutnoriStart::OnBnClickedButton3()
{
	AfxMessageBox("준비중입니다.");
	// 게임 시작
}
