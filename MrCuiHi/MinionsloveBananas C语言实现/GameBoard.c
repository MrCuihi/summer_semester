#include "stdafx.h"
#include "resource.h"
#include "GameBoard.h"


/*****************************************创建游戏区域****************************************/
struct GameBoard* New_GameBoard(HWND parent/*父窗口*/, int x, int y, int nWidth, int nHeight)
{
	WNDCLASSEX wndClass = {sizeof(WNDCLASSEX)};
	struct GameBoard* gameBoard = (struct GameBoard*)calloc( 1, sizeof(struct GameBoard) );

	/* 注册窗口类*/
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//CS_HREDRAW当窗口水平方向的宽度变化时重绘整个窗口.CS_VREDRAW 当窗口垂直方向的宽度变化时重绘整个窗口.
	wndClass.lpfnWndProc = GameBoard_Proc;//lpfnWndProc指向函数的长指针
	wndClass.hCursor = LoadCursor(NULL, IDC_CROSS);//是一个函数功能，该函数从一个与应用事例相关的可执行文件（EXE文件）中载入指定的光标资源。
	wndClass.lpszClassName = GAMEBOARD_NAME;
	RegisterClassEx(&wndClass);

	/* 初始化函数*/
	gameBoard->delete = GameBoard_Delete;

	/* 初始化数据*/
	gameBoard->m_handle = CreateWindow(GAMEBOARD_NAME, NULL, 
		WS_CHILD | WS_VISIBLE, x, y, nWidth, nHeight, parent, NULL, NULL, NULL);
	SetWindowLongPtr( gameBoard->m_handle, GWLP_USERDATA, (LONG_PTR)gameBoard );
	gameBoard->m_bitmap = CreateCompatibleBitmap( GetDC(gameBoard->m_handle), 
		GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT );/*该函数创建与指定的设备环境相关的设备兼容的位图*/
	gameBoard->m_drawer = CreateCompatibleDC(  GetDC(gameBoard->m_handle) );
	SelectObject(gameBoard->m_drawer, gameBoard->m_bitmap);
    return gameBoard;
}

/*****************************************窗口事件处理函数*******************************************/
static LRESULT CALLBACK GameBoard_Proc(HWND window,/*窗口*/
	                                   UINT/*int*/ nMessage,/*事件标识*/
	                                   WPARAM wParam/*事件附加参数*/, 
	                                   LPARAM lParam/*事件附加参数*/)
{
	struct GameBoard* gameBoard = (struct GameBoard*)GetWindowLongPtr(window, GWLP_USERDATA);
	switch (nMessage)
	{
	case WM_PAINT://WndProc处理的第二个消息为WM_PAINT。这个消息在Windows程序设计中是很重要的。当窗口显示区域的一部分显示
		//内容或者全部变为“无效”，以致于必须“更新画面”时，将由这个消息通知程序。
		GameBoard_OnPaint(gameBoard);
		break;
	}
	return DefWindowProc(window, nMessage, wParam, lParam);
}

/***********************重绘事件的响应函数。在这个函数中，将后台缓冲位图绘制到前台**************************/
static void GameBoard_OnPaint(struct GameBoard* gameBoard)
{
	PAINTSTRUCT ps = {0};
	BeginPaint(gameBoard->m_handle, &ps);//BeginPaint函数为指定窗口进行绘图工作的准备，并用将和绘图有关的信息填充到一个PAINTSTRUCT结构中
	BitBlt(ps.hdc, 0, 0, GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT, gameBoard->m_drawer, 0, 0, SRCCOPY);//该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	EndPaint(gameBoard->m_handle, &ps);
}
/***************************************删除游戏区域*************************************/
static void GameBoard_Delete(struct GameBoard* gameBoard)
{
	DeleteObject(gameBoard->m_drawer);
	DeleteObject(gameBoard->m_bitmap);
	DestroyWindow(gameBoard->m_handle);
	free(gameBoard);
}