//
//  "$Id: Page.h 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#ifndef __PAGE_H__
#define __PAGE_H__

#include "Item.h"
#include "BitmapButton.h"
#include "Button.h"
#include "CheckBox.h"
#include "NumberPad.h"
#include "NumberBox.h"
#include "ScrollBar.h"
#include "ListBox.h"
#include "ComboBox.h"
#include "DateTimeCtrl.h"
#include "GroupBox.h"
#include "IPAddressctrl.h"
#include "GridCtrl.h"
#include "Menu.h"
#include "ProgressCtrl.h"
#include "SliderCtrl.h"
#include "Static.h"
#include "Edit.h"
#include "TimingCtrl.h"
#include "Radio.h"
#include "Calendar.h"
#include "TableBox.h"
//#include "TimeTable.h"
#include "RecFileTimeBox.h"

#include "MultiTask/MsgQue.h"
#include <list>

#include "GUI/Pages/BizData.h"


//#define BIZ

//ҳ������
typedef enum
{
	pageDialog	= 0x0001,
	pageFullScreen = 0x0002,
	pageAlignCenter = 0x0004,
	pageNoExpire = 0x0008,		//���Զ�ע��
	pageTopMost = 0x0010,		//��������
	pageActiveMost = 0x0020,	//���Ǽ���
	pageChild = 0x0040, //cj@20110419
}PAGESTYLE;


#define ROW_MAX_NUM 20		//ÿҳ���������
#define MAX_TABLE_NUM 7     //���������õ�TABLE��
/*begin �Զ�����*/
typedef struct _AUTO_LAYOUT
{
	CItem* pCur;			//��ǰ�ؼ�ָ��
	CItem* head;			//��һ���ؼ�
	CItem* pPre;			//��ǰ�����Ŀؼ�����ǰ�ؼ�ָ��
	CItem* pLineHead;		//�еĵ�һ���ؼ�ָ��.�ô�:����һ�����򳬳��ͻ���,��ô��Ҫ�����������,���Ա����¼����
	int row[ROW_MAX_NUM];	//�ؼ����м�������
	int rowNum;			//�ؼ�����
	int curRow;			//��ǰ��������
	int curColumn;			//��ǰ���ڵ�����
	int column;				//��¼������к�
	int width;				//�м��
	int maxBottom;			//��һ�е����׶ˣ������ж���
	VD_BOOL reRowLayout;		//�����Ƿ���Ҫ���²���,һ�г����ͻ�����ʱ��,�õ�
	VD_BOOL secondLayout;		//�Ƿ���Ҫ���β��֣������ж���ؼ�ʱ��������е�left�����仯��Ҫ���β���
	int startX;		//�ؼ�������ʼxλ��
	int startY;		//�ؼ�������ʼyλ��
	int columnLeft[MAX_TABLE_NUM];		//�е�left���ж���ؼ�ʹ��
}AUTO_LAYOUT;
/*end �Զ�����*/
typedef enum
{
	pageIconSmall,
	pageIconNor,
	pageIconSel,

}PAGEICON;

typedef enum{
	PGB_KEY_PAGEUP,
	PGB_KEY_PAGEDOWN,
	PGB_KEY_FUNCTION,
	PGB_KEY_ENTER,
	PGB_CLOSE_BUTTON_NORMAL,
	PGB_CLOSE_BUTTON_SELECTED,
	PGB_CLOSE_BUTTON_PUSHED,
	PGB_CLOSE_BUTTON_DISABLED,
	PGB_ICON_SMALL,
}PageBitmap;

class CGUI;

class CPage : public CItem
{
public:
	enum LayoutSpace
	{
		spaceCheckBoxSmall,
		spaceCount,
	};

	static CPage* m_pDesktop;
private:

	//for page up/down
	int		m_iPage;//�ӿؼ�ҳ��
	int		m_nPage;//�ӿؼ�ҳ��

	friend	class CItem;
	friend  class CItemThread;

	int *m_playoutSpace;

	std::string	m_pTitle;	//�����ַ���

	VD_BOOL	m_bDrawBkgdOnly;	//ֻ������
	VD_BOOL    ishPageMain;     // ��ҳ���Ǻ���˵�����Ҫ��ȥ����������Ӱ�졣
	
/*begin �Զ�����*/
	int m_iSpace; //�ؼ����
	int m_iStartX; //�ؼ���ʼ X ����
	int m_iStartY; //�ؼ���ʼ Y ����
	int m_iTable[MAX_TABLE_NUM]; //�ж���left����
	int SetCtrlLeft(AUTO_LAYOUT *pAL);
	int SetCtrlRight(AUTO_LAYOUT *pAL, int left);
	int SetCtrlTop(AUTO_LAYOUT *pAL);
	int SetCtrlBottom(AUTO_LAYOUT *pAL, int top);
	void LayoutInit(AUTO_LAYOUT *pAL);

protected:

	/*�Զ����ֺ���*/
	void AutoLayout();

	/*�����׿ؼ����ʼ����*/
	void SetStartPoint(int x = CLIENT_X, int y = CLIENT_Y);

	/*���ÿؼ��ļ��*/
	void SetCtrlSpace(int width);

	/*�����ж���ؼ���������*/
	void SetTable(int TableNum, int left);

	// ��SetCtrlSpace���ظ�
	void SetCtrlSpace(LayoutSpace type, int space);

	
/*end �Զ�����*/

protected:
	UDM		m_ExitState;	//ҳ�����һ���˳�ʱ��״̬�� ��ȷ���˳�/ȡ���˳���
	VD_BOOL m_ifPageMain; //�Ƿ���������

	//for mouse buttons double click
	VD_BOOL	m_bLClicked;	//����������
	VD_BOOL	m_bRClicked;	//����Ҽ�����
	VD_BOOL	m_bMClicked;	//����м�����
	
	//for resize page
	CRect	m_RectResizing;	//�ƶ�ҳ��ʱ��ʾ�ı߿�����
	VD_BOOL	m_bResizing;	//ҳ���Ƿ����ƶ�״̬
	VD_BOOL	m_bResizable;	//�Ƿ�����ƶ�ҳ��
	int		m_iResizeXOffs;	//ʹ������ƶ�ҳ��ʱ�������x����
	int		m_iResizeYOffs; //ʹ������ƶ�ҳ��ʱ�������y����

	//child items of page
	CItem *	m_pChildHead;		//the first child item added
	CItem *	m_pChildTail;		//the last child item added
	CItem * m_pChildDflt;		//default item
	CItem * m_pChildFocus;		//the input focus item
	CItem * m_pChildFocusOld;	//store the old focus when it missed
	CItem * m_pChildSelect;		//the select item
	CItem * m_pChildSelectOld;	//store the old select when it missed

	int		m_TitleWidth;		//���������
	int		m_TitleHeight;		//�������߶�

	VD_BITMAP*	m_pIconBmp;     //������ͼ��
	VD_BITMAP* m_pNorBmp;
	VD_BITMAP* m_pSelBmp;
	
	VD_BITMAP *bmp_frame_title;
	VD_BITMAP *bmp_frame_mid;
	VD_BITMAP *bmp_frame_bottom;

	static VD_BITMAP* bmp_key_pageup;
	static VD_BITMAP* bmp_key_pagedown;
	static VD_BITMAP* bmp_key_function;
	static VD_BITMAP* bmp_key_enter;
	static VD_BITMAP* bmp_button_close[4];
	static VD_BITMAP* icon_dvr;

	int m_mouseX;
	int m_mouseY;

private:
	int	m_nAge;//����,��graphics��һ��ʱ��Ҫ����
	CPage* m_pPopupCaller;		//�Ի���ĵ�����

	typedef std::map<std::string, CPage*> PAGE_MAP;
	static  PAGE_MAP m_existentPages[N_GUI_CORE];

	static int sm_layoutSpace[spaceCount];

private:
	void UpdateInputTray();

protected:
	void	Draw();//Draw
	UDM		PopupRun();
	void	AddItem(CItem *pItem);//���ؼ����뵽ҳ��
	void	DeleteItem(CItem *pItem);//���ؼ���ҳ����ɾ��

	CItem*	ShiftItem(int key, CItem *pCur, CItem *pOld);
	CItem* 	FindStartItem();
	CItem*	GetItemAt(int px, int py);

	virtual VD_BOOL InvalidateRgn(CRgn *  pRgn);
	virtual VD_BOOL ValidateRgn(CRgn *  pRgn);
	virtual void UpdateRgn();
	virtual void ClearRgn();

	void	ResizeStart();
	void	ResizeEnd();
	void	KillFocus();
	void	Resize(int ox, int oy);
	void	Scroll(int oPage);
	void	SetTitleSize(int w, int h);
	void	SetMargin(int l, int t, int r, int b);

	virtual void OnChildClosed(CPage *pPage){};

public:	
	virtual void DrawTip(const char *pcTip){} //xym GUI 
	
	void SetPageIcon(PAGEICON pageIcon,VD_BITMAP* bmp);
	VD_BITMAP* GetPageIcon(PAGEICON pageIcon);
	VD_PCSTR  GetTip(void);

	VD_BOOL Open();//��
	VD_BOOL Close();//�ر�
	virtual VD_BOOL Close(UDM mode);//�ر�

	
	virtual void SetMute(BOOL bMute);
	virtual void GetCurPlayMute(uchar* mute);
	virtual void SetCurPlayMute(uchar mute);
	
	virtual VD_BOOL UpdateData(UDM mode){return TRUE;}
	CPage(VD_PCRECT pRect, VD_PCSTR psz, CPage *pParent = NULL, uint style = 0);
	virtual ~CPage();
	void ThreadProc();
	VD_BOOL PreMsgProc(uint msg, uint wpa, uint lpa);//Ԥ������Ϣ
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);//������Ϣ
	virtual VD_BOOL AdjustRect();
	virtual void ShowTip(VD_PCSTR tip, VD_BITMAP * icon = NULL){};
	virtual void SetRect(VD_PCRECT pRect, VD_BOOL bReDraw = TRUE);

	VD_PCSTR	GetTitle();
	void    SetTitle(VD_PCSTR psz);

	CItem*	GetFocusItem();
	CItem*  GetSelectItem();//cw_page
	void	SetDefaultItem(CItem *pItem);
	void	SetSelectItem(CItem *pItem);
	void	SetFocusItem(CItem *pItem);

	CPage* GetParentPage();
	void SetParentPage(CPage *pParent);
	int GetThreadID();
	UDM GetExitState();

	void GetMargin(VD_PRECT rect);
	
	static CPage* FindPage(int screen, VD_PCSTR name, VD_BOOL mustOpened = FALSE);
	//SIZE& GetDefaultSize(void);

	static void SetDefaultBitmap(PageBitmap region, VD_PCSTR name);

	void SetStyle(VD_BOOL iPageMain = FALSE){ishPageMain = iPageMain;};
	
};

#endif //__PAGE_H__
