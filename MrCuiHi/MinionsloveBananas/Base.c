#include "stdafx.h"
#include "Base.h"


/************************* ������ά�����е�ÿ������Ԫ��***************************/

struct BASE* New_BASE(enum BASEType type, int x, int y, HDC drawer)
{
	TCHAR szImageFilePath[_MAX_PATH] = {0};
	struct BASE* base = (struct BASE*)calloc( 1, sizeof(struct BASE) );

	/* ��ʼ��BASE�е�����*/
	base->m_type = type;
	base->m_x = x;
	base->m_y = y;
	base->m_drawer = drawer;
	_stprintf_s(szImageFilePath, _MAX_PATH, IMAGE_FILE_PATH, (type+1)*100);// �����������Ҫ�����ǽ����ɸ�
	//argument����format��ʽ�浽buffer�С�  
	base->m_Down1 = (HBITMAP)LoadImage(NULL,
		szImageFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//װ��ͼ�꣬��꣬��λͼ��
	base->m_HDC = CreateCompatibleDC(drawer);
	SelectObject(base->m_HDC, base->m_Down1);

	/*��ʼ��BASE�еĺ���*/
	base->delete = BASE_Delete;
	base->draw = BASE_Draw;
	base->move = BASE_Move;

	/*��ͼ*/
	base->draw(base);

	return base;
}
/********************************* ����ÿ������Ԫ��********************************/
static void BASE_Draw(struct BASE* base)
{
	BITMAP bmpInfo = { 0 };
	GetObject(base->m_Down1, (int)sizeof(BITMAP), &bmpInfo);//�������ܣ�GetObject�����õ�ָ��ͼ�ζ������Ϣ������ͼ�ζ��󣬺����������Ļ�ṹ�����������߼���ɫ�壩��Ŀ����һ��ָ���Ļ�������
	StretchBlt(base->m_drawer, base->m_x * GRIDS_SIZE, base->m_y * GRIDS_SIZE,
		GRIDS_SIZE, GRIDS_SIZE, base->m_HDC, 0, 0, bmpInfo.bmWidth,
		bmpInfo.bmHeight, SRCCOPY);//��Դ�����и���һ��λͼ��Ŀ�����
								   //http:baike.baidu.com/link?url=zj1FPFc-6zAKT9oq2_eU-mClbNmStmM1YJ9r3CdlEXBDCa9sCG83L1l4dZr-T62L2Fi3t37fGsokKIs729Dti_ljYHtAAPmoZCPdstxWCU3
}

/***********************************�ƶ�����Ԫ��***********************************/
static enum MoveResult BASE_Move(struct BASE* base, UINT Direction)
{
	if (base->m_type == WALL)
	{
		return NO_MOVE;
	}
	return CANNOT_MOVE;
}

/********************************* ɾ�����л���Ԫ��********************************/
void BASE_Delete(struct BASE* base)
{
	DeleteObject(base->m_Down1);//�ú���ɾ��һ���߼��ʡ����ʡ����塢λͼ��������ߵ�ɫ�壬�ͷ�������ö����йص�ϵͳ��Դ���ڶ���ɾ��֮��ָ���ľ��Ҳ��ʧЧ�ˡ�
	DeleteDC(base->m_HDC);//�ú���ɾ��ָ�����豸�����Ļ�����Dc��
	free(base);
}


