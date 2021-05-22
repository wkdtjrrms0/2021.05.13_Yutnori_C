// CNaverOAuthDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Yutnori_C.h"
#include "CNaverOAuthDlg.h"
#include "rapidjson\document.h"
#include <afxinet.h> 
#include <string>
#include "CYutnoriStart.h"

#define CALLBACK_URL _T("http://127.0.0.1/")
#define REDIRECT_URL _T("http%3A%2F%2F127.0.0.1")
#define CONSUMER_CLIENT_ID _T("KFLMkOZ2nYgpreXtbve9")
#define CONSUMER_CLIENT_SECRET _T("9qTBDtIsaf")
#define NAVER_AUTH_URL _T("https://nid.naver.com/oauth2.0/authorize?response_type=code&client_id=%s&redirect_uri=%s&state=%s") //로그인
#define GET_TOKEN_URL _T("https://nid.naver.com/oauth2.0/token?grant_type=authorization_code&client_id=%s&client_secret=%s&code=%s&state=%s") //토큰발급
#define REFRESH_TOKEN_URL _T("https://nid.naver.com/oauth2.0/token?grant_type=refresh_token&client_id=%s&client_secret=%s&refresh_token=%s") //토큰갱신
#define DELETE_TOKEN_URL _T("https://nid.naver.com/oauth2.0/token?grant_type=delete&client_id=%s&client_secret=%s&access_token=%s&service_provider=NAVER") //토큰삭제


// CNaverOAuthDlg 대화 상자

IMPLEMENT_DYNCREATE(CNaverOAuthDlg, CDHtmlDialog)

BEGIN_DHTML_EVENT_MAP(CNaverOAuthDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

CNaverOAuthDlg::CNaverOAuthDlg(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_NAVEROAUTH_DIALOG, 104, pParent)
{
	m_RequestURl = _T("");
	m_strStateCode = _T("");
	m_ResultCode = _T("");
	m_Code = _T("");
	m_State = _T("");
	m_Token = _T("");
	UserID = _T("");
	UserNick = _T("");
	m_ReadJson = _T("");
	IsLogin = 0;
	logincount = 0;
	m_TokenType = _T("");
}

CNaverOAuthDlg::~CNaverOAuthDlg()
{
}

void CNaverOAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CNaverOAuthDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	if (logincount % 2 == 0) { login(); } //인증과정이 짝수번일경우 실행(ex. 최초, 로그인하고 토큰삭제한 후)
	if (logincount % 2 == 1) { logout();} //인증과정이 홀수번일 경우 실행
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BEGIN_MESSAGE_MAP(CNaverOAuthDlg, CDHtmlDialog)
END_MESSAGE_MAP()





// CNaverOAuthDlg 메시지 처리기

HRESULT CNaverOAuthDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CNaverOAuthDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CNaverOAuthDlg::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);
	if (CallbackUrlExtractHtml(pDisp, szUrl) == TRUE)
		EndDialog(IDOK);

}



void CNaverOAuthDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
}


CString CNaverOAuthDlg::ExtractUrl(LPCTSTR szUrl) //메인도메인만 뽑기위해
{
	CString extractUrl(szUrl);

	int findIndex = extractUrl.Find('?');
	if (findIndex <= 0)
	{
		return _T("");
	}
	extractUrl = extractUrl.Left(findIndex);
	return extractUrl;
}

CString CNaverOAuthDlg::FindCode(LPCTSTR szUrl) //url의 코드값을 뽑기위해
{
	CString findcode(szUrl);

	int findIndexR = findcode.Find('&');
	if (findIndexR <= 0)
	{
		return _T("");
	}
	findcode = findcode.Left(findIndexR);
	int findIndexL = findcode.Find('=');
	if (findIndexL <= 0)
	{
		return _T("");
	}
	findcode = findcode.Right(findIndexR - findIndexL - 1);

	if (findcode == "access_denied") {
		return "clickCancel";
	}

	return findcode;
}

CString CNaverOAuthDlg::FindToken(LPCTSTR szUrl) //url의 토큰값을 뽑기위해
{
	CString findtoken(szUrl);

	int findIndexR = findtoken.Find("refresh_token") - 3;
	if (findIndexR <= 0)
	{
		return _T("invalid_request");
	}
	findtoken = findtoken.Left(findIndexR);



	int findIndexL = findtoken.Find("access_token") + 14;
	if (findIndexL <= 0)
	{
		return _T("invalid_request");
	}
	findtoken = findtoken.Right(findIndexR - findIndexL - 1);


	return findtoken;
}
CString CNaverOAuthDlg::FindTokenType(LPCTSTR szUrl) //url의 토큰타입을 뽑기위해
{
	CString findtokentype(szUrl);

	int findIndexR = findtokentype.Find("expires_in") - 3;
	if (findIndexR <= 0)
	{
		return _T("invalid_request");
	}
	findtokentype = findtokentype.Left(findIndexR);



	int findIndexL = findtokentype.Find("token_type") + 12;
	if (findIndexL <= 0)
	{
		return _T("invalid_request");
	}
	findtokentype = findtokentype.Right(findIndexR - findIndexL - 1);


	return findtokentype;
}


/*=============================================================================
IWebBrowser2 객체로부터 브라우저의 Document를 얻고, Document에서 태그에 대한
객체에 접근해서 CallBack 페이지에서 전달된 json 데이타를 얻어 오는 함수----미완성, 토큰으로 json이 출력되는 url을 여는부분까지확인
===============================================================================*/
BOOL CNaverOAuthDlg::CallbackUrlExtractHtml(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CString szCallBackUrl(CALLBACK_URL);
	CString szExtraUrl = ExtractUrl(szUrl);
	if (szCallBackUrl.Compare(szExtraUrl) == 0)
	{
		CString m_Code = FindCode(szUrl);
		if (m_Code == "clickCancel") { EndDialog(IDOK); 
		} //예외처리(취소버튼을 누를시)

		CString gettokenUrl;
		gettokenUrl.Format(GET_TOKEN_URL, CONSUMER_CLIENT_ID, CONSUMER_CLIENT_SECRET, m_Code, m_strStateCode);
		ReadJson(gettokenUrl); //토큰발급
		m_Token = FindToken(m_ReadJson); //json의 토큰값을 얻기 위해
		m_TokenType = FindTokenType(m_ReadJson); //json의 토큰타입을 얻기 위해

		
		return TRUE;
	}
	else
		return FALSE;
}

void CNaverOAuthDlg::ReadJson(CString gettokenUrl)
{
	// JSON읽는 함수
	UpdateData(TRUE); // 세션이 연결된 이후, 웹 사이트의 Html 정보가 변경된 사항을 가져올 경우 
	CInternetSession session(NULL, 1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
	try { // 세션 클래스를 이용하여 열기 
		CInternetFile* pinetFile = (CInternetFile*)session.OpenURL(gettokenUrl);
		if (NULL != pinetFile) {
			char acRead[40960] = { 0, }; // return 값이 길 수 있으므로 충분히 입력 
			memset(acRead, 0, sizeof(acRead)); // json 읽는 부분 
			UINT uiTotalRead = 0; int iIdex = 0; do { // 데이터를 1024씩 읽어 온다. 
				uiTotalRead = pinetFile->Read(&acRead[iIdex], 1024);
				if (uiTotalRead <= 0) break; iIdex += uiTotalRead;
			} while (uiTotalRead > 0); // 읽은 json 표시를 위한 부분 
			std::string strJson = acRead;
			m_ReadJson = CA2W(strJson.c_str(), CP_UTF8);
			pinetFile->Close(); // Html 파일을 닫는다. 
			session.Close(); // 세션 종료 
			delete pinetFile; // Html 파일 읽기에 사용한 객체를 파괴 
			IsLogin = 1;
			logincount++;
		}
	}
	catch (CInternetException* e) { // 에러 
	} UpdateData(FALSE);
}

void CNaverOAuthDlg::login()
{
	// 로그인페이지 이동
	m_strStateCode.Format(_T("%d"), GetTickCount());
	CString sAuthUrl;
	sAuthUrl.Format(NAVER_AUTH_URL, CONSUMER_CLIENT_ID, REDIRECT_URL, m_strStateCode);
	SetRequestUrl(sAuthUrl);
	CComVariant varFlags, varHeader, varTarget, vtPost;
	varTarget = "_self";
	HRESULT hr = m_pBrowserApp->Navigate(sAuthUrl.AllocSysString(), &varFlags, &varTarget, &vtPost, &varHeader);
}

void CNaverOAuthDlg::logout()
{
	// 로그아웃
	CString sAuthUrl;
	sAuthUrl.Format(DELETE_TOKEN_URL, CONSUMER_CLIENT_ID, CONSUMER_CLIENT_SECRET, m_Token);
	SetRequestUrl(sAuthUrl);
	CComVariant varFlags, varHeader, varTarget, vtPost;
	varTarget = "_self";
	Navigate("http://nid.naver.com/nidlogin.logout", 0, "_self", NULL, NULL);
	//HRESULT hr = m_pBrowserApp->Navigate(sAuthUrl.AllocSysString(), &varFlags, &varTarget, &vtPost, &varHeader);
	IsLogin = -1;
	logincount++;
}

