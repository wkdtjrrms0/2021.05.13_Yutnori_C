#include "pch.h"
#include "CChildSocket.h"
#include "Yutnori_SDlg.h"

void CChildSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	POSITION pos = m_pListenSocket->m_pChildSocketList.Find(this);
	m_pListenSocket->m_pChildSocketList.RemoveAt(pos);
	CString strSocketName; UINT uPort;
	this->GetPeerName(strSocketName, uPort);
	CString str; str.Format(_T("[%s:%u] ���� ����\r\n"), strSocketName, uPort);
	CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
	pMain->m_ctrlEdit.ReplaceSel(str);
	this->ShutDown();
	this->Close();
	delete this;

	CSocket::OnClose(nErrorCode);
}


void CChildSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//����� Ŭ���̾�Ʈ�� IP�ּҿ� ��Ʈ ��ȣ�� �˾Ƴ���.
	CString strIPAddress; UINT uPortNumber;
	GetPeerName(strIPAddress, uPortNumber);

	//������ �����͸� �����´�.
	TCHAR szBuffer[1024];
	int nRead = Receive(szBuffer, sizeof(szBuffer));
	switch (nRead) {
	case 0: this->Close(); break;
	case SOCKET_ERROR: if (GetLastError() != WSAEWOULDBLOCK) {
		AfxMessageBox(_T("Error occurred"));
		this->Close();
	} break;
	default:
		//������ �޽����� ȭ�鿡 �����ش�.
		szBuffer[nRead] = _T('\0');
		CString str; str.Format(_T("%s"), szBuffer);
		CString nickname = FindNickName(str);
		CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
		pMain->m_ctrlEdit.ReplaceSel(str);

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