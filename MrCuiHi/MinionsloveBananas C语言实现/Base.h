#pragma once
#include "stdafx.h"

/*����Ԫ�ظ���*/
#define GRIDS_SIZE				55								      // ÿ������Ԫ�ظ��ӵĳߴ�

/* ��Ϸ����*/
#define GAMEBOARD_NAME			_T("GameBoard")							// ��Ϸ���������
#define GAMEBOARD_WIDTH_GRIDS	12										// ��Ϸ����ĺ��������
#define GAMEBOARD_HEIGHT_GRIDS	10										// ��Ϸ��������������
#define GAMEBOARD_WIDTH			(GRIDS_SIZE * GAMEBOARD_WIDTH_GRIDS)	// ��Ϸ����Ŀ��
#define GAMEBOARD_HEIGHT		(GRIDS_SIZE * GAMEBOARD_HEIGHT_GRIDS)	// ��Ϸ����ĸ߶�
#define GAMEBOARD_COLOR			RGB(248, 254, 79)					    // ��Ϸ����ı�����ɫ

/*��Ϸ�ؿ�*/
#define LEVEL_FILE_PATH			_T("./level/%d.txt")					    // ��ʼ���ؿ����ļ�·�����ļ���
#define LEVEL_COUNTS				5										// �ؿ�����

/*ͼƬ*/
#define IMAGE_FILE_PATH			_T("./res/%d.bmp")						// λͼ·�����ļ���

/*ʱ��*/
#define TIMER_ID				WM_USER									// ��ֹ�û��������ϢID��ϵͳ����ϢID��ͻ,С��WM_USER��ID��ϵͳʹ�ã�����WM_USER��ID���û�ʹ��
#define TIMER_ELAPSE			500										// ���

/*�ַ���*/
#define STRING_MAX_LENGTH		32									//�Ի�����ʾ��·������ַ�������

enum BASEType
{
	WALL = 1,			// ǽ
	BOX = 2,			// ����
	SPACE = 3,		    //�յ�
	DESTINATION = 4,	// ���ӵ�Ŀ�ĵ�
	MAN = 5,			// ����
	BACKGROUND = 0		// ����
};

enum MoveResult
{
	NORMAL_MOVED,		// �����ƶ�
	CANNOT_MOVE,		// ���������ƶ�����
	NO_MOVE,			// û���ƶ�
	MOVE_TO_DEST,	    // �ƶ���Ŀ�ĵ�
	LEFT_FROM_DEST		// �뿪Ŀ�ĵ�
};

struct BASE;

typedef void (*BASEDelete)(struct  BASE*);
typedef void (*BASEDraw)(struct BASE*);
typedef enum MoveResult (*BASEMove)(struct BASE*, UINT);

struct  BASE

{
	enum BASEType	m_type;			// ����
	int 			m_x;			// ��������
	int				m_y;			// ��������
	HDC				m_drawer;		// ��ͼ��
	HBITMAP			m_Down1;	    // ����ͼ�������ƶ���ͼ��1
	HDC				m_HDC;          // ����ͼ�񱣴���
	void*			m_child;		// �������
    /*�Ի�������Ļ�������*/
	BASEDraw draw;
	BASEMove move;
	BASEDelete delete;
};

struct BASE* New_BASE(enum BASEType, int ,int, HDC);//������ά�����е�ÿ������Ԫ��
/*��Base����������������*/
static void BASE_Draw(struct BASE*);//����BASE����,����ÿ������Ԫ��
static enum MoveResult BASE_Move(struct BASE*, UINT);//�ƶ�BASE����,�ƶ�����Ԫ��
void BASE_Delete(struct BASE*);//ɾ��BASE����,ɾ�����л���Ԫ��

