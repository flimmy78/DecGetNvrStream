//
//  "$Id: ScrollBar.h 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __CTRL_SCROLL_H__
#define __CTRL_SCROLL_H__

typedef enum
{
	scrollbarY = 0x0001,	//��ֱ��
}SCROLLBARSTYLE;

enum _scrollbar_color_t
{
	SLB_TEXT_TITLE	= 0,
	SLB_BK,//������ɫ
	SLB_BORDER,//���㲻��scroolbar�ı߿���ɫ
	SLB_SELBORDER,//������scroolbar�ı߿���ɫ
	SLB_SEL_ITEM_TEXT,
	SLB_NOSEL_ITEM_TEXT
};

typedef enum{
	SBB_NORMAL,
	SBB_SELECTED,
	SBB_PUSHED,
	SBB_DISABLED,
}ScrollBarBitmap;


typedef void (CObject::*TRACKPROC)(int pos);

class CScrollBar : public CItem
{
	VD_COLORREF	m_BkColor;
	VD_COLORREF	m_BorderColor;
	VD_COLORREF	m_SelBorderColor;
	VD_COLORREF	m_MidLineColor;
	int		min;
	int		max;
	int		curpos;
	int		page;	//ҳ������

	VD_BOOL	track;			//���ڹ���
	int		tracker_height;	//������߶�
	int		tracker_offset;	//������ƫ��
	int		tracker_pick;	//���������ʱ�������λ��

	CTRLPROC m_onTrackMove;

	enum _scroll_act{	//����
	SA_PAGEUP = 0,
		SA_PAGEDOWN,
		SA_TRACK,
	};
	static VD_BITMAP* m_bmpNormal;
	static VD_BITMAP* m_bmpSelected;
	static VD_BITMAP* m_bmpPushed;
	static VD_BITMAP* m_bmpDisabled;
private:
	void UpdateTracker();
	
public:
	/*
		modified history: --by wangqin 20070316
		�޸��Ƿ����ڵ���ʽ�Ŀؼ�ͨ���ؼ�������.ȡ����vpopup����~
	*/
//	CScrollBar(PCRECT pRect, CPage * pParent,uint vstyle = scrollbarY,int vmin = 0,int vmax = 100, int vpage = 1, BOOL vpopup = FALSE);
	CScrollBar(VD_PCRECT pRect, CPage * pParent,uint vstyle = scrollbarY,int vmin = 0,int vmax = 100, int vpage = 1,CTRLPROC onTrackMove = NULL);
	virtual ~CScrollBar();


	void DrawFrame();
	void Draw();
	void DrawBar(VD_BOOL paint);
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
	int GetCurSel();
	int GetAt(int px, int py);
	void SetRange(int vmin, int vmax, VD_BOOL redraw = TRUE);
	void SetPos(int newpos, VD_BOOL redraw = TRUE);
	int GetPos();
	void SetPage(int vpage, VD_BOOL redraw = TRUE);
	void SetColor(int nRegion, VD_COLORREF color, VD_BOOL redraw = FALSE);

	static void SetDefaultBitmap(ScrollBarBitmap region, VD_PCSTR name);
};
CScrollBar* CreateScrollBar(VD_PCRECT pRect, CPage * pParent, uint vstyle = scrollbarY,
								   int vmin = 0,int vmax = 100, int vpage = 1,CTRLPROC onTrackMove = NULL);
#endif //__CTRL_SCROLL_H__
