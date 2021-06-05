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
	CString str; str.Format(_T("[Status] ���� ����\r\n"));
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
	GetPeerName(strIPAddress, uPortNumber); //����� Ŭ���̾�Ʈ�� IP�ּҿ� ��Ʈ ��ȣ�� �˾Ƴ���.
	TCHAR szBuffer[1024]; //(5) Ŭ���̾�Ʈ�� ������ ����
	int nRead = Receive(szBuffer, sizeof(szBuffer));
	switch (nRead) {
	case 0: this->Close(); break;
	case SOCKET_ERROR: if (GetLastError() != WSAEWOULDBLOCK) {
		AfxMessageBox(_T("Error occurred"));
		this->Close();
	} break;
	default:
		szBuffer[nRead] = _T('\0');
		CString str; str.Format(_T("%s"), szBuffer);
		CString nickname = FindNickName(str);
		CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
		pMain->m_ctrlEdit.ReplaceSel(str); //������ �޽����� ȭ�鿡 �����ش�.
		m_pListenSocket->Broadcast(str); //��� Ŭ���̾�Ʈ���� �۽��Ѵ�.
	}
	CSocket::OnReceive(nErrorCode);
}

CString CChildSocket::FindNickName(LPCTSTR szUrl)
{
	CString findnickname(szUrl);
	int findIndexR = findnickname.Find(": ");
	if (findIndexR <= 0) { return _T("invalid_request"); }
	findnickname = findnickname.Left(findIndexR);
	return findnickname;
}