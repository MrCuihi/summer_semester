//#include "stdafx.h"
#include "Box.h"

extern struct BASE* g_bases[GAMEBOARD_HEIGHT_GRIDS][GAMEBOARD_WIDTH_GRIDS];
//extern�������ڱ������ߺ���ǰ���Ա�ʾ�������ߺ����Ķ����ڱ���ļ���

//��ʾ�����������˱����ͺ���ʱ������ģ����Ѱ���䶨�塣

//externҲ��������������ָ����

/********************************** �������Ӷ���************************************/
struct BASE* New_Box(enum BASEType type, int x, int y, HDC drawer, struct BASE* below)
{
	TCHAR szImageFilePath[_MAX_PATH] = {0};

	/* �����������*/
	struct BASE* base = New_BASE(type, x, y, drawer);

	/* �����������*/
	struct Box* box = (struct Box*)calloc( 1, sizeof(struct Box) );

	/*��ʼ����������*/
	base->m_child = box;

	/*��ʼ�����ຯ��*/
	base->delete = Box_Delete;
	base->move = Box_Move;

	/* ��ʼ����������*/
	box->m_parent = base;
	box->m_below = below;
	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type +1)*100 + 1);
	box->m_Down2 = (HBITMAP)LoadImage(NULL, szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	box->m_CurrentImage = base->m_Down1;

	return base;
}

/*************************************�ƶ�����**************************************/
enum MoveResult Box_Move(struct BASE* base, UINT Direction)
{
	struct Box* box = (struct Box*)base->m_child;
	enum MoveResult moveResult = NO_MOVE;
	BOOL bLeftFromDest = FALSE;
	int nDestX = base->m_x, nDestY = base->m_y;	// С��Ҫ�ƶ�����λ�õ�����
	BITMAP bmpInfo = {0};

	switch (Direction)
	{
	case VK_LEFT:
		nDestX--;
		break;
	case VK_RIGHT:
		nDestX++;
		break;
	case VK_UP:
		nDestY--;
		break;
	case VK_DOWN:
		nDestY++;
		break;
	}
    /* ����Ա���ǽ�����ӣ���ֱ�ӷ���*/
	if (g_bases[nDestY][nDestX]->m_type == BOX||g_bases[nDestY][nDestX]->m_type == WALL)
	{
		return moveResult;
	}

	moveResult = NORMAL_MOVED;
    /* �ж��Ƿ�Ҫ�뿪Ŀ�ĵ�*/
	if (base->m_type == BOX && box->m_below->m_type == DESTINATION)
	{
		bLeftFromDest = TRUE;
		moveResult = LEFT_FROM_DEST;
		box->m_CurrentImage = base->m_Down1;
	}

	/* �ñ����������Լ�*/
	box->m_below->draw(box->m_below);
	g_bases[base->m_y][base->m_x] = box->m_below;

	/* ����������λ��*/
	base->m_x = nDestX;
	base->m_y = nDestY;

	/* �����µı������*/
	box->m_below = g_bases[base->m_y][base->m_x];

	/* �ж��Ƿ񵽴�Ŀ�ĵ�*/
	if (base->m_type == BOX && box->m_below->m_type == DESTINATION)
	{
		if (bLeftFromDest)
		{
			moveResult = NORMAL_MOVED;
		}
		else
		{
			moveResult = MOVE_TO_DEST ;
		}
		box->m_CurrentImage = box->m_Down2;
	}

	/* �ػ��Լ�*/
	SelectObject(base->m_HDC, box->m_CurrentImage);
	GetObject( box->m_CurrentImage, (int)sizeof(BITMAP), &bmpInfo);
	StretchBlt(base->m_drawer, base->m_x * GRIDS_SIZE, 
		base->m_y * GRIDS_SIZE,GRIDS_SIZE, GRIDS_SIZE,
		base->m_HDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

	/* ���Լ����浽������*/
	g_bases[base->m_y][base->m_x] = base;

	return moveResult;
}
/******************************************ɾ������**********************************/
void Box_Delete(struct BASE* base)
{
	struct Box* box = (struct Box*)base->m_child;

	/* ɾ���������*/
	DeleteObject(box->m_Down2);
	box->m_below->delete(box-> m_below);
	free(box);

	/*ɾ���������*/
	BASE_Delete(base);
}
