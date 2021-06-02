#pragma once
#include <afxsock.h>
#include "CListenSocket.h"

class CChildSocket :
	public CSocket
{
public:
	CListenSocket *m_pListenSocket;
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	CString CChildSocket::FindNickName(LPCTSTR szUrl);
};

