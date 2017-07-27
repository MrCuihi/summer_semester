#pragma once

#include "box.h"

struct Man;

typedef enum MoveResult (*ManMove)(struct Man*, UINT);
typedef void (*ManDancing)(struct Man*);

struct Man
{
    struct Box*	m_parent;		// ���ڼ̳еĸ������
	HWND		m_gameBoard;	// ��Ϸ����
	HBITMAP		m_Up1;		    // �����ƶ�ͼ��1
	HBITMAP		m_Up2;		    // �����ƶ�ͼ��2
	HBITMAP		m_Left1;	    // �����ƶ�ͼ��1
	HBITMAP		m_Left2;	    // �����ƶ�ͼ��2
	HBITMAP		m_Right1;	    // �����ƶ�ͼ��1
	HBITMAP		m_Right2;	    // �����ƶ�ͼ��2
	HBITMAP		m_image1;		// ��ǰ�任ͼ��1
	HBITMAP		m_image2;		// ��ǰ�任ͼ��2

	ManMove move;
	ManDancing dancing;
};

struct BASE* New_Man(enum BASEType, int, int, HDC, HWND, struct BASE*);//

/*��Man����������������*/
static enum MoveResult Man_Move(struct Man*, UINT);//�ƶ�����
static void Man_Dancing(struct Man*);//�������裬ʹ�������ƶ������б任ͼ��
static void Man_Delete(struct BASE*);//ɾ������