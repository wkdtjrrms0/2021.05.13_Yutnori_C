#pragma once
#include <afxsock.h>

class CListenSocket :
	public CAsyncSocket
{
public:
	CPtrList m_pChildSocketList;
	virtual void OnAccept(int nErrorCode);
	void Broadcast(CString strMessage);
};

