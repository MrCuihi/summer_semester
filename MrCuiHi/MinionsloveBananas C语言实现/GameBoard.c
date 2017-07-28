#include "stdafx.h"
#include "resource.h"
#include "GameBoard.h"


/*****************************************������Ϸ����****************************************/
struct GameBoard* New_GameBoard(HWND parent/*������*/, int x, int y, int nWidth, int nHeight)
{
	WNDCLASSEX wndClass = {sizeof(WNDCLASSEX)};
	struct GameBoard* gameBoard = (struct GameBoard*)calloc( 1, sizeof(struct GameBoard) );

	/* ע�ᴰ����*/
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//CS_HREDRAW������ˮƽ����Ŀ�ȱ仯ʱ�ػ���������.CS_VREDRAW �����ڴ�ֱ����Ŀ�ȱ仯ʱ�ػ���������.
	wndClass.lpfnWndProc = GameBoard_Proc;//lpfnWndProcָ�����ĳ�ָ��
	wndClass.hCursor = LoadCursor(NULL, IDC_CROSS);//��һ���������ܣ��ú�����һ����Ӧ��������صĿ�ִ���ļ���EXE�ļ���������ָ���Ĺ����Դ��
	wndClass.lpszClassName = GAMEBOARD_NAME;
	RegisterClassEx(&wndClass);

	/* ��ʼ������*/
	gameBoard->delete = GameBoard_Delete;

	/* ��ʼ������*/
	gameBoard->m_handle = CreateWindow(GAMEBOARD_NAME, NULL, 
		WS_CHILD | WS_VISIBLE, x, y, nWidth, nHeight, parent, NULL, NULL, NULL);
	SetWindowLongPtr( gameBoard->m_handle, GWLP_USERDATA, (LONG_PTR)gameBoard );
	gameBoard->m_bitmap = CreateCompatibleBitmap( GetDC(gameBoard->m_handle), 
		GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT );/*�ú���������ָ�����豸������ص��豸���ݵ�λͼ*/
	gameBoard->m_drawer = CreateCompatibleDC(  GetDC(gameBoard->m_handle) );
	SelectObject(gameBoard->m_drawer, gameBoard->m_bitmap);
    return gameBoard;
}

/*****************************************�����¼�������*******************************************/
static LRESULT CALLBACK GameBoard_Proc(HWND window,/*����*/
	                                   UINT/*int*/ nMessage,/*�¼���ʶ*/
	                                   WPARAM wParam/*�¼����Ӳ���*/, 
	                                   LPARAM lParam/*�¼����Ӳ���*/)
{
	struct GameBoard* gameBoard = (struct GameBoard*)GetWindowLongPtr(window, GWLP_USERDATA);
	switch (nMessage)
	{
	case WM_PAINT://WndProc����ĵڶ�����ϢΪWM_PAINT�������Ϣ��Windows����������Ǻ���Ҫ�ġ���������ʾ�����һ������ʾ
		//���ݻ���ȫ����Ϊ����Ч���������ڱ��롰���»��桱ʱ�����������Ϣ֪ͨ����
		GameBoard_OnPaint(gameBoard);
		break;
	}
	return DefWindowProc(window, nMessage, wParam, lParam);
}

/***********************�ػ��¼�����Ӧ����������������У�����̨����λͼ���Ƶ�ǰ̨**************************/
static void GameBoard_OnPaint(struct GameBoard* gameBoard)
{
	PAINTSTRUCT ps = {0};
	BeginPaint(gameBoard->m_handle, &ps);//BeginPaint����Ϊָ�����ڽ��л�ͼ������׼�������ý��ͻ�ͼ�йص���Ϣ��䵽һ��PAINTSTRUCT�ṹ��
	BitBlt(ps.hdc, 0, 0, GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT, gameBoard->m_drawer, 0, 0, SRCCOPY);//�ú�����ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	EndPaint(gameBoard->m_handle, &ps);
}
/***************************************ɾ����Ϸ����*************************************/
static void GameBoard_Delete(struct GameBoard* gameBoard)
{
	DeleteObject(gameBoard->m_drawer);
	DeleteObject(gameBoard->m_bitmap);
	DestroyWindow(gameBoard->m_handle);
	free(gameBoard);
}