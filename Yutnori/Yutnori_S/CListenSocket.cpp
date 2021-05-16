#include "pch.h"
#include "CListenSocket.h"
#include "Yutnori_SDlg.h"
#include "CChildSocket.h"


void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CChildSocket* pChild = new CChildSocket;
	BOOL check = Accept(*pChild);
	if (check == FALSE) { delete pChild; return; } //접속허용실패
	
	//클라이언트 접속이 성공하면 리스트에 추가한다.
	m_pChildSocketList.AddTail(pChild);

	//서버화면에 클라이언트 소켓 정보를 보여준다.
	CString strSocketName; UINT uPort;
	pChild->GetPeerName(strSocketName, uPort);
	CString str; str.Format(_T("[%s:%u] 서버 접속을 승인합니다.\r\n"), strSocketName, uPort);
	CYutnoriSDlg* pMain = (CYutnoriSDlg*)AfxGetMainWnd();
	pMain->m_ctrlEdit.ReplaceSel(str);

	//클라이언트에게 접속이 성공했음을 알려준다.
	pChild->Send(str.GetBuffer(), str.GetLength());

	//CListenSocket 객체의 주소를 CChildSocket 객체에 저장한다.
	pChild->m_pListenSocket = this;

	CAsyncSocket::OnAccept(nErrorCode);
}


void CListenSocket::Broadcast(CString strMessage)
{
	// TODO: 여기에 구현 코드 추가.

	POSITION pos = m_pChildSocketList.GetHeadPosition();
	while (pos != NULL) {
		CChildSocket* pChild = (CChildSocket*)m_pChildSocketList.GetNext(pos);
		if (pChild != NULL) pChild->Send(strMessage.GetBuffer(), strMessage.GetLength());
	}
}
