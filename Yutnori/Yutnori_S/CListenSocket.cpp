#include "pch.h"
#include "CListenSocket.h"
#include "Yutnori_SDlg.h"
#include "CChildSocket.h"

/*(5) Connect로 연결시도하는것들 붙게해주자.*/
void CListenSocket::OnAccept(int nErrorCode)
{
	CChildSocket* pChild = new CChildSocket;
	BOOL check = Accept(*pChild);
	if (check == FALSE) { delete pChild; return; } //접속허용실패
	m_pChildSocketList.AddTail(pChild); //클라이언트 접속이 성공하면 리스트에 추가한다.
	CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
	CString strSocketName; UINT uPort; //서버화면에 클라이언트 소켓 정보를 보여준다.
	pChild->GetPeerName(strSocketName, uPort);
	CString str; str.Format(_T("[Status] 서버 접속을 승인합니다.\r\n"));
	pMain->m_ctrlEdit.ReplaceSel(str); //클라이언트의 접속이 성공했음을 알려준다.
	pChild->Send(str.GetBuffer(), str.GetLength());
	pChild->m_pListenSocket = this; //CListenSocket 객체의 주소를 CChildSocket 객체에 저장한다.
	int count = m_pChildSocketList.GetCount();
	CString gamestart = _T("[Game] 게임이 시작됩니다!\r\n");
	CString gamewait = _T("[Status] 상대방이 접속할때까지 대기합니다....\r\n");
	if (count == 1) { Broadcast(gamewait); pMain->m_ctrlEdit.ReplaceSel(gamewait); } //대기할때
	if (count == 2) { Broadcast(gamestart); pMain->m_ctrlEdit.ReplaceSel(gamestart); } //게임시작할때
	CAsyncSocket::OnAccept(nErrorCode);
}

/*모든 클라이언트에 뿌려줌.*/
void CListenSocket::Broadcast(CString strMessage)
{
	POSITION pos = m_pChildSocketList.GetHeadPosition();
	while (pos != NULL) {
		CChildSocket* pChild = (CChildSocket*)m_pChildSocketList.GetNext(pos);
		if (pChild != NULL) pChild->Send(strMessage.GetBuffer(), strMessage.GetLength());
	}
}
