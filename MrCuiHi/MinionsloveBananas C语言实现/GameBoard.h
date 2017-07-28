#pragma once
#include"Base.h"
struct GameBoard;

typedef void (*GameBoardDelete)(struct GameBoard*);

struct GameBoard
{
	HWND	m_handle;	// 窗口
	HBITMAP	m_bitmap;	// 后台缓冲位图
	HDC		m_drawer;	// 绘图者

	GameBoardDelete delete;
};

struct GameBoard* New_GameBoard(HWND, int, int, int, int);
/*对GameBoard基本操作函数声明*/
static LRESULT CALLBACK GameBoard_Proc(HWND, UINT, WPARAM, LPARAM);//LRESULT 就是long型,是Windows API的一种返回类型;CALLBACK表示这个函数是给系统调用的,实际上CALLBACK就是__stdcall(回调函数)
static void GameBoard_OnPaint(struct GameBoard*);//将游戏窗口显示在API上
static void GameBoard_Delete(struct GameBoard*);//删除游戏窗口区域