#pragma once
#include <afxsock.h>

class CListenSocket :
	public CAsyncSocket
{
public:
	CPtrList m_pChildSocketList; //����� ���ϸ���Ʈ �����Ҹ���Ʈ
	virtual void OnAccept(int nErrorCode);
	void Broadcast(CString strMessage);
	BOOL StartServer(UINT port);
	void StopServer();
};

