#include "stdafx.h"
#include"MoveGenerator.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
//��¼m_nMoveList���߷�������
int m_nMoveCount;
CHESSPOS m_qi[20];
int m_type[20];
int m_count[20];//��¼ÿ�����������
int m_step[20][10][10]; //������==1 ������==2  ������==3   ������==4
int m_step_king_kongge1[20][10][10];
int m_step_king_kongge2[20][10][10];
int m_lianJ[20];  //ͬһ�����ӵ��߷� ����
int m_MaxLen;
int m_MaxPos[20];
Node_YouSha* m_pHead;
Node_YouSha* m_pEnd;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//����
CMoveGenerator::CMoveGenerator()//��ʼ��
{
	
}
//����
CMoveGenerator::~CMoveGenerator()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//�������������ϵ����кϷ����߷�
int CMoveGenerator::CreatePossibleMove(int position[10][10],int nPly,int nSide)
{
	int nCount = -1;
	int nMethod = 0;
	ClearData();
	for(int i=0;i < 10;i++)
	{
		for(int j=0;j < 10;j++)
		{
			//1.�ҵ��Լ�����-��¼��Ϣ
			if(nSide == position[i][j] || WANG*nSide == position[i][j])//2.��ͨ����
			{
				nCount++;
				//�ݹ�Ѱ��-�浽������ҵ������������߷�
				CommonCode(i,j,position[i][j],0,0,nSide,position,nCount);
				//��������ӵ��߷���¼�����������������ɱ�������߷�
				NodeCount(nCount);
			}
		}
	}
	for(int i=0;i<10;i++)
			for(int j=0;j<10;j++)
				if(3 == position[i][j])
					position[i][j] = 0;
	if(true == PangDuan_ShaShangli())//����ɱ��
	{   //�������ӿ���ɱ
		for(int i=0;i<m_MaxLen;i++)
		{   //ÿ�����Ӳ�ͬɱ��
			for(int j=0;j<m_lianJ[m_MaxPos[i]];j++)
			{   //װ��ȥ
				AddMove_ShaQi(i,j,nSide,nPly,nMethod);
			}
		}
	}
	else                       //����ɱ��
	{
		nCount = -1;
		int kongge1 = 0;//����
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<10;j++)
			{
				if(nSide == position[i][j])
				{
					nCount++;
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
					if(nSide == WHITE)
					{
						if(i<9 && j>0 && 0==position[i+1][j-1])//����
						{
							AddMove_BuJv(nCount,nSide,Qi,Left_Down,nPly,nMethod);
						}
						if(i<9 && j<9 && 0==position[i+1][j+1])//����
						{
							AddMove_BuJv(nCount,nSide,Qi,Right_Down,nPly,nMethod);
						}
					}
					if(nSide == BLACK)
					{
						if(i>0 && j>0 && 0==position[i-1][j-1])//����
						{
							AddMove_BuJv(nCount,nSide,Qi,Left_Up,nPly,nMethod);
						}
						if(i>0 && j<9 && 0==position[i-1][j+1])//����
						{
							AddMove_BuJv(nCount,nSide,Qi,Right_Up,nPly,nMethod);
						}
					}
				}
				else if(WANG*nSide == position[i][j])
				{
					nCount++;
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;

					kongge1 = ZuoXia(i,j,position);//����
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Left_Down,nPly,nMethod,k);
					}
					kongge1 = YouXia(i,j,position);//����
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Right_Down,nPly,nMethod,k);
					}
					kongge1 = ZuoShang(i,j,position);//����
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Left_Up,nPly,nMethod,k);
					}
					kongge1 = YouShang(i,j,position);//����
					for(int k=0;k<kongge1;k++)
					{
						AddMove_BuJv(nCount,nSide,WANG,Right_Up,nPly,nMethod,k);
					}
				}
			}
		}
	}
	return nMethod;
}
//���ں��������õ��ĺ�������
void CMoveGenerator::CommonCode(/*λ��*/int i,int j,/*��ֵ*/int value,/*���*/int shu,/*��һ��ɱ�巽��*/int fangxiang,/*��һ��*/int TURN,int Board[10][10],/*�ڼ�������*/int nCount)
{
	int kongge1=0;//��¼�ո���������
	int kongge2=0;
	int m=0;      //����ѭ�����õ�
	int ntype;  //��¼���Ե�����ֵ
	if(TURN == value)
	{
		m_type[nCount] = Qi;//��¼��ͨ����
		if(j>=2 && i<=7 && Right_Up!=fangxiang)//������������ �߽�
		{   //������·��ǶԷ��ģ�������������
			if((Board[i+1][j-1]==(-1*TURN) || Board[i+1][j-1]==(-1*WANG*TURN)) && 0==Board[i+2][j-2])
			{
				if(-1 != nCount)
				{   //��һ�ν�����¼�������꣬�Ժ���벻�ڼ�¼
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+1][j-1]; //��¼����
				addLike(&m_pHead,&m_pEnd,shu+1,Left_Down);
				Move_z_x_x(i,j,Board);
				CommonCode(i+2,j-2,1*TURN,shu+1,Left_Down,TURN,Board);
				MoveF_y_s_s (i+2,j-2,ntype,Board);
			}
		}
		if(j<=7 && i<=7 && Left_Up!=fangxiang)//������������
		{
			if((Board[i+1][j+1]==(-1*TURN) || Board[i+1][j+1]==(-1*WANG*TURN))&&0==Board[i+2][j+2])//������Ϸ��ǶԷ��ģ�������������
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+1][j+1];
				addLike(&m_pHead,&m_pEnd,shu+1,Right_Down);
				Move_y_x_x(i,j,Board);
				CommonCode(i+2,j+2,1*TURN,shu+1,Right_Down,TURN,Board);
				MoveF_z_s_s (i+2,j+2,ntype,Board);	
			}
		}
		if(j>=2 && i>=2 && Right_Down!=fangxiang)//������������
		{
			if((Board[i-1][j-1]==(-1*TURN) || Board[i-1][j-1]==(-1*WANG*TURN))&&0==Board[i-2][j-2])//������Ϸ��ǶԷ��ģ�������������
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-1][j-1];
				addLike(&m_pHead,&m_pEnd,shu+1,Left_Up);
				Move_z_s_s(i,j,Board);
				CommonCode(i-2,j-2,1*TURN,shu+1,Left_Up,TURN,Board);
				MoveF_y_x_x (i-2,j-2,ntype,Board);
			}
		}
		if( j<=7 && i>=2 && Left_Down!=fangxiang)//������������
		{
			if((Board[i-1][j+1]==(-1*TURN) || Board[i-1][j+1]==(-1*WANG*TURN))&&0==Board[i-2][j+2])//������Ϸ��ǶԷ��ģ�������������
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-1][j+1];
				addLike(&m_pHead,&m_pEnd,shu+1,Right_Up);
				Move_y_s_s(i,j,Board);
				CommonCode(i-2,j+2,1*TURN,shu+1,Right_Up,TURN,Board);
				MoveF_z_x_x (i-2,j+2,ntype,Board);
			}
		}
	}
	if(2*TURN==value)
	{
		m_type[nCount] = WANG;//��¼����
		if(j>=2 && i<=7 && Right_Up!=fangxiang)//������������
		{
			kongge1=ZuoXia (i,j,Board);
			kongge2=ZuoXia (i+kongge1+1,j-kongge1-1,Board);
			//������Ϸ��ǶԷ��ģ�������������
			if(kongge2>0&& ((-1*Qi*TURN)==Board[i+kongge1+1][j-kongge1-1] || (-1*WANG*TURN)==Board[i+kongge1+1][j-kongge1-1]))
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+kongge1+1][j-kongge1-1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Left_Down,kongge1,m);
					Move_z_x_x_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i+kongge1+m+1,j-kongge1-m-1,2*TURN,shu+1,Left_Down,TURN,Board);
					MoveF_y_s_s_Wang(i+kongge1+m+1,j-kongge1-m-1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
		if(j<=7 && i<=7 && Left_Up!=fangxiang)//������������
		{
			kongge1=YouXia (i,j,Board);
			kongge2=YouXia (i+kongge1+1,j+kongge1+1,Board);
			if(kongge2>0&&((-1*Qi*TURN)==Board[i+kongge1+1][j+kongge1+1] || (-1*WANG*TURN)==Board[i+kongge1+1][j+kongge1+1]))//������Ϸ��ǶԷ��ģ�������������
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i+kongge1+1][j+kongge1+1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Right_Down,kongge1,m);
					Move_y_x_x_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i+kongge1+m+1,j+kongge1+m+1,2*TURN,shu+1,Right_Down,TURN,Board);
					MoveF_z_s_s_Wang(i+kongge1+m+1,j+kongge1+m+1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
		if(j>=2 && i>=2 && Right_Down!=fangxiang)//������������
		{
			kongge1=ZuoShang (i,j,Board);
			kongge2=ZuoShang (i-kongge1-1,j-kongge1-1,Board);
			if(kongge2>0&&((-1*Qi*TURN)==Board[i-kongge1-1][j-kongge1-1] ||(-1*WANG*TURN)==Board[i-kongge1-1][j-kongge1-1]))//������Ϸ��ǶԷ��ģ�������������
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-kongge1-1][j-kongge1-1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Left_Up,kongge1,m);
					Move_z_s_s_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i-kongge1-m-1,j-kongge1-m-1,2*TURN,shu+1,Left_Up,TURN,Board);
					MoveF_y_x_x_Wang(i-kongge1-m-1,j-kongge1-m-1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
		if(j<=7 && i>=2 && Left_Down!=fangxiang)//������������
		{
			kongge1=YouShang (i,j,Board);
			kongge2=YouShang (i-kongge1-1,j+kongge1+1,Board);
			if(kongge2>0&&((-1*Qi*TURN)==Board[i-kongge1-1][j+kongge1+1] ||(-1*WANG*TURN)==Board[i-kongge1-1][j+kongge1+1]))//������Ϸ��ǶԷ��ģ�������������
			{
				if(-1 != nCount)
				{
					m_qi[nCount].x = j;
					m_qi[nCount].y = i;
				}
				ntype = Board[i-kongge1-1][j+kongge1+1];
				for(m=1;m<=kongge2;m++)
				{
					addLike(&m_pHead,&m_pEnd,shu+1,Right_Up,kongge1,m);
					Move_y_s_s_Wang(i,j,kongge1,m,TURN,Board);
					CommonCode(i-kongge1-m-1,j+kongge1+m+1,2*TURN,shu+1,Right_Up,TURN,Board);
					MoveF_z_x_x_Wang(i-kongge1-m-1,j+kongge1+m+1,kongge1,m,TURN,ntype,Board);
				}
			}
		}
	}
}
//�������.����ʱ���ǿ���ɱ20��
void CMoveGenerator::NodeCount(int nCount)//value�Ǳ�ʾ��ǰ����ͨ������
{
	int l,lianMax=0,lianI=0;
	int MaxPos[600]={0};
	int chu1[600]={0};//���
	int chu2[600]={0};//��ȶ�Ӧ���巽��
	int kong1[600]={0};//��ȶ�Ӧ������֮��ո�������������
	int kong2[600]={0};//��ȶ�Ӧ�Է����Ӻ�ո�������������
	Node_YouSha *pp=NULL;
	//����ѭ������ֵ����һ������
	if(m_pHead != NULL)
	{
		//��������0 
		lianMax=0;
		for(l=0;l<600;l++)
		{
			chu1[l]=0;
			chu2[l]=0;
			kong1[l]=0;
			kong2[l]=0;
			MaxPos[l]=0;
		}
		l=0;
		pp = m_pHead;
		while(pp != NULL)
		{
			chu1[l]=pp->shendu ;
			chu2[l]=pp->tongdao ;
			kong1[l]=pp->kongge1 ;
			kong2[l]=pp->kongge2 ;
			l++;
			pp = pp->pNext ;
		}
		//�ͷ�����
		ClearLike(&m_pHead);
		m_pHead=NULL;
		m_pEnd=NULL;
		lianMax = chu1[0];
		for(lianI=0;lianI<l;lianI++)
		{//�������ݴ����ҵ�����ֵ
			if(lianMax < chu1[lianI])
			{//�ҵ���󣬼�¼������
				lianMax = chu1[lianI];
			}
		}
		m_lianJ[nCount] = 0;//�߷���ʼ��Ϊ0
		for(lianI=0;lianI<l;lianI++)
		{//����������нڵ�
			if(lianMax == chu1[lianI])
			{//�ҵ���������λ�ã���¼λ�ã��߷�+1
				MaxPos[m_lianJ[nCount]]=lianI;
				m_lianJ[nCount]++;
			}
		}
		//��ֵ�ŵ�����������
		for(lianI=0;lianI<m_lianJ[nCount];lianI++)
		{
			switch(lianMax)
			{
			case 1:
				CommonFind(1,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 2:
				CommonFind(2,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 3:
				CommonFind(3,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 4:
				CommonFind(4,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 5:
				CommonFind(5,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 6:
				CommonFind(6,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 7:
				CommonFind(7,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 8:
				CommonFind(8,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 9:
				CommonFind(9,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 10:
				CommonFind(10,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 11:
				CommonFind(11,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 12:
				CommonFind(12,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 13:
				CommonFind(13,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 14:
				CommonFind(14,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 15:
				CommonFind(15,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 16:
				CommonFind(16,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 17:
				CommonFind(17,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 18:
				CommonFind(18,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 19:
				CommonFind(19,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			case 20:
				CommonFind(20,nCount,lianI,chu1,chu2,MaxPos,kong1,kong2);
				break;
			}
		}	
	}
}
void CMoveGenerator::CommonFind(int depth,int nCount,int lianI,int chu1[600],int chu2[600],int MaxPos[600],int kong1[600],int kong2[600])
{
	m_count[nCount] = depth;//2 ��¼���
	m_step[nCount][lianI][depth-1]=chu2[MaxPos[lianI]];//��¼����
	m_step_king_kongge1[nCount][lianI][depth-1]= kong1[MaxPos[lianI]];//��¼�ո�
	m_step_king_kongge2[nCount][lianI][depth-1]= kong2[MaxPos[lianI]];
	for(int m=0;m<depth-1;m++)
	{
		for(int k=MaxPos[lianI];k>=0;k--)
		{
			if(depth-1-m == chu1[k-1])
			{
				m_step[nCount][lianI][depth-2-m]=chu2[k-1];
				m_step_king_kongge1[nCount][lianI][depth-2-m]= kong1[k-1];
				m_step_king_kongge2[nCount][lianI][depth-2-m]= kong2[k-1];
				MaxPos[lianI] = k-1;//Ѱ��λ���Ѿ�ǰ�ƣ����¸�ֵ
				break;
			}
			MaxPos[lianI] = k-1;////Ѱ��λ���Ѿ�ǰ�ƣ����¸�ֵ
		}
	}
}
//�ж�ɱ����,ɱ������������
bool CMoveGenerator::PangDuan_ShaShangli()
{
	short max = 0;
	m_MaxLen = 0;
	max = m_count[0];
	//�ҵ����ɱ����max
	for(int i=1;i<20;i++)
	{
		if(m_count[i] > max)
		{
			max = m_count[i];
		}
	}
	if(0 == max)//����ɱ��
	{
		return false;
	}//������Ϣ����¼�ĸ����ӿ���ɱ�����
	for(int i=0;i<20;i++)
	{
		if(m_count[i] == max)
		{
			m_MaxPos[m_MaxLen] = i;
			m_MaxLen++;
			//break;
		}
	}
	return true;
}
//����һ���߷� ���� ɱ��
void CMoveGenerator::AddMove_BuJv(int nCount,int nSide,int nType,int mFangXiang,int nPly,int& nMethod,int nKongge)
{
	CHESSMOVE move;
	move.From = m_qi[nCount];
	move.m_Count = 0;
	move.m_Side = nSide;
	move.m_Type = nType;
	move.step[0] = mFangXiang;
	move.step_king_kongge1[0] = nKongge;
	m_nMoveList[nPly][nMethod] = move;
	nMethod++;
}
void CMoveGenerator::AddMove_ShaQi(int i,int j,int nSide,int nPly,int& nMethod)
{
	CHESSMOVE move;
	move.From = m_qi[m_MaxPos[i]];
	move.m_Count = m_count[m_MaxPos[i]];
	move.m_Side = nSide;
	move.m_Type = m_type[m_MaxPos[i]];
	for(int k=0;k<m_count[m_MaxPos[i]];k++)
	{
		move.step[k] = m_step[m_MaxPos[i]][j][k]; 
		move.step_king_kongge1[k] = m_step_king_kongge1[m_MaxPos[i]][j][k]; 
		move.step_king_kongge2[k] = m_step_king_kongge2[m_MaxPos[i]][j][k]; 
	}
	m_nMoveList[nPly][nMethod] = move;
	nMethod++;
}
//ģ��ɱ��
int CMoveGenerator::Move_z_x_x(int i,int j,int Board[10][10]) 
{
	int ntype = Board[i+1][j-1];
	Board[i+2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j-1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_z_x_x(int i,int j,int n,int Board[10][10]) 
{
	Board[i+2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j-1]=n;
}
int CMoveGenerator::Move_z_s_s(int i,int j,int Board[10][10])
{
	int ntype = Board[i-1][j-1];
	Board[i-2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j-1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_z_s_s(int i,int j,int n,int Board[10][10]) 
{
	Board[i-2][j-2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j-1]=n;
}
int CMoveGenerator::Move_y_x_x(int i,int j,int Board[10][10]) 
{
	int ntype = Board[i+1][j+1];
	Board[i+2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j+1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_y_x_x(int i,int j,int n,int Board[10][10]) 
{
	Board[i+2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i+1][j+1]=n;
}
int CMoveGenerator::Move_y_s_s(int i,int j,int Board[10][10]) 
{
	int ntype = Board[i-1][j+1];
	Board[i-2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j+1]=0;
	return ntype;
}
void CMoveGenerator::MoveF_y_s_s(int i,int j,int n,int Board[10][10])
{
	Board[i-2][j+2]=Board[i][j];
	Board[i][j]=0;
	Board[i-1][j+1]=n;
}
int CMoveGenerator::Move_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) 
{
	int ntype = Board[i+kongge1+1][j-kongge1-1];
	Board[i][j]=0;
	Board[i+kongge1+1][j-kongge1-1]=3;
	Board[i+kongge1+m+1][j-kongge1-m-1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_z_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) 
{
	Board[i][j]=0;
	Board[i+m][j-m]=ntype;
	Board[i+kongge1+m+1][j-kongge1-m-1]=2*TURN;
}
int CMoveGenerator::Move_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10])
{
	int ntype = Board[i-kongge1-1][j-kongge1-1];
	Board[i][j]=0;
	Board[i-kongge1-1][j-kongge1-1]=3;
	Board[i-kongge1-m-1][j-kongge1-m-1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_z_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) 
{
	Board[i][j]=0;
	Board[i-m][j-m]=ntype;
	Board[i-kongge1-m-1][j-kongge1-m-1]=2*TURN;
}
int CMoveGenerator::Move_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) 
{
	int ntype = Board[i+kongge1+1][j+kongge1+1];
	Board[i][j]=0;
	Board[i+kongge1+1][j+kongge1+1]=3;
	Board[i+kongge1+m+1][j+kongge1+m+1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_y_x_x_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10]) 
{
	Board[i][j]=0;
	Board[i+m][j+m]=ntype;
	Board[i+kongge1+m+1][j+kongge1+m+1]=2*TURN;
}
int CMoveGenerator::Move_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int Board[10][10]) 
{
	int ntype = Board[i-kongge1-1][j+kongge1+1];
	Board[i][j]=0;
	Board[i-kongge1-1][j+kongge1+1]=3;
	Board[i-kongge1-m-1][j+kongge1+m+1]=2*TURN;
	return ntype;
}
void CMoveGenerator::MoveF_y_s_s_Wang(int i,int j,int kongge1,int m,int TURN,int ntype,int Board[10][10])
{
	Board[i][j]=0;
	Board[i-m][j+m]=ntype;
	Board[i-kongge1-m-1][j+kongge1+m+1]=2*TURN;
}
//�ҳ��ĸ�����Ŀո���
BYTE CMoveGenerator::ZuoShang(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(0>=x||0>=y)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x-i][y-i])
		{
			return i-1;
		}
		if(0==x-i||0==y-i)
		{
			return i;
		}
	}
}
BYTE CMoveGenerator::YouShang(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(0>=x||9<=y)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x-i][y+i])
		{
			return i-1;
		}
		if(0==x-i||9==y+i)
		{
			return i;
		}
	}
}
BYTE CMoveGenerator::ZuoXia(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(9<=x||0>=y)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x+i][y-i])
		{
			return i-1;
		}
		if(9==x+i||0==y-i)
		{
			return i;
		}
	}
}
BYTE CMoveGenerator::YouXia(BYTE x,BYTE y,int Board[10][10])
{
	BYTE i=0;
	if(9<=x||9<=y)
	{
		return 0;
	}
	while(1)
	{
		i++;
		if(0!=Board[x+i][y+i])
		{
			return i-1;
		}
		if(9==x+i||9==y+i)
		{
			return i;
		}
	}
}

//��������
void CMoveGenerator::addLike(Node_YouSha **pHead,Node_YouSha **pEnd,int sd,int td,int kg1,int kg2)
{
	Node_YouSha *pNew=(Node_YouSha *)malloc(sizeof(Node_YouSha));
	pNew->shendu=sd;
	pNew->tongdao=td;
	pNew->kongge1 =kg1;
	pNew->kongge2 =kg2;
	pNew->pNext=NULL;
	if(NULL==*pHead)
	{
		*pHead=pNew;
	}else
	{
		(*pEnd)->pNext=pNew;
	}
	*pEnd=pNew;
}
void CMoveGenerator::ClearLike(Node_YouSha **ppHead)
{
	Node_YouSha *pDel=NULL;
	while(*ppHead!=NULL)
	{
		pDel=*ppHead;
		(*ppHead)=(*ppHead)->pNext;
		free(pDel);
	}
}
void CMoveGenerator::ClearData()
{
	m_MaxLen = 0;
	m_nMoveCount = 0;
	for(int i=0;i<20;i++)
	{   
		m_qi[i].x = 0;
		m_qi[i].y = 0;
		m_type[i] = 0;
		m_count[i] = 0;
		m_lianJ[i] = 0;
		m_MaxPos[i] = 0;
		for(int j=0;j<10;j++)
		{
			for(int k=0;k<10;k++)
			{
				m_step[i][j][k]; //������==1 ������==2  ������==3   ������==4
				m_step_king_kongge1[i][j][k];
				m_step_king_kongge2[i][j][k];
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

