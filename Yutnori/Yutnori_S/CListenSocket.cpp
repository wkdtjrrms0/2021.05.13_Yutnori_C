#include "pch.h"
#include "CListenSocket.h"
#include "Yutnori_SDlg.h"
#include "CChildSocket.h"

/*(5) Connect�� ����õ��ϴ°͵� �ٰ�������.*/
void CListenSocket::OnAccept(int nErrorCode)
{
	CChildSocket* pChild = new CChildSocket;
	BOOL check = Accept(*pChild);
	if (check == FALSE) { delete pChild; return; } //����������
	m_pChildSocketList.AddTail(pChild); //Ŭ���̾�Ʈ ������ �����ϸ� ����Ʈ�� �߰��Ѵ�.
	CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
	CString strSocketName; UINT uPort; //����ȭ�鿡 Ŭ���̾�Ʈ ���� ������ �����ش�.
	pChild->GetPeerName(strSocketName, uPort);
	CString str; str.Format(_T("[Status] ���� ������ �����մϴ�.\r\n"));
	pMain->m_ctrlEdit.ReplaceSel(str); //Ŭ���̾�Ʈ�� ������ ���������� �˷��ش�.
	pChild->Send(str.GetBuffer(), str.GetLength());
	pChild->m_pListenSocket = this; //CListenSocket ��ü�� �ּҸ� CChildSocket ��ü�� �����Ѵ�.
	int count = m_pChildSocketList.GetCount();
	CString gamestart = _T("[Game] ������ ���۵˴ϴ�!\r\n");
	CString gamewait = _T("[Status] ������ �����Ҷ����� ����մϴ�....\r\n");
	if (count == 1) { Broadcast(gamewait); pMain->m_ctrlEdit.ReplaceSel(gamewait); } //����Ҷ�
	if (count == 2) { Broadcast(gamestart); pMain->m_ctrlEdit.ReplaceSel(gamestart); } //���ӽ����Ҷ�
	CAsyncSocket::OnAccept(nErrorCode);
}

/*��� Ŭ���̾�Ʈ�� �ѷ���.*/
void CListenSocket::Broadcast(CString strMessage)
{
	POSITION pos = m_pChildSocketList.GetHeadPosition();
	while (pos != NULL) {
		CChildSocket* pChild = (CChildSocket*)m_pChildSocketList.GetNext(pos);
		if (pChild != NULL) pChild->Send(strMessage.GetBuffer(), strMessage.GetLength());
	}
}
