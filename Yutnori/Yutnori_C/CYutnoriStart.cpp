// CYutnoriStart.cpp: 구현 파일
//

#include "pch.h"
#include "Yutnori_C.h"
#include "CYutnoriStart.h"
#include "afxdialogex.h"
#include "CNaverOAuthDlg.h"


// CYutnoriStart 대화 상자

IMPLEMENT_DYNAMIC(CYutnoriStart, CDialogEx)

CYutnoriStart::CYutnoriStart(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YUTNORI_START, pParent)
{

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
END_MESSAGE_MAP()


// CYutnoriStart 메시지 처리기


void CYutnoriStart::OnClickedButton1()
{
	CNaverOAuthDlg dlg;
	dlg.DoModal();
	// 로그인 및 시작
}


void CYutnoriStart::OnClickedButton2()
{
	AfxMessageBox("준비중입니다.");
	// 랭킹 조회
}
