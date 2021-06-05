// Yutnori_SDlg.cpp: 구현 파일//

#include "pch.h"
#include "framework.h"
#include "Yutnori_S.h"
#include "Yutnori_SDlg.h"
#include "afxdialogex.h"
#include "CChildSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.*/
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
// CYutnoriSDlg 대화 상자
CYutnoriSDlg::CYutnoriSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YUTNORI_S_DIALOG, pParent)
	, m_pListenSocket(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CYutnoriSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
}
BEGIN_MESSAGE_MAP(CYutnoriSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

BOOL CYutnoriSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	/*(1) 소켓을 만들고 (4)서버가 요청을 듣는다*/
	m_pListenSocket = new CListenSocket;
	if (m_pListenSocket->Create(7000, SOCK_STREAM)) {
		if (m_pListenSocket->Listen()) { m_ctrlEdit.ReplaceSel(_T("[서버] Port(7000)이 Listen 소켓으로 열렸습니다.\r\n")); }
		else AfxMessageBox(_T("ERROR: Failed to LISTEN.")); //이미 포트가 열려있다.
	}
	else AfxMessageBox(_T("ERROR: Failed to create a listen socket.")); //ex. 메모리가 꽉찬경우
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
void CYutnoriSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
void CYutnoriSDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}
HCURSOR CYutnoriSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CYutnoriSDlg::DestroyWindow()
{
	POSITION pos = m_pListenSocket->m_pChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;
	while (pos != NULL) {
		pChild = (CChildSocket*)(m_pListenSocket->m_pChildSocketList.GetNext(pos));
		if (pChild != NULL) { pChild->ShutDown(); pChild->Close(); delete pChild; }
	}
	m_pListenSocket->ShutDown(); //소켓중지(listen을 멈춤)
	m_pListenSocket->Close(); // 소켓종료
	delete m_pListenSocket; //소켓의 목록을 지움
	return CDialogEx::DestroyWindow();
}