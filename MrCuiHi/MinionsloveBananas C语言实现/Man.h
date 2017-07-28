#pragma once

#include "box.h"

struct Man;

typedef enum MoveResult (*ManMove)(struct Man*, UINT);
typedef void (*ManDancing)(struct Man*);

struct Man
{
    struct Box*	m_parent;		// 用于继承的父类对象
	HWND		m_gameBoard;	// 游戏区域
	HBITMAP		m_Up1;		    // 向上移动图像1
	HBITMAP		m_Up2;		    // 向上移动图像2
	HBITMAP		m_Left1;	    // 向左移动图像1
	HBITMAP		m_Left2;	    // 向左移动图像2
	HBITMAP		m_Right1;	    // 向右移动图像1
	HBITMAP		m_Right2;	    // 向右移动图像2
	HBITMAP		m_image1;		// 当前变换图像1
	HBITMAP		m_image2;		// 当前变换图像2

	ManMove move;
	ManDancing dancing;
};

struct BASE* New_Man(enum BASEType, int, int, HDC, HWND, struct BASE*);//

/*对Man基本操作函数声明*/
static enum MoveResult Man_Move(struct Man*, UINT);//移动人物
static void Man_Dancing(struct Man*);//人物跳舞，使人物在移动过程中变换图像
static void Man_Delete(struct BASE*);//删除人物