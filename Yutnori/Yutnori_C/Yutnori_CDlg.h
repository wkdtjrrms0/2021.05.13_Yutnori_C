
// Yutnori_CDlg.h: 헤더 파일
//

#pragma once
#include "CClientSocket.h"

// CYutnoriCDlg 대화 상자
class CYutnoriCDlg : public CDialogEx
{
// 생성입니다.
public:
	CYutnoriCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YUTNORI_C_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	CString m_strAddress;
	int m_nPort;
	CEdit m_ctrlEdit;
	CString m_strMessage;
	CClientSocket* m_pClientSocket;
	afx_msg void OnBnClickedConnect();
	virtual BOOL DestroyWindow();
	virtual void OnOK();
};
