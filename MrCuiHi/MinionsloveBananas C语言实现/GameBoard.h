#pragma once
#include"Base.h"
struct GameBoard;

typedef void (*GameBoardDelete)(struct GameBoard*);

struct GameBoard
{
	HWND	m_handle;	// ����
	HBITMAP	m_bitmap;	// ��̨����λͼ
	HDC		m_drawer;	// ��ͼ��

	GameBoardDelete delete;
};

struct GameBoard* New_GameBoard(HWND, int, int, int, int);
/*��GameBoard����������������*/
static LRESULT CALLBACK GameBoard_Proc(HWND, UINT, WPARAM, LPARAM);//LRESULT ����long��,��Windows API��һ�ַ�������;CALLBACK��ʾ��������Ǹ�ϵͳ���õ�,ʵ����CALLBACK����__stdcall(�ص�����)
static void GameBoard_OnPaint(struct GameBoard*);//����Ϸ������ʾ��API��
static void GameBoard_Delete(struct GameBoard*);//ɾ����Ϸ��������