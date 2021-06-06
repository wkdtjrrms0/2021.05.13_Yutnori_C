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

	/*(1) 서버와 연결하고, 소켓을 만듬 (4)서버가 요청을 듣는다*/
	if (mysqlConnect()) {
		this->m_ctrlEdit.ReplaceSel(_T("MySQL 서버와 연결되었습니다.\r\n"));
		m_pListenSocket = new CListenSocket;
		if (m_pListenSocket->StartServer(7000)) {
			m_ctrlEdit.ReplaceSel(_T("Port(7000)이 Listen 소켓으로 열렸습니다.\r\n"));
		}
		else AfxMessageBox(_T("ERROR: Failed to LISTEN.")); // 이미 포트가 열려있다!
	}
	else AfxMessageBox(_T("ERROR: Failed to create a listen socket."));
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

/*DB연결*/
bool CYutnoriSDlg::mysqlConnect()
{
	mysql_init(&m_mysql);
	MYSQL* conn = mysql_real_connect(&m_mysql, "localhost", "root", "pw01", "db1", 3306, (char*)NULL, 0);
	if (conn == NULL) {	// 만약 연결에 실패하면,
		AfxMessageBox((LPCTSTR)mysql_error(&m_mysql));
		::PostQuitMessage(0); return FALSE;
	}
	mysql_set_character_set(&m_mysql, "euckr");	// 연결 문자집합을 지정합니다.
	return TRUE;
}

/*DB에 값넣음*/
void CYutnoriSDlg::InputData(CString str)
{
	CString NaverID = str.Mid(str.Find("NaverID: ") + 9, str.Find(", NaverNick") - str.Find("NaverID: ") - 9); //ID만 추출
	CString NaverNick = str.Mid(str.Find("NaverNick: ") + 11, str.GetLength() - str.Find("NaverNick: ") - 13); //닉네임만 추출
	CString query;
	query.Format(_T("select * from member where NaverID='%s'"), NaverID); //ID가 있는건지 찾아봄
	mysql_query(&m_mysql, query);
	int status = mysql_query(&m_mysql, query);
	MYSQL_RES* result = mysql_store_result(&m_mysql);
	int nRowCount = mysql_num_rows(result);
	if (nRowCount == 0) { //ID가 없으면
		query.Format(_T("insert into member (NaverID, NaverNick) values ('%s', '%s')"), NaverID, NaverNick);
		mysql_query(&m_mysql, query);	// ID, 닉네임넣음
	}
	query.Format(_T("update member set Score = Score + 1 where NaverID='%s'"), NaverID);
	mysql_query(&m_mysql, query);	// ID가 있으면 값 증가시킴
}

/*DB에서 값 가져옴*/
void CYutnoriSDlg::ReadData()
{
	CString query; CString str; str = _T("[---TOP 10 랭킹조회---]\r\n");
	query.Format(_T("select NaverNick, Score from member order by Score DESC;")); //점수로 정렬해서 닉네임, 점수 가져옴
	mysql_query(&m_mysql, query);
	MYSQL_RES* result = mysql_store_result(&m_mysql);
	int nRowCount = mysql_num_rows(result);
	int nFieldCount = mysql_num_fields(result);
	if (nRowCount == 0) {
		str = str + (_T("랭킹에 있는 유저가 없습니다.\r\n"));
	}
	else {
		MYSQL_FIELD* fields = mysql_fetch_fields(result);
		MYSQL_ROW row;
		int i = 1;
		while (row = mysql_fetch_row(result)) {
			int Score = atoi(row[1]); //Score
			str.AppendFormat(_T("%d위  닉네임=%s  점수=%d\r\n"), i, row[0], Score);
			i++;
		}
	}
	this->m_ctrlEdit.ReplaceSel(str);
	m_pListenSocket->Broadcast(str);
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


void CYutnoriSDlg::PostNcDestroy()
{
	mysql_close(&m_mysql);
	CDialogEx::PostNcDestroy();
}
