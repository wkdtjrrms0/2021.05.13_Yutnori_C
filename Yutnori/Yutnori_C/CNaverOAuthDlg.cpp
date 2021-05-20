// CNaverOAuthDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Yutnori_C.h"
#include "CNaverOAuthDlg.h"
#include "rapidjson\document.h"

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
	CDHtmlDialog::OnInitDialog(); //로그인페이지 이동
	m_strStateCode.Format(_T("%d"), GetTickCount());
	CString sAuthUrl;
	sAuthUrl.Format(NAVER_AUTH_URL, CONSUMER_CLIENT_ID, REDIRECT_URL, m_strStateCode);
	SetRequestUrl(sAuthUrl);
	CComVariant varFlags, varHeader, varTarget, vtPost;
	varTarget = "_self";
	HRESULT hr = m_pBrowserApp->Navigate(sAuthUrl.AllocSysString(), &varFlags, &varTarget, &vtPost, &varHeader);
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
	if (CallbackUrlExtractHtml(pDisp, szUrl) == TRUE)
		EndDialog(IDOK);
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);
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
		if (m_Code == "clickCancel") { EndDialog(IDOK); } //예외처리(취소버튼을 누를시)

		CString gettokenUrl;
		gettokenUrl.Format(GET_TOKEN_URL, CONSUMER_CLIENT_ID, CONSUMER_CLIENT_SECRET, m_Code, m_strStateCode);
		SetRequestUrl(gettokenUrl); //토큰발급
		
		CComVariant varFlags, varHeader, varTarget, vtPost;
		varTarget = "_self";
		HRESULT hr = m_pBrowserApp->Navigate(gettokenUrl.AllocSysString(), &varFlags, &varTarget, &vtPost, &varHeader);
		

		CComQIPtr<IWebBrowser2> spWebBrowser2(pDisp);
		CComPtr<IDispatch> spDocDisp;
		spWebBrowser2->get_Document(&spDocDisp);
		CComQIPtr<IHTMLDocument2> spDoc(spDocDisp);

		
		hr = E_FAIL;
		CComPtr<IHTMLElement> lpBodyElm;
		hr = spDoc->get_body(&lpBodyElm);
		if (FAILED(hr) || lpBodyElm == NULL){return FALSE;}

		hr = E_FAIL;
		CComPtr<IHTMLElement> lpParentElm;
		hr = lpBodyElm->get_parentElement(&lpParentElm);
		if (FAILED(hr) || lpParentElm == NULL){return FALSE;}

		BSTR bstrHtml;
		lpBodyElm->get_outerText(&bstrHtml);
		char* pbstr;
		USES_CONVERSION;
		pbstr = OLE2A(bstrHtml);
		
		rapidjson::Document rjdoc;
		rjdoc.Parse<0>(pbstr);
		SetResultCode((CString)rjdoc["access_token"].GetString());
		SetCode((CString)rjdoc["result_data"]["refresh_token"].GetString());
		SetState((CString)rjdoc["result_data"]["token_type"].GetString());

		/////////////////////테스트라인////////////////
		CString test = (CString)rjdoc["access_token"].GetString(); AfxMessageBox(test);
		///////////////////////////////////////////////

		return TRUE;
	}
	else
		return FALSE;
}