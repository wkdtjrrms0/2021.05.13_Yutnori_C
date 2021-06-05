#pragma once

class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	int count; //턴이 넘어가기전에 다시던지는 것을 방지하기 위한 예외처리
	CString Find(LPCTSTR receive);
	int sort; //0은 채팅창, 1은 윷정보, 2는 게임시작, 3은 승리, 4는 패배
};

