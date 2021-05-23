#pragma once
#include "afxdhtml.h"

#ifdef _WIN32_WCE
#error "Windows CE에서는 CDHtmlDialog가 지원되지 않습니다."
#endif

// CNaverOAuthDlg 대화 상자

class CNaverOAuthDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CNaverOAuthDlg)

public:
	CNaverOAuthDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNaverOAuthDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVEROAUTH_DIALOG, IDH = 104 };
#endif


protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

public:
	CString m_RequestURl;
	CString m_strStateCode;
	CString m_ResultCode;
	CString m_Code;
	CString m_State;
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	void SetRequestUrl(CString szURl) { m_RequestURl = szURl; }
	void SetResultCode(CString szResultCode) { m_ResultCode = szResultCode; }
	void SetCode(CString szCode) { m_Code = szCode; }
	void SetState(CString szState) { m_State = szState; }
	BOOL CallbackUrlExtractHtml(LPDISPATCH pDisp, LPCTSTR szUrl);
	CString ExtractUrl(LPCTSTR szUrl);
	CString FindCode(LPCTSTR szUrl);
	CString FindToken(LPCTSTR szUrl);
	CString FindTokenType(LPCTSTR szUrl);
	CString FindNaverID(LPCTSTR szUrl);
	CString FindNaverNickName(LPCTSTR szUrl);
	CString GetRequestUrl() { return m_RequestURl; }
	CString GetResultCode() { return m_ResultCode; }
	CString GetCode() { return m_Code; }
	CString GetState() { return m_State; }
	CString m_Token;
	CString UserID;
	CString UserNick;
	void ReadJson(CString gettokenUrl);
	CString m_ReadJson;
	int IsLogin; //0은 로그인 성공 전, 1은 로그인 성공 후, -1은 로그아웃 후
	int logincount; // 로그인 시도횟수를 카운트합니다.
	void login(); //로그인
	void logout(); //로그아웃
	CString m_TokenType;
	BOOL CallApi();
	CString m_finaljson;
	CString m_NaverID;
	CString m_NaverNickName;
};
