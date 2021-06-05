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
	void Move(int n); //n만큼 이동
	int catched; //잡히면 1, 평소에는 0
};

