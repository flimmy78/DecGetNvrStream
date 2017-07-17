//
//	"$Id: ListBox.h 117 2008-11-27 09:44:43Z liwj $"
//
//	Copyright (c)2008-2010, RealVision Technology CO., LTD.
//	All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day	SVN-Author	Modification
//


#ifndef __CTRL_LIST_H__
#define __CTRL_LIST_H__

#if defined(WIN32)
	#pragma warning (disable : 4786)
#endif
#include <vector>


typedef enum
{
	LIS_CHECKED =	0x0001,	//�Ƿ�ѡ��, ֻ���������Ч
	LIS_VALID =		0x0002,	//�Ƿ���Ч, ֻ���������Ч
	LIS_SELECTED =	0x0004,	//�Ƿ�ѡ��, ֻ���������Ч
	LIS_ASCENDING = 0x0008,	//��������, ֻ�Ա�������Ч
}
LIST_ITEM_STATE;

typedef enum
{
	LIF_TEXTALIGN =			0x0000FFFF,	//�ı����뷽ʽ����
	LIF_FORMAT =			0xFFFF0000,	//ͼƬ��ʾ��ʽ����
	LIF_IMAGE =				0x00010000,	//�Ƿ���ͼƬ, ֻ�Ա�������Ч
	LIF_IMAGE_ON_RIGHT =	0x00020000,	//ͼƬ��ʾ���ұ�, ֻ�Ա�������Ч
}
LIST_ITEM_FORMAT;

struct LIST_ITEM
{
	ushort width;				//����, ֻ�Ա�������Ч
	ushort height;			//��߶�, ֻ���������Ч
	uint state;			//��״̬, �ɸ���״̬��϶���
	VD_BITMAP* image;			//ͼƬ
	int	 format;			//�ַ������뷽ʽ��ͼƬ��ʾ��ʽ
	uint data;				//�û�����
	std::string string;		//�ַ���
	std::string customData;
};

typedef std::vector<LIST_ITEM> LIST_ITEM_VECTOR;

typedef enum
{
	//LTS_POPUP = 0x0001,		//����ʽ  ���պ����ʹ��stylePopup�����棬��Ҫ�ٴ�ʹ�������־��20070326--by wangqin 
	listboxIndex = 0x0001,		//�����
	listboxTitle = 0x0002,		//��������
	listboxCheck = 0x0004,  //����ѡ��
	listboxIcon = 0x0008,		//��Ŵ�ͼ��
	listboxAutoIndex = 0x0010, //�Զ�������
	listboxTopCheck = 0x0020, //�ڱ�������ʾȫѡ��
}LISTSTYLE;

enum _list_color_t
{
	LTC_TEXT	= 0,
	LTC_BK,
	LTC_BORDER,
	LTC_SELBORDER,
	LTC_SEL,
	LTC_NOSEL,
	LTC_TITLELINE,
};

typedef enum{
	LTB_NORMAL,
	LTB_SELECTED,
	LTB_DISABLED,
}ListBoxBitmap;

class CListBox : public CItem
{
//	BOOL	hheader;	//��ʾ������
//	BOOL	vheader;	//��ʾ�����
	VD_COLORREF	m_TextColor;
	VD_COLORREF	m_BkColor;
	VD_COLORREF	m_BorderColor;
	VD_COLORREF	m_SelBorderColor;
	VD_COLORREF	m_SelectColor;
	VD_COLORREF	m_NoSelectColor;
	VD_COLORREF 	m_TitleLine;//�����������ݵķָ���
	VD_COLORREF	m_PopupColor;
	VD_COLORREF	m_CtrlSelect;//��ѡ�е���Ŀ��ɫ
	
//	uint	style;		//�б���ʽ
	int		num;		//������, ����������
	int		column;		//������, ���������
	int		cursel;		//��ǰѡ����
	int		curhl;		//��ǰ������
	int		index_start;//��ǰҳ��ſ�ʼ��
	int		col_start;	//��ǰҳ����ʼ��
	int		rows;		//ҳ������
	LIST_ITEM_VECTOR items;  //�б���
	CScrollBar* vscrollbar;	//��ֱ������
	CScrollBar* hscrollbar;	//ˮƽ������
	int		m_iTextAlign;	//Ĭ���ַ������뷽ʽ
	VD_BOOL   m_FlagSelectAll;  // ȫѡ��־λ   zgzhit 
	//BOOL   m_bSetCheckAll;  //�Ƿ���ȫѡ���־   zgzhit //ʹ��listboxTopCheck��־���ԭ�е�bool����m_bSetCheckAll WQ deleted  2007-05-05
	CTRLPROC m_onEntered;
	CTRLPROC m_onChecked;
	CTRLPROC m_onChanged;
protected:
	static VD_BITMAP* bmp_ltb_normal;
	static VD_BITMAP* bmp_ltb_selected;
	static VD_BITMAP* bmp_ltb_disabled;
private:
	void DrawSingle(int index, int col = -1, VD_BOOL drawback = TRUE);
	void DrawFrame();

public:
	CListBox(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz, CItem *p = NULL, uint vstyle = listboxIndex | listboxTitle | listboxAutoIndex,
					CTRLPROC onEntered = NULL, CTRLPROC onChecked = NULL, CTRLPROC onChanged = NULL);
	virtual ~CListBox();
	
	void Draw();
	VD_BOOL Open();
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
	VD_BOOL GetIfSelectAll();
	void SetRows();
	
	//������
	int	InsertColumn(int col, LIST_ITEM *pItem, VD_BOOL redraw = FALSE);
	VD_BOOL DeleteColumn(int col, VD_BOOL redraw = FALSE);
	VD_BOOL SetColumn(int nCol, LIST_ITEM *pItem, VD_BOOL redraw = FALSE);
	VD_BOOL GetColumn(int nCol, LIST_ITEM *pItem);
	void RemoveAllColumn(VD_BOOL redraw = FALSE);

	//�����
	int InsertItem(int index, LIST_ITEM *pItem, VD_BOOL redraw = FALSE);
	VD_BOOL DeleteItem(int index, VD_BOOL redraw = FALSE);
	VD_BOOL SetItem(int index, LIST_ITEM *pItem, VD_BOOL redraw = FALSE);
	VD_BOOL GetItem(int index, LIST_ITEM *pItem);
	VD_BOOL SetItemData(int index, uint data);
	VD_BOOL SetCustomData(int index, const char *data);
	VD_BOOL SetSubData(int index, int col, uint data);
	uint GetSubData(int index, int col);
	uint GetItemData(int index);
	std::string GetCustomItemData(int index);
	void RemoveAll(VD_BOOL redraw = FALSE);

	//�������
	int SetSubItem(int index, int col, LIST_ITEM *pItem, VD_BOOL redraw = FALSE);
	int GetSubItem(int index, int col, LIST_ITEM *pItem);

	//�ַ�������
	void SetTitle(VD_PCSTR psz);
	int AddString(VD_PCSTR string, VD_PCSTR header = "", VD_BOOL redraw = FALSE);
	void DeleteString(VD_BOOL redraw = FALSE);
	void SetString(VD_PCSTR string, VD_PCSTR header = "", VD_BOOL redraw = FALSE);
	void SetCorner(VD_PCSTR psz, VD_BOOL redraw = FALSE);
	VD_BOOL SetSubString(int index, int col, VD_PCSTR psz, VD_BOOL redraw = FALSE);
	void SetTextAlign(int align = VD_TA_LEFTTOP);

	int GetCurSel();
	int GetAt(int px, int py);
	void SetCurSel(int sel, VD_BOOL redraw = FALSE);
	int GetCount();
	VD_BOOL GetCheck(int index);
	VD_BOOL SetCheck(int index, VD_BOOL checked = TRUE, VD_BOOL redraw = FALSE);
	VD_BOOL GetValid(int index);
	VD_BOOL SetValid(int index, VD_BOOL valid = TRUE, VD_BOOL redraw = FALSE);
	void ShowScrollBar(VD_BOOL show);
	void Select(VD_BOOL flag);
	void Show(VD_BOOL flag);
	VD_BOOL Close();
	void SetRect(VD_PCRECT pRect, VD_BOOL bReDraw = TRUE);
	void SetColor(int nRegion, VD_COLORREF color, VD_BOOL redraw = FALSE);
	 //����ӵ��ȫ����ѡ��Ľӿں���
	void SetSelectAll(VD_BOOL bSet);
	VD_SIZE& GetDefaultSize(void);

	static void SetDefaultBitmap(ListBoxBitmap region, VD_PCSTR name);
};
CListBox* CreateListBox(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz, CItem *p = NULL, uint vstyle = listboxIndex | listboxTitle | listboxAutoIndex,
							  CTRLPROC onEntered = NULL, CTRLPROC onChecked = NULL, CTRLPROC onChanged = NULL);
#endif //__CTRL_LIST_H__
