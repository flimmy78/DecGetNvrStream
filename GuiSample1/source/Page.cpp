

#include "GUI/Ctrls/Page.h"
#include "System/Locales.h"
#include "GUI/Pages/BizData.h"
#include "GUI/Pages/PageSearch.h"
#include "GUI/Pages/PageMessageBox.h"
#include "GUI/Pages/PagePtzCtl.h"

VD_BITMAP* CPage::bmp_key_pageup;
VD_BITMAP* CPage::bmp_key_pagedown;
VD_BITMAP* CPage::bmp_key_function;
VD_BITMAP* CPage::bmp_key_enter;
VD_BITMAP* CPage::bmp_button_close[4];
VD_BITMAP* CPage::icon_dvr;


CPage::PAGE_MAP CPage::m_existentPages[N_GUI_CORE];
int				CPage::sm_layoutSpace[spaceCount] = {6};

///////////////////////////////////////////////////////////////////////////////
/////////             CPage

CPage::CPage(VD_PCRECT pRect, VD_PCSTR psz,CPage *pParent /* = NULL */, uint style /* = 0 */)
	: CItem(pRect, pParent, IT_PAGE, stylePopup|styleEditable|styleAutoFocus|style)
{
//	printf("CPage:CPage start\n");
	
//	printf("CPage:CPage 111\n");
	
	m_playoutSpace = sm_layoutSpace;
 	m_ifPageMain = FALSE;
	bmp_frame_title = VD_LoadBitmap(DATA_DIR"/bmp_frame_title.bmp");
	bmp_frame_mid = VD_LoadBitmap(DATA_DIR"/bmp_frame_mid.bmp");
	bmp_frame_bottom = VD_LoadBitmap(DATA_DIR"/bmp_frame_bottom.bmp");
	
//	printf("CPage:CPage 222\n");
	
	m_pIconBmp = icon_dvr;
	m_pNorBmp =  NULL;
	m_pSelBmp = NULL;
	m_iSpace = 10;
	m_iStartX = CLIENT_X;
	m_iStartY = CLIENT_Y;

	for(int i = 0; i <MAX_TABLE_NUM; i++)
	{
		m_iTable[i] = 0;
	}
	
	m_nAge = -1;
	m_iPage = 0;
	m_nPage = 0;
	m_bResizing = FALSE;
	m_bResizable = FALSE;

	m_pChildFocus = m_pChildFocusOld = NULL;
	m_pChildSelect = m_pChildSelectOld = NULL;
	m_pChildDflt = NULL;
	m_pChildSelect = NULL;
	m_pChildHead = m_pChildTail = NULL;

//	printf("CPage:CPage 333\n");

	if(psz)
	{
		string = GetParsedString(psz);
	}

//	printf("CPage:CPage 444\n");

	m_MutexMgr.Enter();
	m_existentPages[m_screen].insert(PAGE_MAP::value_type(string, this));
	m_MutexMgr.Leave();

//	printf("CPage:CPage 555\n");

	std::string name = "Page-" + string;
	//��������·��볬�����ܵ���thread name������m_Name[32]Խ�絼�³���ɵ������Խ������⴦��������������ʱû������Ӧ�����
	//����ڷ�Ӣ�ĵ�����´�ӡ����thread name�����룬����ֱ�Ӵ����"Page-Long"������Ӱ���������ԡ�
	//2007-05-15 modified by wangqin 
	if (strlen(name.c_str()) >= 31)
	{
		name = "Page-Long";
	}
	m_pThread = new CItemThread(name.c_str(), this);
	
	m_pPopupCaller = NULL;
	
	m_bDrawBkgdOnly = FALSE;
	
//	printf("CPage:CPage 666\n");
	
	if(psz)
	{
		 m_pTitle = GetParsedString(psz);
	}
	
//	printf("CPage:CPage 777\n");

	if(pRect)
	{
		SetRect(pRect, FALSE);
	}

//	printf("CPage:CPage 888\n");

	if(m_dwStyle & pageFullScreen)
	{
		VD_RECT rect;

		m_pDevGraphics->GetRect(&rect);
//		printf("CPage:CPage 81\n");
		SetRect(&rect, FALSE);
//		printf("CPage:CPage 82\n");
		SetTitleSize(rect.right, rect.bottom);
//		printf("CPage:CPage 83\n");
		SetMargin(0, 0, 0, 0);
//		printf("CPage:CPage 84\n");
	}
	
//	printf("CPage:CPage 999\n");

	m_TitleHeight = m_Rect.Height();
	m_TitleWidth = m_Rect.Width();
	m_Margin.SetRectEmpty();
	ishPageMain = FALSE;

//	printf("CPage:CPage end\n");
}

CPage::~CPage()
{
	//printf("CPage::~CPage this=0x%08x\n",(unsigned int)this);
	//fflush(stdout);
	
	m_MutexMgr.Enter();
	m_existentPages[m_screen].erase(string);
	m_MutexMgr.Leave();
	
	if(m_pThread)
	{
		//printf("CPage::~CPage delete m_pThread\n");
		//fflush(stdout);
		
		delete m_pThread;
	}
	
	if(m_playoutSpace != sm_layoutSpace)
	{
		delete m_playoutSpace;
	}
	
	//delete all child items
	CItem *p = m_pChildHead, *temp;
	while(p)
	{
		temp = p->m_pBrother;
		delete p;
		p = temp;
	}
}

void CPage::SetMute(BOOL bMute)
{
	
}

void CPage::GetCurPlayMute(u8* mute)
{
	
}

void CPage::SetCurPlayMute(u8 mute)
{
	
}

CPage* CPage::FindPage(int screen, VD_PCSTR name, VD_BOOL mustOpened /* = FALSE */)
{
	CGuard guard(m_MutexMgr);

	PAGE_MAP::iterator pi;
	pi = m_existentPages[screen].find(GetParsedString(name));
	if(pi != m_existentPages[screen].end())
	{
		if(mustOpened && !((*pi).second)->IsOpened())
		{
			return NULL;
		}
		return (*pi).second;
	}
	else
	{
		return NULL;
	}
}

//�����
void CPage::AddItem(CItem *pItem)
{
	if(!pItem){
		return;
	}

	if(pItem->m_pBrother || pItem->m_pBrotherR || m_pChildHead == pItem)
	{
		printf("AddItem error %p\n", pItem);
		return;
	}

	CGuard guard(m_MutexMgr);

	//	trace("add item (%d,%d,%d,%d)\n",pItem->x,pItem->y,pItem->m_Rect.Width(),pItem->m_Rect.Height());
	if(m_pChildHead == NULL){
		m_pChildHead = m_pChildTail = pItem;
	}else{
		m_pChildTail->m_pBrother = pItem;
		pItem->m_pBrotherR = m_pChildTail;
		m_pChildTail = pItem;
	}
}

//ɾ����
void CPage::DeleteItem(CItem *pItem)
{
	if(!pItem){
		return;
	}

	if(!pItem->m_pBrother && !pItem->m_pBrotherR && pItem != pItem)
	{
		printf("DeleteItem error %p\n", pItem);
		return;
	}

	CGuard guard(m_MutexMgr);

	if(m_pChildHead == pItem)
	{
		m_pChildHead = m_pChildHead->m_pBrother;
	}
	else
	{
		pItem->m_pBrotherR->m_pBrother = pItem->m_pBrother;
	}

	if(m_pChildTail == pItem)
	{
		m_pChildTail = m_pChildTail->m_pBrotherR;
	}
	else
	{
		pItem->m_pBrother->m_pBrotherR = pItem->m_pBrotherR;
	}

	pItem->m_pBrotherR = NULL;
	pItem->m_pBrother = NULL;
}

CItem* CPage::FindStartItem()
{
	CItem *p = m_pChildHead;
	CItem *best = NULL;
	VD_RECT rect, best_rect;
	VD_BOOL flag;
	
	while(p){
		p->GetRect(&rect);
		flag = FALSE;
		if(p->GetFlag(IF_OPENED) && p->GetFlag(IF_ENABLED) && p->GetFlag(IF_EDITABLED)
			&& !p->GetFlag(IF_POPUP) && p->GetFlag(IF_SHOWN) && p->GetFlag(IF_ORDERED)){
			if(best){
				best->GetRect(&best_rect);
				if(rect.top <= best_rect.bottom
					&& best_rect.top <= rect.bottom){//��ͬһ�У��ж�x����
					flag = rect.left < best_rect.left;
				}else{//����ͬһ�У��ж�y����
					flag = rect.top < best_rect.top;
				}
			}
			if(!best || flag){
				best = p;
			}
		}
		p = p->m_pBrother;
	}
	return best;
}

void CPage::SetDefaultItem(CItem *pItem)
{
	m_pChildDflt = pItem;
}

CItem * CPage::GetFocusItem()
{
	return m_pChildFocus;
}

void CPage::SetFocusItem(CItem *pItem)
{
	if(m_pChildFocus != pItem){
		if(m_pChildFocus){
			m_pChildFocus->SetFocus(FALSE);
			m_pChildFocusOld = m_pChildFocus;
		}
		if(pItem){
			pItem->SetFocus(TRUE);
		}
		m_pChildFocus = pItem;
		UpdateInputTray();
	}
}

CItem* CPage::GetSelectItem()
{
	return m_pChildSelect;
}

//����ѡ���ͬʱ�����Զ��۽�
void CPage::SetSelectItem(CItem *pItem)
{
	if(m_pChildSelect != pItem){
		
		if(m_pChildSelect){
			if(m_pChildSelect->GetFlag(IF_AUTOFOCUS) && m_pChildSelect == m_pChildFocus)
			{
				m_pChildSelect->SetFocus(FALSE);
				m_pChildFocusOld = m_pChildFocus;
			}
			m_pChildSelect->Select(FALSE);
			m_pChildSelectOld = m_pChildSelect;
		}
		
auto_focus:

		#if 1
		if(pItem){
			//printf("pItem->GetFlag(IF_AUTOFOCUS) \n");
			if(pItem->GetFlag(IF_AUTOFOCUS))
			{
				if(m_pChildFocus)
				{
					m_pChildFocus->SetFocus(FALSE);
				}
				pItem->SetFocus(TRUE);
			}
			//printf("item tpye = %d \n",pItem->m_iType);

			if(pItem->m_iType != IT_PAGE)
			{
				pItem->Select(TRUE);
			}
				
		}	
		#endif
		
		if(m_pChildSelect == m_pChildFocus && m_pChildSelect && m_pChildSelect->GetFlag(IF_AUTOFOCUS))
		{
			m_pChildFocus = NULL;
		}
		if(pItem && pItem->GetFlag(IF_AUTOFOCUS))
		{
			m_pChildFocus = pItem;
		}
		UpdateInputTray();
		m_pChildSelect = pItem;
	}
	else if(pItem && pItem->GetFlag(IF_AUTOFOCUS) && m_pChildFocus != pItem)
	{
		goto auto_focus;
	}
}

//�����ı��༭�ķ�ʽ���ƶ��ؼ�
//���Ҽ��������ƶ��������׻���ĩ����
//���¼��ƶ�����һ�л���һ�е�ͬһx����Ŀؼ���
//������һ���п���ߵ�x��������Ŀؼ�
CItem* CPage::ShiftItem(int key, CItem *pCur, CItem *pOld)
{
	if(!pCur){				//��û��ָ����ǰ��
		if(!pOld || !pOld->GetFlag(IF_OPENED) || !pOld->GetFlag(IF_ENABLED) || !pOld->GetFlag(IF_EDITABLED) || !pOld->GetFlag(IF_SHOWN)){
			return pCur;	//��ǰ�����ǰ�����������,����,����������
		}
		pCur = pOld;		//��ǰ����������,��ǰ���Ϊ��ǰ��
	}

	//powered distance, distance in x-axis, distance in y-axis, nearest distance
//	int dn=0;
	CItem *best = NULL;
	CItem *p = m_pChildHead;
	VD_BOOL flag;	//�Ƿ������
	int offs;	//x��y����Ҫ�ӵ�ƫ��
	int offsn = 0;	//�����ӵ�ƫ��

	VD_RECT rect;
	VD_RECT cur_rect;
	VD_RECT best_rect;

	pCur->GetRect(&cur_rect);		//��õ�ǰ����������
	switch(key){
	case KEY_UP:
	case KEY_PREV:
	case KEY_DOWN:
	case KEY_NEXT:
		while(p){
			p->GetRect(&rect);
			flag = FALSE;
			offs = 0;
			if(p != pCur && p->GetFlag(IF_OPENED) && p->GetFlag(IF_ENABLED) //�ؼ���Ч
				&& p->GetFlag(IF_EDITABLED) && !p->GetFlag(IF_POPUP) && p->GetFlag(IF_SHOWN) && p->GetFlag(IF_ORDERED)
				&& (p->m_iType != IT_RADIO || ((CRadio*)p)->IsHead()))//�ǵ�ѡ�����ͷ��
			{
				if(0){
				}else{
					if(rect.bottom < cur_rect.top){
						offs = m_Rect.Height();
					}
					if(best){
						best->GetRect(&best_rect);
						if(rect.top + offs < cur_rect.bottom
							&& cur_rect.top < rect.bottom + offs){//�͵�ǰ����ͬһ�У�ʧ��
							flag = FALSE;
						}
						else if(rect.top + offs < best_rect.bottom + offsn
							&& best_rect.top + offsn < rect.bottom + offs){//��ͬһ�У��ж�x����

							if(rect.left < cur_rect.right && cur_rect.left < rect.right)
							{
								if(best_rect.left < cur_rect.right && cur_rect.left < best_rect.right)
								{
									flag = (rect.left < best_rect.left);
								}
								else
								{
									flag = TRUE;
								}
							}
							else if(best_rect.left < cur_rect.right && cur_rect.left < best_rect.right)
							{
								flag = FALSE;
							}
							else if(rect.left < cur_rect.left)
							{
								if(best_rect.left > cur_rect.left)
								{
									flag = TRUE;
								}
								else
								{
									flag = (rect.left > cur_rect.left);
								}
							}
							else
							{
								if(best_rect.left < cur_rect.left)
								{
									flag = FALSE;
								}
								else
								{
									flag = (rect.left < cur_rect.left);
								}
							}

						}else{//����ͬһ�У��ж�y����
							flag = ((key==KEY_DOWN || key==KEY_NEXT)?
								(rect.top + offs < best_rect.top + offsn)
								:(rect.top + offs > best_rect.top + offsn));
						}
					}
					if((!best
						&& (rect.top + offs > cur_rect.bottom || cur_rect.top > rect.bottom + offs))//�͵�ǰ���ͬһ��
						|| flag){
						best = p;
						offsn = offs;
					}
				}
			}
			p = p->m_pBrother;
		}
		break;
	case KEY_LEFT:
		if(key == KEY_LEFT)
		{
			p = pCur;
			while (p->m_pRelatedItems && p->m_pRelatedItems->m_pLeftRelated)//�����ؼ�����
			{
				p = p->m_pRelatedItems->m_pLeftRelated;
				if(p->GetFlag(IF_OPENED) && p->GetFlag(IF_ENABLED))
				{
					return p;
				}
			}
		}
	case KEY_RIGHT:
		if(key == KEY_RIGHT)
		{
			p = pCur;
			while (p->m_pRelatedItems && p->m_pRelatedItems->m_pRightRelated)//�����ؼ�����
			{
				p = p->m_pRelatedItems->m_pRightRelated;
				if(p->GetFlag(IF_OPENED) && p->GetFlag(IF_ENABLED))
				{
					return p;
				}
			}
		}
		p = m_pChildHead;
		while(p){
			p->GetRect(&rect);
			flag = FALSE;
			offs = 0;
			if(p != pCur && p->GetFlag(IF_OPENED) && p->GetFlag(IF_ENABLED) //�ؼ���Ч
				&& p->GetFlag(IF_EDITABLED) && !p->GetFlag(IF_POPUP) && p->GetFlag(IF_SHOWN) && p->GetFlag(IF_ORDERED)
				&& (p->m_iType != IT_RADIO || ((CRadio*)p)->IsHead()))//�ǵ�ѡ�����ͷ��
			{
				if(rect.top < cur_rect.bottom && cur_rect.top < rect.bottom){
					if(rect.left < cur_rect.left){
						offs = m_Rect.Height();
					}
				}else if(rect.top < cur_rect.top){
					offs = m_Rect.Height();
				}
				if(best)
				{
					best->GetRect(&best_rect);
					if(rect.top + offs < best_rect.bottom + offsn
						&& best_rect.top + offsn < rect.bottom + offs)
					{//��ͬһ�У��ж�x����
						if (best_rect.top < cur_rect.bottom && cur_rect.top < best_rect.bottom)
						{
							flag = ((key == KEY_RIGHT)
								? ((rect.left < best_rect.left) && (rect.left > cur_rect.left))
								: ((rect.left > best_rect.left) && (rect.left < cur_rect.left)));
						}
						else
						{
							flag = ((key == KEY_RIGHT)?(rect.left < best_rect.left):(rect.left > best_rect.left));
						}
					}
					else
					{//����ͬһ�У��ж�y����
						flag = ((key==KEY_RIGHT)?(rect.top + offs < best_rect.top + offsn):(rect.top + offs > best_rect.top + offsn));
					}
				}
				if(!best || flag){
					best = p;
					offsn = offs;
				}
			}
			p = p->m_pBrother;
		}
		break;
	default:
		return pCur;
	}
	if(best){
		return best;
	}
	return pCur;
}

CItem * CPage::GetItemAt(int px, int py)
{
	//trace("GetItem(%d,%d)\n", px, py);
	CItem *p;
	VD_RECT rect;

	p = m_pTop[m_screen];
	while(p && p != this)
	{
		p->GetRect(&rect);
		if(p->m_pParent == this && p->GetFlag(IF_ENABLED) && p->GetFlag(IF_EDITABLED) && PtInRect(&rect, px, py) && p->GetFlag(IF_OPENED) && p->GetFlag(IF_SHOWN)){	
			return p;
		}
		p = p->m_pDown;
	}

	p = m_pChildHead;
	while(p){
		p->GetRect(&rect);
		if(!p->GetFlag(IF_POPUP) && p->GetFlag(IF_ENABLED) && p->GetFlag(IF_EDITABLED) && PtInRect(&rect, px, py) && p->GetFlag(IF_OPENED) && p->GetFlag(IF_SHOWN)){	
			return p;
		}
		p = p->m_pBrother;
	}
	return NULL;
}

VD_BOOL CPage::AdjustRect()
{
	CGuard guard(m_MutexMgr);

	VD_RECT rect;
	int age = m_pDevGraphics->GetAge();

	//���䲻һ��ʱ��Ҫ����
	if(age == m_nAge)
	{
		return FALSE;
	}

	m_nAge = age;

	CItem *p;
	p = m_pChildHead;
	while(p)
	{
		p->m_DC.Lock();
		p->m_DC.Update();
		p->m_DC.UnLock();
		p = p->m_pBrother;
	}

	m_DC.Lock();
	m_DC.Update();
	m_DC.UnLock();

	m_pDevGraphics->GetRect(&rect);

	if(m_dwStyle & pageFullScreen)
	{
		SetRect(&rect,FALSE);//�Ȳ�ˢ�£������drawһ��
	}
	else if(m_dwStyle & pageAlignCenter)
	{
		if(ishPageMain)
		{
			if(0/*g_CapsEx.ForATM*/)
			{
				if((rect.right - rect.left == 704) && (rect.bottom - rect.top == 528))//���640*480�ֱ�����ҳ�泬����Ļ����
				{
					SetRect(CRect((rect.right-m_Rect.Width())/2, (rect.bottom-m_Rect.Height()-20), 
						(rect.right+m_Rect.Width())/2, (rect.bottom-20)), FALSE);
				}
				else
				{
					SetRect(CRect((rect.right-m_Rect.Width())/2, (rect.bottom-m_Rect.Height()-70), 
						(rect.right+m_Rect.Width())/2, (rect.bottom-70)), FALSE);
				}
			}
			else
			{
				
				SetRect(CRect((rect.right-m_Rect.Width())/2, (rect.bottom-m_Rect.Height()-70), 
					(rect.right+m_Rect.Width())/2, (rect.bottom-70)), FALSE);
			}
		}
		else
		{
			SetRect(CRect((rect.right-m_Rect.Width())/2, (rect.bottom-m_Rect.Height())/2, 
				(rect.right+m_Rect.Width())/2, (rect.bottom+m_Rect.Height())/2), FALSE);
		}
	}

	return TRUE;
}

void CPage::SetRect(VD_PCRECT pRect, VD_BOOL bReDraw)
{
//	printf("CPage:SetRect 1\n");
	CGuard guard(m_MutexMgr);
	
	assert(pRect);
	//save old rectangle
	VD_RECT old_rect = m_Rect;
	CRgn rgn;
	
	//set owner rectangle, clip parent rectangle
	m_Rect = *pRect;
	
	//m_DC.SetBound(pRect);
//	printf("CPage:SetRect2\n");
	if(memcmp(pRect, &old_rect, sizeof(VD_RECT)) == 0)
	{
		return;
	}
	
//	printf("CPage:SetRect 3\n");
	//set children rectangle
	CRect item_rect;
	CItem *p = m_pChildHead;
	while(p)
	{
		if(!p->GetFlag(IF_POPUP) )
		{
			if(!(p->GetOwner()&& (p->m_iType ==IT_SCROLLBAR)))
			{
				//caculate items rectangle
				p->GetRect(&item_rect);
				item_rect.OffsetRect(m_Rect.left - old_rect.left, m_Rect.top - old_rect.top);
				p->SetRect(&item_rect, FALSE);
			}
		}
		p = p->m_pBrother;
	}

//	printf("CPage:SetRect 4\n");
	rgn.CreateRectRgn(&old_rect);
//	printf("CPage:SetRect 5\n");
	rgn.SubtractRect(m_Rect);

//	printf("CPage:SetRect 6\n");
	//����������������
	GlobalInvalidateRect(m_Rect);
//	printf("CPage:SetRect 7\n");
	GlobalValidateRgn(&rgn);
	if(m_TitleWidth){
		m_TitleWidth = m_Rect.Width();
	}

//	printf("CPage:SetRect 8\n");
	//��������������
	UpdateRgn();
	
//	printf("CPage:SetRect 9\n");
	if(bReDraw)
	{
		Draw();
	}
//	printf("CPage:SetRect 10\n");
}

void CPage::Resize(int ox, int oy)
{
	VD_RECT rect;
	CDC dc;

	if(ox == 0 && oy == 0)
	{
		return;
	}
	
	m_pDevGraphics->GetRect(&rect);
	
	dc.Enable(TRUE);
	dc.Lock();
	dc.SetRop(ROP_XOR);
	//modefied by nike.xie
	//dc.SetPen(GetSysColor(COLOR_CTRLTEXTSELECTED), PS_SOLID, 2);	
	dc.SetPen(VD_GetSysColor(COLOR_POPUP), VD_PS_SOLID, 2);
	//end
	dc.SetRgnStyle(RS_HOLLOW);
	dc.Rectangle(&m_RectResizing);
	m_pCursor->SetCursorPos(m_RectResizing.left + m_iResizeXOffs, m_RectResizing.top + m_iResizeYOffs);
	m_RectResizing.OffsetRect(ox, oy);
	if(m_RectResizing.left + 32 > rect.right)
	{
		m_RectResizing.OffsetRect(rect.right - m_RectResizing.left - 32, 0);
	}
	if(m_RectResizing.top + 32 > rect.bottom)
	{
		m_RectResizing.OffsetRect(0, rect.bottom - m_RectResizing.top - 32);
	}
	if(m_RectResizing.right - 32 < rect.left)
	{
		m_RectResizing.OffsetRect(rect.left - m_RectResizing.right + 32, 0);
	}
	if(m_RectResizing.bottom - 32 < rect.top)
	{
		m_RectResizing.OffsetRect(0, rect.top - m_RectResizing.bottom + 32);
	}
	dc.Rectangle(&m_RectResizing);
	dc.SetRop(ROP_COPY);
	dc.UnLock();
	dc.Enable(FALSE);
}

void CPage::ResizeStart()
{
	m_bResizing = TRUE;
	SetFlag(IF_CAPTURED, true);
	GetRect(&m_RectResizing);
}

void CPage::ResizeEnd()
{
	m_bResizing = FALSE;
	SetFlag(IF_CAPTURED, false);
	Show(FALSE);  //new version code
	SetRect(m_RectResizing);
	Show(TRUE);   //new version code
}

//���뽹�㶪ʧ
void CPage::KillFocus()
{
	m_bLClicked = FALSE;
	m_bRClicked = FALSE;
	m_bMClicked = FALSE;
}

void CPage::Scroll(int oPage)
{
	
}

void CPage::SetTitleSize(int w, int h)
{
	m_TitleHeight = h;
	m_TitleWidth = w;
}

void CPage::SetMargin(int l, int t, int r, int b)
{
	m_Margin.SetRect(l, t, r, b);
}

CPage* CPage::GetParentPage()
{
	return m_pParent;
}

//��ҳ��
VD_BOOL CPage::Open()
{
	//CDevMouse* mouse1 = GetDevMouse();
	//mouse1->ClearSignal();
	
	//SetActive();
	//WaitForSem();
	
	//ReleaseSem();
	
	//this->Activate();
	//m_pActive[m_screen] = this;
	
	//printf("VD_BOOL CPage::Open()111111111111111   \n");
	//m_pCursor->Enable(FALSE);
	
	//SetSystemLockStatus(1);
	m_pCursor->SetCursor(CR_BUSY);
	if(!UpdateData(UDM_OPEN))
	{
		//update failed
		m_pCursor->SetCursor(CR_NORMAL);
		//m_pCursor->Enable(TRUE);
		//ReleaseSem();
		//SetSystemLockStatus(0);
		
		return FALSE;
	}
	m_pCursor->SetCursor(CR_NORMAL);
	
	m_MutexMgr.Enter();
	if(GetFlag(IF_OPENED))
	{
		//trace("CPage::Open(),page `%s` is already opened.\n", m_pTitle);
		//printf("page '%s' is already opened.\n",m_pTitle.c_str());
		printf("page is already opened.\n");
		m_MutexMgr.Leave();
		SetActive();// ����Ѿ��򿪣������Լ�
		//m_pCursor->Enable(TRUE);
		//ReleaseSem();
		//SetSystemLockStatus(0);
		
		return FALSE;
	}
	SetFlag(IF_OPENED, TRUE);//��������״̬��λ,��ֹ��δ�
	m_MutexMgr.Leave();
	//printf("CPage::Open 1\n");
	AdjustRect();
	//printf("CPage::Open 2\n");
	m_DC.Lock();
	m_DC.Enable(TRUE);
	m_DC.UnLock();
	
	//printf("CPage::Open 3\n");
	
	//�����пؼ�
	CItem *p = m_pChildHead;
	while(p)
	{
		if(!p->GetFlag(IF_POPUP))
		{
			p->Open();
			//p->Draw();
		}
		p = p->m_pBrother;
	}
	
	m_pChildFocusOld = m_pChildSelectOld = NULL;
	
	//printf("CPage::Open 4\n");
	p = FindStartItem();
	
	//printf("CPage::Open 5\n");
	if(!p || p->GetFlag(IF_AUTOFOCUS))
	{
		m_pChildFocus = p;
	}
	m_pChildSelect = p;
	
	//printf("CPage::Open 6\n");
	
	//printf("Show(TRUE, FALSE);\n");
	Show(TRUE);
	
	//printf("CPage::Open 7\n");
	UpdateInputTray();
	//printf("CPage::Open 8\n");
	//ҳ����ȫ�򿪺�Ĳ����������������ý���
	//set focus state, no need draw
	if(m_pChildSelect)
	{
		if(m_pChildSelect->GetFlag(IF_AUTOFOCUS))
		{
			m_pChildSelect->SetFocus(TRUE);
		}
		m_pChildSelect->Select(TRUE);
	}
	//printf("CPage::Open 9\n");
	
	UpdateData(UDM_OPENED);
	//printf("CPage::Open 10\n");
	//delete by nike.xie 2009-08-04 ɾ���˹��ܺ�,�ؼ�����ָ�����
	
	#if 0
	//�����еľ�̬�ؼ�������������
	CItem *pItem = m_pChildHead;
	
	m_pCursor->ShowCursor(FALSE);
	while(pItem)
	{
		if(pItem->m_iType == IT_STATIC)
		{
			CRect rect;
			
			pItem->GetRect(rect);
			CDevGraphics::instance()->DeFlicker(rect);
		}
		pItem = pItem->m_pBrother;
	}
	m_pCursor->ShowCursor(TRUE);
	#endif
	
	//printf("CPage::Open 11\n");
	KillFocus();
	//printf("CPage::Open 12\n");
	
	if(m_pThread && !(m_dwStyle & pageDialog))
	{
		//printf("CPage::Open CreateThread\n");
		//fflush(stdout);
		
		m_pThread->CreateThread();
	}
	//printf("CPage::Open 13\n");
	
	//m_pCursor->Enable(TRUE);
	
	//printf("VD_BOOL CPage::Open()222222222222222222  \n");
	//ReleaseSem();
	
	//SetSystemLockStatus(0);
	
	//GraphicsSyncVgaToCvbs();//csp modify
	
	//csp modify
	if(CDC::GetModify())
	{
		GraphicsSyncVgaToCvbs();
		CDC::SetModify(FALSE);
	}
	
	//printf("CPage::Open finish\n");
	//fflush(stdout);
	
	return TRUE;
}

VD_BOOL CPage::Close(UDM mode)
{
	m_pCursor->SetCursor(CR_BUSY);
	if(!UpdateData(mode)){
		m_pCursor->SetCursor(CR_NORMAL);
		return FALSE;
	}
	m_pCursor->SetCursor(CR_NORMAL);
	m_ExitState = mode;
	
	return Close();
}

//#define GUI_TEST//csp modify

//��
VD_BOOL CPage::Close()
{
	if (m_dwStyle &pageChild)
	{
		//printf(" page child close \n");
		Show(FALSE);
		return TRUE;
	}
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-1\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	m_MutexMgr.Enter();
	if(!GetFlag(IF_OPENED)){
		//printf("CPage::Close(),page is already closed.\n");
		m_MutexMgr.Leave();
		return FALSE;
	}
	SetFlag(IF_OPENED, FALSE);
	m_MutexMgr.Leave();
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-2\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	m_DC.Lock();
	m_DC.Enable(FALSE);
	m_DC.UnLock();
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-3\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	//printf("CPage::Close\n");
	//�ر����пؼ�
	CItem *p = m_pChildHead;
	while(p)
	{
		if(!p->GetFlag(IF_POPUP))
		{
			p->Close();
		}
		p = p->m_pBrother;
	}
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-4\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	Show(FALSE);
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-5\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	UpdateInputTray();
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-6\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	//set focus state, no need draw
	//���ԭ����ؼ��ĸ����ؼ��򿪻�رգ��˴����ܻ�����������
	SetFocusItem(NULL);
	SetSelectItem(NULL);
	UpdateData(UDM_CLOSED);
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-7\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	KillFocus();
	
#ifdef GUI_TEST
	if(CDC::GetModify())//csp modify
	{
		printf("CPage::Close:framebuffer changed-8\n");
		CDC::SetModify(FALSE);
	}
#endif
	
	if(m_pThread && !(m_dwStyle & pageDialog))
	{
		m_pThread->DestroyThread();
	}
	
	return TRUE;
}

VD_BOOL CPage::InvalidateRgn(CRgn * pRgn)
{
	CItem *pItem;
	pItem = m_pChildHead;
	while(pItem){
		if(!pItem->GetFlag(IF_POPUP))
		{
			pItem->InvalidateRgn(pRgn);
		}
		pItem = pItem->m_pBrother;
	}

	CItem::InvalidateRgn(pRgn);

	return TRUE;
}

//���ܻ�ı�pRgn������
VD_BOOL CPage::ValidateRgn(CRgn * pRgn)
{
	//draw validate region
	CItem *pItem;
	UpdateRgn();
	pItem = m_pChildHead;
	while(pItem){
		if(!pItem->GetFlag(IF_POPUP))
		{
			pItem->m_DC.Lock();
			pItem->m_DC.IntersectRgn(pRgn);
			pItem->m_DC.UnLock();
		}
		pItem = pItem->m_pBrother;
	}

	m_DC.Lock();
	m_DC.IntersectRgn(pRgn);
	m_DC.UnLock();

	Draw();

	//recover visible region
	UpdateRgn();

	return TRUE;
}

void CPage::UpdateRgn()
{
	CRgn rgn;
	VD_RECT rect;
	//printf("CPage:UpdateRgn 0\n");
	CGuard guard(m_MutexMgr);

	//printf("CPage:UpdateRgn 1\n");
	if(!GetFlag(IF_OPENED))
	{
		return;
	}
	
	
	CItem *pChild = m_pChildHead;
	CItem * pItem = m_pUp;
	
	m_DC.Lock();
//	printf("CPage:UpdateRgn 2\n");
	m_DC.SetBound(m_Rect);
	//printf("CPage:UpdateRgn 3\n");

	//m_DC.UnLock();

	//printf("this = 0x%x child = 0x%x \n", this, pChild );

	while(pChild){
		//caculate items rectangle
		//printf("pChild 0x%x", pChild);
		pChild->UpdateRgn();
		pChild = pChild->m_pBrother;
	}

	//m_DC.Lock();

//	printf("CPage:UpdateRgn 4\n");
	while(pItem)
	{
		//get rect of top item
		pItem->GetRect(&rect);
		
		//invalid unvisible region
		pChild = m_pChildHead;
		while(pChild)
		{
			if(!pChild->GetFlag(IF_POPUP))
			{
				pChild->m_DC.Lock();
				pChild->m_DC.SubtractRect(&rect);
				pChild->m_DC.UnLock();
			}
			pChild = pChild->m_pBrother;
		}
		m_DC.SubtractRect(&rect);
		
		//point to next top item
		pItem = pItem->m_pUp;
	}
//	printf("CPage:UpdateRgn 5\n");
	m_DC.UnLock();
//	printf("CPage:UpdateRgn 6\n");
}

void CPage::ClearRgn()
{
	CItem *pChild = m_pChildHead;
	
	m_DC.Lock();
	m_DC.SetBound(m_Rect);
	m_DC.UnLock();
	
	while(pChild)
	{
		//caculate items rectangle
		pChild->ClearRgn();
		pChild = pChild->m_pBrother;
	}
}

//����ҳ��
void CPage::Draw()
{
	if(!DrawCheck()){
		return;
	}
	
	m_DC.Lock();//����m_bDrawBkgdOnly
	//�����ٵ�������Ļ��ƺ���������������ѭ����
	if(m_bDrawBkgdOnly)
	{
		m_DC.UnLock();
		return;
	}
	
	CItem *p = m_pChildHead;
	while(p){
		//p->GetRect(&rect);
		//trace("item (%d %d %d %d)\n", rect.left, rect.top, rect.right, rect.bottom);
		if(!p->GetFlag(IF_POPUP) && p->GetFlag(IF_SHOWN))
		{
			//printf("Draw:  %x \n",p);
			p->Draw();
			//p->Open();
		}
		p = p->m_pBrother;
	}
	m_DC.UnLock();
}

//���Ƚ���������ʾ����
VD_BOOL CPage::PreMsgProc(uint msg, uint wpa, uint lpa)
{
	CItem *pItemSpecial = NULL;

	switch(msg)
	{
	case XM_KEYDOWN:
	case XM_KEYUP:
		pItemSpecial = m_pInputTray[m_screen];
		if(pItemSpecial && pItemSpecial->GetOwner() == m_pChildFocus && m_pChildFocus
			&& (m_pChildFocus->GetType() == IT_EDIT || (m_pChildFocus->GetType() == IT_NUMBERBOX && !GetKeyInputFlag(KIF_NUMERIC)))
			&& pItemSpecial->IsOpened() && pItemSpecial->MsgProc(msg, wpa, lpa))
		{
			return TRUE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}

VD_BOOL CPage::MsgProc(uint msg, uint wpa, uint lpa)
{
	//printf("CPage::MsgProc 22222\n");
	//printf("CPage::MsgProc msg: 0x%x\n", msg);
	
	if(m_dwStyle & pageChild)
	{
		m_pParent->MsgProc(msg, wpa, lpa);
	}
	
	CItem *pItemSpecial = NULL;
	int ret;
	int key;
	int px;
	int py;
	CItem* pItem;
	VD_RECT full_rect;
	m_ExitState = UDM_EMPTY;
	if(m_bResizing){
		printf("CPage::MsgProc m_bResizing\n");
		goto resizing_proc;
	}
	switch(msg)
	{
	case XM_KEYDOWN://��������
		//printf("file:%s, line:%d\n", __FILE__, __LINE__);
		key = wpa;
		switch(key){
		case KEY_RET:		//ȷ�ϼ�
			ret = m_pChildFocus?m_pChildFocus->MsgProc(msg, wpa, lpa):FALSE;
			if(!ret)
			{
				if(m_pChildDflt)
				{
					SetFocusItem(m_pChildDflt);
					SetSelectItem(m_pChildDflt);
				}
				else
				{
					//return Close(UDM_OK);  //cw_panel close induce page exit
				}
				UpdateInputTray();
			}
			break;
		case KEY_ESC:		//ȡ����
			ret = (m_pChildFocus)?m_pChildFocus->MsgProc(msg, wpa, lpa):FALSE;
			if(m_pChildFocus && !m_pChildFocus->GetFlag(IF_AUTOFOCUS))//�ؼ��˳�����״̬
			{
				m_pChildFocus->SetFocus(FALSE);
				m_pChildFocus = NULL;
			}
			else if(!ret)
			{
				return Close(UDM_CANCEL);
			}
			UpdateInputTray();
			break;
		case KEY_SHUT://cw_panel
			printf("recvfrom frontboard, Forced shutdown\n");
			SetSystemLockStatus(1);
			if(TRUE!=FxnKeycodeShut())
			{
				SetSystemLockStatus(0);//cw_lock
			}
			break;
		case KEY_LEFT:
		case KEY_RIGHT:
		case KEY_UP:
		case KEY_DOWN:
		case KEY_PREV:
		case KEY_NEXT:
			if(!m_pChildFocus || !m_pChildFocus->MsgProc(msg, wpa, lpa))
			{
				pItem = ShiftItem(key, m_pChildSelect, m_pChildSelectOld);//�л�ѡ����
				if(pItem == NULL){//���㲻���ڻ��߽���Է��������Ӧ
					return FALSE;
				}
				SetSelectItem(pItem);//����ѡ����
			}
			break;
		/*  //cw_panel
		case KEY_SPLIT:
			PreviewToNextMod();
			break;
		*/
		case KEY_CHNPLUS:
			//printf("KEY_CHNPLUS - PreviewToNextPic\n");
			PreviewToNextPic();
			break;
		case KEY_CHNMINUS:
			PreviewToLastPic();
			break;
		case KEY_MUTE:
			{
				CPage** page = GetPage();
				if(this == page[EM_PAGE_PLAYBACK])
				{
					u8 mute;
					this->GetCurPlayMute(&mute);
					this->SetCurPlayMute(mute?0:1);
				}
				else
				{
					u8 chn, vol, mute;
					if(0 == GetAudioOutInfo(&chn, &vol, &mute))
					{
						SetAudioOutInfo(chn, vol, mute?0:1);
						ControlMute(mute?0:1);
						CPage** page = GetPage();
						page[EM_PAGE_START]->SetMute((BOOL)mute?0:1);
					}
				}
			}	
			break;
		case KEY_VOIP:
			break;
		case KEY_REC://Enter record page
		case KEY_SEARCH://Enter search page
		case KEY_PLAY://Enter search page
		case KEY_INFO://Enter info address page
		case KEY_ALARM:
		case KEY_MENU:
		case KEY_BACKUP:
		case KEY_ALARMIN:
		case KEY_PTZ:
			{
				CPage** page = GetPage();
				if(this == page[EM_PAGE_PLAYBACK])
				{
					break;
				}
			}
			
			if(!IsGUIUserLogined())
			{
				break;
			}
			ret = m_pChildFocus?m_pChildFocus->MsgProc(msg, wpa, lpa):FALSE;
			//if(!ret && !(m_dwStyle & pageDialog) && m_pParent != m_pInputTray[m_screen])//����ʽ���ں�ѡ�ʴ��ڲ��ô���ϵͳ��,��������
			//printf("ret = %d\n", ret);
			if(!ret)
			{
				if(key == KEY_REC)
				{
					pItemSpecial = FindPage(m_screen, "&CfgPtn.Record", FALSE);
				}
				else if(key == KEY_SEARCH || key == KEY_PLAY)
				{
					//pItemSpecial = FindPage(m_screen, "&titles.search");
					//yaogang modify 20141106
					//pItemSpecial = FindPage(m_screen, "&CfgPtn.DataSearch", FALSE);
					pItemSpecial = FindPage(m_screen, "&CfgPtn.VideoPlayback", FALSE);
				}
				else if(key == KEY_INFO)
				{
					pItemSpecial = FindPage(m_screen, "&CfgPtn.System", FALSE);
				}
				else if(key == KEY_ALARM)
				{
					pItemSpecial = FindPage(m_screen, "&CfgPtn.Alarm", FALSE);
				}
				else if (key == KEY_BACKUP)
				{
					pItemSpecial = FindPage(m_screen, "&CfgPtn.Backup");
				}
				else if(key == KEY_MENU)
				{
					pItemSpecial = FindPage(m_screen, "&CfgPtn.MAINMENU", FALSE);					
				}
				else if (key == KEY_ALARMIN)
				{					
					pItemSpecial = FindPage(m_screen, "&CfgPtn.SensorTitle", FALSE);
				}
				else if (key == KEY_PTZ)
				{					
					pItemSpecial = FindPage(m_screen, " Ptz Control", FALSE);
				}
				if(pItemSpecial)
				{
					if(key==KEY_PTZ)
					{
						if(((CPagePtzCtl* )pItemSpecial)->GetCruiseState())
							return FALSE;
					}
					
					if(pItemSpecial == this) //���˵��Ѿ���, ������Ӧ�˰���, ����CloseAll+Open����������
					{
						pItemSpecial = NULL;
					}
					else
					{
						CloseAll();
					}
				}
				
				if(pItemSpecial)
				{
					//cw_panel
					if(key == KEY_SEARCH || key == KEY_PLAY)
					{
						((CPageSearch* )pItemSpecial)->UpdateData(UDM_EMPTY);
					}
					
					if(pItemSpecial->Open())
					{
						if (key == KEY_BACKUP)
						{
							//ģ�ⷢ��
							pItemSpecial->MsgProc(msg, wpa, lpa);
							pItemSpecial->MsgProc(XM_KEYUP, wpa, lpa);
						}
                        //add by Lirl on Nov/08/2011
                        if(key == KEY_SEARCH || key == KEY_PLAY)
                        {
                            ((CPageSearch*)pItemSpecial)->FromPanel();
                        }
                        //end
					}
				}
			}
			break;
		default://other keys
			{
				//printf("default key\n");
				return m_pChildFocus?m_pChildFocus->MsgProc(msg, wpa, lpa):FALSE;
			}
		}
		break;

	case XM_MOUSEMOVE:		//����ƶ�
		px = VD_HIWORD(lpa);
		py = VD_LOWORD(lpa);
		if(!m_pChildFocus || !m_pChildFocus->GetFlag(IF_CAPTURED))//����û�б�����
		{
			pItem = GetItemAt(px, py);
			SetSelectItem(pItem);
		}
		ret = m_pChildFocus?m_pChildFocus->MsgProc(msg, wpa, lpa):FALSE;
		return ret;

	case XM_LBUTTONDOWN:	//����������
	case XM_LBUTTONDBLCLK:	//������˫��
		//printf("CPAGE:MAG:LBUTTONDOWN 111, m_bLClicked: %d\n", m_bLClicked);
		if(!m_bLClicked){//˫��ת��Ϊ����
			msg = XM_LBUTTONDOWN;
			m_bLClicked = TRUE;
		}
		goto mouse_proc;
	case XM_RBUTTONDOWN:	//����Ҽ�����
	case XM_RBUTTONDBLCLK:	//����Ҽ�˫��
		if(!m_bRClicked){//˫��ת��Ϊ����
			msg = XM_RBUTTONDOWN;
			m_bRClicked = TRUE;
		}
		goto mouse_proc;
	case XM_MBUTTONDOWN:	//����м�����
	case XM_MBUTTONDBLCLK:	//����м�˫��
		if(!m_bMClicked){//˫��ת��Ϊ����
			msg = XM_MBUTTONDOWN;
			m_bMClicked = TRUE;
		}
		goto mouse_proc;
	case XM_LBUTTONUP:		//�������
	case XM_RBUTTONUP:		//�Ҽ�����
	case XM_MBUTTONUP:		//�м�����
mouse_proc:
		//printf("CPAGE:MAG:LBUTTONDOWN mouse_proc\n");
		m_mouseX = px = VD_HIWORD(lpa);
		m_mouseY = py = VD_LOWORD(lpa);
		if(m_pChildFocus && m_pChildFocus->GetFlag(IF_CAPTURED) && m_pChildFocus->MsgProc(msg, wpa, lpa))
		{//���뱻���㲶�������ɽ��㴦��
			//printf("yg mouse_proc 1\n");
			return TRUE;
		}else{
			//printf("yg mouse_proc 2\n");
			pItem = GetItemAt(px, py);		//��ȡ��ǰ����ؼ�
			if(msg == XM_LBUTTONDOWN || msg == XM_LBUTTONDBLCLK)
			{
				//printf("yg mouse_proc 3\n");
				if(!pItem)
				{
					//printf("yg mouse_proc 4\n");
					SetSelectItem(NULL);
					m_pDevGraphics->GetRect(&full_rect);
					if(m_iType == IT_PAGE
						&& (msg == XM_LBUTTONDOWN)
						&& px >= m_Rect.left && py >= m_Rect.top && px <= m_Rect.right && py <= m_Rect.top + m_TitleHeight
						&& m_Rect != full_rect && m_bResizable)
					{//��ʼ�ƶ�ҳ��
						//printf("yg mouse_proc 5\n");
						m_iResizeXOffs = px - m_Rect.left;
						m_iResizeYOffs = py - m_Rect.top;
						ResizeStart();
						break;
					}
					return FALSE;
				}
				//printf("yg mouse_proc 6\n");
				if(pItem != m_pChildFocus )
				{
					//printf("yg mouse_proc 7\n");
					SetFocusItem(pItem);
					SetSelectItem(pItem);
				}
				//printf("yg mouse_proc 8\n");
				if(pItem->MsgProc(msg, wpa, lpa)){		//ת����Ӧ�ؼ��Ĵ�����
					//printf("cc pItem->MsgProc \n");
					break;
				};
			}
			
			if(msg == XM_RBUTTONDOWN || msg == XM_RBUTTONDBLCLK){//�Ҽ�˫���򵥻�����ȡ���˳�
				//if(m_bPlayBackPage)
				//{
				//
				//}
				//else
				//{
					//printf("CPage::Close(UDM_CANCEL)\n");
					if(Close(UDM_CANCEL)){
						break;
					}
				//}
			}
			/*if(msg == XM_LBUTTONDBLCLK){//���˫�����򱣴��˳�
				if(Close(UDM_OK)){
					break;
				};
			}*/
			//printf("yg mouse_proc m_pChildFocus: %d\n", m_pChildFocus);
			return m_pChildFocus?m_pChildFocus->MsgProc(msg, wpa, lpa):FALSE;
		}
		break;

	default:
		return m_pChildFocus?m_pChildFocus->MsgProc(msg, wpa, lpa):FALSE;
	}
	
	return TRUE;
	
resizing_proc:
	switch(msg){
	case XM_KEYDOWN:
		key = wpa;
		switch(key){
		case KEY_LEFT:
			Resize(-16, 0);
			break;
			
		case KEY_RIGHT:
			Resize(16, 0);
			break;
			
		case KEY_UP:
		case KEY_PREV:
			Resize(0, -16);
			break;
			
		case KEY_DOWN:
		case KEY_NEXT:
			Resize(0, 16);
			break;
			
		default://other keys
			return FALSE;
		}
		break;
		
	case XM_MOUSEMOVE:
		px = VD_HIWORD(lpa);
		py = VD_LOWORD(lpa);
		Resize(px - (m_iResizeXOffs + m_RectResizing.left), py - (m_iResizeYOffs + m_RectResizing.top));
		break;
		
	case XM_LBUTTONUP:
		ResizeEnd();
		break;
		
	default:
		return FALSE;
	}
	return TRUE;
}

UDM CPage::PopupRun()
{
	if(!(m_dwStyle & pageDialog))
	{
		printf("CPage::PopupRun() only for dialog\n");
		return UDM_EMPTY;
	}

	// �����Ի���ĵ�����
	m_MutexMgr.Enter();
	CItem* pItem = m_pTop[m_screen];
	int id = ThreadGetID();
	while(pItem)
	{
		if(pItem->m_pThread && pItem->m_pThread->GetThreadID() == id)
		{
			m_pPopupCaller = reinterpret_cast<CPage*>(pItem);
			break;
		}
		pItem = pItem->m_pDown;
	}
	m_MutexMgr.Leave();

	//save old cursor
	int old_cursor = m_pCursor->GetCursor();

	Open();
	m_pThread->ClearMessage();
	m_pThread->m_bLoop = TRUE;
	VD_MSG msg;
	while(m_pThread->m_bLoop)
	{
		//printf("m_pThread = %x \n",m_pThread);
		m_pThread->VD_RecvMessage(&msg);
		if(!PreMsgProc(msg.msg, msg.wpa, msg.lpa))
		{
			MsgProc(msg.msg, msg.wpa, msg.lpa);
			if(m_ExitState == UDM_OK || m_ExitState == UDM_CANCEL)
			{
				goto run_end;//already be closed
			}
		}
	}
	Close(UDM_CANCEL);
	
run_end:
	//restore old cursor
	m_pCursor->SetCursor(old_cursor);
	
	return m_ExitState;
}

VD_PCSTR CPage::GetTitle()
{
	return m_pTitle.c_str();
}

void  CPage::SetTitle(VD_PCSTR psz)
{
	if(psz)
	{
		m_pTitle = psz;
	}
	else
	{
		m_pTitle.erase();
	}
}

int CPage::GetThreadID()
{
	if(m_pThread)
	{
		return m_pThread->GetThreadID();
	}
	return -1;
}

// �õ�ҳ����˳�״̬
// UDM_EMPTY  - û���˳�
// UDM_OK     -	ȷ���˳�
// UDM_CANCEL - ȡ���˳�
UDM CPage::GetExitState()
{
	return m_ExitState;
}

void CPage::GetMargin(VD_PRECT rect)
{
	if (rect)
	{
		rect->left = m_Margin.left;
		rect->top = m_Margin.top;
		rect->right = m_Margin.right;
		rect->bottom = m_Margin.bottom;	
	}
}

void CPage::UpdateInputTray()
{
	CItem *pCurrent = CItem::GetActive(m_screen);
	CItem *pOldOwner;

	while(pCurrent && pCurrent->m_iType != IT_PAGE)
	{
		pCurrent = pCurrent->m_pParent;
	}

	if(pCurrent && pCurrent->m_iType == IT_PAGE)
	{
		
		CPage *pPage = ((CPage*)pCurrent);
		CItem *pChildFocus;
		VD_RECT rect;
		VD_RECT owner_rect;
		CItem *pInputTray = m_pInputTray[m_screen];

		pChildFocus = pPage->GetFocusItem();
		if(pInputTray && (pOldOwner = pInputTray->GetOwner()) != pChildFocus)//������ת��
		{
			pInputTray->SetOwner(pChildFocus);
			
			//��ǰҳ���л���ѡ�������ߴ�ѡ�����л�������ҳ��,�����账��,���򷴶���������
			if(pPage->m_pParent != pInputTray
				&& (!pOldOwner || !pOldOwner->m_pParent || pOldOwner->m_pParent->m_pParent != pInputTray))
			{
				//if(pChildFocus && (pChildFocus->m_iType == IT_EDIT
				//	|| (pChildFocus->m_iType == IT_NUMBERBOX && !CItem::GetKeyInputFlag(KIF_NUMERIC))))//û�����ּ�����Ҫ��������ת����ʾ
				if(pChildFocus && (pChildFocus->m_iType == IT_EDIT))
				{
					pInputTray->GetRect(&rect);
					if(pChildFocus->m_pOwner)
					{
						pChildFocus->m_pOwner->GetRect(&owner_rect);
						::OffsetRect(&rect, owner_rect.right - rect.left, owner_rect.bottom - rect.bottom);
					}
					else
					{
						::OffsetRect(&rect, pChildFocus->m_Rect.right - rect.left, pChildFocus->m_Rect.bottom - rect.bottom);
					}
					pInputTray->Show(FALSE);
					pInputTray->SetRect(&rect, FALSE);
					//printf("aaaaaaaaaa \n");
					pInputTray->Show(TRUE);
				}
				else
				{
					pInputTray->Show(FALSE);
				}
			}
		}
	}
}

/*
* �Զ��ؼ����ֹ���1
*���÷�Χͨ��ҳ����Զ��ؼ�����,������ָҳ���Ͼ�ֻ�пؼ��Ĳ���.
*���ҳ���ϻ���ҳ���Լ����ƵĲ���.�Զ����ֿ��ܳ����ص�e.g.¼������
*���ַ��: ��һ���ؼ���λ�ù���ʦ�����Լ��趨Ĭ��ΪCLIENT_X,�Ժ�ؿؼ����Ǹ��ݵ�һ���ؼ���λ�������β���
* ������һ����ʾ���ǰ�ť�Ļ�,���Ż��ڵ׶˾�����ʾ
*/
void CPage::AutoLayout()
{
	AUTO_LAYOUT m_LayOut;  
	int left,top,right,bottom; //�ؼ�����
	int pre_bottom;//����ǰһ�е�bottom�����������²���
	VD_BOOL mark_row = FALSE; //��Ǹ����Ƿ��Ѿ����²��ֹ�
	VD_BOOL mark_second = FALSE; //����Ƿ��Ѿ����β��ֹ�
	int tempTable[4] = {0,0,0,0}; //���ǰһ�ν��沼��ʱ��Table1��Table4������

	LayoutInit(&m_LayOut);

	pre_bottom = m_LayOut.maxBottom; 

	int inRowTab[MAX_TABLE_NUM] = {0,0,0,0,0,0,0}; //���һ����ͬһTable�Ѿ����ֹ�����0Ϊ�Ѿ����ֹ�
	//����м��пؼ�,ÿ���м����ؼ�
	while(m_LayOut.pCur)
	{
		/*������һ���ؼ����л������ԣ���ȥ�������ԣ��Ա�����洦�����*/
		if(m_LayOut.pCur->m_pBrother == NULL)
		{
			if((m_LayOut.pCur->m_dwStyle & changeLine) == changeLine)
			{
				m_LayOut.pCur->m_dwStyle = m_LayOut.pCur->m_dwStyle & ~changeLine;
			}
			else if(m_LayOut.pCur->m_pOwner != NULL && (m_LayOut.pCur->m_pOwner->m_dwStyle & changeLine) == changeLine) //���һ���Ǹ��Ͽؼ����ҹ�����˻���
			{
				m_LayOut.pCur->m_pOwner->m_dwStyle = m_LayOut.pCur->m_pOwner->m_dwStyle & ~changeLine;
			}
		}

		/*���һ�������μ���������ʹ��ͬһTableʱ����ѭ��������
		ֻ�����һ��ʹ�ø�Table���ԵĿؼ��������ͬһ�������ؼ��ĸ�Table����*/
		if((m_LayOut.pCur->m_dwStyle & alignTableMask) != 0)
		{
			int table = (m_LayOut.pCur->m_dwStyle & alignTableMask) >> 24;
			if(!inRowTab[table - 1])
			{
				inRowTab[table - 1] += 1;
			}
			else
			{
				printf("@@@@@@@@@@@@@@@@@>>>>>>>>>>>>>>WARNING!!!!!!> Same Table in same row .......\n");
				m_LayOut.pCur->m_dwStyle &= ~(m_LayOut.pCur->m_dwStyle & alignTableMask);
			}
		}

		m_LayOut.row[m_LayOut.rowNum-1]++;

		if(	(((m_LayOut.pCur->m_dwStyle & changeLine) == changeLine) && 
			(m_LayOut.pCur->m_pBrother->m_pOwner == NULL))	
			|| ((m_LayOut.pCur->m_pOwner != NULL) && 	/*�����Ͽؼ�������������*/
			(m_LayOut.pCur->m_pBrother != NULL) && 
			(m_LayOut.pCur->m_pBrother->m_pOwner == NULL) && 
			((m_LayOut.pCur->m_pOwner->m_dwStyle & changeLine) == changeLine)))
		{
			m_LayOut.rowNum++;
			for(int i = 0; i < MAX_TABLE_NUM; i++)
			{
				inRowTab[i] = 0;
			}
			if(m_LayOut.rowNum > ROW_MAX_NUM)
				break;
		}
		m_LayOut.pCur = m_LayOut.pCur->m_pBrother;
	}
	//endof
	
	//��ʼ�������пؼ�
	m_LayOut.pCur = m_LayOut.head;
	while(m_LayOut.pCur)		
	{
		if(m_LayOut.pPre == m_LayOut.pCur)
		{
			m_LayOut.pLineHead = m_LayOut.pCur;  //��¼����ָ��
		}

		//if(m_LayOut.curRow == 3 && m_LayOut.curColumn == 4)
		//{
		//	tracepoint();
		//}
		/*����ؼ�������*/
		left = SetCtrlLeft(&m_LayOut);
		top = SetCtrlTop(&m_LayOut);
		right = SetCtrlRight(&m_LayOut, left);
		bottom = SetCtrlBottom(&m_LayOut, top);

		//trace("@@@@@@@@@@@ left = %d ,top = %d ,right = %d , bottom = %d  \n ", left, top, right, bottom);
		//���õ�ǰ�ؼ�������
		m_LayOut.pCur->m_Rect.SetRect(left,top,right,bottom);
		m_LayOut.pCur->m_Margin.SetRectEmpty();
		this->ClientToScreen(&(m_LayOut.pCur->m_Rect));//���������תΪ��������

		/*--------����--------*/
		if( ( (m_LayOut.pCur->m_dwStyle & changeLine) == changeLine && 
			(m_LayOut.pCur->m_pBrother->m_pOwner == NULL) )
			|| (m_LayOut.pCur->m_pOwner != NULL &&      //������Ͽؼ�������������
			(m_LayOut.pCur->m_pOwner->m_dwStyle & changeLine) == changeLine &&
			m_LayOut.pCur->m_pBrother->m_pOwner == NULL))
		{
			if((m_LayOut.pCur->m_Rect.right + m_LayOut.startX) > m_Rect.right)
			{
				if(!mark_row)//���û���Ź���
				{
					m_LayOut.pCur = m_LayOut.pLineHead;
					m_LayOut.pPre = m_LayOut.pCur;
					m_LayOut.width = m_LayOut.width/4; //�����²��� �м�����
					m_LayOut.curColumn = 1;
					mark_row = TRUE;
					m_LayOut.maxBottom = pre_bottom;
					m_LayOut.reRowLayout = TRUE;
					continue; //�����²���
				}
			}
			pre_bottom = m_LayOut.maxBottom;
			m_LayOut.width = m_iSpace;;
			mark_row = FALSE;
			m_LayOut.pCur = m_LayOut.pCur->m_pBrother;
			m_LayOut.pPre = m_LayOut.pCur;
			m_LayOut.curRow++;
			m_LayOut.curColumn = 1;
			m_LayOut.reRowLayout = FALSE;
		}
		else 
		{
			m_LayOut.pPre = m_LayOut.pCur;
			m_LayOut.pCur = m_LayOut.pCur->m_pBrother;
			m_LayOut.curColumn++;
		}

		/*******���´������ڶ��β���**********/	
		if(m_LayOut.curRow >= m_LayOut.rowNum)
		{
			//��Table����columnLeft[i]��ǰһ�ε�Table����tempTable[i]���Ƚ�
			//�������ȫ��ȣ������empTable[i]���������²��ֱ�־secondLayoutΪTRUE
			for(int i = 0; i < 4; i++)
			{
				if(tempTable[i] != m_LayOut.columnLeft[i])
				{
					tempTable[i] = m_LayOut.columnLeft[i];
					mark_second = FALSE;
					m_LayOut.secondLayout = TRUE;
				}
			}
			if(!mark_second)
			{
				if(m_LayOut.secondLayout)
				{
					m_LayOut.pCur = m_LayOut.head;
					m_LayOut.pPre = m_LayOut.pCur;
					m_LayOut.pLineHead = m_LayOut.pCur;
					m_LayOut.reRowLayout = FALSE;
					m_LayOut.curRow = 1;
					m_LayOut.curColumn = 1;
					mark_second = TRUE;
					mark_row = FALSE;
					continue; //���β���
				}		
			}
			m_LayOut.secondLayout = FALSE;
		}
	}
	return ;
}

/*���ÿؼ��� Left ֵ*/
int CPage::SetCtrlLeft(AUTO_LAYOUT *pAL)
{
	int left = 0;
	int column = 0;
	CRect rect;

	if((pAL->pPre == pAL->pCur) && 
		(pAL->pCur->m_dwStyle & alignCenter) == alignCenter) //����
	{
		
		CItem *pp;
		int total_width = 0;
		int ctrl_width = 0;
		int i = 0;
		CRect w_rect(m_Rect);
		this->ScreenToClient(w_rect);
		for(i = pAL->curColumn, pp = pAL->pCur; i <= pAL->row[pAL->curRow - 1]; i++, pp = pp->m_pBrother)
		{
			if(pp->m_Rect.Width() != 0)
			{
				ctrl_width = pp->m_Rect.Width();
			}
			else if(pp->GetDefaultSize().w != 0)
			{
				if(pp->m_iType == IT_STATIC
					|| pp->m_iType == IT_BUTTON
					|| pp->m_iType == IT_COMBOBOX)
				{
					int offset = 0;
					offset = pp->m_iType == IT_STATIC ? 0 : 8;
					ctrl_width = pp->GetDefaultSize().w >= (g_Locales.GetTextExtent(pp->GetText()) + offset) ?	
										pp->GetDefaultSize().w : (g_Locales.GetTextExtent(pp->GetText()) + offset); 		
				}
				else
				{
					ctrl_width = pp->GetDefaultSize().w;
				}
			}
			else
			{
				printf(">>>>>>>>>>>> Ctrl %d don't set width!!!!!! \n",pp->m_iType);
				ctrl_width = TEXT_WIDTH * 6; 
			}
			if(NULL == pp->m_pOwner)
			{
				total_width += ctrl_width + pAL->width;
			}
		}
		
		left = m_Rect.Width()/2 - (total_width - pAL->width)/2;
	}
	else if((pAL->pCur->m_dwStyle & alignTableMask) != 0) //Table
	{
		column = (pAL->pCur->m_dwStyle & alignTableMask) >> 24;

		//����Table�����꣬һ����ÿ��Table�����궼������ǰһ���ؼ���Rightȷ��
		//���columnLeft[column - 1]�д�Ÿ�����ӦTable�����ֵ
		if(column > 4) //�Զ�����ǰ4��Table���꣬���������Զ�����
		{
			left = pAL->columnLeft[column - 1];			
		}
		else if(column > 0 && column <= 4)
		{
			if(1 == column) //Table1������Ϊ������ʼX����
			{
				left = pAL->startX;
			}
			else //
			{
				CRect preRect(pAL->pPre->m_Rect);
				this->ScreenToClient(preRect);
				
				//�еĵ�һ���ؼ����������Table1,��ֱ�Ӷ�����columnLeft[column - 1]��
				//����������ǰһ���ؼ�ȷ������Ϊ����ǰһ���ؼ�����һ�е�β�ؼ�
				if((pAL->pCur != pAL->pPre) 
					&& ((preRect.right + pAL->width) > pAL->columnLeft[column - 1]))
				{					
					pAL->columnLeft[column - 1] = preRect.right + pAL->width;					
				}
				left = pAL->columnLeft[column - 1];
			}
		}
	}
	else if((pAL->pCur->m_dwStyle & inflateAll) != 0) //���뷽ʽ
	{
		switch(pAL->pCur->m_dwStyle & inflateAll)
		{
		case (alignRight | alignBottom):
		case alignRight:
			{
				CItem *pp;
				int column_num = 0;
				int total_width = 0;
				int ctrl_width = 0;
				int i = 0;
				VD_BOOL mark = FALSE;
				CRect w_rect(m_Rect);
				this->ScreenToClient(w_rect);
				for(i = pAL->curColumn, pp = pAL->pCur; i <= pAL->row[pAL->curRow - 1]; i++, pp = pp->m_pBrother)
				{
					if(pp->m_Rect.Width() != 0)
					{
						ctrl_width = pp->m_Rect.Width();
					}
					else if(pp->GetDefaultSize().w != 0)
					{
						if(pp->m_iType == IT_STATIC
							|| pp->m_iType == IT_BUTTON
							|| pp->m_iType == IT_COMBOBOX)
						{
							int offset = 0;
							offset = pp->m_iType == IT_STATIC ? 0 : 8;
							ctrl_width = pp->GetDefaultSize().w >= (g_Locales.GetTextExtent(pp->GetText()) + offset) ?  
												pp->GetDefaultSize().w : (g_Locales.GetTextExtent(pp->GetText()) + offset);			
						}
						else
						{
							ctrl_width = pp->GetDefaultSize().w;
						}
					}
					else
					{
						printf(">>>>>>>>>>>> Ctrl %d don't set width!!!!!! \n",pp->m_iType);
						ctrl_width = TEXT_WIDTH * 6; 
					}
					if((pp->m_dwStyle & alignTableMask) != 0)
					{
						column = (pp->m_dwStyle & alignTableMask) >> 24;
						left = pAL->columnLeft[column - 1] - total_width - pAL->width * column_num ;
						mark = TRUE;
						break; //�˳�ѭ�������ٱ����ؼ�
					}
					total_width += ctrl_width;
					column_num++;
				}

				if(!mark)
				{
					left = w_rect.right - total_width - pAL->width * (column_num - 1) - pAL->startX;
				}
			}
			break;
		default:
			if(pAL->pPre == pAL->pCur)
			{
				left = pAL->startX;
			}
			break;
		}
	}
	else  //��Table,���޶��뷽ʽ
	{
		if(pAL->pPre == pAL->pCur)  //ÿһ�ŵĵ�һ���ؼ�
		{			
			left = pAL->startX;
		}
		else// ����ÿһ�еĵ�һ��
		{
			//this->ClientToScreen(&(p->m_Rect));//
			//����ÿһ�еĵ�һ��,��ô����left ��top ��Ҫ����ǰһ����������ȷ��
			rect = pAL->pPre->m_Rect;
			this->ScreenToClient(&rect);//��������תΪ�������
			switch(pAL->pPre->m_iType)
			{
				//�������һ���ؼ����������ؼ���ɵ�,����ʱ�����ڿ�.
				//��ô������Ҫ������ؼ����в���.
			case IT_DATETIMECTRL:
			case IT_TIMINGCTRL:
				left = rect.left + TEXTBOX_X/*���ÿؼ��ļ��*/;
				break;
			case IT_IPADDRESSCTRL:
				left = rect.left + 2/*���ÿؼ��ļ��*/;
				break;
			default :

				if((pAL->pCur->m_pOwner)!= NULL)
				{
					switch((pAL->pCur->m_pOwner)->m_iType)
					{
					case IT_DATETIMECTRL:
						left = rect.left + rect.Width() + (pAL->pCur->m_pOwner->GetDefaultSize().w)/DTC_ITEMS;
						break;
					case IT_IPADDRESSCTRL:
						left = rect.left +TEXT_WIDTH*5/2;
						break;
					case IT_TIMINGCTRL:
						left = rect.left + rect.Width()+TEXT_WIDTH/2;
						break;
					default:
						left = rect.left + rect.Width() + TEXT_WIDTH;
						break;
					}
				}
				else
				{
					//��������������checkbox�ؼ��ļ��
					if((pAL->pPre->m_iType == IT_CHECKBOX) && (pAL->pCur->m_iType == IT_CHECKBOX))
					{
						if(strcmp(pAL->pCur->GetText(),""))
						{
							left = rect.left + rect.Width()+3;
						}
						else
						{
							if((pAL->pCur->m_dwStyle & smallWidth) == smallWidth) //��smallWidth��CheckBox�����������������
							{
								left = rect.left + rect.Width() + m_playoutSpace[spaceCheckBoxSmall]; 
							}
							else
							{
								left = rect.left + rect.Width() + TEXT_WIDTH; //�����������CheckBox
							}
						}
					}
					else if(pAL->pPre->m_pOwner != NULL)
					{
						CRect owner_rect;
						owner_rect  = pAL->pPre->m_pOwner->m_Rect;
						this->ScreenToClient(&owner_rect);
						if((pAL->pCur->m_dwStyle & smallWidth) == smallWidth)
						{
							left =  owner_rect.right + 2;
						}
						else if(pAL->pCur->m_iType == IT_CHECKBOX && !strcmp(pAL->pCur->GetText(),"")) //��¼��ʱ����� CheckBox ���⴦��
						{
							left =  owner_rect.right + pAL->width * 3/2;
						}
						else
						{
							left =  owner_rect.right + pAL->width;
						}				
					}
					else
					{
						if((pAL->pCur->m_dwStyle & smallWidth) == smallWidth)
						{
							left = rect.left + rect.Width() + 2;  /*�����ؼ����м��*/
						}
						else
						{
							left = rect.left + rect.Width() + pAL->width;  /*�����ؼ����м��*/
						}
					}
					break;
				}
			}
		}
	}

	return left;
}

/*���ÿؼ��� Top ֵ*/
int CPage::SetCtrlTop(AUTO_LAYOUT *pAL)
{
	int top = 0;
	CRect temp_rect = m_Rect;
	this->ScreenToClient(&temp_rect);

	if(pAL->pPre == pAL->pCur) //ÿ�еĵ�һ���ؼ�
	{
		if(((pAL->pCur->m_dwStyle & inflateY) != inflateY)
			&& ((pAL->pCur->m_dwStyle & alignBottom) == alignBottom))
		{
			int temp = pAL->rowNum - pAL->curRow;
			//modefid by xuaiwei20070918 �����������Margin�ĵײ�ֵʱ���Զ�����ҳ���������⣻
			top = temp_rect.bottom - 2*CLIENT_Y  - (temp + 1) * CTRL_HEIGHT - 4*temp-m_Margin.bottom;	
		}
		else if(1 == pAL->curRow) //��һ��
		{
			top = pAL->startY;
		}
		else 
		{
			top = pAL->maxBottom + 4;
		}
	}
	else  //����ÿ�еĵ�һ���ؼ�
	{
		CRect rect;
		rect = pAL->pPre->m_Rect;
		this->ScreenToClient(&rect);
		switch(pAL->pPre->m_iType)
		{
			//�������һ���ؼ����������ؼ���ɵ�,����ʱ�����ڿ�.
			//��ô������Ҫ������ؼ����в���.
			//begin
		case IT_DATETIMECTRL:
		case IT_TIMINGCTRL:
			top = rect.top + 2;
			break;
		case IT_IPADDRESSCTRL:
			top = rect.top + 2;
			break;
		default :
			if(((pAL->pCur->m_dwStyle & inflateY) != inflateY)
				&& ((pAL->pCur->m_dwStyle & alignBottom) == alignBottom))
			{
				int temp = pAL->rowNum - pAL->curRow;
				top = m_Rect.Height() - 2*CLIENT_Y - m_TitleHeight - (temp + 1) * CTRL_HEIGHT - 4*temp;	
			}
			else
			{
				top = rect.top;
			}
			break;
		}
	}
	return top;
}

/*���ÿؼ��� Right ֵ�� ���� right ǰ�����ȼ��� left ���Ұ� left ��Ϊ��������ȥ*/
int CPage::SetCtrlRight(AUTO_LAYOUT *pAL, int left)
{
	int right = 0;
	if((pAL->pCur->m_dwStyle & inflateX) == inflateX)
	{
		CItem *pp;
		int column_num = 0;
		int total_width = 0;
		int ctrl_width = 0;
		int i = 0;
		VD_BOOL mark = FALSE;
		CRect temp_rect = m_Rect;
		this->ScreenToClient(&temp_rect);

		for(i = pAL->curColumn + 1, pp = pAL->pCur->m_pBrother; i <= pAL->row[pAL->curRow - 1]; i++, pp = pp->m_pBrother)
		{
			if(pp->m_Rect.Width() != 0)
			{
				ctrl_width = pp->m_Rect.Width();
			}
			else if(pp->GetDefaultSize().w != 0)
			{
				ctrl_width = pp->GetDefaultSize().w;
			}
			else
			{
				printf(">>>>>>>>>>>> Ctrl %d don't set width!!!!!! \n",pp->m_iType);
				ctrl_width = TEXT_WIDTH * 6; 
			}

			if((pAL->pCur->m_dwStyle & alignTableMask) != 0)
			{
				int column = (pAL->pCur->m_dwStyle & alignTableMask) >> 24;
				right = pAL->columnLeft[column - 1] - total_width - pAL->width * column_num;
				mark = TRUE;
				break; //�˳�ѭ�������ٱ����ؼ�
			}
			total_width += ctrl_width;
			column_num++;
		}

		if(!mark)
		{
			right = temp_rect.right - total_width - pAL->width * column_num - pAL->startX;
		}
	}
	else
	{
		if(pAL->pCur->m_Rect.Width() != 0)
		{
			if(pAL->reRowLayout && 
				pAL->pCur->m_iType == IT_NUMBERBOX &&
				pAL->pCur->m_pOwner == NULL)
			{
				right = left + pAL->pCur->m_Rect.Width()/2;
			}
			else
			{
				right = left + pAL->pCur->m_Rect.Width();
			}	
		}
		else if(pAL->pCur->GetDefaultSize().w != 0)
		{

			if(pAL->pCur->m_iType == IT_STATIC
				|| pAL->pCur->m_iType == IT_BUTTON
				|| pAL->pCur->m_iType == IT_COMBOBOX)
			{
				int offset = pAL->pCur->m_iType == IT_STATIC ? 0 : 16;
				int temp = pAL->pCur->GetDefaultSize().w >= (g_Locales.GetTextExtent(pAL->pCur->GetText()) + offset) ?  
							pAL->pCur->GetDefaultSize().w : (g_Locales.GetTextExtent(pAL->pCur->GetText()) + offset);
				right = left + temp;
			}
			else if(pAL->pCur->m_iType == IT_CHECKBOX && strcmp(pAL->pCur->GetText(),""))
			{
				right = left + pAL->pCur->GetDefaultSize().w + 4;
			}
			else
			{
				right = left + pAL->pCur->GetDefaultSize().w;
			}
		}
		else
		{
			if(pAL->pCur->m_iType == IT_GROUPBOX)
			{
				right = m_Rect.Width() - CLIENT_X * 2;
			}
			else // �������O�Ļ�,��ô�ǲ��ǳ���ʲô����.����.
			{
				printf(">>>>>>>>>>>> Ctrl %d don't set width!!!!!! \n",pAL->pCur->m_iType);
				right = left + TEXT_WIDTH * 6;
			}
		}
	}
	return right;
}

/*���ÿؼ��� bottom ֵ�� ���� bottom ǰ�����ȼ��� top ���Ұ� top ��Ϊ��������ȥ*/
int CPage::SetCtrlBottom(AUTO_LAYOUT *pAL, int top)
{
	int bottom = 0;
	CRect temp_rect = m_Rect;
	this->ScreenToClient(&temp_rect);
	if((pAL->pCur->m_dwStyle & inflateY) == inflateY)
	{
		int temp = pAL->rowNum - pAL->curRow;
		//modefid by xuaiwei20070918 ����ڽ�������ʱ���Զ����ֿؼ�����ҳ������⣻
		bottom = temp_rect.bottom - (CTRL_HEIGHT + 4) * temp - 2 * (pAL->startY)-m_Margin.bottom; 
	}
	else
	{
		if(pAL->pCur->m_Rect.Height() != 0)
		{
			bottom = top + pAL->pCur->m_Rect.Height();
		}
		else if(pAL->pCur->GetDefaultSize().h != 0)
		{
			if(pAL->pCur->m_iType == IT_CHECKBOX && strcmp(pAL->pCur->GetText(), ""))
			{
				bottom = top + pAL->pCur->GetDefaultSize().h + 4;
			}
			else if(pAL->pCur->m_iType == IT_GROUPBOX && !strcmp(pAL->pCur->GetText(), ""))
			{
				bottom = top + pAL->pCur->GetDefaultSize().h / 2;
			}
			else
			{
				bottom = top + pAL->pCur->GetDefaultSize().h;
			}			
		}
		else
		{
			printf(">>>>>>>>>>>> Ctrl %d no height!!!!!!!!!!!! \n", pAL->pCur->m_iType);
			bottom = top + CTRL_HEIGHT;
		}
	}

	/*���� maxBottom */
	if(pAL->pCur == pAL->pPre)
	{
		pAL->maxBottom = bottom;
	}
	else
	{
		pAL->maxBottom = pAL->maxBottom >= bottom ? pAL->maxBottom : bottom;
	}
	return bottom;
}
void CPage::LayoutInit(AUTO_LAYOUT *pAL)
{
	pAL->pCur = m_pChildHead;
	pAL->head = pAL->pCur;
	pAL->pPre = pAL->pCur;
	pAL->pLineHead = NULL;

	pAL->rowNum = 1;
	pAL->curRow = 1;
	pAL->curColumn = 1;
	pAL->column = 0;
	pAL->width = m_iSpace;

	pAL->reRowLayout = FALSE;
	pAL->secondLayout = FALSE; 
	
	memset(pAL->row,0,sizeof(pAL->row));

	/*��ʼ����ʼ����*/
	pAL->startX = m_iStartX;
	pAL->startY = m_iStartY;

	pAL->maxBottom = pAL->startY;

	//��ʼ���ж���Left
	for(int i = 0; i < MAX_TABLE_NUM; i++)
	{
		pAL->columnLeft[i] = m_iTable[i];
	}

	//���ҳ��ؼ���Ҫ�Զ����ֵ��׿ؼ�ָ��
	//�����ҳ�����йرտؼ�,Ӧ�ò������Զ�������.
	//������Ϊ������һ���ؼ�Ϊ�׿ؼ�
	while((pAL->pCur && !pAL->pCur->GetFlag(IF_ORDERED)) || (pAL->pCur && pAL->pCur->GetFlag(IF_POPUP)))
	{
		pAL->pCur = pAL->pCur->m_pBrother;
	}
	pAL->head = pAL->pCur;
	pAL->pPre = pAL->pCur;
	return;
}

/*���ÿؼ����*/
void CPage::SetCtrlSpace(int width)
{
	if(width > 0)
	{
		m_iSpace = width;
	} 
	return ;
}
void CPage::SetStartPoint(int x/* = CLIENT_X*/, int y/* = CLIENT_Y*/)
{
	m_iStartX = x;
	m_iStartY = y;
	return;
}

void CPage::SetCtrlSpace(LayoutSpace type, int space)
{
	if (m_playoutSpace == sm_layoutSpace)
	{
		m_playoutSpace = new int[spaceCount];
	}
	m_playoutSpace[type] = space;
}

void CPage::SetPageIcon(PAGEICON pageIcon, VD_BITMAP* bmp)
{
	if (bmp)
	{
		switch(pageIcon)
		{
		case pageIconSmall:
			m_pIconBmp = bmp;
			break;
		case pageIconNor:
			m_pNorBmp = bmp;
			break;
		case pageIconSel:
			m_pSelBmp = bmp;
			break;
		default:
			break;
		}
	}
}
VD_BITMAP* CPage::GetPageIcon(PAGEICON pageIcon)
{
	VD_BITMAP* bmp = NULL;
	switch(pageIcon)
	{
	case pageIconSmall:
		  bmp = m_pIconBmp;
		break;
	case pageIconNor:
		bmp = m_pNorBmp;
		break;
	case pageIconSel:
		bmp = m_pSelBmp;
		break;
	default:
		break;
	}
	return bmp;
}

VD_PCSTR  CPage::GetTip(void)
{
	return tip.c_str();
}

void CPage::SetParentPage(CPage* pParent)
{
	if(m_pParent)
	{
		m_pParent->DeleteItem(this);
	}
	m_pParent = pParent;
	if(m_pParent)
	{
		m_pParent->AddItem(this);
	}
} 
void CPage::SetTable(int TableNum, int left)
{
	if(TableNum >= 1 && TableNum <= MAX_TABLE_NUM)
	{
		m_iTable[TableNum-1] = m_iStartX + left;
	}
	return;
}

void CPage::SetDefaultBitmap(PageBitmap region, VD_PCSTR name)
{
	switch(region) {
	case PGB_KEY_PAGEUP:
		bmp_key_pageup = VD_LoadBitmap(name);
		break;
	case PGB_KEY_PAGEDOWN:
		bmp_key_pagedown = VD_LoadBitmap(name);
		break;
	case PGB_KEY_FUNCTION:
		bmp_key_function = VD_LoadBitmap(name);
		break;
	case PGB_KEY_ENTER:
		bmp_key_enter = VD_LoadBitmap(name);
		break;
	case PGB_CLOSE_BUTTON_NORMAL:
		bmp_button_close[0] = VD_LoadBitmap(name);
		break;
	case PGB_CLOSE_BUTTON_SELECTED:
		bmp_button_close[1] = VD_LoadBitmap(name);
		break;
	case PGB_CLOSE_BUTTON_PUSHED:
		bmp_button_close[2] = VD_LoadBitmap(name);
		break;
	case PGB_CLOSE_BUTTON_DISABLED:
		bmp_button_close[3] = VD_LoadBitmap(name);
		break;
	case PGB_ICON_SMALL:
		icon_dvr = VD_LoadBitmap(name);
		break;
	default:
		break;
	}
}
