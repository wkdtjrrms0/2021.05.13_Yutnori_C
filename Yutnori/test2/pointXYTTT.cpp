// pointXYTTT.cpp: 구현 파일
//

#include "pch.h"
#include "test2.h"
#include "pointXYTTT.h"
#include "afxdialogex.h"


// pointXYTTT 대화 상자

IMPLEMENT_DYNAMIC(pointXYTTT, CDialogEx)

pointXYTTT::pointXYTTT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

	x = 0;
	y = 0;
}

pointXYTTT::~pointXYTTT()
{
}

void pointXYTTT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(pointXYTTT, CDialogEx)
END_MESSAGE_MAP()


// pointXYTTT 메시지 처리기


void pointXYTTT::pointXY1()
{
	x = 50;
	y = 40;
	// TODO: 여기에 구현 코드 추가.
}
