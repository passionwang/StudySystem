#include "stdafx.h"
#include "MakeMove.h"

CMakeMove::CMakeMove()
{

}
CMakeMove::~CMakeMove()
{

}
void CMakeMove::MakeMoveQi(CHESSMOVE* move,int CurPosition[10][10])
{
	if(1 == move->m_Type)
	{
		for(int i=0;i<move->m_Count;i++)
		{
			switch(move->step[i])
			{
			case Left_Down:
				CurPosition[move->From.y+2][move->From.x-2] = -5;
				if(-2 == CurPosition[move->From.y+1][move->From.x-1])
				{
					CurPosition[move->From.y+1][move->From.x-1] = -4;
				}
				else
				{
					CurPosition[move->From.y+1][move->From.x-1] = 4;
				}
				move->From.y += 2;
				move->From.x -= 2;
				break;
			case Right_Down:
				CurPosition[move->From.y+2][move->From.x+2] = -5;
				if(-2 == CurPosition[move->From.y+1][move->From.x+1])
				{
					CurPosition[move->From.y+1][move->From.x+1] = -4;
				}
				else
				{
					CurPosition[move->From.y+1][move->From.x+1] = 4;
				}
				move->From.y += 2;
				move->From.x += 2;
				break;
			case Left_Up:
				CurPosition[move->From.y-2][move->From.x-2] = -5;
				if(-2 == CurPosition[move->From.y-1][move->From.x-1])
				{
					CurPosition[move->From.y-1][move->From.x-1] = -4;
				}
				else
				{
					CurPosition[move->From.y-1][move->From.x-1] = 4;
				}
				move->From.y -= 2;
				move->From.x -= 2;
				break;
			case Right_Up:
				CurPosition[move->From.y-2][move->From.x+2] = -5;
				if(-2 == CurPosition[move->From.y-1][move->From.x+1])
				{
					CurPosition[move->From.y-1][move->From.x+1] = -4;
				}
				else
				{
					CurPosition[move->From.y-1][move->From.x+1] = 4;
				}
				move->From.y -= 2;
				move->From.x += 2;
				break;
			}
		}
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y+1][move->From.x-1] = 5; 
				move->From.y += 1;
				move->From.x -= 1;
				break;
			case Right_Down:
				CurPosition[move->From.y+1][move->From.x+1] = 5; 
				move->From.y += 1;
				move->From.x += 1;
				break;
			case Left_Up:
				CurPosition[move->From.y-1][move->From.x-1] = 5; 
				move->From.y -= 1;
				move->From.x -= 1;
				break;
			case Right_Up:
				CurPosition[move->From.y-1][move->From.x+1] = 5; 
				move->From.y -= 1;
				move->From.x += 1;
				break;
			}
		}
		else
		{
			CurPosition[move->From.y][move->From.x] = 5;
		}
	}
	else
	{
		for(int i=0;i<move->m_Count;i++)
		{
			switch(move->step[i])
			{
			case Left_Down:
				CurPosition[move->From.y + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]][move->From.x - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1])
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] = -4;
				}
				else
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] = 4;
				}
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]][move->From.x + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x + move->step_king_kongge1[i] + 1])
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x + move->step_king_kongge1[i] + 1] = -4;
				}
				else
				{
					CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x + move->step_king_kongge1[i] + 1] = 4;
				}
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]][move->From.x - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x - move->step_king_kongge1[i] - 1])
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x - move->step_king_kongge1[i] - 1] = -4;
				}
				else
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x - move->step_king_kongge1[i] - 1] = 4;
				}
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y - move->step_king_kongge1[i] - 1 - move->step_king_kongge2[i]][move->From.x + move->step_king_kongge1[i] + 1 + move->step_king_kongge2[i]] = -5;
				if(-4 == CurPosition[move->From.y + move->step_king_kongge1[i] + 1][move->From.x - move->step_king_kongge1[i] - 1] || -2 == CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x + move->step_king_kongge1[i] + 1])
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x + move->step_king_kongge1[i] + 1] = -4;
				}
				else
				{
					CurPosition[move->From.y - move->step_king_kongge1[i] - 1][move->From.x + move->step_king_kongge1[i] + 1] = 4;
				}
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			}
		}
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = 5; 
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = 5; 
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = 5; 
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = 5; 
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			}
		}
		else
		{
			CurPosition[move->From.y][move->From.x] = 5;
		}
	}
}
void CMakeMove::MakeMoveSure(int m,int n,int nType,int CurPosition[10][10])
{
	CurPosition[m][n] = 0;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			switch(CurPosition[i][j])
			{
			case 5:
				CurPosition[i][j] = nType;
				break;
			case -5:
			case 4:
			case -4:
			case 6:
			case -6:
				CurPosition[i][j] = 0;
				break;
			}
		}
	}
}
void CMakeMove::UnMakeMove(int CurPosition[10][10])
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			switch(CurPosition[i][j])
			{
			case 5:
			case -5:
				CurPosition[i][j] = 0;
				break;
			case 4:
				CurPosition[i][j] = -1;
				break;
			case -4:
				CurPosition[i][j] = -2;
				break;
			case 6:
				CurPosition[i][j] = 1;
				break;
			case -6:
				CurPosition[i][j] = 2;
				break;
			}
		}
	}
}
void CMakeMove::BecomeKing(int CurPosition[10][10])
{
	for(int i=0;i<10;i++)
	{
		if(1 == CurPosition[0][i])
		{
			CurPosition[0][i] = 2;
		}
		if(-1 == CurPosition[9][i])
		{
			CurPosition[9][i] = -2;
		}
	}
}


void CMakeMove::MakeMove1(CHESSMOVE* move,int type,int CurPosition[10][10])
{
	//CurPosition改变数组
	if(Qi == move->m_Type)
	{   //吃子
		MakeMoveQi1(move,CurPosition,type);
	}
	else
	{
		MakeMoveWANG1(move,CurPosition,type);
	}
}

void CMakeMove::UnMakeMove1(CHESSMOVE* move,int CurPosition[10][10])
{
	//CurPosition改变数组
	if(Qi == move->m_Type)
	{
		UnMakeMoveQi1(move,CurPosition);
	}
	else
	{
		UnMakeMoveWANG1(move,CurPosition);
	}
}

void CMakeMove::MakeMoveQi1(CHESSMOVE* move,int CurPosition[10][10],int type)
{
	for(int i=0;i<move->m_Count;i++)
	{
		switch(move->step[i])
		{
		case Left_Down:
			m_ntype[i] = m_pMG1.Move_z_x_x(move->From.y,move->From.x,CurPosition);
			move->From.y += 2;
			move->From.x -= 2;
			break;
		case Right_Down:
			m_ntype[i] = m_pMG1.Move_y_x_x(move->From.y,move->From.x,CurPosition);
			move->From.y += 2;
			move->From.x += 2;
			break;
		case Left_Up:
			m_ntype[i] = m_pMG1.Move_z_s_s(move->From.y,move->From.x,CurPosition);
			move->From.y -= 2;
			move->From.x -= 2;
			break;
		case Right_Up:
			m_ntype[i] = m_pMG1.Move_y_s_s(move->From.y,move->From.x,CurPosition);
			move->From.y -= 2;
			move->From.x += 2;
			break;
		}
	}
	
	if(0 == move->m_Count)//没有吃子，只有行棋
	{
		switch(move->step[0])
		{
		case Left_Down:
			CurPosition[move->From.y+1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y += 1;
			move->From.x -= 1;
			break;
		case Right_Down:
			CurPosition[move->From.y+1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y += 1;
			move->From.x += 1;
			break;
		case Left_Up:
			CurPosition[move->From.y-1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y -= 1;
			move->From.x -= 1;
			break;
		case Right_Up:
			CurPosition[move->From.y-1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
			CurPosition[move->From.y][move->From.x] = 0;
			move->From.y -= 1;
			move->From.x += 1;
			break;
		}
		if(0 != type && 9 != move->From.y && -1 == move->m_Side)
		{
			CurPosition[move->From.y][move->From.x] = 4;
		}
		if(0 != type && 0 != move->From.y && 1 == move->m_Side)
		{
			CurPosition[move->From.y][move->From.x] = 6;
		}
	}
	else
	{
		if(0 != type && 9 != move->From.y&& -1 == move->m_Side)
		{
			CurPosition[move->From.y][move->From.x] = 4;
		}
		if(0 != type && 0 != move->From.y&& 1 == move->m_Side)
		{
			CurPosition[move->From.y][move->From.x] = 6;
		}
	}
	BecomeKing(CurPosition);
}
void CMakeMove::UnMakeMoveQi1(CHESSMOVE* move,int CurPosition[10][10])
{
	if(TRUE == m_bIsBecomeKing)
	{
		CurPosition[move->From.y][move->From.x] = move->m_Side;
		m_bIsBecomeKing = FALSE;
	}
	for(int i=move->m_Count-1;i>=0;i--)
		{
			switch(move->step[i])
			{
			case Left_Down:
				m_pMG1.MoveF_y_s_s(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y -= 2;
				move->From.x += 2;
				break;
			case Right_Down:
				m_pMG1.MoveF_z_s_s(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y -= 2;
				move->From.x -= 2;
				break;
			case Left_Up:
				m_pMG1.MoveF_y_x_x(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y += 2;
				move->From.x += 2;
				break;
			case Right_Up:
				m_pMG1.MoveF_z_x_x(move->From.y,move->From.x,m_ntype[i],CurPosition);
				move->From.y += 2;
				move->From.x -= 2;
				break;
			}
		}
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y-1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= 1;
				move->From.x += 1;
				break;
			case Right_Down:
				CurPosition[move->From.y-1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= 1;
				move->From.x -= 1;
				break;
			case Left_Up:
				CurPosition[move->From.y+1][move->From.x+1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += 1;
				move->From.x += 1;
				break;
			case Right_Up:
				CurPosition[move->From.y+1][move->From.x-1] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += 1;
				move->From.x -= 1;
				break;
			}
		}
}
void CMakeMove::MakeMoveWANG1(CHESSMOVE* move,int CurPosition[10][10],int type)
{
	for(int i=0;i<move->m_Count;i++)
		{
			switch(move->step[i])
			{
			case Left_Down:
				m_ntype[i] = m_pMG1.Move_z_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Down:
				m_ntype[i] = m_pMG1.Move_y_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Left_Up:
				m_ntype[i] = m_pMG1.Move_z_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Up:
				m_ntype[i] = m_pMG1.Move_y_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			}
		}
		for(int i=0;i<10;i++)
			for(int j=0;j<10;j++)
				if(3 == CurPosition[i][j])
					CurPosition[i][j] = 0;
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			}
			if(0 != type && -1 == move->m_Side)
			{
				CurPosition[move->From.y][move->From.x] = -4;
			}
			if(0 != type && 1 == move->m_Side)
			{
				CurPosition[move->From.y][move->From.x] = -6;
			}
		}
		else
		{
			if(0 != type && -1 == move->m_Side)
			{
				CurPosition[move->From.y][move->From.x] = -4;
			}
			if(0 != type && 1 == move->m_Side)
			{
				CurPosition[move->From.y][move->From.x] = -6;
			}
		}
}
void CMakeMove::UnMakeMoveWANG1(CHESSMOVE* move,int CurPosition[10][10])
{
	for(int i=move->m_Count-1;i>=0;i--)
		{
			switch(move->step[i])
			{
			case Left_Down:
				m_pMG1.MoveF_y_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Down:
				m_pMG1.MoveF_z_s_s_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Left_Up:
				m_pMG1.MoveF_y_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			case Right_Up:
				m_pMG1.MoveF_z_x_x_Wang(move->From.y,move->From.x,move->step_king_kongge1[i],move->step_king_kongge2[i],move->m_Side,m_ntype[i],CurPosition);
				move->From.y += (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				move->From.x -= (move->step_king_kongge1[i] + move->step_king_kongge2[i] + 1);
				break;
			}
		}
		if(0 == move->m_Count)//没有吃子，只有行棋
		{
			switch(move->step[0])
			{
			case Left_Down:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Right_Down:
				CurPosition[move->From.y-1-move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y -= (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			case Left_Up:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x+1+move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x += (move->step_king_kongge1[0] + 1);
				break;
			case Right_Up:
				CurPosition[move->From.y+1+move->step_king_kongge1[0]][move->From.x-1-move->step_king_kongge1[0]] = CurPosition[move->From.y][move->From.x]; 
				CurPosition[move->From.y][move->From.x] = 0;
				move->From.y += (move->step_king_kongge1[0] + 1);
				move->From.x -= (move->step_king_kongge1[0] + 1);
				break;
			}
		}
}
