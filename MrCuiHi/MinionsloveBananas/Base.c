#include "stdafx.h"
#include "Base.h"


/************************* 创建二维数组中的每个基本元素***************************/

struct BASE* New_BASE(enum BASEType type, int x, int y, HDC drawer)
{
	TCHAR szImageFilePath[_MAX_PATH] = {0};
	struct BASE* base = (struct BASE*)calloc( 1, sizeof(struct BASE) );

	/* 初始化BASE中的数据*/
	base->m_type = type;
	base->m_x = x;
	base->m_y = y;
	base->m_drawer = drawer;
	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type+1)*100);// 这个函数的主要作用是将若干个
	//argument按照format格式存到buffer中。  
	base->m_Down1 = (HBITMAP)LoadImage(NULL,
		szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//装载图标，光标，或位图。
	base->m_HDC = CreateCompatibleDC(drawer);
	SelectObject(base->m_HDC, base->m_Down1);

	/*初始化BASE中的函数*/
	base->delete = BASE_Delete;
	base->draw = BASE_Draw;
	base->move = BASE_Move;

	/*画图*/
	base->draw(base);

	return base;
}
/********************************* 绘制每个基本元素********************************/
static void BASE_Draw(struct BASE* base)
{
	BITMAP bmpInfo = { 0 };
	GetObject(base->m_Down1, (int)sizeof(BITMAP), &bmpInfo);//函数功能：GetObject函数得到指定图形对象的信息，根据图形对象，函数把填满的或结构，或表项（用于逻辑调色板）数目放入一个指定的缓冲区。
	StretchBlt(base->m_drawer, base->m_x * GRIDS_SIZE, base->m_y * GRIDS_SIZE,
		GRIDS_SIZE, GRIDS_SIZE, base->m_HDC, 0, 0, bmpInfo.bmWidth,
		bmpInfo.bmHeight, SRCCOPY);//从源矩形中复制一个位图到目标矩形
								   //http:baike.baidu.com/link?url=zj1FPFc-6zAKT9oq2_eU-mClbNmStmM1YJ9r3CdlEXBDCa9sCG83L1l4dZr-T62L2Fi3t37fGsokKIs729Dti_ljYHtAAPmoZCPdstxWCU3
}

/***********************************移动基本元素***********************************/
static enum MoveResult BASE_Move(struct BASE* base, UINT Direction)
{
	if (base->m_type == WALL)
	{
		return NO_MOVE;
	}
	return CANNOT_MOVE;
}

/********************************* 删除所有基本元素********************************/
void BASE_Delete(struct BASE* base)
{
	DeleteObject(base->m_Down1);//该函数删除一个逻辑笔、画笔、字体、位图、区域或者调色板，释放所有与该对象有关的系统资源，在对象被删除之后，指定的句柄也就失效了。
	DeleteDC(base->m_HDC);//该函数删除指定的设备上下文环境（Dc）
	free(base);
}


