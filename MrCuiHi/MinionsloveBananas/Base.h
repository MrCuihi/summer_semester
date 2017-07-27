#pragma once
#include "stdafx.h"

/*基本元素格子*/
#define GRIDS_SIZE				55								      // 每个基本元素格子的尺寸

/* 游戏区域*/
#define GAMEBOARD_NAME			_T("GameBoard")							// 游戏区域的名字
#define GAMEBOARD_WIDTH_GRIDS	12										// 游戏区域的横向格子数
#define GAMEBOARD_HEIGHT_GRIDS	10										// 游戏区域的纵向格子数
#define GAMEBOARD_WIDTH			(GRIDS_SIZE * GAMEBOARD_WIDTH_GRIDS)	// 游戏区域的宽度
#define GAMEBOARD_HEIGHT		(GRIDS_SIZE * GAMEBOARD_HEIGHT_GRIDS)	// 游戏区域的高度
#define GAMEBOARD_COLOR			RGB(248, 254, 79)					    // 游戏区域的背景颜色

/*游戏关卡*/
#define LEVEL_FILE_PATH			_T("./level/%d.txt")					    // 初始化关卡的文件路径及文件名
#define LEVEL_COUNTS				5										// 关卡总数

/*图片*/
#define IMAGE_FILE_PATH			_T("./res/%d.bmp")						// 位图路径及文件名

/*时钟*/
#define TIMER_ID				WM_USER									// 防止用户定义的消息ID与系统的消息ID冲突,小于WM_USER的ID被系统使用，大于WM_USER的ID被用户使用
#define TIMER_ELAPSE			500										// 间隔

/*字符串*/
#define STRING_MAX_LENGTH		32									//对话框显示及路径最大字符串长度

enum BASEType
{
	WALL = 1,			// 墙
	BOX = 2,			// 箱子
	SPACE = 3,		    //空地
	DESTINATION = 4,	// 箱子的目的地
	MAN = 5,			// 人物
	BACKGROUND = 0		// 背景
};

enum MoveResult
{
	NORMAL_MOVED,		// 正常移动
	CANNOT_MOVE,		// 本身不具有移动能力
	NO_MOVE,			// 没有移动
	MOVE_TO_DEST,	    // 移动到目的地
	LEFT_FROM_DEST		// 离开目的地
};

struct BASE;

typedef void (*BASEDelete)(struct  BASE*);
typedef void (*BASEDraw)(struct BASE*);
typedef enum MoveResult (*BASEMove)(struct BASE*, UINT);

struct  BASE

{
	enum BASEType	m_type;			// 类型
	int 			m_x;			// 横向坐标
	int				m_y;			// 纵向坐标
	HDC				m_drawer;		// 绘图者
	HBITMAP			m_Down1;	    // 自身图像向下移动的图像1
	HDC				m_HDC;          // 自身图像保存者
	void*			m_child;		// 子类对象
    /*对基本对象的基本操作*/
	BASEDraw draw;
	BASEMove move;
	BASEDelete delete;
};

struct BASE* New_BASE(enum BASEType, int ,int, HDC);//创建二维数组中的每个基本元素
/*对Base基本操作函数声明*/
static void BASE_Draw(struct BASE*);//绘制BASE对象,绘制每个基本元素
static enum MoveResult BASE_Move(struct BASE*, UINT);//移动BASE对象,移动基本元素
void BASE_Delete(struct BASE*);//删除BASE对象,删除所有基本元素

