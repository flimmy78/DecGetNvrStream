

#ifndef __ITEM_H__
#define __ITEM_H__

#include "Devices/DevFrontboard.h"
//#include "Devices/DevVideo.h"
//#include "Devices/DevSplit.h"
#include "Devices/DevMouse.h"
#include "System/Object.h"
#include "MultiTask/Thread.h"
#include "GUI/GDI/DC.h"
#include "Defs.h"
#include <string>

typedef void (CObject::*CTRLPROC)();

typedef enum
{
	IT_BITMAPBUTTON,
		IT_BUTTON,
		IT_CHECKBOX,
		IT_COMBOBOX,
		IT_DATETIMECTRL,
		IT_EDIT,
		IT_GRIDCTRL,
		IT_GROUPBOX,
		IT_IPADDRESSCTRL,
		IT_LISTBOX,
		IT_MENU,
		IT_NUMBERBOX,
		IT_NUMBERPAD,
		IT_PAGE,
		IT_PROGRESSCTRL,
		IT_SCROLLBAR,
		IT_SLIDERCTRL,
		IT_STATIC,
		IT_TIMINGCTRL,
		IT_RADIO,
		IT_CALENDAR,
		//IT_TIMETABLE,
		IT_TABLEBOX,
		IT_RECTIMEBOX,
		IT_NR,
}ITEM_TYPE;

typedef enum
{
	IF_EDITABLED = 0,	//�ؼ��Ƿ��ܱ��༭
	IF_ENABLED ,		//�Ƿ�������̻��������
	IF_POPUP,		//�ؼ��Ƿ񵯳�ʽ
	IF_AUTOFOCUS,	//�Զ��۽������ؼ�ѡ�к��������Ա༭
	IF_CAPTURED ,	//���벶��
	IF_FOCUSED,		//�Ƿ�Ϊ���뽹��
	IF_SHOWN ,		//�Ƿ���ʾ
	IF_SELECTED ,	//�Ƿ�ѡ��
	IF_OPENED ,		//�Ƿ��Ѵ�
	IF_ORDERED,		//�Ƿ����ؼ�����
}ITEM_FLAG;

typedef enum {
	KIF_NUMERIC,			//�����ּ�
		KIF_SHIFT,		//��Shift��
		KIF_CORRECTMAP,	//������ĸӳ������
		KIF_NEWATM,	//�µĶ���ATMǰ���
		KIF_NR,
}KEY_INPUT_FLAG;

typedef enum {
	KI_NUMERIC,			//����
		KI_UPPERCASE,	//��д��ĸ
		KI_LOWERCASE,	//Сд��ĸ
		KI_MARK,		//���÷���
		KI_PINYIN,		//ƴ������
		KI_CONTROL,		//ATMԭʼ���Ƽ�
		KI_ALL,
}KEY_INPUT_TYPE;

typedef enum {
	UDM_EMPTY = 0,
	UDM_OPEN,
	UDM_OK,
	UDM_DFLT,
	UDM_CANCEL,
	UDM_COPY,
	UDM_PASTE,
	UDM_OPENED,
	UDM_CLOSED,
	UDM_GUIDE,
}UDM;

enum
{
	NORMAL,
	SELECT,
	PUSH,
	ELAPSED = PUSH,
	DISABLE,
	BACKGROUND,
};

typedef struct {
	char *name;
	int size;
}CTRL_CLASS;

class CPage;
class CItem;
class CItemThread : public CThread
{
	friend class CPage;
	friend class CItem;
	void ThreadProc();
	CItem * m_pItem;

public:
	CItemThread(VD_PCSTR name, CItem * pItem);
	virtual ~CItemThread();
};

//�ؼ�״̬ 20070412 added by wangqin 
typedef enum {
	ctrlStateNormal		= 0x0001,//��ͨ״̬
	ctrlStateSelect		= 0x0002,//ѡ��
	ctrlStatePushed		= 0x0003,//����
	ctrlstateDisabled	= 0x0004,
}CTRL_STATE;

typedef struct  
{
	CItem * m_pLeftRelated;			//������ؼ�	zx add 2006-03-01
	CItem * m_pRightRelated;		//�ҹ����ؼ�
	CItem * m_pUpRelated;		//�Ϲ����ؼ�
	CItem * m_pDownRelated;	//�¹����ؼ�
}RELATEDITEM;

/*!
	�ؼ����ԣ������Զ����ֺ͹������� 
	8-16λ���ڱ�ʾ�ؼ��������Լ���������������
	��16λ���ڱ�ʾItem�ؼ��Զ��������ԡ�
	ͨ��λ������������жϡ�
*/
typedef enum
{
	//�ؼ��������� 8-16λ��ʾ
	styleNoBorder		= 0x00000100,//�ޱ߿򣬳���Ա�Լ����á�
	styleUseBmp			= 0x00000200,//�ؼ���ͼ���ԣ��ɳ����Լ����ƣ�����Ա�������á�
	//Item���з�����ԣ����д�Item�̳����Ŀؼ����д�����
	stylePopup			= 0x0000400, //popup���Ϳؼ���
	styleEditable		= 0x00000800, //�Ƿ�ɱ༭
	styleAutoFocus		= 0x00001000, //�Զ��۽�    //�Զ��������� ��16λ��ʾ
	//ʣ��3����־λnow
	changeLine		= 0x00010000, // ����
	alignAlt		= 0x00020000, // �ؼ�������ʾ������
	smallWidth		= 0x00040000, // С���
	alignCenter		= 0x00080000, //�о��ж��룬ֻ���еĵ�һ���ؼ���Ч
	alignLeft			= 0x00100000, // �������
	alignRight			= 0x00200000, // ���Ҷ���
	alignTop			= 0x00400000, // ���϶���
	alignBottom			= 0x00800000, // ���¶���
	inflateX			= 0x00300000, // ˮƽ����
	inflateY			= 0x00C00000, // ��ֱ����
	inflateAll			= 0x00F00000, // ȫ��������
	alignTable1		= 0x01000000, // ���뵽��1�У������ǰһ�пؼ��Զ�����
	alignTable2		= 0x02000000, // ���뵽��2�У������ǰһ�пؼ��Զ�����
	alignTable3		= 0x03000000, // ���뵽��3�У������ǰһ�пؼ��Զ�����
	alignTable4		= 0x04000000, // ���뵽��4�У������ǰһ�пؼ��Զ�����
	alignTable5		= 0x05000000, // ���뵽��5�У�Ϊ��Ҫ����ָ��X�������Ŀؼ�����
	alignTable6		= 0x06000000, // ���뵽��6�У�Ϊ��Ҫ����ָ��X�������Ŀؼ�����
	alignTable7		= 0x07000000, // ���뵽��7�У�Ϊ��Ҫ����ָ��X�������Ŀؼ�����
	alignTableMask	= 0x0F000000, //�ж�������

}ITEM_STYLE;
/*end wangqin 20070309 added*/


class CDC;
class CItem : public CObject
{
	friend class CPage;
	friend class CDC;
	friend  class CItemThread;

public:
	const int		m_screen;			//screen ID for each item

protected:
	CPage *	m_pParent;	//��ҳ
	CItem *	m_pOwner;	//ֱ���ؼ�

	//brother items in popup item
	CItem * m_pBrother;		//the brother item
	CItem * m_pBrotherR;		//the reverse brother item
	RELATEDITEM *m_pRelatedItems;	//�����ؼ�

	//opened popup items
	CItem * m_pUp;				//the up popup item
	CItem * m_pDown;			//the down popup item
	static	CItem * m_pTop[N_GUI_CORE];		//the top popup item
	static	CItem * m_pNoTopMost[N_GUI_CORE];//the top popup item, excluding top-most item
	static	CItem * m_pActive[N_GUI_CORE];	//the current popup item
	static	CItem * m_pInputTray[N_GUI_CORE]; //the input tray item
	static	CItem * m_pSpellPad[N_GUI_CORE];  //the spell pad item

	//key input flags
	static VD_BOOL m_bKeyInputFlags[KIF_NR];

protected:
	static int m_nInstance[IT_NR];

	uint m_dwStyle; //ITEM�ؼ�����
	//for states
	uint		m_dwFlags;//�ؼ���־

	CDevGraphics* m_pDevGraphics; //the graphics device
	CCursor* m_pCursor;	//the cursor


protected:
	ITEM_TYPE	m_iType;//�ؼ�����
	std::string	tip;	//��ʾ�ַ���
	CDC		m_DC;//�豸������
	CRect	m_Rect;
	CRect	m_Margin;//4�������ϵı߾�
	CItemThread * m_pThread;
	static CMutex m_MutexMgr;

protected:
	std::string	string;	//�ؼ��ַ��� --by wangqin 20070406�Ӹ����ؼ��Ƶ��˴���

private:
	//region clipping operations
	virtual VD_BOOL InvalidateRgn(CRgn *  pRgn);
	virtual VD_BOOL ValidateRgn(CRgn *  pRgn);
	virtual void UpdateRgn();
	virtual void ClearRgn();

	void GlobalInvalidateRect(VD_PCRECT pRect);
	void GlobalValidateRect(VD_PCRECT pRect);
	void GlobalInvalidateRgn(CRgn *  pRgn);
	void GlobalValidateRgn(CRgn *  pRgn);

private:
	// transform link
	void Attach();
	void Detach();
	void Activate();

protected:
	void ItemProc(CTRLPROC proc);
	VD_BOOL DrawCheck();
	
public:

	uint GetStyle();
	CItem(VD_PCRECT pRect, CPage * pParent, ITEM_TYPE type,uint style = 0);					//���캯��
	virtual ~CItem();										//��������			
	virtual void SetTip(VD_PCSTR psz);									//������ʾ�ַ���
	virtual VD_SIZE& GetDefaultSize(void);//�����ؼ���Ĭ�ϴ�С������֧���Զ����ֵĿؼ��Լ�ʵ�֡�--by wangqin 20070401
	virtual void Draw() = 0;									//����
	virtual VD_BOOL PreMsgProc(uint msg, uint wpa, uint lpa){return FALSE;};	//�¼�Ԥ����
	virtual VD_BOOL MsgProc(uint msg, uint wpa, uint lpa){return FALSE;};	//�¼�����
	VD_BOOL VD_SendMessage(uint msg, uint wpa = 0, uint lpa = 0);
	virtual void SetText(VD_PCSTR psz){};						//���ÿؼ��ַ���
	virtual VD_PCSTR GetText(){return NULL;}					//�õ��ؼ��ַ���
	virtual void Editable(VD_BOOL flag);						//�����Ƿ��ܱ��༭
	virtual void Show(VD_BOOL flag, VD_BOOL redraw = TRUE);							//��ʾ/���ؿؼ�
	virtual void Enable(VD_BOOL flag);							//����/��ֹ�ؼ�
	virtual void Select(VD_BOOL flag);							//ѡ��/��ѡ�пؼ�
	virtual void SetFocus(VD_BOOL flag);						//����Ϊ���뽹��
	virtual void ShowTip(VD_BOOL flag);
	virtual VD_BOOL Open();//��
	virtual VD_BOOL Close();//�ر�
	virtual VD_BOOL AdjustRect(){return TRUE;};
	virtual void AddItem(CItem *pItem){};
	virtual void DeleteItem(CItem *pItem){};
	VD_BOOL IsOpened();										//�õ���/�ر�״̬
	VD_BOOL GetFlag(ITEM_FLAG index);
	void SetFlag(ITEM_FLAG index, VD_BOOL flag);
	uint ReadFlag();//cw_mainpage
	void SetAutoFocus(VD_BOOL flag);			//�����Զ��۽�
	VD_BOOL IsShown();							//�õ���ʾ/����״̬
	VD_BOOL IsEnabled();						//�õ�����/��ֹ״̬
	VD_BOOL IsSelected();						//�õ�ѡ��/��ѡ��״̬
	void SetCapture(VD_BOOL flag);				//������겶��
	VD_BOOL GetCapture();						//�õ���겶��״̬
	VD_BOOL GetFocus();						//�õ����뽹��״̬
	VD_BOOL SetOwner(CItem * pOwner);			//����ӵ����
	CItem * GetOwner();						//�õ�ӵ����
	CItem* GetParent();						//�õ����ڿؼ�
	CItem* GetBrother();					//�õ��ֵܿؼ�
	static	CItem* GetActive(int screen);	//�õ���ǰ�ļ���ؼ�
	CItem*	SetActive();					//����ǰ�ؼ�����
	VD_BOOL	IsActive();						//�����Ƿ�Ϊ����ؼ�
	void	GetRect(VD_PRECT pRect) const;
	virtual void	SetRect(VD_PCRECT pRect, VD_BOOL bReDraw = TRUE);
	CDC * GetDC();
	ITEM_TYPE GetType();

	int		GetScreen(CPage* page);		//��ҳ��ָ�루����αָ�룩�õ�ID��

	static void DumpInstances();
	void	ScreenToClient(VD_PPOINT pPoint);
	void	ScreenToClient(VD_PRECT pRect);
	void	ClientToScreen(VD_PRECT pRect);
	void	GetClientRect(VD_PRECT pRect);
	void	DrawBackground();

	void    SetRelatedItem(CItem *up = NULL, CItem *down = NULL, CItem *left = NULL, CItem *right = NULL);

	std::string GetName();

	static VD_BOOL VD_DispatchMessage(int screen, uint msg, uint wpa, uint lpa);
	static void CloseAll(int iPageCloseFlag = 0);    //modified by nike.xie 20090903//����ҳ���жϱ�־��ȱʡ����
	static void AdjustRectAll();
	static void SetKeyInputFlag(KEY_INPUT_FLAG index, int flag);
	static int GetKeyInputFlag(KEY_INPUT_FLAG index);

	CItemThread* GetThread();
};
#endif //__ITEM_H__
