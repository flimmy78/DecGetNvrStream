//
//  "$Id: Menu.h 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __CTRL_MENU_H__
#define __CTRL_MENU_H__

#define MENU_MAX 32   //�����˵�������32��

class CMenu;

typedef enum{
	styleVertical		 = 0x0001, //��ͨ�����Ų���menu���������Ҽ��˵�
	styleNavigation		 = 0x0002, //���������˵�ʽ���Ų���ʽ,������ʾ��ͼ�����棬��AeBell�Ķ����˵�
	styleHorizontal		 = 0x0004, //�����Ų��Ĳ˵�����honywell��һ���˵�
	styleDeskTop		 = 0x0008, //����
	styleDeskTopNew		 = 0x0010, //����_cj
	styleNavTextInside	 = 0x0040, //�Զ�����styleNavigation���ԣ�,������������ʾ��ͼ���ڲ�����CDJF
}MENU_STYLE;

//��ɫö�� 
typedef enum{
	colBKNormal			= 0x00,  //������ɫ
	colBKTitle			= 0x01,  //���ⱳ��ɫ
	colBKFrame			= 0x02,  //�����߿�ɫ
	colSingleNormal		= 0x03,  //����δ��ѡ�е���ɫ
	colSingleSelect		= 0x04,  //���ѡ�е���ɫ
	colFontNormal		= 0x05,  //����������ɫ
	colFontSelect		= 0x06,  //ѡ��������ɫ
	colFontTitle		= 0x07,  //���ⱳ����ɫ
	colFrameNormal		= 0x08,  //�����߿���ɫ������
	colFrameSelect		= 0x09,  //ѡ�б߿���ɫ������
}MENU_COLOR_TYPE;
enum MenuType
{
	MT_NULL,	// ������
	MT_NOBODER,	// û�б߿�Ĳ˵�
	//.. ���������������
};
typedef enum{
	MNB_PANEL,
	MNB_TITLED_PANEL,
	MNB_BAR,
	MNB_ITEM_NORMAL,
	MNB_ITEM_SELECTED,
	MNB_ITEM_DISABLED,
}MenuBitmap;

typedef struct tagMENU_ITEM{
	VD_BITMAP*		bmp1;
	VD_BITMAP*		bmp2;
	VD_BITMAP*		bmp3;
	std::string	string;
	std::string	tip;
	CMenu*		sub;
    CTRLPROC	proc;
	CPage*		child;
	VD_BOOL		sprtr;	//�ָ����
	int			y_offset;
	int			x_offset;
}MENU_ITEM;


class CMenu : public CItem   //���ڵ����༶�˵��Ŀؼ�
{
	int			num;
	int			y_offset;
	int			x_offset;
	MENU_ITEM	items[MENU_MAX];
	CMenu*		pParentMenu;
	int			cursel;     //��ǰ��

	int			lastssel;

	//BITMAP*		templet;
	int			clicked;	//�ǵ���ʽ�˵���չ��
	std::string	title;
	VD_BOOL		m_IFDTop;
	int			m_iItemHeight; //menuÿ��ĸ߶�
	int			m_iItemWidth; //menuÿ��Ŀ��
	int			m_iySpace; //styleVertical����menu��ֱ�߾�
	int			m_ixSpace; //styleVertical����menuˮƽ�߾�
	int			m_iStartX; //styleVertical���͵�menuͼ������ֵ���ʼx����

	int			m_iConstColNum;
	int			m_iColNum;
	int			m_iRowNum;
	int			m_iFrameWidth; //�߿���
	VD_BOOL         isRevPopup;   //�Ƿ��򵯳���ʽ��ʾ
	int item_height;
	VD_BOOL		m_autoSpread;
	VD_BITMAP*		m_bmpBKNormal; //����ͼƬ
	VD_BITMAP*     m_bmpBKNormal1;
	VD_BITMAP*		m_bmpSingleNormal;//����δ��ѡ�е�ͼƬ
	VD_BITMAP*		m_bmpSingleSelect; //���ѡ�е�ͼƬ
	VD_SIZE		m_customSize;
	VD_SIZE		m_customInterval;
	VD_COLORREF	m_colBKNormal; //������ɫ
	VD_COLORREF	m_colBKTitle; //���ⱳ��ɫ
	VD_COLORREF	m_colBKFrame; //�����߿���ɫ
	VD_COLORREF	m_colSingleNormal; //����δ��ѡ�е���ɫ
	VD_COLORREF	m_colSingleSelect; //���ѡ�е���ɫ
	VD_COLORREF	m_colFontNormal; //����������ɫ
	VD_COLORREF	m_colFontSelect; //ѡ��������ɫ
	VD_COLORREF	m_colFontTitle; //���ⱳ����ɫ
	VD_COLORREF	m_colFrameNormal; //�����߿���ɫ������
	VD_COLORREF	m_colFrameSelect; //ѡ�б߿���ɫ������
	
	static VD_BITMAP* m_bmpPanel;
	static VD_BITMAP* m_bmpTitledPanel;
	static VD_BITMAP* m_bmpBar;
	static VD_BITMAP* m_bmpItemNormal;
	static VD_BITMAP* m_bmpItemSelected;
	static VD_BITMAP* m_bmpItemDisabled;

	void Init(void);
	void SetIconArray(int number, MENU_ITEM *pitem);
	void SetItemRight(VD_RECT* pRect);
	void SetItemArray(void);
	void DrawBk(void);
	enum MenuType m_menuType;
public:
	int                  move_sel;  //���˵���һ������ƶ�ѡ����
	
public: 

	CMenu(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz = NULL, CItem *pOwner = NULL,uint style = stylePopup | styleVertical, VD_BOOL IfDeskTop = FALSE);
	virtual ~CMenu();   

	void DrawTip(int sel); //xym GUI //��ʾ��ʾ��Ϣ
	void SetMenuType(enum MenuType type) {m_menuType = type;}
	void Draw(void);
	void DrawSingle(int index, VD_BOOL bDrawBk = TRUE);
	void Select(VD_BOOL flag);
	int  GetAt(int px, int py);
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
	int  GetCurSel(void);   
	void SetCurSel(int sel);   
	void ApendItem(VD_PCSTR psz, CTRLPROC vproc, VD_BITMAP* icon1 = NULL,VD_BITMAP* icon2 = NULL,VD_BITMAP* icon3 = NULL,VD_PCSTR tip = NULL);
	void ApendItem(VD_PCSTR psz, CMenu *pMenu, VD_BITMAP* icon1 = NULL,VD_BITMAP* icon2 = NULL,VD_BITMAP* icon3 = NULL,VD_PCSTR tip = NULL);
	void ApendItem(VD_PCSTR psz, CPage *pPage, VD_BITMAP* icon1 = NULL,VD_BITMAP* icon2 = NULL,VD_BITMAP* icon3 = NULL,VD_PCSTR tip = NULL);
	void ApendItem(VD_BITMAP* icon1 = NULL);
	VD_BOOL SetCustomPosition(int sel, const VD_POINT &top);
	VD_BOOL SetCustomColumnSize(const VD_SIZE &size);
	VD_BOOL SetCustomInterval(const VD_SIZE &size);
	void RemoveAllItems();
	void TrackPopup(int px, int py);
	VD_BOOL Close();
	void TrackSubMenu();
	void SetItemCallback(int index, CTRLPROC vproc);
	void SetItemText(int index, VD_PCSTR psz, VD_BOOL redraw = FALSE);
	void SetItemBmp(int index, VD_BITMAP* icon1 = NULL, VD_BITMAP* icon2 = NULL, VD_BITMAP* icon3 = NULL, VD_BOOL redraw = FALSE); //���ò˵�ÿ���ͼƬadd langzi 2009-11-9 
	int GetCount();
	VD_BOOL GetItem(int index, MENU_ITEM* pItem);

	/*����menuÿ��ĸ߶�*/
	void SetItemHeight(int itemHeight); 

	/*����menuÿ��Ŀ��*/
	void SetItemWidth(int itemWidth);

	/*����styleVertical����menu��ֱ�߾�*/
	void SetYSpace(int ySpace);

	/*����styleVertical����menuˮƽ�߾�*/
	void SetXSpace(int xSpace);

	/*����menu��ɫ�ӿ�*/
	void SetMenuColor(MENU_COLOR_TYPE colorType, VD_COLORREF color, VD_BOOL redraw = TRUE);

	/*����menut��ͼ�ӿ�*/
	void SetMenuBmp(VD_BITMAP* bkNor, VD_BITMAP* singleNor = NULL, VD_BITMAP* singleSel = NULL, VD_BOOL redraw = TRUE);

	/*����Navigation�͵�menu������*/
	void SetNavColumnNum(int columnNum);

	/*����menu�߿�Ŀ��*/
	void SetFrameWidth(int width, VD_BOOL redraw = FALSE);

	void SetRevPopup(VD_BOOL iRevPopup = FALSE) { isRevPopup = iRevPopup ;};

	void SetAutoSpread(VD_BOOL autoSpread);
	
	static void SetDefaultBitmap(MenuBitmap region, VD_PCSTR name);
};

CMenu* CreateCMenu(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz = NULL, CItem *pOwner = NULL,uint style = stylePopup | styleVertical, VD_BOOL IfDeskTop = FALSE);

#endif //__CTRL_MENU_H__

