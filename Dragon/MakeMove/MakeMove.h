#pragma once

#ifndef __INCLUDE_MAKEMOVE_H__
#define __INCLUDE_MAKEMOVE_H__

#include "MoveGenerator.h"
#include <iostream>
using namespace std;

class CMakeMove : public CMoveGenerator
{
public:
	CMakeMove();
	~CMakeMove();
public:
	void MakeMove1(CHESSMOVE* move,int type,int CurPosition[10][10]);
	void UnMakeMove1(CHESSMOVE* move,int CurPosition[10][10]);
	void MakeMoveQi1(CHESSMOVE* move,int CurPosition[10][10],int type);
	void UnMakeMoveQi1(CHESSMOVE* move,int CurPosition[10][10]);
	void MakeMoveWANG1(CHESSMOVE* move,int CurPosition[10][10],int type);
	void UnMakeMoveWANG1(CHESSMOVE* move,int CurPosition[10][10]);
	void BecomeKing(int CurPosition[10][10]);
	void MakeMoveSure(int m,int n,int nType,int CurPosition[10][10]);
	void UnMakeMove(int CurPosition[10][10]);
	void MakeMoveQi(CHESSMOVE* move,int CurPosition[10][10]);
public:
	CMoveGenerator m_pMG1;
	int m_ntype[20];
	BOOL m_bIsBecomeKing;
};


#endif //__INCLUDE_MAKEMOVE_H__