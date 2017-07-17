//
//  "$Id: Static.h 285 2008-12-17 06:15:33Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __STATIC_H__
#define __STATIC_H__

enum _static_style_t
{
	staticTransparent	= 0x0001,
	staticEnclosed		= 0x0002,
	staticBorder		= 0x0004,
	staticSelfBk		= 0x0008,	//��ͼƬ�ؼ���Ч, ��ʾ��m_BkColor��Ϊ������ɫ
	staticAutoRect		= 0x0010,	//������Զ�����ʹ�õ�ʱ��stringҲΪNULL������Կ��Ǽ����������
	staticCrochet       = 0x0020,	//add langzi 2009-12-3 ���� ������֣��ұ���͸����Ҫ����staticTransparent
};

enum _static_color_t
{
	STC_TEXT	= 0,
	STC_BK,
	STC_BORDER,
	STC_ENCLOSED,
};

class CStatic : public CItem
{
	//std::string string;	//�ؼ��ַ���
	VD_BITMAP*		bmp;//��̬λͼ
	VD_RECT		rect_normal;
	FONT_STYLE	m_TextFont;  //renxs �ؼ�����
	VD_COLORREF	m_TextColor;
	VD_COLORREF	m_BkColor;
	VD_COLORREF	m_BorderColor;
	VD_COLORREF	m_EnclosedColor;
	int			m_bTextAlign;
	VD_BOOL		m_bTitle;

	int m_BkWidth;
	
public:

	CStatic(VD_PCRECT pRect, CPage * pParent,VD_PCSTR psz,VD_BOOL title = TRUE,  uint style = 0, FONT_STYLE TextFont = FS_NORMAL); 
	CStatic(VD_PCRECT pRect, CPage * pParent,VD_BITMAP* buf, uint style = 0);

	virtual ~CStatic();
	VD_SIZE& GetDefaultSize(void);
	void Draw();
	void SetText(VD_PCSTR psz);
	VD_PCSTR GetText();
	void SetTextColor(VD_COLORREF color, VD_BOOL redraw = FALSE);
	void SetBkColor(VD_COLORREF color, VD_BOOL redraw = FALSE);
	void SetBkWidth(int bkWidth, VD_BOOL redraw = TRUE);
	void SetColor(int nRegion, VD_COLORREF color, VD_BOOL redraw = FALSE);
	void SetTextAlign(int align = VD_TA_LEFTTOP);
	void SetBitmap(VD_BITMAP* buf, VD_PCRECT pRectNormal = NULL);
	void SetTextFont(FONT_STYLE TextFont);
	void AutoSetRect();

};

CStatic* CreateStatic(VD_PCRECT pRect, CPage * pParent,VD_PCSTR psz,VD_BOOL title = TRUE,  uint style = 0, FONT_STYLE TextFont = FS_NORMAL);
CStatic* CreateStatic(VD_PCRECT pRect, CPage * pParent,VD_BITMAP* buf, uint style = 0);
#endif //__STATIC_H__
