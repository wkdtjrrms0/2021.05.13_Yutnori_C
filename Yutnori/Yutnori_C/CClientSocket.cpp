
#include "pch.h"
#include "CClientSocket.h"
#include "CYutnoriBoard.h"
#include "Yutnori_C.h"
#include "afxwin.h"
#include "Yutnori_CDlg.h"

CClientSocket::CClientSocket()
{
	count = 0;
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	this->ShutDown();
	this->Close();
	AfxMessageBox(_T("서버가 연결을 종료하였습니다."));
	::PostQuitMessage(0);
	CSocket::OnClose(nErrorCode);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	TCHAR szBuffer[1024];
	int nRead = Receive(szBuffer, sizeof(szBuffer));
	switch (nRead) {
	case 0: this->Close(); break;
	case SOCKET_ERROR: if (GetLastError() != WSAEWOULDBLOCK) {
		AfxMessageBox(_T("Error occurred"));
		this->Close();
	} break;
	default:
		szBuffer[nRead] = _T('\0');
		CYutnoriBoard* pMain = (CYutnoriBoard*)AfxGetMainWnd();
		if (count == 0) {
			pMain->m_ctrlEditN.ReplaceSel(pMain->m_nickname);
			pMain->m_ctrlEditN.ReplaceSel(_T("\r\n"));
		} //최초만 실행
		pMain->m_ctrlEdit.ReplaceSel(szBuffer);
		count++;
	}


	CSocket::OnReceive(nErrorCode);
}
