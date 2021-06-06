#pragma once
#include <afxsock.h>

class CListenSocket :
	public CAsyncSocket
{
public:
	CPtrList m_pChildSocketList; //연결된 소켓리스트 저장할리스트
	virtual void OnAccept(int nErrorCode);
	void Broadcast(CString strMessage);
	BOOL StartServer(UINT port);
	void StopServer();
};

