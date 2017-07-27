//#include "stdafx.h"
#include "Box.h"

extern struct BASE* g_bases[GAMEBOARD_HEIGHT_GRIDS][GAMEBOARD_WIDTH_GRIDS];
//extern可以置于变量或者函数前，以标示变量或者函数的定义在别的文件中

//提示编译器遇到此变量和函数时在其他模块中寻找其定义。

//extern也可用来进行链接指定。

/********************************** 创建箱子对象************************************/
struct BASE* New_Box(enum BASEType type, int x, int y, HDC drawer, struct BASE* below)
{
	TCHAR szImageFilePath[_MAX_PATH] = {0};

	/* 创建父类对象*/
	struct BASE* base = New_BASE(type, x, y, drawer);

	/* 创建本类对象*/
	struct Box* box = (struct Box*)calloc( 1, sizeof(struct Box) );

	/*初始化父类数据*/
	base->m_child = box;

	/*初始化父类函数*/
	base->delete = Box_Delete;
	base->move = Box_Move;

	/* 初始化本类数据*/
	box->m_parent = base;
	box->m_below = below;
	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type +1)*100 + 1);
	box->m_Down2 = (HBITMAP)LoadImage(NULL, szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	box->m_CurrentImage = base->m_Down1;

	return base;
}

/*************************************移动箱子**************************************/
enum MoveResult Box_Move(struct BASE* base, UINT Direction)
{
	struct Box* box = (struct Box*)base->m_child;
	enum MoveResult moveResult = NO_MOVE;
	BOOL bLeftFromDest = FALSE;
	int nDestX = base->m_x, nDestY = base->m_y;	// 小人要移动到的位置的坐标
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
    /* 如果旁边是墙或箱子，则直接返回*/
	if (g_bases[nDestY][nDestX]->m_type == BOX||g_bases[nDestY][nDestX]->m_type == WALL)
	{
		return moveResult;
	}

	moveResult = NORMAL_MOVED;
    /* 判断是否要离开目的地*/
	if (base->m_type == BOX && box->m_below->m_type == DESTINATION)
	{
		bLeftFromDest = TRUE;
		moveResult = LEFT_FROM_DEST;
		box->m_CurrentImage = base->m_Down1;
	}

	/* 用背后对象擦除自己*/
	box->m_below->draw(box->m_below);
	g_bases[base->m_y][base->m_x] = box->m_below;

	/* 计算新坐标位置*/
	base->m_x = nDestX;
	base->m_y = nDestY;

	/* 设置新的背后对象*/
	box->m_below = g_bases[base->m_y][base->m_x];

	/* 判断是否到达目的地*/
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

	/* 重绘自己*/
	SelectObject(base->m_HDC, box->m_CurrentImage);
	GetObject( box->m_CurrentImage, (int)sizeof(BITMAP), &bmpInfo);
	StretchBlt(base->m_drawer, base->m_x * GRIDS_SIZE, 
		base->m_y * GRIDS_SIZE,GRIDS_SIZE, GRIDS_SIZE,
		base->m_HDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

	/* 将自己保存到数组中*/
	g_bases[base->m_y][base->m_x] = base;

	return moveResult;
}
/******************************************删除箱子**********************************/
void Box_Delete(struct BASE* base)
{
	struct Box* box = (struct Box*)base->m_child;

	/* 删除本类对象*/
	DeleteObject(box->m_Down2);
	box->m_below->delete(box-> m_below);
	free(box);

	/*删除父类对象*/
	BASE_Delete(base);
}
