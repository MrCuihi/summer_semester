#include <stdio.h>
#include "stdafx.h"
#include <commctrl.h>

#include <mmsystem.h> 
#include "resource.h"
#include "MainWindow.h"



struct BASE* g_bases[GAMEBOARD_HEIGHT_GRIDS][GAMEBOARD_WIDTH_GRIDS];	

																			
																			
/******************************************创建游戏**************************************/
struct Game* New_Game(HWND mainWindow, HWND gameBoard, HDC drawer/*绘图者*/)
{
	struct Game* game = (struct Game*)calloc(1, sizeof(struct Game));

	/*初始化数据*/
	game->m_mainWindow = mainWindow;
	game->m_gameBoard = gameBoard;
	game->m_drawer = drawer;
	game->m_status = END;
	game->m_nLevel = 1;
	ZeroMemory(g_bases, sizeof(g_bases));//用0来填充一块内存区域

	/*初始化函数*/
	game->replay = Game_Replay;
	game->gotoLevel = Game_GotoLevel;
	game->onKeyPress = Game_OnKeyPress;
	game->onTimer = Game_OnTimer;
	game->delete = Game_Delete;

	/* 进入第1关*/
	game->gotoLevel(game, 1);

	return game;
}

/******************************************进入关卡******************************************/
static void Game_GotoLevel(struct Game* game, int nLevel)
{
	/* 获取关卡文件路径*/
	TCHAR szLevelFilePath[_MAX_PATH] = { 0 };
	FILE* file = NULL;
	RECT gameBoardRect = { 0, 0, GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT };
	char cBuffer = 0;
	enum BASEType BASEType;
	int x = 0, y = 0;

	game->m_nBoxCount = 0;

	/* 读取关卡文件*/
	_stprintf_s(szLevelFilePath, _MAX_PATH, LEVEL_FILE_PATH, nLevel);
	_tfopen_s(&file, szLevelFilePath, _T("r"));//接收到打开的文件指针指向的指针 
	/* 重绘游戏区域*/
	SetBkColor(game->m_drawer, GAMEBOARD_COLOR);//用指定的颜色值来设置当前的背景色
	ExtTextOut(game->m_drawer, 0, 0, ETO_OPAQUE/*用当前的背景色来填充矩形。*/, 
		&gameBoardRect, NULL, 0, NULL);//用当前选择的字体、背景颜色和正文颜色来绘制一个字符串

	/* 创建所有基础元素*/
	for (y = 0; y < GAMEBOARD_HEIGHT_GRIDS; y++)
	{
		for (x = 0; x < GAMEBOARD_WIDTH_GRIDS; x++)
		{
			fread(&cBuffer, 1, 1, file);//从文件流中读数据，最多读取count个项，每个项size个字节，如果调用成功返回实际读取到的项个数（小于或等于count），
			//如果不成功或读到文件末尾返回 0。
			if (cBuffer == '0')		// 空白区域背景
			{
				continue;
			}
			if (cBuffer == '\r' || cBuffer == '\n')
			{
				x--;
				continue;
			}

			BASEType = (enum BASEType)atoi(&cBuffer);

			
			if (BASEType == BOX)// 创建箱子
			{
				g_bases[y][x] = New_Box(BASEType, x, y,
					game->m_drawer, New_BASE(SPACE, x, y, game->m_drawer));
				game->m_nBoxCount++;
				continue;
			}
			
			else if (BASEType == MAN)// 创建小人
			{
				g_bases[y][x] = New_Man(BASEType, x, y, game->m_drawer,
					game->m_gameBoard, New_BASE(SPACE, x, y, game->m_drawer));
				game->m_man = (struct Man*)(((struct Box*)(g_bases[y][x]->m_child))->m_child);
				continue;
			}
			
			g_bases[y][x] = New_BASE(BASEType, x, y, game->m_drawer);// 创建背景或墙
		}
	}
	/* 重置成员变量*/
	game->m_nLevel = nLevel;
	game->m_status = PLAYING;

	/* 刷新游戏区域*/
	InvalidateRect(game->m_gameBoard, NULL, FALSE);//向指定的窗体更新区域添加一个矩形，然后窗口客户区域的这一部分将被重新绘制
    /* 开启时钟,使人物跳舞*/
	SetTimer(game->m_mainWindow, TIMER_ID, TIMER_ELAPSE, NULL);
	/*播放音效*/
	HANDLE hThread;
	DWORD ThreadID;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread1, NULL, 0, &ThreadID);


}

/*******************************************重玩本关***************************************/
static void Game_Replay(struct Game* game)
{
	game->gotoLevel(game, game->m_nLevel);
}

/******************************************退出游戏***************************************/
static void Game_Esc(struct Game* game)
{
	Game_Delete(game);
}

/*******************************按键消息响应函数。在这个函数中，移动人物*********************/
static void Game_OnKeyPress(struct Game* game, UINT nKey)
{
	enum MoveResult moveResult = NO_MOVE;
    /* 移动小人。如果不能移动，则直接返回*/
	moveResult = game->m_man->move(game->m_man, nKey);
	if (moveResult == NO_MOVE)
	{
		return;
	}
	/*如果没在玩，直接返回*/
	if (game->m_status != PLAYING)
	{
		return;
	}
	/*根据移动结果，检查是否胜利*/
	if (moveResult == MOVE_TO_DEST)
	{
		game->m_nBoxCount--;
		game->m_BoxCount++;
		Game_CheckWon(game);
	}
	else if (moveResult == LEFT_FROM_DEST)
	{
		game->m_nBoxCount++;
		game->m_BoxCount--;
	}
    /*重绘游戏区域*/
	InvalidateRect(game->m_gameBoard, NULL, FALSE);
}

/**********************************时钟消息的响应函数。在这个函数中，让人物跳舞************************/
static void Game_OnTimer(struct Game* game)
{
	game->m_man->dancing(game->m_man);
}

/**************************************判断游戏是否胜利************************************/
static void Game_CheckWon(struct Game* game)
{
	TCHAR szText[STRING_MAX_LENGTH] = { 0 };

	/* 如果还有箱子未到达目的地，则直接返回*/
	if (game->m_nBoxCount != 0)
	{
		return;
	}

	KillTimer(game->m_mainWindow, TIMER_ID);// 停止时钟
	game->m_status = END;

	if (game->m_nLevel == LEVEL_COUNTS)// 全部通关了，弹出提示
	{
		
		LoadString(NULL, IDS_GAME_WON_MAX, szText, STRING_MAX_LENGTH);//资源里加载字符串到CString对象 
		MessageBox(game->m_gameBoard, szText, _T("Boxman"), MB_ICONINFORMATION);
	}
	else// 进入下一关
	{
		
		LoadString(NULL, IDS_GAME_WON, szText, STRING_MAX_LENGTH);
		MessageBox(game->m_gameBoard, szText, _T("Boxman"), MB_ICONINFORMATION);
		game->gotoLevel(game, game->m_nLevel + 1);
	}
}
/****************************************删除游戏***********************************/
static void Game_Delete(struct Game* game)
{
	free(game);
}



/******************************************main函数，程序入口***************************************/
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,//程序当前实例的句柄,_in_这个变量或参数是输入值，即你必须给这个变量填写好以后提交给某个函数去执行。
	                   _In_opt_ HINSTANCE hPrevInstance,//_in：输入参数； __out：输出参数 ；__in_opt：可选的输入参数。
	                   _In_ LPTSTR lpCmdLine, //指向以/0结尾的命令行
	                   _In_ int nCmdShow)//指明应该以什么方式显示主窗口
{
	struct MainWindow* mainWindow = New_MainWindow();
	mainWindow->doModal(mainWindow);

	UNREFERENCED_PARAMETER(hInstance);//告诉编译器，已经使用了该变量，不必检测警告！
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

   return 0;
}

/******************************************设置音效***************************************/

void Thread1()
{
	/*打开背景音乐*/
    mciSendString("open 小黄人 - 香蕉之歌.mp3 alias mymusic", NULL, 0, NULL);
	/*播放音乐*/
	mciSendString("play mymusic", NULL, 0, NULL);
	/*延迟*/
	Sleep(2100);
	/* 停止播放音乐*/
	mciSendString("stop mymusic", NULL, 0, NULL);
	/*关闭音乐*/
	mciSendString("close music", NULL, 0, NULL);
	/*销毁线程*/
	ExitThread(0);

}
/********************************************创建主窗口****************************************/
struct MainWindow* New_MainWindow()
{
	struct MainWindow* mainWindow = (struct MainWindow*)calloc( 1, sizeof(struct MainWindow) );

	/* 初始化数据*/
	mainWindow->m_handle = CreateDialog( NULL, 
		MAKEINTRESOURCE(IDD_BOXMAN_DIALOG), NULL, MainWindow_Proc );
	SetWindowLongPtr( mainWindow->m_handle, GWLP_USERDATA, (LONG_PTR)mainWindow );
	mainWindow->m_gameBoard = New_GameBoard(mainWindow->m_handle, 
		0, 0, GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT);
	mainWindow->m_game = New_Game(mainWindow->m_handle, 
		mainWindow->m_gameBoard->m_handle, mainWindow->m_gameBoard->m_drawer);

	/* 初始化函数*/
	mainWindow->doModal = MainWindow_DoModal;
	
   return mainWindow;
}



/***********************************************以模态显示窗口*************************************/
static void MainWindow_DoModal(struct MainWindow* mainWindow)
{
	MSG msg = {0};

	/* 显示窗口*/
	ShowWindow(mainWindow->m_handle, SW_SHOW);//该函数设置指定窗口的显示状态
	UpdateWindow(mainWindow->m_handle);//意思是更新指定窗口的客户区
	//如果窗口更新的区域不为空，UpdateWindow函数就发送一个WM_PAINT消息来更新指
	//定窗口的客户区。函数绕过应用程序的消息队列，直接发送WM_PAINT消息给指定窗口
	//的窗口过程，如果更新区域为空，则不发送消息。

	/*窗口消息循环*/
	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		if (msg.message == WM_KEYDOWN/*当一个非系统键被按下时该消息发送给具有键盘焦点的窗口*/)
		{
			MainWindow_OnKeyDown(mainWindow, msg.wParam);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/****************************************窗口消息处理函数***************************************/
static INT_PTR CALLBACK MainWindow_Proc(HWND window/*窗口*/,
	                                    UINT nMessage/*消息标识*/,
	                                    WPARAM wParam,/*消息附加参数*/
	                                    LPARAM lParam/*消息附加参数*/)
{
	struct MainWindow* mainWindow = (struct MainWindow*)GetWindowLongPtr(window, GWLP_USERDATA);
	WORD wID = LOWORD(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (nMessage)
	{
	case WM_COMMAND://程序代码，在窗口菜单程序中使用
		switch (wID)
		{
		case IDC_MENU_REPLAY:
			MainWindow_OnMenuItemReplay(mainWindow);
			break;
		case  ID_40003:
			MainWindow_OnMenuDelete(mainWindow);
			break;
       case IDCANCEL:
			MainWindow_Delete(mainWindow);
			break;
		}
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_TIMER:
		MainWindow_OnTimer(mainWindow);
		return TRUE;
	}
	return FALSE;
}

/**********************按键事件的响应函数。在这个函数中，将消息转交给游戏对象来处理****************/
static void MainWindow_OnKeyDown(struct MainWindow* mainWindow, UINT nKey)
{
	if (nKey >= VK_LEFT && nKey <= VK_DOWN)
	{
		mainWindow->m_game->onKeyPress(mainWindow->m_game, nKey);
	}
}

/***************************时钟消息的响应函数。在这个函数中，将消息转交给游戏对象来处理******************/
static void MainWindow_OnTimer(struct MainWindow* mainWindow)
{
	mainWindow->m_game->onTimer(mainWindow->m_game);
}

/***************************************“重玩本关”菜单项的响应函数**************************************/
static void MainWindow_OnMenuItemReplay(struct MainWindow* mainWindow)
{
	mainWindow->m_game->replay(mainWindow->m_game);
}

/***************************************“退出本关”菜单项的响应函数**************************************/
static void MainWindow_OnMenuDelete(struct MainWindow* mainWindow)
{
	mainWindow->m_gameBoard->delete(mainWindow->m_gameBoard);
	DestroyWindow(mainWindow->m_handle);
	free(mainWindow);
}

/************************************************删除主窗口***************************************/
static void MainWindow_Delete(struct MainWindow* mainWindow)
{
	mainWindow->m_gameBoard->delete(mainWindow->m_gameBoard);
	DestroyWindow(mainWindow->m_handle);
	free(mainWindow);
}
