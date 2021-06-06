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

	this->m_ctrlEdit.ReplaceSel(_T("MySQL 서버와 연결되었습니다.\r\n"));
	m_pListenSocket = new CListenSocket;
	if (m_pListenSocket->StartServer(7000)) {
		m_ctrlEdit.ReplaceSel(_T("Port(7000)이 Listen 소켓으로 열렸습니다.\r\n"));
	}
	else AfxMessageBox(_T("ERROR: Failed to LISTEN.")); // 이미 포트가 열려있다!


	/*(1) 서버와 연결하고, 소켓을 만듬 (4)서버가 요청을 듣는다*/
	/*
	if (mysqlConnect()) {
		this->m_ctrlEdit.ReplaceSel(_T("MySQL 서버와 연결되었습니다.\r\n"));
		m_pListenSocket = new CListenSocket;
		if (m_pListenSocket->StartServer(7000)) {
			m_ctrlEdit.ReplaceSel(_T("Port(7000)이 Listen 소켓으로 열렸습니다.\r\n"));
		}
		else AfxMessageBox(_T("ERROR: Failed to LISTEN.")); // 이미 포트가 열려있다!
	}
	else AfxMessageBox(_T("ERROR: Failed to create a listen socket."));
	*/
	
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

bool CYutnoriSDlg::mysqlConnect()
{
	mysql_init(&m_mysql);
	AfxMessageBox(_T("연결"));
	MYSQL* conn = mysql_real_connect(&m_mysql, "localhost", "root", "qlalf2603!", "db1", 3306, (char*)NULL, 0);
	if (conn == NULL) {	// 만약 연결에 실패하면,
		AfxMessageBox((LPCTSTR)mysql_error(&m_mysql));
		::PostQuitMessage(0); return FALSE;
	}
	
	mysql_set_character_set(&m_mysql, "euckr");	// 연결 문자집합을 지정합니다.
	return TRUE;
}

bool CYutnoriSDlg::mysqlAccess(CString strMessage)
{
	//	CString strMessage(_T("ID=홍길동 PW=2222"));
	if (strMessage.Left(3) != _T("ID=")) return false;
	CString strUsername = strMessage.Mid(3, 6);
	CString strPassword = strMessage.Mid(13, 4);
	if (this->isValidUser(strUsername, strPassword)) {
		CString str; str.Format(_T("ID= User=U\r\n"));
		this->m_ctrlEdit.ReplaceSel(str);
		m_pListenSocket->Broadcast(str);
		return true;
	}
	else {
		CString str; str.Format(_T("error: ID(%s) or PW is incorrect!\r\n"), strUsername);
		this->m_ctrlEdit.ReplaceSel(str);
		m_pListenSocket->Broadcast(str);	// 공사중 = 접속자에게만 전송하는 것으로 수정 요망
	}
	return false;
}

bool CYutnoriSDlg::isValidUser(CString strUsername, CString strPassword)
{
	CString query;
	query.Format(_T("select * from test"));
	int status = mysql_query(&m_mysql, query);	// ID와 PW가 DB와 일치한가?
	MYSQL_RES* result = mysql_store_result(&m_mysql);
	int nRowCount = mysql_num_rows(result);
	return nRowCount;
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
	mysql_close(&m_mysql);
	m_pListenSocket->StopServer();
	return CDialogEx::DestroyWindow();
}