#include "pch.h"
#include "CChildSocket.h"
#include "Yutnori_SDlg.h"
#include "../Yutnori_C/resource.h"

void CChildSocket::OnClose(int nErrorCode)
{
	POSITION pos = m_pListenSocket->m_pChildSocketList.Find(this);
	m_pListenSocket->m_pChildSocketList.RemoveAt(pos);
	CString strSocketName; UINT uPort;
	this->GetPeerName(strSocketName, uPort);
	CString str; str.Format(_T("[Status] 연결 종료\r\n"));
	CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
	pMain->m_ctrlEdit.ReplaceSel(str);
	this->ShutDown();
	this->Close();
	delete this;
	CSocket::OnClose(nErrorCode);
}

void CChildSocket::OnReceive(int nErrorCode)
{
	CString strIPAddress; UINT uPortNumber;
	GetPeerName(strIPAddress, uPortNumber); //연결된 클라이언트의 IP주소와 포트 번호를 알아낸다.
	TCHAR szBuffer[1024]; //(5) 클라이언트가 보낸값 저장
	int nRead = Receive(szBuffer, sizeof(szBuffer));
	switch (nRead) {
	case 0: this->Close(); break;
	case SOCKET_ERROR: if (GetLastError() != WSAEWOULDBLOCK) {
		AfxMessageBox(_T("Error occurred"));
		this->Close();
	} break;
	default:
		szBuffer[nRead] = _T('\0');
		CString str(szBuffer);
		CString nickname = FindNickName(str);
		CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
		pMain->m_ctrlEdit.ReplaceSel(str); //수신한 메시지를 화면에 보여준다.

		if (str.Left(11) == _T("[DataInput]")) {
			pMain->InputData(str);
		}
		else if (str.Left(10) == _T("[DataRead]")) {
			pMain->ReadData();
		}
		else {
			m_pListenSocket->Broadcast(str);	// 모든 클라이언트에게 송신한다.
		}
	}
	CSocket::OnReceive(nErrorCode);
}

CString CChildSocket::FindNickName(LPCTSTR message)
{
	CString findnickname(message);
	int findIndexR = findnickname.Find(": ");
	if (findIndexR <= 0) { return _T("invalid_request"); }
	findnickname = findnickname.Left(findIndexR);
	return findnickname;
}