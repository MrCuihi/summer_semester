#include "stdafx.h"
#include "Man.h"
#include "GameBoard.h"

extern struct BASE* g_bases[GAMEBOARD_HEIGHT_GRIDS][GAMEBOARD_WIDTH_GRIDS];

/****************************************创建人物****************************************/
struct BASE* New_Man(enum BASEType type, int x, int y,
	HDC drawer, HWND gameBoard, struct BASE* below/*背后对象*/)
{
	TCHAR szImageFilePath[_MAX_PATH] = {0};

	/*创建创建父类对象*/
	struct BASE* base = New_Box(type, x, y, drawer, below);
	struct Box* box = (struct Box*)(base->m_child);

	/*创建本类对象*/
	struct Man* man = (struct Man*)calloc( 1, sizeof(struct Man) );

	/*初始化父类数据*/
	box->m_child = man;

	// 初始化父类函数
	base->delete = Man_Delete;

	// 初始化本类数据
	man->m_parent = box;
	man->m_gameBoard = gameBoard;
	man->m_image1 = base->m_Down1;
	man->m_image2 = ( (struct Box*)(base->m_child) )->m_Down2;
	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type + 1) * 100 + 2);
	man->m_Up1 = (HBITMAP)LoadImage(NULL,szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type + 1) * 100 + 3);
	man->m_Up2 = (HBITMAP)LoadImage(NULL,szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type + 1) * 100 + 4);
	man->m_Left1 = (HBITMAP)LoadImage(NULL,szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type + 1) * 100 + 5);
	man->m_Left2 = (HBITMAP)LoadImage(NULL,szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type + 1) * 100 + 6);
	man->m_Right1 = (HBITMAP)LoadImage(NULL,szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type + 1) * 100 + 7);
	man->m_Right2 = (HBITMAP)LoadImage(NULL,szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	/* 初始化本类函数*/
	man->move = Man_Move;
	man->dancing = Man_Dancing;

	return base;
}


/***********************************************移动人物***************************************/
static enum MoveResult Man_Move(struct Man* man, UINT Direction)
{
	enum MoveResult manMoveResult = NO_MOVE;	// 小人移动结果
	enum MoveResult boxMoveResult = NO_MOVE;	// 箱子移动结果
	struct Box* box = man->m_parent;
	struct BASE* base = box->m_parent;

	int nDestX = base->m_x, nDestY = base->m_y;		// 新位置的坐标

	/* 根据方向，计算新位置的坐标和图像*/
	switch (Direction)
	{
	case VK_LEFT:
		nDestX--;
		man->m_image1 = man->m_Left1;
		man->m_image2 = man->m_Left2;
		break;
	case VK_RIGHT:
		nDestX++;
		man->m_image1 = man->m_Right1;
		man->m_image2 = man->m_Right2;
		break;
	case VK_UP:
		nDestY--;
		man->m_image1 = man->m_Up1;
		man->m_image2 = man->m_Up2;
		break;
	case VK_DOWN:
		nDestY++;
		man->m_image1 = base->m_Down1;
		man->m_image2 = box->m_Down2;
		break;
	}

	
	box->m_CurrentImage = man->m_image1;// 重新设置图像

	/* 先移动人物旁边的基本元素*/
	boxMoveResult = g_bases[nDestY][nDestX]->move(g_bases[nDestY][nDestX], Direction);
	if (boxMoveResult == NO_MOVE)
	{
		return boxMoveResult;
	}

	/* 移动小人*/
	manMoveResult = Box_Move(base, Direction);
	if (boxMoveResult != NO_MOVE)
	{
		return boxMoveResult;
	}

	return manMoveResult;
}

/*****************************************人物跳舞***************************************/
static void Man_Dancing(struct Man* man)
{
	struct Box* box = man->m_parent;
	struct BASE* base = box->m_parent;
	BITMAP bmpInfo = {0};

	if (box->m_CurrentImage == man->m_image1)
	{
		box->m_CurrentImage = man->m_image2;
	}
	else
	{
		box->m_CurrentImage = man->m_image1;
	}
	SelectObject(base->m_HDC, box->m_CurrentImage);
	GetObject( box->m_CurrentImage, (int)sizeof(BITMAP), &bmpInfo );
	StretchBlt(base->m_drawer, base->m_x * GRIDS_SIZE, base->m_y * GRIDS_SIZE,
		GRIDS_SIZE, GRIDS_SIZE, base->m_HDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	InvalidateRect(man->m_gameBoard, NULL, FALSE);
}
/*************************************************删除人物************************************/
static void Man_Delete(struct BASE* base)
{
	struct Box* box = (struct Box*)base->m_child;
	struct Man* man = (struct Man*)box->m_child;

	/*删除本类*/
	DeleteObject(man->m_Up1);
	DeleteObject(man->m_Up2);
	DeleteObject(man->m_Left1);
	DeleteObject(man->m_Left2);
	DeleteObject(man->m_Right1);
	DeleteObject(man->m_Right2);
	free(man);

	/*删除父类*/
	Box_Delete(base);
}

