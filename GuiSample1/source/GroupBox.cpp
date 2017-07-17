//
//  "$Id: GroupBox.cpp 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "GUI/Ctrls/Page.h"

VD_BITMAP*	CGroupBox::bmp_tab = NULL;

///////////////////////////////////////////////////////////////////
////////////        CGroupBox
CGroupBox::CGroupBox(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz,uint style)
:CItem(pRect, pParent, IT_GROUPBOX,style|styleAutoFocus)//, FALSE)
{
	if(psz)
	{
		string = GetParsedString(psz);
	}
}

CGroupBox::~CGroupBox()
{
}

CGroupBox* CreateGroupBox(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz,uint style /* = 0 */)
{
	return new CGroupBox(pRect, pParent, psz,style);
}

VD_SIZE& CGroupBox::GetDefaultSize(void)
{
	static VD_SIZE size = {0,CTRL_HEIGHT};
	return size;
}
void CGroupBox::Draw()
{
	if(!DrawCheck()){
		return;
	}
	VD_POINT start_point, end_point;
	VD_COLORREF start_color, end_color;
	int dx, dy;	//�����������ֵ
	int dr, dg, db; //��������ɫ��ֵ
	int i;
	int num = 10; //�������߶���Ŀ
	
	//����
	m_DC.Lock();
	m_DC.SetBrush(VD_GetSysColor(VD_COLOR_WINDOW));
	m_DC.SetRgnStyle(RS_FLAT);
	m_DC.Rectangle(CRect(0,0,m_Rect.Width(),m_Rect.Height()));


	//���ֱ���
	/*m_DC.SetBrush(GetSysColor(COLOR_CTRL));
	m_DC.SetRgnStyle(RS_NORMAL);
	m_DC.Rectangle(CRect(2,0,TEXT_WIDTH*7,TEXT_HEIGHT), 3, 3);*/
	//����
	m_DC.SetFont(VD_GetSysColor(COLOR_FRAME));
	m_DC.SetTextAlign(VD_TA_LEFTTOP);
	m_DC.SetBkMode(BM_NORMAL);
	m_DC.SetBkColor(VD_GetSysColor(VD_COLOR_WINDOW));
	m_DC.VD_TextOut(CRect(0 ,0,m_Rect.Width(),m_Rect.Height()),string.c_str());

	//���õ����߲���
	start_color = VD_GetSysColor(COLOR_FRAME);
	end_color = VD_GetSysColor(VD_COLOR_WINDOW);
	m_DC.SetPen(VD_GetSysColor(COLOR_FRAME),VD_PS_SOLID,2);
	start_point.x = g_Locales.GetTextExtent(string.c_str()) + 2;
	start_point.y = m_Rect.Height()*2/3;
	end_point.x = m_Rect.Width();
	end_point.y = m_Rect.Height()*2/3;
	//printf("m_Rect left=[%d] top=[%d] right=[%d] bottom=[%d]\n",m_Rect.left,m_Rect.top,m_Rect.right,m_Rect.bottom);
	//printf("point left=[%d] top=[%d] right=[%d] bottom=[%d]\n",start_point.x,start_point.y,end_point.x,end_point.y);


	//�ֳ�10���߶λ�������
	dr = (VD_GetRValue(end_color) - VD_GetRValue(start_color)) * 4 / 5;
	dg = (VD_GetGValue(end_color) - VD_GetGValue(start_color)) * 4 / 5;
	db = (VD_GetBValue(end_color) - VD_GetBValue(start_color)) * 4 / 5;
	dx = (end_point.x - start_point.x);
	dy = (end_point.y - start_point.y);
	m_DC.MoveTo(start_point.x, start_point.y);
	for(i = 0; i < num; i++)
	{

		m_DC.SetPen(ModulateColor(m_pDevGraphics->GetColorFormat(), start_color,
			dr * i / num, dg * i / num, db * i / num), VD_PS_SOLID, 2);

		m_DC.LineTo(start_point.x + dx * (i + 1) / num, start_point.y + dy * (i + 1) / num);
		//if(i == 0) break;
	}
	m_DC.UnLock();
}

void CGroupBox::SetText(VD_PCSTR psz)
{
	if(!psz || !string.compare(psz))
		return;

	string = GetParsedString(psz);
	Draw();
}

VD_PCSTR CGroupBox::GetText(void)
{
	return string.c_str();
}

void CGroupBox::SetDefaultBitmap(GroupBoxBitmap region, VD_PCSTR name)
{
	switch(region) {
	case GBB_TAB:
		bmp_tab = VD_LoadBitmap(name);
		break;
	default:
		break;
	}
}
