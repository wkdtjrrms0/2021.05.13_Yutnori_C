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
					pMain->SetDlgItemText(IDC_STATIC2, _T("상대방 차례입니다..."));
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
				pMain->SetDlgItemText(IDC_STATIC2, _T("당신의 차례입니다..."));

			}


		}
		else if (sort == 2) { //게임시작 메시지
			pMain->m_ctrlEdit.ReplaceSel(szBuffer);
			pMain->SetDlgItemText(IDC_STATIC2, _T("윷을 먼저 던져 옮기는 사람이 선공입니다."));
			pMain->IsGameStart = 1;
		}
		else if (sort == 3) { //게임승리 메시지
			int len = receive.GetLength();
			receivenick = receive.Left(len - 3);
			if (strcmp(receivenick, pMain->m_nickname) == 0) { //받은 닉네임과 클라이언트가 동일할 경우=>나의 승리
				pMain->m_ctrlEdit.ReplaceSel(_T("게임에 승리하셨습니다.\r\n"));
				pMain->m_ctrlEdit.ReplaceSel(_T("[Status] 연결 종료\r\n"));
				this->ShutDown();
				this->Close();
				pMain->InitPiece();
				
			}
			else if (strcmp(receivenick, pMain->m_nickname) != 0) { //받은 닉네임과 클라이언트가 다를 경우=>나의 패배
				pMain->m_ctrlEdit.ReplaceSel(_T("게임에 패배하셨습니다.\r\n"));
				this->ShutDown();
				this->Close();
				pMain->InitPiece();
				pMain->SetDlgItemText(IDC_STATIC2, _T("안녕하세요. 윷놀이 게임입니다."));
			}
			pMain->IsGameStart = 0;

		}
		else if (sort == 4) { //게임패배 메시지
			int len = receive.GetLength();
			receivenick = receive.Left(len - 4);
			if (strcmp(receivenick, pMain->m_nickname) == 0) { //받은 닉네임과 클라이언트가 동일할 경우=>나의 패배
				pMain->m_ctrlEdit.ReplaceSel(_T("게임에 패배하셨습니다.\r\n"));
				pMain->InitPiece();
			}
			else if (strcmp(receivenick, pMain->m_nickname) != 0) { //받은 닉네임과 클라이언트가 다를 경우=>나의 승리
				pMain->m_ctrlEdit.ReplaceSel(_T("게임에 승리하셨습니다.\r\n"));
				pMain->m_ctrlEdit.ReplaceSel(_T("[Status] 연결 종료\r\n"));
				this->ShutDown();
				this->Close();
				pMain->InitPiece();
			}
			pMain->IsGameStart = 0;
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
	else if (find.Left(8) == _T("[Status]")) { //메시지가 상태정보일 경우
		find = find.Mid(find.GetLength() - 20, find.GetLength());
		if (strcmp(find, _T("게임이 시작됩니다!\r\n")) == 0) { //받은 문자가 게임이 시작된다는 문자일경우
			sort = 2; return find;
		}
		else {
			sort = 0; return find;
		}
	}

	else if (find.Left(6) == _T("[Game]")) { //메시지가 게임정보일 경우
		if (strcmp(find.Mid(find.Find(": Win") + 2,3), _T("Win")) == 0) { //승리메시지일 경우=>[Game]CBNU JSK: Win\r\n => 반환=>CBNU JSKWin
			sort = 3;
			int findIndexR = find.Find(": Win");
			find = find.Left(findIndexR);
			int findIndexL = find.Find("[Game]") + 5;
			find = find.Right(findIndexR - findIndexL - 1);
			find = find + _T("Win");
			return find;
		}
		else { //패배메시지일 경우
			sort = 4;
			int findIndexR = find.Find(": Lose");
			find = find.Left(findIndexR);
			int findIndexL = find.Find("[Game]")+5;
			find = find.Right(findIndexR - findIndexL - 1);
			find = find + _T("Lose");
			return find;
		}
	}

	else { //메시지가 접속정보일 경우
		sort = 0;
		return find;
	}
}
