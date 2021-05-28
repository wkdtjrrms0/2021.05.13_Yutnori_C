// CBoardDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Yutnori_C.h"
#include "CBoardDlg.h"
#include "afxdialogex.h"
#include "../test2/resource.h"


// CBoardDlg 대화 상자

IMPLEMENT_DYNAMIC(CBoardDlg, CDialogEx)

CBoardDlg::CBoardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nRand1 = 0;
	m_nRand2 = 0;
	m_nRand3 = 0;
	m_nRand4 = 0;
	m_nMove = 0;
}

CBoardDlg::~CBoardDlg()
{
}

void CBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBoardDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBoardDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CBoardDlg 메시지 처리기


void CBoardDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClientDC dc(this);
	CDC* pDC = GetDC();
	int num1 = rand() % 100;		// 윷을 던질 때 쓸 난수입니다.
	int num2 = rand() % 100;
	int num3 = rand() % 100;
	int num4 = rand() % 100;



	m_nRand1 = num1;
	m_nRand2 = num2;
	m_nRand3 = num3;
	m_nRand4 = num4;
	

	CString str; str.Format(_T("난수 1: %d, 난수 2: %d, 난수 3: %d, 난수 4: %d"), m_nRand1, m_nRand2, m_nRand3, m_nRand4);
	dc.TextOut(620, 250, str);

	if (m_nRand1 % 2 == 0)				//난수에 따른 윷 단면 표현
	{
		dc.Rectangle(620, 270, 640, 320);
		
	}
	else
	{
		CBrush brush(RGB(0, 0, 0));
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
		dc.Rectangle(620, 270, 640, 320);
		dc.SelectObject(pOldBrush);
		brush.DeleteObject();
	}
	if (m_nRand2 % 2 == 0)
	{
		dc.Rectangle(650, 270, 670, 320);
	}
	else
	{
		CBrush brush(RGB(0, 0, 0));
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
		dc.Rectangle(650, 270, 670, 320);
		dc.SelectObject(pOldBrush);
		brush.DeleteObject();
	}
	if (m_nRand3 % 2 == 0)
	{
		dc.Rectangle(680, 270, 700, 320);
	}
	else
	{
		CBrush brush(RGB(0, 0, 0));
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
		dc.Rectangle(680, 270, 700, 320);
		dc.SelectObject(pOldBrush);
		brush.DeleteObject();
	}
	if (m_nRand4 % 2 == 0)
	{
		dc.Rectangle(710, 270, 730, 320);
	}
	else
	{
		CBrush brush(RGB(0, 0, 0));
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
		dc.Rectangle(710, 270, 730, 320);
		dc.SelectObject(pOldBrush);
		brush.DeleteObject();
	}


	if (m_nRand1 % 2 + m_nRand2 % 2 + m_nRand3 % 2 + m_nRand4 % 2 == 1)
	{
		CBitmap bmpBack3; bmpBack3.LoadBitmapA(IDB_BITMAP1);	//도
		BITMAP bitmap3; bmpBack3.GetBitmap(&bitmap3);
		int nWidth = bitmap3.bmWidth;
		int nHeight = bitmap3.bmHeight;
		CDC memDC3; memDC3.CreateCompatibleDC(pDC);
		memDC3.SelectObject(&bmpBack3);
		pDC->BitBlt(500, 10, nWidth, nHeight, &memDC3, 0, 0, SRCCOPY);
		memDC3.DeleteDC();
		bmpBack3.DeleteObject();
	}
	else if (m_nRand1 % 2 + m_nRand2 % 2 + m_nRand3 % 2 + m_nRand4 % 2 == 2)
	{
		CBitmap bmpBack4; bmpBack4.LoadBitmapA(IDB_BITMAP1);		//개
		BITMAP bitmap4; bmpBack4.GetBitmap(&bitmap4);
		int nWidth = bitmap4.bmWidth;
		int nHeight = bitmap4.bmHeight;
		CDC memDC4; memDC4.CreateCompatibleDC(pDC);
		memDC4.SelectObject(&bmpBack4);
		pDC->BitBlt(500, 10, nWidth, nHeight, &memDC4, 0, 0, SRCCOPY);
		memDC4.DeleteDC();
		bmpBack4.DeleteObject();
	}
	else if (m_nRand1 % 2 + m_nRand2 % 2 + m_nRand3 % 2 + m_nRand4 % 2 == 3)
	{
		CBitmap bmpBack5; bmpBack5.LoadBitmapA(IDB_BITMAP1);		//걸
		BITMAP bitmap5; bmpBack5.GetBitmap(&bitmap5);
		int nWidth = bitmap5.bmWidth;
		int nHeight = bitmap5.bmHeight;
		CDC memDC5; memDC5.CreateCompatibleDC(pDC);
		memDC5.SelectObject(&bmpBack5);
		pDC->BitBlt(500, 10, nWidth, nHeight, &memDC5, 0, 0, SRCCOPY);
		memDC5.DeleteDC();
		bmpBack5.DeleteObject();
	}
	else if (m_nRand1 % 2 + m_nRand2 % 2 + m_nRand3 % 2 + m_nRand4 % 2 == 0)
	{
		CBitmap bmpBack6; bmpBack6.LoadBitmapA(IDB_BITMAP1);		//윷
		BITMAP bitmap6; bmpBack6.GetBitmap(&bitmap6);
		int nWidth = bitmap6.bmWidth;
		int nHeight = bitmap6.bmHeight;
		CDC memDC6; memDC6.CreateCompatibleDC(pDC);
		memDC6.SelectObject(&bmpBack6);
		pDC->BitBlt(500, 10, nWidth, nHeight, &memDC6, 0, 0, SRCCOPY);
		memDC6.DeleteDC();
		bmpBack6.DeleteObject();
	}
	else if (m_nRand1 % 2 + m_nRand2 % 2 + m_nRand3 % 2 + m_nRand4 % 2 == 4)
	{
		CBitmap bmpBack7; bmpBack7.LoadBitmapA(IDB_BITMAP1);		//모
		BITMAP bitmap7; bmpBack7.GetBitmap(&bitmap7);
		int nWidth = bitmap7.bmWidth;
		int nHeight = bitmap7.bmHeight;
		CDC memDC7; memDC7.CreateCompatibleDC(pDC);
		memDC7.SelectObject(&bmpBack7);
		pDC->BitBlt(500, 10, nWidth, nHeight, &memDC7, 0, 0, SRCCOPY);
		memDC7.DeleteDC();
		bmpBack7.DeleteObject();
	}


}


BOOL CBoardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	srand((unsigned)time(NULL));	//난수 시드 설정

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBoardDlg::OnPaint()
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
		int nWidth = bitmap.bmWidth;							//경기장 크기 700*408	
		int nHeight = bitmap.bmHeight;
		CDC memDC; memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmpBack);
		pDC->BitBlt(10, 10, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
		memDC.DeleteDC();
		bmpBack.DeleteObject();
	}
}


HCURSOR CBoardDlg::OnQueryDragIcon()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return static_cast<HCURSOR>(m_hIcon);
}
