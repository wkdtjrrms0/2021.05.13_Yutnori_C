#pragma once
#include <afxwin.h>
class CPiece :
	public CWnd
{
public:
	CPiece();
	int x;
	int y;
	int count;
	void Move(int n);
	int catched; //������ 1, ��ҿ��� 0
};

