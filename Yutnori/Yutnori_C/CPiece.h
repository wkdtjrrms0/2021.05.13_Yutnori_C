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
	int catched; //잡히면 1, 평소에는 0
};

