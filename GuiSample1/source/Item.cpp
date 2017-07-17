//
//  "$Id: Item.cpp 279 2008-12-17 05:58:45Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "GUI/Ctrls/Page.h"
//#include "GUI/Pages/PagePlay.h"

/*///////////////////////////////////////////////////////////////////////////////////////////////
GUI�ں�����Ҫ��2������������֤GUI�����״̬һ���ԣ�ͬʱ��֤���߳�ͬʱ������Ч�ʡ�������ҳ���ڲ���
��һ������m_DC.m_Mutex ��������m_DC������GDI���󣬵ȵȣ�ͨ������m_DC.Lock()��m_DC.UnLock()�����ʡ�
�ڶ�������m_MutexMgr ���������ҳ��������������ҳ�棬���лҳ��֮������ӹ�ϵ���ȵȡ����͵�
	Ӧ�ó����Ǵ򿪻�ر�ҳ��ʱ����Ҫ�õ����������֤ҳ��Ĺ�ϵ�������
Ϊ�˱������������뱣֤m_MutexMgr�ı�����Χ����Ӧ����m_Mutex,���ܳ�������������
ͬʱҳ��m_DC.m_Mutex�����ķ�ΧӦ�������ӿؼ�m_DC.m_Mutex�����ķ�Χ��

ҳ���ڲ��Ŀؼ������Լ�������Ա������û�м����ģ���Ϊ�󲿷�����·��ʳ�Ա������ֻ��һ���̣߳���
����ҳ����̻߳���ҳ���������Ϣ�����̡߳�������ر���Ҫ����Ӧ�ò��ṩ������������Ӧ�ò�����ķ�Χ
Ӧ��ȷ�������ܺ�m_MutexMgr����m_DC.m_Mutex��˫��İ�����ϵ��

����Draw()��������Ϊ(Ҫ��)�䲻�ı�ؼ������Ժ�m_DC����������ʹ��m_DC.m_Mutex�������ɡ�
///////////////////////////////////////////////////////////////////////////////////////////////*/

///////////////////////////////////////////////////////////////////////////////
/////////             CItemThread
void CItemThread::ThreadProc()
{
	VD_MSG msg;
	
	//printf("CItemThread::ThreadProc:start......\n");
	//fflush(stdout);
	
	do
	{
		//printf("CItemThread::ThreadProc:entry loop......\n");
		//fflush(stdout);
		
		VD_RecvMessage(&msg);
		
		//printf("CItemThread::ThreadProc:VD_RecvMessage finish......\n");
		//fflush(stdout);

		//printf("yg VD_RecvMessage msg: 0x%x, %d,%d \n", msg.msg, VD_HIWORD(msg.lpa),VD_LOWORD(msg.lpa));
		
		if(msg.msg == XM_QUIT)
		{
			//printf("CItemThread::ThreadProc:XM_QUIT\n");
			//fflush(stdout);
			
			break;//�̳߳���1
		}
		
		//printf("CItemThread::ThreadProc:recv msg:%d\n",msg.msg);
		//fflush(stdout);
		
		//������300����֮ǰ���͵���Ϣ// Ϊʲô����300����ǰ����Ϣ?  nike.xie
		//modefied by xie  ����©�����Ϣ��
		if ( (SystemGetMSCount() - msg.time < 300) || (XM_LBUTTONDOWN == msg.msg)  || (XM_LBUTTONUP == msg.msg))	
		{
			if(!m_pItem->PreMsgProc(msg.msg, msg.wpa, msg.lpa))
			{
				//printf("yg PreMsgProc\n");
				m_pItem->MsgProc(msg.msg, msg.wpa, msg.lpa);
				
				//csp modify
				//printf("CItemThread::ThreadProc:GetModify=%d\n",CDC::GetModify());
				//fflush(stdout);
				if(CDC::GetModify())
				{
					//printf("CItemThread::ThreadProc:GraphicsSyncVgaToCvbs\n");
					//fflush(stdout);
					GraphicsSyncVgaToCvbs();
					CDC::SetModify(FALSE);
				}
			}
			else
			{
				//csp modify
				if(CDC::GetModify())
				{
					GraphicsSyncVgaToCvbs();
					CDC::SetModify(FALSE);
				}
			}
		}
		
		//printf("CItemThread::ThreadProc:deal msg finish\n");
		//fflush(stdout);
	} while (m_bLoop);//�̳߳���2
	
	//printf("CItemThread::ThreadProc:quit loop\n");
	//fflush(stdout);
	
	if(m_pItem->m_iType == IT_PAGE)
	{
		CPage *pParentPage = ((CPage*)m_pItem)->GetParentPage();
		if(pParentPage)
		{
			pParentPage->OnChildClosed((CPage*)m_pItem);
			
			//csp modify
			if(CDC::GetModify())
			{
				GraphicsSyncVgaToCvbs();
				CDC::SetModify(FALSE);
			}
		}
	}
	
	//printf("CItemThread::ThreadProc:thread over\n");
	//fflush(stdout);
}

//���ö���Ϣ���в��ԣ�����ƶ�ʱ��ռ�õ�CPU��һ�㣬��Ҳ�����������Ϣ©�������?
CItemThread::CItemThread(VD_PCSTR name, CItem * pItem) : CThread(name, TP_GUI, 16)
{
	m_pItem = pItem;
}

CItemThread::~CItemThread()
{
	
}

///////////////////////////////////////////////////////////////////
////////////        CItem
int CItem::m_nInstance[IT_NR] = {0, };
CItem *CItem::m_pActive[] = {NULL, };
CItem *CItem::m_pTop[] = {NULL, };
CItem *CItem::m_pNoTopMost[] = {NULL, };
CItem * CItem::m_pInputTray[] = {NULL, };
CItem * CItem::m_pSpellPad[] = {NULL, };
VD_BOOL CItem::m_bKeyInputFlags[KIF_NR] = {TRUE, TRUE, TRUE, TRUE};
CMutex CItem::m_MutexMgr(MUTEX_RECURSIVE);

/************************************************************************/
/*
1/�޸���CItem���ʵ�֣����ؼ������ԣ�����POPUP/EDIT������ͨ�����ñ�־λ���У���
�ؼ�������ʱ�򽫱�־���룬����styleΪ�ؼ��Լ��Ĳ���.
POPUP/styleEditable�������ɴ��������룬ͨ����m_dwStyle����λ�����á�

2/ Item�ؼ��������κ�Ĭ�����ԡ����е�������ֲ���ؼ��С�
����г�ͻ�ĵط���������ڴ�����ʱ���ֶ��������������
						--by wangqin  20070315		
*/
/************************************************************************/
//CItem::CItem(PCRECT pRect, CPage * pParent, ITEM_TYPE type, BOOL vedit /* = TRUE */,BOOL vpopup /* = FALSE */, BOOL vautofocus /* = TRUE */)
CItem::CItem(VD_PCRECT pRect, CPage * pParent, ITEM_TYPE type,uint style)
:m_screen(GetScreen(pParent)), m_dwStyle(style),m_dwFlags(0),m_DC(m_screen + DC_SCREEN)
{
	//csp modify 20130323
	//if(m_screen < 0 || m_screen > 2)
	if(m_screen != 0)
	{
		printf("#####################################################warning:m_screen error:0x%08x\n",m_screen);
	}
	
	m_pThread = NULL;
	m_pDown = m_pUp = NULL;
	m_pBrother = NULL;
	m_pBrotherR = NULL;
	m_pRelatedItems = NULL;
	m_iType = type;
	m_pParent = reinterpret_cast<CPage *>(pParent);
	m_pOwner = NULL;
	m_dwFlags = 0;
	
	m_pDevGraphics = CDevGraphics::instance(m_screen);
	m_pCursor = CCursor::instance(m_screen);
	
	if(pRect)
	{
		m_Rect = *pRect;
	}
	else//��ֹ���Զ�����ʱ��������Ҫ�����m_Rect
	{
		m_Rect.SetRectEmpty();
	}
	
	tip.erase();
	
	//states
	SetFlag(IF_OPENED, FALSE);
	SetFlag(IF_EDITABLED, m_dwStyle&styleEditable);
	SetFlag(IF_POPUP, m_dwStyle&stylePopup);
	SetFlag(IF_AUTOFOCUS, m_dwStyle&styleAutoFocus);
	
	SetFlag(IF_ENABLED, TRUE);
	SetFlag(IF_CAPTURED, FALSE);
	SetFlag(IF_FOCUSED, FALSE);
	
	if(m_dwStyle&stylePopup)
	{
		SetFlag(IF_SHOWN, FALSE);
	}
	else
	{
		SetFlag(IF_SHOWN, TRUE);
	}
	SetFlag(IF_SELECTED, FALSE);
	SetFlag(IF_ORDERED, TRUE);

	m_Margin.SetRectEmpty();

	//increase instances count
	m_nInstance[m_iType]++;

	//rect
	if(m_pParent)
	{
		m_pParent->ClientToScreen(&m_Rect);
		m_pParent->AddItem(this);//�ڸ����е�������������Ӵ���
	}
}

CItem::~CItem()
{
	if(m_pParent)
	{
		m_pParent->DeleteItem(this);
	}

	//decrease instances count
	m_nInstance[m_iType]--;
}

void CItem::ItemProc(CTRLPROC proc)
{
	if(proc)
	{
		m_pCursor->SetCursor(CR_WAIT);
		if(m_pOwner)
		{
			//printf("ItemProc:m_pOwner \n");
			(m_pOwner->*proc)();
		}
		else if (m_pParent)
		{
			//printf("ItemProc:m_pParent \n");
			(m_pParent->*proc)();
		}
		m_pCursor->SetCursor(CR_NORMAL);
	}
}

void CItem::ShowTip(VD_BOOL flag)
{
	if(m_pParent){
		if(flag && tip.size()){
			m_pParent->ShowTip(tip.c_str());
		}else{
			m_pParent->ShowTip(NULL);
		}
	}
}

VD_SIZE& CItem::GetDefaultSize(void)
{
	static VD_SIZE size = {0};
	return size;
}

//�����򣺿ؼ��Ѵ򿪣�������ʾ�����Ҹ�ҳ���Ѵ�������ʾ
VD_BOOL CItem::DrawCheck()
{
	return (GetFlag(IF_OPENED) && GetFlag(IF_SHOWN) && (dynamic_cast<CPage*>(this) || !m_pParent || m_pParent->DrawCheck()));
}

//�����Ƿ��ܱ��༭
void CItem::Editable(VD_BOOL flag)
{
	SetFlag(IF_EDITABLED, flag);
}

void CItem::Attach()
{
	if(m_dwStyle & (pageTopMost | pageActiveMost))
	{

		if(m_pTop[m_screen])
		{
			m_pTop[m_screen]->m_pUp = this;
		}
		m_pDown = m_pTop[m_screen];
		m_pUp = NULL;
		m_pTop[m_screen] = this;

	}
	else
	{

		if(m_pNoTopMost[m_screen])
		{
			m_pUp = m_pNoTopMost[m_screen]->m_pUp;
			if(m_pUp)
			{
				m_pUp->m_pDown = this;
			}
			m_pNoTopMost[m_screen]->m_pUp = this;
		}
		m_pDown = m_pNoTopMost[m_screen];
		if(m_pTop[m_screen] == m_pNoTopMost[m_screen])
		{
			m_pTop[m_screen] = this;
		}
		m_pNoTopMost[m_screen] = this;
	}
}

void CItem::Detach()
{
	if(m_pUp)
	{
		m_pUp->m_pDown = m_pDown;
	}
	if(m_pDown)
	{
		m_pDown->m_pUp = m_pUp;
	}
	if(m_pNoTopMost[m_screen] == this)
	{
		m_pNoTopMost[m_screen] = m_pDown;
	}
	if(m_pTop[m_screen] == this)
	{
		m_pTop[m_screen] = m_pDown;
	}
}

void CItem::Activate()
{
	if(dynamic_cast<CPage*>(m_pActive[m_screen]))
	{
		m_pActive[m_screen]->VD_SendMessage(XM_DEACTIVATE);
	}
	m_pActive[m_screen] = this;
	if(dynamic_cast<CPage*>(m_pActive[m_screen]))
	{
		m_pActive[m_screen]->VD_SendMessage(XM_ACTIVATE);
	}
}

//#define GUI_TEST//csp modify

void CItem::Show(VD_BOOL flag, VD_BOOL redraw /* = TRUE */)
{
	if(flag && GetFlag(IF_SHOWN) || !flag && !GetFlag(IF_SHOWN)) //״̬û�иı䣬�˳�
	{
		return;
	}
	
	CGuard guard(m_MutexMgr);
	
//	printf("CItem::Show 1 \n");
	SetFlag(IF_SHOWN, flag);
	
//	printf("CItem::Show 2 \n");
	if(GetFlag(IF_POPUP))
	{
//		printf("CItem::Show 3 flag:%d \n", flag);
		if(flag)
		{
			// attach to link
//			printf("CItem::Show 3.1 \n");
			Attach();

//			printf("CItem::Show 3.2 \n");
			// activate it
			if(!(m_dwStyle & pageActiveMost))
			{
				Activate();
			}

//			printf("CItem::Show 3.3 \n");
			//set rect when opening, needed?
			UpdateRgn();
//			printf("CItem::Show 3.4 \n");
			GlobalInvalidateRect(m_Rect);
			
//			printf("CItem::Show 3.5 \n");
			//draw items
			if(redraw)
			{
				Draw();
			}
//			printf("CItem::Show 3.6 \n");
		}
		else
		{
#ifdef GUI_TEST
			if(CDC::GetModify())//csp modify
			{
				printf("CItem::Show:framebuffer changed-1\n");
				CDC::SetModify(FALSE);
			}
#endif
			
//			printf("CItem::Show 3.7 \n");
			// detech from link
			Detach();
			
#ifdef GUI_TEST
			if(CDC::GetModify())//csp modify
			{
				printf("CItem::Show:framebuffer changed-2\n");
				CDC::SetModify(FALSE);
			}
#endif
			
//			printf("CItem::Show 3.8 \n");
			// if this item is active, activate the down item of it in stead.
			if(m_pActive[m_screen] == this)
			{
				if(m_pDown)
				{
					m_pActive[m_screen] = NULL;
					m_pDown->Activate();
				}
			}
			
#ifdef GUI_TEST
			if(CDC::GetModify())//csp modify
			{
				printf("CItem::Show:framebuffer changed-3\n");
				CDC::SetModify(FALSE);
			}
#endif
			
//			printf("CItem::Show 3.9 \n");
			GlobalValidateRect(m_Rect);
			
#ifdef GUI_TEST
			if(CDC::GetModify())//csp modify
			{
				printf("CItem::Show:framebuffer changed-4\n");
				CDC::SetModify(FALSE);
			}
#endif
			
//			printf("CItem::Show 3.10 \n");
			ClearRgn();
			
#ifdef GUI_TEST
			if(CDC::GetModify())//csp modify
			{
				printf("CItem::Show:framebuffer changed-5\n");
				CDC::SetModify(FALSE);
			}
#endif
			
//			printf("CItem::Show 3.11 \n");
			m_pUp = m_pDown = NULL;
		}
	}
	else
	{
//		printf("CItem::Show 4 \n");
		if(flag)
		{
			//���¼�������
			VD_RECT rect;
			m_pParent->m_DC.Lock();
			m_DC.Lock();

			m_DC.SetBound(m_Rect);
			m_pParent->GetRect(&rect);
			m_pParent->m_DC.SubtractRect(m_Rect);
			m_DC.IntersectRect(&rect);
			
			CItem * pItem = m_pParent->m_pUp;
			while(pItem)
			{
				pItem->GetRect(&rect);
				m_DC.SubtractRect(&rect);
				pItem = pItem->m_pUp;
			}
			
			m_DC.UnLock();
			m_pParent->m_DC.UnLock();
			
			if(redraw)
			{
				Draw();
			}
		}
		else
		{
			VD_RECT rect;
			CRgn rgn;
			m_pParent->m_DC.Lock();
			m_DC.Lock();

			m_DC.SwapRgn(&rgn);

			// �ػ��ֵܿؼ����ڵ����򣬶��ֵܿؼ�������Ҫ���¡���һ��bug�����������ϵĿؼ��ռ��ص�ʱ��
			// �ػ�˳��һ�����������ģ��Ժ��ֵܿؼ�����ҲҪ����Z������.
			CItem *p = m_pParent->m_pChildHead;
			while(p){
				if(!p->GetFlag(IF_POPUP) && p->GetFlag(IF_SHOWN) && !p->m_pOwner && p != this)
				{
					p->GetRect(&rect);
					if(rgn.RectInRegion(&rect))
					{
						if(redraw)
						{
							CRgn rgnItem;
							p->m_DC.Lock();
							p->m_DC.SwapRgn(&rgnItem);
							p->m_DC.SetBound(&p->m_Rect);
							p->m_DC.IntersectRgn(&rgn);
							p->Draw();
							p->m_DC.SwapRgn(&rgnItem);
							p->m_DC.UnLock();
						}
						rgn.SubtractRect(&rect);
					}
				}
				p = p->m_pBrother;
			}

			// �ػ游ҳ�汻�ڵ����򣬱�����rgn�С�
			if(redraw)
			{
				m_pParent->m_DC.SwapRgn(&rgn);
				m_pParent->m_bDrawBkgdOnly = TRUE;
				m_pParent->Draw();
				m_pParent->m_bDrawBkgdOnly = FALSE;
				m_pParent->m_DC.SwapRgn(&rgn);
			}

			// Ȼ����¸�ҳ������
			m_pParent->m_DC.UnionRgn(&rgn);

			m_DC.UnLock();
			m_pParent->m_DC.UnLock();
		}
	}
}

//����/��ֹ�ؼ�
void CItem::Enable(VD_BOOL flag)
{
	if(flag && GetFlag(IF_ENABLED) || !flag && !GetFlag(IF_ENABLED)) //״̬û�иı䣬�˳�
	{
		return;
	}

	SetFlag(IF_ENABLED, flag);
	Draw();
}

//ѡ��/��ѡ�пؼ�
void CItem::Select(VD_BOOL flag)
{
	
	if(m_pParent){
		if(flag && tip.size()){
			m_pParent->ShowTip(tip.c_str());
		}else{
			m_pParent->ShowTip(NULL);
		}
	}
	
	
	SetFlag(IF_SELECTED, flag);
	if(m_pOwner && m_pOwner->m_iType != IT_LISTBOX)
	{
		m_pOwner->Select(flag);
	}
	//printf("Select \n");

	
	Draw();
}

void CItem::SetFocus(VD_BOOL flag)
{
	SetFlag(IF_FOCUSED, flag);
	if(!flag)
	{
		SetFlag(IF_CAPTURED, FALSE);
	}
	if(!GetFlag(IF_AUTOFOCUS))
	{
		//printf("!GetFlag(IF_AUTOFOCUS)  \n");
		Draw();
	}
}

VD_BOOL CItem::GetFlag(ITEM_FLAG index)
{
	return (m_dwFlags&BITMSK(index)) ? TRUE : FALSE;
}

void CItem::SetFlag(ITEM_FLAG index, VD_BOOL flag)
{
	if(flag)
	{
		m_dwFlags |= BITMSK(index);
	}
	else
	{
		m_dwFlags &= ~BITMSK(index);
	}
}

uint CItem:: ReadFlag()//cw_mainpage
{
	return m_dwFlags;
}
//�����Զ��۽�
void CItem::SetAutoFocus(VD_BOOL flag)
{
	SetFlag(IF_AUTOFOCUS, TRUE);
}

//�õ���ʾ/����״̬
VD_BOOL CItem::IsShown()
{
	return GetFlag(IF_SHOWN);
}

//�õ�����/��ֹ״̬
VD_BOOL CItem::IsEnabled()
{
	return GetFlag(IF_ENABLED);
}

//�õ�ѡ��/��ѡ��״̬
VD_BOOL CItem::IsSelected()
{
	return GetFlag(IF_SELECTED);
}

//������겶��
void CItem::SetCapture(VD_BOOL flag)
{
	SetFlag(IF_CAPTURED, flag);
}

//�õ���겶��״̬
VD_BOOL CItem::GetCapture()
{
	return GetFlag(IF_CAPTURED);
}

//�õ����뽹��״̬
VD_BOOL CItem::GetFocus()
{
	return GetFlag(IF_FOCUSED);
}

//����ӵ����
VD_BOOL CItem::SetOwner(CItem * pOwner)
{
	m_pOwner = pOwner;
	return TRUE;
}

//�õ�ӵ����
CItem * CItem::GetOwner()
{
	return m_pOwner;
}

//�õ����ڿؼ�
CItem* CItem::GetParent()
{
	return m_pParent;
}

//�õ��ֵܿؼ�
CItem* CItem::GetBrother()
{
	return m_pBrother;
}

CItem* CItem::GetActive(int screen)
{
	return m_pActive[screen];
}

CItem* CItem::SetActive()
{
	return m_pActive[m_screen]; // ������ʾ����ҳ�湦��
	
#if 0//csp modify
	CGuard guard(m_MutexMgr);
	CPage* pHolder = dynamic_cast<CPage*>(this);
	
	if(this == m_pActive[m_screen] || pHolder == NULL)
	{
		return m_pActive[m_screen];
	}
	
	CItem* pNewActive = this;
	
	// �ؼ��Ǵ���Ի��򼤻������
	if((m_dwStyle & pageDialog) && pHolder->m_pPopupCaller)
	{
		pHolder = pHolder->m_pPopupCaller;
	}
	
	// Ŀ��ҳ��ĶԻ����Ѿ�������账��
	CItem *pItem = pHolder->m_pUp;
	while (pItem)
	{
		CPage *pPage = dynamic_cast<CPage*>(pItem);
		if(pPage && pPage->m_pPopupCaller == pHolder && pPage == m_pActive[m_screen])
		{
			return m_pActive[m_screen];
		}
		pItem = pItem->m_pUp;
	}
	
	// ����û������ҳ���ҳ�汻��Ϊ�����棬��������ʱֻ���ü���״̬�����ı�Z��
	if(m_pDown == NULL)
	{
		Activate();
		return m_pActive[m_screen];
	}
	
	// ����Z�򣬼�������µ�����
	CRgn rgn;
	pHolder->Detach();
	pHolder->Attach();
	rgn.SetRectRgn(pHolder->m_Rect);
	
	pItem = pHolder->m_pUp;
	while(pItem && pItem != pHolder)
	{
		CPage *pPage = dynamic_cast<CPage*>(pItem);
		if(pPage && pPage->m_pPopupCaller == pHolder)
		{
			pItem->Detach();
			pItem->Attach();
			rgn.SubtractRect(pItem->m_Rect);
			rgn.UnionRect(pItem->m_Rect);
			pNewActive = pItem;
		}
		else
		{
			rgn.IntersectRect(pItem->m_Rect);
		}
		pItem = pItem->m_pUp;
	}
	
	//ʵʩ����
	pHolder->UpdateRgn();
	pItem = pHolder->m_pUp;
	while(pItem && pItem != pHolder)
	{
		CPage *pPage = dynamic_cast<CPage*>(pItem);
		if(pPage && pPage->m_pPopupCaller == pHolder)
		{
			pItem->UpdateRgn();
		}
		pItem = pItem->m_pUp;
	}
	
	pHolder->GlobalInvalidateRgn(&rgn);
	
	pItem = m_pTop[m_screen];
	while(pItem)
	{
		CPage *pPage = dynamic_cast<CPage*>(pItem);
		VD_RECT rect;
		pItem->GetRect(&rect);
		if(pPage && (pPage == pHolder || pPage->m_pPopupCaller == pHolder))
		{
			if(rgn.RectInRegion(&rect))
			{
				CRgn validRgn;
				validRgn.CreateRectRgn(&rect);
				validRgn.IntersectRgn(&rgn);
				pItem->ValidateRgn(&validRgn);
			}
		}
		rgn.SubtractRect(&rect);
		pItem = pItem->m_pDown;
		if(pItem == pHolder)
		{
			break;
		}
	}
	pHolder->GlobalValidateRgn(&rgn);
	
	//�����µ�ҳ��
	pNewActive->Activate();
	
	return m_pActive[m_screen];
#endif
}

VD_BOOL CItem::IsActive()
{
	return (this == m_pActive[m_screen]);
}

void CItem::SetTip(VD_PCSTR psz)
{
	if(psz)
	{
		tip = GetParsedString(psz);
	}
	else
	{
		tip.erase();
	}
}

void  CItem::GetRect(VD_PRECT pRect) const
{
	*pRect = m_Rect;
}

void CItem::ScreenToClient(VD_PPOINT pPoint)
{
	pPoint->x -= m_Rect.left + m_Margin.left;
	pPoint->y -= m_Rect.top + m_Margin.top;
}

void CItem::ScreenToClient(VD_PRECT pRect)
{
	::OffsetRect(pRect, -(m_Rect.left + m_Margin.left), -(m_Rect.top + m_Margin.top));
}

void CItem::ClientToScreen(VD_PRECT pRect)
{
	::OffsetRect(pRect, (m_Rect.left + m_Margin.left), (m_Rect.top + m_Margin.top));
}

void  CItem::GetClientRect(VD_PRECT pRect)
{
	pRect->left = 0;
	pRect->top = 0;
	pRect->right = m_Rect.Width() - m_Margin.left - m_Margin.right;
	pRect->bottom = m_Rect.Height() - m_Margin.top - m_Margin.bottom;
}

void CItem::DrawBackground()
{

	if(!GetFlag(IF_OPENED) || !m_pParent || m_pParent->m_bDrawBkgdOnly == TRUE)
	{
		return;
	}

	
	CRgn rgn;

	//ע��ؼ���Lock��ҳ���LockǶ�׵�˳��, ֻ����ҳ��Ƕ�׿ؼ���
	m_pParent->m_DC.Lock();
	m_DC.Lock();
	m_DC.SwapRgn(&rgn);
	m_pParent->m_DC.SwapRgn(&rgn);
	m_pParent->m_bDrawBkgdOnly = TRUE;
	m_pParent->Draw();
	m_pParent->m_bDrawBkgdOnly = FALSE;
	m_pParent->m_DC.SwapRgn(&rgn);
	m_DC.SwapRgn(&rgn);
	m_DC.UnLock();
	m_pParent->m_DC.UnLock();
}

CTRL_CLASS ctrl_classes[IT_NR] = {
	{"BitmapButtons    ", sizeof(CBitmapButton)},
	{"Buttons          ", sizeof(CButton)},
	{"CheckBoxs        ", sizeof(CCheckBox)},
	{"ComboBoxs        ", sizeof(CComboBox)},
	{"DateTimeCtrls    ", sizeof(CDateTimeCtrl)},
	{"Edits            ", sizeof(CEdit)},
	{"GridCtrls        ", sizeof(CGridCtrl)},
	{"GroupBoxs        ", sizeof(CGroupBox)},
	{"IPAddressCtrls   ", sizeof(CIPAddressCtrl)},
	{"ListBoxs         ", sizeof(CListBox)},
	{"Menus            ", sizeof(CMenu)},
	{"NumberBoxs       ", sizeof(CNumberBox)},
	{"NumberPads       ", sizeof(CNumberPad)},
	{"Pages            ", sizeof(CPage)},
	{"ProgressCtrls    ", sizeof(CProgressCtrl)},
	{"ScrollBars       ", sizeof(CScrollBar)},
	{"SliderCtrls      ", sizeof(CSliderCtrl)},
	{"Statics          ", sizeof(CStatic)},
	{"TimingCtrls      ", sizeof(CTimingCtrl)},
	{"Radios           ", sizeof(CRadio)},
	{"Calendars        ", sizeof(CCalendar)},
//	{"TimeTable        ", sizeof(CTimeTable)},
	{"TableBoxs        ", sizeof(CTableBox)},
	{"RecTimeBox       ", sizeof(CRecFileTimeBox)},
};

void CItem::DumpInstances()
{
	/*
	trace("GUI Objects:\n");
	unsigned long size = 0;
	int n = 0;
	for(int i = 0; i < IT_NR; i++)
	{
		trace("%16s %8d x %8d\n", ctrl_classes[i].name, m_nInstance[i], ctrl_classes[i].size);
		size += m_nInstance[i] * ctrl_classes[i].size;
		n += m_nInstance[i];
	}
	trace("%d Items have been created, Total size = %ld\n", n, size);
	*/
}

//��ҳ��
VD_BOOL CItem::Open()
{
	m_MutexMgr.Enter();
	if(GetFlag(IF_OPENED)){
		
		m_MutexMgr.Leave();
		return FALSE;
	}
	SetFlag(IF_OPENED, TRUE);//��������״̬��λ,��ֹ��δ�
	m_MutexMgr.Leave();

	//data exchange
	m_DC.Lock();
	m_DC.Enable(TRUE);
	m_DC.UnLock();

	if(GetFlag(IF_POPUP))
	{
		Show(TRUE);
	}
	else
	{
		if(m_pParent && m_pParent->IsShown()) //���ҳ���Ѿ���, �����򿪿ؼ���Ҫ������������
		{
			UpdateRgn();
		}
	}
	return TRUE;
}

//��
VD_BOOL CItem::Close()
{
	m_MutexMgr.Enter();
	if(!GetFlag(IF_OPENED)){
		//trace("CItem::Close(),page `%s` is already closed.\n", pTitle);
		m_MutexMgr.Leave();
		//printf("CItem::Close() 111 \n");
		return FALSE;
	}
	SetFlag(IF_OPENED, FALSE);
	m_MutexMgr.Leave();

	m_DC.Lock();
	m_DC.Enable(FALSE);
	m_DC.UnLock();

	if(GetFlag(IF_POPUP))
	{
		//printf("CItem::Close() 222 \n");
		Show(FALSE);
		//printf("CItem::Close() 333 \n");
	}
	return TRUE;
}

//�õ���/�ر�״̬
VD_BOOL CItem::IsOpened()
{
	CGuard guard(m_MutexMgr);

	return GetFlag(IF_OPENED);
}

VD_BOOL CItem::InvalidateRgn(CRgn * pRgn)
{
	m_DC.Lock();
	m_DC.SubtractRgn(pRgn);
	m_DC.UnLock();

	return TRUE;
}

//���ܻ�ı�pRgn������
VD_BOOL CItem::ValidateRgn(CRgn * pRgn)
{
	//draw validate region
	m_DC.Lock();
	m_DC.SwapRgn(pRgn);
	m_DC.UnLock();

//	printf("ValidateRgn 000 \n");
	Draw();
//	printf("ValidateRgn 111 \n");
	//union old region, but rectangles are not mergered
	m_DC.Lock();
	m_DC.UnionRgn(pRgn);
	m_DC.UnLock();

	return TRUE;
}

void CItem::UpdateRgn()
{
	CRgn rgn;
	VD_RECT rect;

	CGuard guard(m_MutexMgr);
	if(!GetFlag(IF_OPENED))
	{
		return;
	}

	if(GetFlag(IF_POPUP))
	{
		m_DC.Lock();
		CItem * pItem = m_pUp;

		m_DC.SetBound(m_Rect);

		while(pItem)
		{
			//get rect of top item
			pItem->GetRect(&rect);
			
			//invalid unvisible region
			m_DC.SubtractRect(&rect);
			
			//point to next top item
			pItem = pItem->m_pUp;
		}
		m_DC.UnLock();
	}
	else
	{
		if(GetFlag(IF_SHOWN))
		{
			m_DC.Lock();
			m_DC.SetBound(m_Rect);
			m_pParent->GetRect(&rect);
			m_DC.IntersectRect(&rect);
			m_DC.UnLock();
			
			m_pParent->m_DC.Lock();
			m_pParent->m_DC.SubtractRect(m_Rect);
			m_pParent->m_DC.UnLock();
		}
	}
}

void CItem::ClearRgn()
{
	m_DC.Lock();
	m_DC.SetBound(m_Rect);
	m_DC.UnLock();
}

void CItem::GlobalInvalidateRect(VD_PCRECT pRect)
{
	CRgn rgn;

	rgn.CreateRectRgn(pRect);
	GlobalInvalidateRgn(&rgn);
}

void CItem::GlobalValidateRect(VD_PCRECT pRect)
{
	CRgn rgn;
	
	//printf("GlobalValidateRect %d %d %d %d \n", pRect->left, pRect->top, pRect->right, pRect->bottom);
	
	rgn.CreateRectRgn(pRect);
	GlobalValidateRgn(&rgn);
}

void CItem::GlobalInvalidateRgn(CRgn * pRgn)
{
	CItem * pItem = m_pDown;
	while(pItem)
	{
		pItem->InvalidateRgn(pRgn);
		pItem = pItem->m_pDown;
	}
}

void CItem::GlobalValidateRgn(CRgn * pRgn)
{
	CRgn rgn;
	CItem * pItem;
	VD_RECT rect;
	
	//��ȥ�ϲ�ҳ�������
	pItem = m_pUp;
	while(pItem)
	{
		pItem->GetRect(&rect);
		pRgn->SubtractRect(&rect);
		pItem = pItem->m_pUp;
	}
	
	//printf("GlobalValidateRgn 000 \n");
	
	//ˢ���²�ҳ�������
	pItem = m_pDown;
	while(pItem)
	{
		//printf("GlobalValidateRgn 111 \n");
		pItem->GetRect(&rect);
		//printf("GlobalValidateRgn 222 \n");
		if(pRgn->RectInRegion(&rect))
		{
			//printf("GlobalValidateRgn 333 \n");
			rgn.CreateRectRgn(&rect);
			rgn.IntersectRgn(pRgn);
			//printf("GlobalValidateRgn 444 \n");
			pItem->ValidateRgn(&rgn);
			//printf("GlobalValidateRgn 555 \n");
			pRgn->SubtractRect(&rect);
			//printf("GlobalValidateRgn 666 \n");
		}
		pItem = pItem->m_pDown;
	}

	//printf("GlobalValidateRgn end \n");
}

void CItem::SetRect(VD_PCRECT pRect, VD_BOOL bReDraw)
{
	CGuard guard(m_MutexMgr);
	
	assert(pRect);
	//save old rectangle
	VD_RECT old_rect = m_Rect;
	CRgn rgn;
	
	if(memcmp(pRect, &old_rect, sizeof(VD_RECT)) == 0)
	{
		return;
	}
	
	if(GetFlag(IF_POPUP))
	{
		m_Rect = *pRect;

		rgn.CreateRectRgn(&old_rect);
		rgn.SubtractRect(m_Rect);

		//����������������
		GlobalInvalidateRect(m_Rect);
		GlobalValidateRgn(&rgn);

		//��������������
		UpdateRgn();

		if(bReDraw)
		{
			Draw();
		}
	}
	else
	{
		if(GetFlag(IF_SHOWN))
		{	
			m_DC.Lock();
			m_DC.SubtractRect(pRect); // ��ȥ�µ����򣬱��ⲻ��Ҫ�ػ�
			m_DC.UnLock();
			Show(FALSE, bReDraw);
			m_Rect = *pRect;
			Show(TRUE, bReDraw);
		}
		else
		{
			m_Rect = *pRect;
		}
	}
}

VD_BOOL CItem::VD_SendMessage(uint msg, uint wpa, uint lpa)
{
	if(m_pThread)
	{
		return m_pThread->VD_SendMessage(msg, wpa, lpa);
	}

	CItem *pItem = m_pParent;

	while(pItem)
	{
		if(pItem->m_pThread)
		{
			return pItem->m_pThread->VD_SendMessage(msg, wpa, lpa);
		}
		pItem = pItem->m_pParent;
	}

	return FALSE;
}

VD_BOOL CItem::VD_DispatchMessage(int screen, uint msg, uint wpa, uint lpa)
{
	//printf("VD_DispatchMessage :screen:%d, wpa: %d \n", screen, wpa);
	//printf("VD_DispatchMessage :msg:0x%x %d,%d \n", msg,VD_HIWORD(lpa),VD_LOWORD(lpa));
	
	if(screen < 0 || screen >= N_GUI_CORE)
	{
		//printf("VD_DispatchMessage FALSE 1\n");
		return FALSE;
	}

	CGuard guard(m_MutexMgr);

	if(m_pActive[screen] && (msg < XM_MOUSEMOVE || msg > XM_MBUTTONDBLCLK)) // �������Ϣ
	{
		//printf("VD_DispatchMessage not mouse msg\n");
		m_pActive[screen]->VD_SendMessage(msg, wpa, lpa);
	}
	else // �����Ϣ
	{
		int px = VD_HIWORD(lpa);
		int py = VD_LOWORD(lpa);
		CItem * pItem;
		VD_RECT rect;

		pItem = m_pTop[screen];
		while(pItem)
		{
			// ����ÿؼ���������,��ֱ�ӽ���Ϣ���͸���
			if (pItem->GetFlag(IF_CAPTURED))
			{
				//printf("item capture input\n");
				return pItem->VD_SendMessage(msg, wpa, lpa);
			}
			pItem->GetRect(&rect);
			if(PtInRect(&rect, px, py)) // �ҵ�������ڵĿؼ�
			{
				//printf("find cursor of item\n");
				if(pItem->m_dwStyle & pageActiveMost)
				{
					return pItem->VD_SendMessage(msg, wpa, lpa);
				}
				
				if (msg == XM_MOUSEMOVE) // ����ƶ���Ϣ��ʱֻ��������Ŀؼ�
				{
					CPage *pPage = dynamic_cast<CPage*>(pItem);
					if(pItem == m_pActive[screen] || 
						(dynamic_cast<CMenu*>(m_pActive[screen]) &&  pPage  && (m_pActive[screen]->GetParent() == pPage) && dynamic_cast<CMenu*>(pPage->GetItemAt(px,py))) || 
						(dynamic_cast<CMenu*>(m_pActive[screen]) && dynamic_cast<CMenu*>(pItem) && (m_pActive[screen]->GetParent() == pItem->GetParent())))
					{
						return pItem->VD_SendMessage(msg, wpa, lpa);
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					// ������갴����Ϣ
					//printf("mouse VD_DispatchMessage :msg:0x%x\n", msg);
					if((msg == XM_LBUTTONDOWN || msg == XM_MBUTTONDOWN || msg == XM_RBUTTONDOWN) && pItem != m_pActive[screen])
					{
						// ���������Ŀؼ��Ƿ�ҳ��ĵ���ʽ�ؼ���ֱ�ӷ���������
						if(m_pActive[screen] && m_pActive[screen]->GetFlag(IF_POPUP) && dynamic_cast<CPage*>(m_pActive[screen]) == 0)
						{
							//printf("1\n");
							return m_pActive[screen]->VD_SendMessage(msg, wpa, lpa);
						}

						// ���������µĿؼ��� ����ɹ�����Ϣ������.
						// ���ÿؼ���һ��ҳ��������ҳ���ʱ��, ���ܻἤ��ʧ��.
						if(pItem->SetActive() == pItem)
						{
							//printf("2\n");
							return m_pActive[screen]->VD_SendMessage(msg, wpa, lpa);
						}
					}

					if(!m_pActive[screen]) 
					{
						//printf("3\n");
						return FALSE;
					}
					//printf("4\n");
					return m_pActive[screen]->VD_SendMessage(msg, wpa, lpa);
				}
			}
			pItem = pItem->m_pDown;
		}
	}
	
	return FALSE;
}

void CItem::CloseAll(int iPageCloseFlag)   //modified by nike.xie 20090903//����ҳ���жϱ�־��ȱʡ����
{
	CGuard guard(m_MutexMgr);
	
	//close all pages
	for(int i = 0; i < N_GUI_CORE; i++)
	{
		CItem * pItem = m_pTop[i];
		if(!pItem)
		{
			continue;
		}
		CItem * temp = pItem->m_pDown;
		while(pItem)
		{
			temp = pItem->m_pDown;
			if(pItem->m_iType == IT_PAGE)
			{
				//add by nike.xie 20090903 //�����жϹرյ�ҳ���Ƿ�¼��ط�
				if ( (iPageCloseFlag == 1) && (((CPage*)pItem)->string == LOADSTR("titles.search")) )
				{
					//���¼��طŴ��ڲ���״̬���򲻹رմ�ҳ��
// 					if(((CPagePlay*)pItem)->GetPagePlayState() == PAGEPLAY_STATE_PLAYING)
// 					{
// 						break;
// 					}
				}
				//end
				
				if(((CPage*)pItem)->m_dwStyle & pageDialog)
				{
					pItem->m_pThread->m_bLoop = FALSE;
					pItem->VD_SendMessage(XM_KEYDOWN, KEY_ESC);//������ͳ���˵��������������ʱm_bLoop���Ա�֤�߳�ѭ���˳�
				}
				else if(!(((CPage*)pItem)->m_dwStyle & pageNoExpire))
				{
					m_MutexMgr.Leave();
					((CPage*)pItem)->Close(UDM_CANCEL);
					m_MutexMgr.Enter();
				}
			}
			else//����ʽ�ؼ�,���û�����´��룬Honeywell�˵�ע��ʱ����������û�������������
			{
				pItem->Close();
			}
			pItem = temp;
		}
	}
}

CDC * CItem::GetDC()
{
	return &m_DC;
}

ITEM_TYPE CItem::GetType()
{
	return m_iType;
}

int CItem::GetScreen(CPage* page)
{
	CPage* p = reinterpret_cast<CPage *>(page);
	if(p)
	{
		return p->m_screen;
	}
	else
	{
		//csp modify 20130323
		if(page)
		{
			printf("#########################GetScreen error:0x%08x\n",(int)(page));
			//exit(-1);
		}
	}
	return (int)(page);
}

void CItem::AdjustRectAll()
{
	CGuard guard(m_MutexMgr);
	
	for(int i = 0; i < N_GUI_CORE; i++)
	{
		CItem * pItem = m_pTop[i];
		while(pItem)
		{
			pItem->AdjustRect();
			pItem = pItem->m_pDown;
		}
	}
}

void CItem::SetKeyInputFlag(KEY_INPUT_FLAG index, int flag)
{
	m_bKeyInputFlags[index] = flag;
}

int CItem::GetKeyInputFlag(KEY_INPUT_FLAG index)
{
	return m_bKeyInputFlags[index];
}

void CItem::SetRelatedItem(CItem *up, CItem *down, CItem *left, CItem *right)
{
	if (m_pRelatedItems == NULL) 
	{
		m_pRelatedItems = new RELATEDITEM;
	}
	m_pRelatedItems->m_pUpRelated = up;
	m_pRelatedItems->m_pDownRelated = down;
	m_pRelatedItems->m_pLeftRelated = left;
	m_pRelatedItems->m_pRightRelated = right;

	return;
}

std::string CItem::GetName()
{
	return string;
}

CItemThread* CItem::GetThread()
{
	return  m_pThread;

}

uint CItem::GetStyle()
{
	return m_dwStyle;
}

