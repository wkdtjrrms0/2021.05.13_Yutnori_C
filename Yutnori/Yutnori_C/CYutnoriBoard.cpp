// CYutnoriBoard.cpp: 구현 파일
//

#include "pch.h"
#include "Yutnori_C.h"
#include "CYutnoriBoard.h"
#include "afxdialogex.h"


// CYutnoriBoard 대화 상자

IMPLEMENT_DYNAMIC(CYutnoriBoard, CDialogEx)

CYutnoriBoard::CYutnoriBoard(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YUTNORI_Board, pParent)
{

}

CYutnoriBoard::~CYutnoriBoard()
{
}

void CYutnoriBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CYutnoriBoard, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CYutnoriBoard::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO2, &CYutnoriBoard::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CYutnoriBoard 메시지 처리기


void CYutnoriBoard::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


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


void CYutnoriBoard::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
