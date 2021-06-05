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
	AfxMessageBox(_T("������ ������ �����Ͽ����ϴ�."));
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
		else if (sort == 1) { //�޽����� �������ΰ��
			int len = receive.GetLength();
			receivenick = receive.Left(len - 2);
			Piece = _ttoi(receive.Mid(len - 2, 1));
			YutNum = _ttoi(receive.Right(1));
			if (strcmp(receivenick, pMain->m_nickname) ==0) { //���� �г��Ӱ� Ŭ���̾�Ʈ�� ������ ���
				if (count != 1) {
					count = 1;

					pMain->IsTurn = -1;
					pMain->SetDlgItemText(IDC_STATIC2, _T("���� �����Դϴ�..."));
				}
				else {
					
					pMain->m_ctrlEdit.ReplaceSel(_T("������ ���������� ��ٸ�����.\r\n"));
				}
			}
			else if (strcmp(receivenick, pMain->m_nickname) != 0) { //���� �г��Ӱ� Ŭ���̾�Ʈ�� �ٸ� ���
				count = -1;
				pMain->m_ctrlEdit.ReplaceSel(_T("�ٸ�Ŭ���̾�Ʈ�� �������ϴ�.\r\n"));
				pMain->opPiece(Piece, YutNum);
				pMain->IsTurn = 1;
				pMain->SetDlgItemText(IDC_STATIC2, _T("����� �����Դϴ�..."));

			}


		}
		else if (sort == 2) { //���ӽ��� �޽���
			pMain->m_ctrlEdit.ReplaceSel(szBuffer);
			pMain->SetDlgItemText(IDC_STATIC2, _T("���� ���� ���� �ű�� ����� �����Դϴ�."));
			pMain->IsGameStart = 1;
		}
		else if (sort == 3) { //���ӽ¸� �޽���
			int len = receive.GetLength();
			receivenick = receive.Left(len - 3);
			if (strcmp(receivenick, pMain->m_nickname) == 0) { //���� �г��Ӱ� Ŭ���̾�Ʈ�� ������ ���=>���� �¸�
				pMain->m_ctrlEdit.ReplaceSel(_T("���ӿ� �¸��ϼ̽��ϴ�.\r\n"));
				pMain->m_ctrlEdit.ReplaceSel(_T("[Status] ���� ����\r\n"));
				this->ShutDown();
				this->Close();
				pMain->InitPiece();
				
			}
			else if (strcmp(receivenick, pMain->m_nickname) != 0) { //���� �г��Ӱ� Ŭ���̾�Ʈ�� �ٸ� ���=>���� �й�
				pMain->m_ctrlEdit.ReplaceSel(_T("���ӿ� �й��ϼ̽��ϴ�.\r\n"));
				this->ShutDown();
				this->Close();
				pMain->InitPiece();
				pMain->SetDlgItemText(IDC_STATIC2, _T("�ȳ��ϼ���. ������ �����Դϴ�."));
			}
			pMain->IsGameStart = 0;

		}
		else if (sort == 4) { //�����й� �޽���
			int len = receive.GetLength();
			receivenick = receive.Left(len - 4);
			if (strcmp(receivenick, pMain->m_nickname) == 0) { //���� �г��Ӱ� Ŭ���̾�Ʈ�� ������ ���=>���� �й�
				pMain->m_ctrlEdit.ReplaceSel(_T("���ӿ� �й��ϼ̽��ϴ�.\r\n"));
				pMain->InitPiece();
			}
			else if (strcmp(receivenick, pMain->m_nickname) != 0) { //���� �г��Ӱ� Ŭ���̾�Ʈ�� �ٸ� ���=>���� �¸�
				pMain->m_ctrlEdit.ReplaceSel(_T("���ӿ� �¸��ϼ̽��ϴ�.\r\n"));
				pMain->m_ctrlEdit.ReplaceSel(_T("[Status] ���� ����\r\n"));
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
	if (find.Left(6) == _T("[Chat]")) { //�޽����� ä���� ���
		sort = 0;
		return find;
	}
	else if (find.Left(9) == _T("[Yutinfo]")) { //�޽����� �������� ��� ����=>[Yutinfo]CBNU JSK: Piece: 3, YutNum: 3 =>��ȯ=>CBNU JSK33
		CString PY;
		PY = find;
		int findIndexR = find.Find(": Piece");
		find = find.Left(findIndexR);
		int findIndexL = find.Find("[Yutinfo]") + 8;
		find = find.Right(findIndexR - findIndexL - 1);
		PY = PY.Right(14); //�޽��� 
		find = find + PY.Left(1) + PY.Mid(11,1);
		sort = 1;
		return find;
	}
	else if (find.Left(8) == _T("[Status]")) { //�޽����� ���������� ���
		find = find.Mid(find.GetLength() - 20, find.GetLength());
		if (strcmp(find, _T("������ ���۵˴ϴ�!\r\n")) == 0) { //���� ���ڰ� ������ ���۵ȴٴ� �����ϰ��
			sort = 2; return find;
		}
		else {
			sort = 0; return find;
		}
	}

	else if (find.Left(6) == _T("[Game]")) { //�޽����� ���������� ���
		if (strcmp(find.Mid(find.Find(": Win") + 2,3), _T("Win")) == 0) { //�¸��޽����� ���=>[Game]CBNU JSK: Win\r\n => ��ȯ=>CBNU JSKWin
			sort = 3;
			int findIndexR = find.Find(": Win");
			find = find.Left(findIndexR);
			int findIndexL = find.Find("[Game]") + 5;
			find = find.Right(findIndexR - findIndexL - 1);
			find = find + _T("Win");
			return find;
		}
		else { //�й�޽����� ���
			sort = 4;
			int findIndexR = find.Find(": Lose");
			find = find.Left(findIndexR);
			int findIndexL = find.Find("[Game]")+5;
			find = find.Right(findIndexR - findIndexL - 1);
			find = find + _T("Lose");
			return find;
		}
	}

	else { //�޽����� ���������� ���
		sort = 0;
		return find;
	}
}
