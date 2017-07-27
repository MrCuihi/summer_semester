#pragma once
#include"Base.h"
#include "GameBoard.h"
#include "Man.h"


enum Status
{
	PLAYING, // 游戏中
	END      // 结束
};

struct Game;

typedef void(*GameReplay)(struct Game*);
typedef void(*GameSelectLevel)(struct Game*);
typedef void(*GameGotoLevel)(struct Game*, int);
typedef void(*GameOnKeyPress)(struct Game*, UINT);
typedef void(*GameOnTimer)(struct Game*);
typedef void(*GameDelete)(struct Game*);

struct Game
{
    HWND		m_mainWindow;	// 主窗口
	HWND		m_gameBoard;	// 游戏区域
	HDC			m_drawer;		// 绘图者
	enum Status	m_status;		// 游戏状态
	struct Man*	m_man;			// 小人
	int			m_nLevel;		// 关卡数
	int         m_BoxCount;     //到达目的地的箱子数目
	int			m_nBoxCount;	// 未到达目的地的箱子数

    GameReplay replay;
	GameGotoLevel gotoLevel;
	GameOnKeyPress onKeyPress;
	GameOnTimer onTimer;
	GameDelete delete;
};

struct Game* New_Game(HWND, HWND, HDC);//创建游戏
static void Game_GotoLevel(struct Game*, int);//进入关卡
static void Game_Replay(struct Game*);//重玩本关
static void Game_Esc(struct Game* );//退出游戏
static void Game_OnKeyPress(struct Game*, UINT);//按键消息响应函数。在这个函数中，移动人物
static void Game_OnTimer(struct Game*);//时钟消息的响应函数。在这个函数中，让人物跳舞
static void Game_CheckWon(struct Game*);//判断游戏是否胜利
static void Game_Delete(struct Game*);//删除游戏
void Thread1();


struct MainWindow;

typedef void (*MainWindowDoMoal)(struct MainWindow*);
typedef void (*MainWindowDelete)(struct MainWindow*);

struct MainWindow
{
	HWND				m_handle;//主窗口
	struct GameBoard*	m_gameBoard;//游戏区域
	struct Game*		m_game;//游戏
	MainWindowDoMoal doModal;
};

struct MainWindow* New_MainWindow();//创建主窗口
void Thread1();//设置背景音乐
/*对MainWindow基本操作函数声明*/
static void MainWindow_DoModal(struct MainWindow*);//以模态显示窗口
static INT_PTR CALLBACK MainWindow_Proc(HWND, UINT, WPARAM, LPARAM);//窗口消息处理函数
static void MainWindow_OnKeyDown(struct MainWindow*, UINT);//按键事件的响应函数。在这个函数中，将消息转交给游戏对象来处理
static void MainWindow_OnTimer(struct MainWindow*);//时钟消息的响应函数。在这个函数中，将消息转交给游戏对象来处理
static void MainWindow_OnMenuItemReplay(struct MainWindow*);//“重玩本关”菜单项的响应函数
static void MainWindow_OnMenuDelete(struct MainWindow*);//“退出本关”菜单项的响应函数
static void MainWindow_Delete(struct MainWindow*);//删除主窗口

