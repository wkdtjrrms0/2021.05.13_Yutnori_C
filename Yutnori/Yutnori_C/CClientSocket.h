#pragma once

class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	int count; //���� �Ѿ������ �ٽô����� ���� �����ϱ� ���� ����ó��
	CString Find(LPCTSTR receive);
	int sort; //0�� ä��â, 1�� ������, 2�� ���ӽ���, 3�� �¸�, 4�� �й�
};

