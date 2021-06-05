#include "pch.h"
#include "CClientSocket.h"
#include "CYutnoriBoard.h"
#include "Yutnori_C.h"
#include "afxwin.h"

CClientSocket::CClientSocket()
{
	count = 0;
	sort = 0;
}

CClientSocket::~CClientSocket(){}

void CClientSocket::OnClose(int nErrorCode)
{
	this->ShutDown();
	this->Close();
	AfxMessageBox(_T("서버가 연결을 종료하였습니다."));
	::PostQuitMessage(0);
	CSocket::OnClose(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode)
{
	TCHAR szBuffer[1024];
	int nRead = Receive(szBuffer, sizeof(szBuffer));
	switch (nRead) {
	case 0: this->Close(); break;
	case SOCKET_ERROR: if (GetLastError() != WSAEWOULDBLOCK) { AfxMessageBox(_T("Error occurred")); this->Close(); } break;
	default:
		szBuffer[nRead] = _T('\0');
		CYutnoriBoard* pMain = (CYutnoriBoard*)AfxGetMainWnd();
		CString receive;
		CString receivenick; int Piece; int YutNum;
		receive = Find(szBuffer);

		if (sort == 0) {
			pMain->m_ctrlEdit.ReplaceSel(szBuffer);
		}
		else if (sort == 1) { //메시지가 윷정보인경우
			int len = receive.GetLength();
			receivenick = receive.Left(len - 2);
			Piece = _ttoi(receive.Mid(len - 2, 1));
			YutNum = _ttoi(receive.Right(1));
			if (strcmp(receivenick, pMain->m_nickname) ==0) { //받은 닉네임과 클라이언트가 동일할 경우
				if (count != 1) {
					count = 1;

					pMain->IsTurn = -1;
				}
				else {
					
					pMain->m_ctrlEdit.ReplaceSel(_T("상대방이 던질때까지 기다리세요.\r\n"));
				}
			}
			else if (strcmp(receivenick, pMain->m_nickname) != 0) { //받은 닉네임과 클라이언트가 다를 경우
				count = -1;
				pMain->m_ctrlEdit.ReplaceSel(_T("다른클라이언트가 던졌습니다.\r\n"));
				pMain->opPiece(Piece, YutNum);
				pMain->IsTurn = 1;

			}


		}
		
	}
	CSocket::OnReceive(nErrorCode);
}


CString CClientSocket::Find(LPCTSTR receive)
{
	CString find(receive);
	if (find.Left(6) == _T("[Chat]")) { //메시지가 채팅일 경우
		sort = 0;
		return find;
	}
	else if (find.Left(9) == _T("[Yutinfo]")) { //메시지가 윷정보일 경우 예시=>[Yutinfo]CBNU JSK: Piece: 3, YutNum: 3 =>반환=>CBNU JSK33
		CString PY;
		PY = find;
		int findIndexR = find.Find(": Piece");
		find = find.Left(findIndexR);
		int findIndexL = find.Find("[Yutinfo]") + 8;
		find = find.Right(findIndexR - findIndexL - 1);
		PY = PY.Right(14); //메시지 
		find = find + PY.Left(1) + PY.Mid(11,1);
		sort = 1;
		return find;
	}
	else { //메시지가 접속정보일 경우
		sort = 0;
		return find;
	}
}
