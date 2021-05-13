#pragma once
#include <afxsock.h>
class CListenSocket :
	public CAsyncSocket
{
public:
	CPtrList m_pChildSocketList;
};

