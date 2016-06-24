#pragma once

#ifndef __INCLUDE_DEFINE_H__
#define __INCLUDE_DEFINE_H__

#include<iostream>
using namespace std;

#define BORDERWIDTH 50 //����(����)��Ե�Ŀ��
#define BORDERHEIGHT 50 //����(����)��Ե�ĸ߶�
#define GRILLEWIDTH 50  //������ÿ�����ӵĸ߶�
#define GRILLEHEIGHT 50 //������ÿ�����ӵĿ��

#define WHITE   (-1)
#define BLACK   (1)

#define Left_Down   (1)
#define Right_Down  (2)
#define Left_Up     (3)
#define Right_Up    (4)

#define WANG     (2)
#define Qi       (1)
struct CHESSPOS
{
	unsigned char x;
	unsigned char y;
};
struct Node_YouSha
{
public:
	BYTE shendu;
	BYTE tongdao;
	BYTE kongge1;
	BYTE kongge2;
	Node_YouSha *pNext;
};
struct CHESSMOVE
{
public:
	CHESSMOVE() //��ʼ��
	{
		m_Count = 0;
		for(BYTE i=0;i<20;i++)
		{
			step[i] = 0;
			step_king_kongge1[i] = 0;
			step_king_kongge2[i] = 0;
		}
	}
public:
	int m_Side;     //���廹�ǰ���
	int m_Type;    //���廹����ͨ��
	CHESSPOS From;    //�������
	int m_Count;    //ɱ������ (���岽��)
	BYTE step[20];     //ÿһ���ķ������20����
	BYTE step_king_kongge1[20];//ÿһ�����������пո���
	BYTE step_king_kongge2[20];//ÿһ�������ո���
	BYTE Score;        //��¼����
	bool operator==(const CHESSMOVE& move)
	{   //�ж���ʼ����
		if(From.x==move.From.x && From.y==move.From.y && m_Count==move.m_Count)
		{   //ѭ��ÿ���߷�
			for(BYTE i=0;i < m_Count+1;i++)
			{   //�ж��Ƿ���ȫ��ͬ
				if(step[i]!=move.step[i] || step_king_kongge1[i]!=move.step_king_kongge1[i] || step_king_kongge2[i]!=move.step_king_kongge2[i])
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
};


#endif //__INCLUDE_DEFINE_H__