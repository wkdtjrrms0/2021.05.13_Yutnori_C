#pragma once

class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	int count;
	CString Find(LPCTSTR receive);
	int sort; //0은 채팅창, 1은 윷정보
};

