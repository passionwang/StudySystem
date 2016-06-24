#pragma once
#ifndef __INCLUDE_CMOVEGENERATOR_H__
#define __INCLUDE_CMOVEGENERATOR_H__
#include <iostream>
#include"define.h"
using namespace std;


class CMoveGenerator
{
public:///////////////////////////////////////////////////////////////////////////////////////
	CMoveGenerator();
	virtual ~CMoveGenerator();
public:///////////////////////////////////////////////////////////////////////////////////////
	//�������
	void ClearData();
	//�������������ϵ����кϷ����߷�
	int CreatePossibleMove(int position[10][10],int nPly,int nSide);  
	//����һ���߷�
	void AddMove_ShaQi(int i,int j,int nSide,int nPly,int& nMethod);
	void AddMove_BuJv(int nCount,int nSide,int nType,int mFangXiang,int nPly,int& nMethod,int nKongge = 0);
	//���õݹ����
	void CommonCode(int i,int j,int value,int shu,int fangxiang,int TURN,int Board[10][10],int nCount = -1);
	void CommonFind(int depth,int nCount,int lianI,int chu1[600],int chu2[600],int MaxPos[600],int kong1[600],int kong2[600]);
	//�����������ݣ����ÿ�����ӵ�ɱ�巽ʽ
	void NodeCount(int nCount);
	//�ж�ɱ����
	bool PangDuan_ShaShangli();
	//ɱ��ģ��
	int Move_z_x_x(int i,int j,int Board[10][10]);
	void MoveF_z_x_x(int i,int j,int n,int Board[10][10]);
	int Move_z_s_s(int i,int j,int Board[10][10]);
	void MoveF_z_s_s(int i,int j,int n,int Board[10][10]);
	int Move_y_x_x(int i,int j,int Board[10][10]);
	void MoveF_y_x_x(int i,int j,int n,int Board[10][10]);
	int Move_y_s_s(int i,int j,int Board[10][10]);
	void MoveF_y_s_s(int i,int j,int n,int Board[10][10]);
	int Move_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) ;
	void MoveF_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) ;
	int Move_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]);
	void MoveF_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) ;
	int Move_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) ;
	void MoveF_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) ;
	int Move_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) ;
	void MoveF_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]);
	//�����ĸ�����ո�
	BYTE ZuoShang(BYTE x,BYTE y,int Board[10][10]);
	BYTE YouShang(BYTE x,BYTE y,int Board[10][10]);
	BYTE ZuoXia(BYTE x,BYTE y,int Board[10][10]);
	BYTE YouXia(BYTE x,BYTE y,int Board[10][10]);
	//����
	void addLike(Node_YouSha **pHead,Node_YouSha **pEnd,int sd,int td,int kg1=0,int kg2=0);
	void ClearLike(Node_YouSha **ppHead);
public://///////////////////////////////////////////////////////////////////////////////////////////	
	//����߷�����
	CHESSMOVE m_nMoveList[10][100];
};

#endif //__INCLUDE_CMOVEGENERATOR_H__