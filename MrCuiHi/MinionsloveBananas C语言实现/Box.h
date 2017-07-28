#pragma once

#include "Base.h"

struct Box;

typedef void (*BoxDelete)(struct BASE*);

struct Box
{
	struct BASE*	m_parent;		// 用于继承属性的父类对象
	struct BASE*	m_below;	    // 位于当前元素对象下面的元素对象
	HBITMAP			m_Down2;	    // 向下移动的图像2
	HBITMAP			m_CurrentImage;	// 当前显示的图像
	void*			m_child;		// 子类对象
	
};

struct BASE* New_Box(enum BASEType, int, int, HDC, struct BASE*);
/*对Base基本操作函数声明*/
enum MoveResult Box_Move(struct BASE*, UINT);
void Box_Delete(struct BASE*);




