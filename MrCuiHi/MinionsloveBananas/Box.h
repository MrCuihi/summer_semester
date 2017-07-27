#pragma once

#include "Base.h"

struct Box;

typedef void (*BoxDelete)(struct BASE*);

struct Box
{
	struct BASE*	m_parent;		// ���ڼ̳����Եĸ������
	struct BASE*	m_below;	    // λ�ڵ�ǰԪ�ض��������Ԫ�ض���
	HBITMAP			m_Down2;	    // �����ƶ���ͼ��2
	HBITMAP			m_CurrentImage;	// ��ǰ��ʾ��ͼ��
	void*			m_child;		// �������
	
};

struct BASE* New_Box(enum BASEType, int, int, HDC, struct BASE*);
/*��Base����������������*/
enum MoveResult Box_Move(struct BASE*, UINT);
void Box_Delete(struct BASE*);




