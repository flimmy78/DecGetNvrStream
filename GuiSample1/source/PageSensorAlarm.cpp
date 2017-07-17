#include "GUI/Pages/PageSensorAlarm.h"

#include "GUI/Pages/PageAlarmDeal.h"
#include "GUI/Pages/BizData.h"

static	std::vector<CItem*>	items[SENSOR_SUBPAGES];

static VD_BITMAP * pBmp_button_normal;
static VD_BITMAP * pBmp_button_push;
static VD_BITMAP * pBmp_button_select;

static VD_BITMAP * pBmp_tab_normal;
static VD_BITMAP * pBmp_tab_push;
static VD_BITMAP * pBmp_tab_select;

static VD_BITMAP * pBmp_infoicon;

#define ALARMDEAL_SET_ALL

static char* szAlarmDelayTime[7] = {
	"5",
	"10",
	"20",
	"30",
	"60",
	"120",
	//"&CfgPtn.Always",
};
void bizData_SaveSensorAlarmPara(int ch, SSensorAlarmPara* pGAlarm);
void bizData_GetSensorAlarmDefault(int ch, SSensorAlarmPara* pGAlarm);
void bizData_GetSensorAlarmPara(int ch, SSensorAlarmPara* pGAlarm);

int bizData_GetSENSORAlarmDealDefault(u8 nCh, SGuiAlarmDispatch* pGDispatch);
extern void bizData_SaveSENSORAlarmDeal(u8 nCh, SGuiAlarmDispatch* pDispatch);
extern void bizData_GetSENSORAlarmDeal(u8 nCh, SGuiAlarmDispatch* pDispatch);

//void bizData_SaveVLOSTAlarmDeal(u8 nCh, SGuiAlarmDispatch* pDispatch);

//void bizData_SaveVMOTIONAlarmDeal(u8 nCh, SGuiAlarmDispatch* pDispatch);

static STabSize TabSzSensorAlarm[] = {
	{ 4, 6 },
	{ 3, 6 }
};

//static int GetMaxChnNum() { return GetVideoMainNum(); }
static int GetMaxSensorChnNum() { return GetSenSorNum(); }

CPageSensorAlarm::CPageSensorAlarm( VD_PCRECT pRect,VD_PCSTR psz,VD_BITMAP* icon /*= NULL*/,CPage * pParent /*= NULL*/ )
:CPageFrame(pRect, psz, icon, pParent)
, m_nCurID(0)
{
	//nChMax = GetMaxChnNum();
	nChMax = GetMaxSensorChnNum();
	STabSize* pTabSz = TabSzSensorAlarm;
	for(int i=0; i<TAB_NUM; i++)
	{
		if(pTabSz[i].nRow >= nChMax)
		{
			nRealRow[i] = nChMax;
			nPageNum[i] = 1;
			nHideRow[i] = 0;
		}
		else
		{
			nRealRow[i] = pTabSz[i].nRow;
			nPageNum[i] = (nChMax+nRealRow[i]-1)/nRealRow[i];			
			nHideRow[i] = nPageNum[i]*nRealRow[i]-nChMax;
		}
		
		nCurPage[i] = 0;
	}
	
	pTabPara = (SSensorAlarmPara *)malloc(sizeof(SSensorAlarmPara)*nChMax);
	if(!pTabPara)
	{
		PUBPRT("Here");  //zlb20111117 malloc
		exit(1);
	}
	
	SetMargin(0,0,0,0);
	
	//pBmp_button_normal = VD_LoadBitmap(DATA_DIR"/button_normal.bmp");
	pBmp_button_normal = VD_LoadBitmap(DATA_DIR"/temp/btn.bmp");
	ReplaceBitmapBits(pBmp_button_normal, VD_GetSysColor(COLOR_TRANSPARENT) , VD_GetSysColor(VD_COLOR_WINDOW));
	
	//pBmp_button_push = VD_LoadBitmap(DATA_DIR"/button_push.bmp");
	pBmp_button_push = VD_LoadBitmap(DATA_DIR"/temp/btn_f.bmp");
	ReplaceBitmapBits(pBmp_button_push, VD_GetSysColor(COLOR_TRANSPARENT) , VD_GetSysColor(VD_COLOR_WINDOW));
	
	//pBmp_button_select = VD_LoadBitmap(DATA_DIR"/button_select.bmp");
	pBmp_button_select = VD_LoadBitmap(DATA_DIR"/temp/btn_f.bmp");
	ReplaceBitmapBits(pBmp_button_select, VD_GetSysColor(COLOR_TRANSPARENT) , VD_GetSysColor(VD_COLOR_WINDOW));
	
	pBmp_tab_normal = VD_LoadBitmap(DATA_DIR"/temp/tab.bmp");
	pBmp_tab_push = VD_LoadBitmap(DATA_DIR"/temp/tab_check.bmp");
	pBmp_tab_select = VD_LoadBitmap(DATA_DIR"/temp/tab_f.bmp");
	
	//��һ��ҳ�����ҳ����ư�ť  ¼�����ã��ֶ�¼��¼�񲼷�
	char* szSubPgName[SENSOR_COMMBUTTON] = {
		"&CfgPtn.Basic",
		"&CfgPtn.AlarmHandling",	
		"&CfgPtn.Schedule",
		"&CfgPtn.Default",
		"&CfgPtn.apply",
		"&CfgPtn.Exit",
	};	

	int szSubPgNameLen[SENSOR_COMMBUTTON] = {
		TEXT_WIDTH*4,//"&CfgPtn.Basic",
		TEXT_WIDTH*6,//"&CfgPtn.AlarmHandling",	
		TEXT_WIDTH*4,//"&CfgPtn.Schedule",
		TEXT_WIDTH*3,//"&CfgPtn.Default",
		TEXT_WIDTH*2,//"&CfgPtn.apply",
		TEXT_WIDTH*2,//"&CfgPtn.Exit",
	};


	pTabFrame = CreateTableBox(CRect(17, 55+pBmp_tab_normal->height, m_Rect.Width()-17, m_Rect.Height()-45),this,1,1);
	pTabFrame->SetFrameColor(VD_RGB(56,108,148));


	CRect rtSub(26, 55, m_Rect.Width()-20, 55+pBmp_tab_normal->height);
	int i;
	for (i = 0; i<SENSOR_SUBPAGES; i++)
	{
		int btWidth = szSubPgNameLen[i]+10;
		rtSub.right = rtSub.left + btWidth;
		pButton[i] = CreateButton(rtSub, this, szSubPgName[i], (CTRLPROC)&CPageSensorAlarm::OnClickSubPage, NULL, buttonNormalBmp, 1);
		pButton[i]->SetBitmap(pBmp_tab_normal, pBmp_tab_select, pBmp_tab_push, pBmp_tab_push);
		rtSub.left = rtSub.right;
	}


	int btwidth = 85;
	int btspace = 16;
	rtSub.top = m_Rect.Height()-38;
	rtSub.bottom = rtSub.top + pBmp_button_normal->height;
	rtSub.left = m_Rect.Width() - 20 - btwidth*(SENSOR_COMMBUTTON -SENSOR_SUBPAGES) - btspace*(SENSOR_COMMBUTTON -SENSOR_SUBPAGES-1);
	rtSub.right = rtSub.left + btwidth;

	pInfoBar = CreateStatic(CRect(40, 
								rtSub.top,
								317,
								rtSub.top+22), 
							this, 
							"");
	//pInfoBar->SetBkColor(VD_RGB(8,66,189));

	pBmp_infoicon = VD_LoadBitmap(DATA_DIR"/temp/icon_info.bmp");
	pIconInfo = CreateStatic(CRect(17, 
								rtSub.top+1,
								37,
								rtSub.top+21), 
							this, 
							"");
	pIconInfo->SetBitmap(pBmp_infoicon,NULL);
	pIconInfo->Show(FALSE);
	
	for (i=SENSOR_SUBPAGES; i<SENSOR_COMMBUTTON; i++)
	{
		pButton[i] = CreateButton(rtSub, this, szSubPgName[i], (CTRLPROC)&CPageSensorAlarm::OnClickSubPage, NULL, buttonNormalBmp);
		pButton[i]->SetBitmap(pBmp_button_normal, pBmp_button_select, pBmp_button_push);

		rtSub.left = rtSub.right + btspace;
		rtSub.right = rtSub.left + btwidth;
		
	}
	
	bSchChange = (uchar*)malloc(sizeof(uchar)*nChMax);
	sch = (uint**)malloc(sizeof(uint*)*nChMax);
	for(int i=0; i<nChMax; i++)
	{
		sch[i] = (uint*)malloc(sizeof(uint)*7);

		for(int j=0; j<7; j++)
		{
			sch[i][j] = 0;
		}
	}

	InitPage0();
 	InitPage1();

	InitPage2();
#ifndef DISABLE_ALARM_SCH
#else
	pButton[2]->Show(FALSE, TRUE);
#endif

	ShowSubPage(0,TRUE);
	pButton[0]->Enable(FALSE);

	CRect rtPage(0,0,642,418);
	m_pPageAlarmDeal = new CPageAlarmDeal(rtPage, "&CfgPtn.AlarmHandling", NULL, this, 0);
	
	psAlarmDispatchIns = (SGuiAlarmDispatch *)malloc(sizeof(SGuiAlarmDispatch)*(nChMax+1));

}

CPageSensorAlarm::~CPageSensorAlarm()
{

}


void CPageSensorAlarm::SwitchPage( int subID )
{
	if (subID<0 || subID>=SENSOR_SUBPAGES )
	{
		return;
	}

	if (subID == m_nCurID)
	{
		return;
	}

	//LoadPara(); //yzw del
	
	//���ص�ǰ��ҳ�棬��ʾ�µ���ҳ��
	ShowSubPage(m_nCurID, FALSE);
	pButton[m_nCurID]->Enable(TRUE);
	m_nCurID = subID;
	ShowSubPage(m_nCurID, TRUE);
	pButton[subID]->Enable(FALSE);
}

void CPageSensorAlarm::ShowSubPage( int subID, BOOL bShow )
{
	if (subID<0 || subID>=SENSOR_SUBPAGES )
	{
		return;
	}

	int count = items[subID].size();

	//printf("page %d, bshow=%d ,count=%d \n", subID, bShow, count);

	for (int i=0; i<count; i++)
	{
		if(items[subID][i])
		{
			items[subID][i]->Show(bShow);
		}
	}
	AdjHide();
}

void CPageSensorAlarm::OnClickSubPage()
{
	int i = 0;
	BOOL bFind = FALSE;
	CButton *pFocusButton = (CButton *)GetFocusItem();
	for (i = 0; i < SENSOR_COMMBUTTON; i++)
	{
		if (pFocusButton == pButton[i])
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{	
		//printf("find the focus button %d\n", i);
		if (i<SENSOR_SUBPAGES)
		{
			SwitchPage(i);	
		} 
		else
		{
			if(i==SENSOR_SUBPAGES+1) // apply
			{
				WriteLogs(m_nCurID);
				
				BIZ_DT_DBG("start sensor alarm m_nCurID %d\n", m_nCurID);

				switch(m_nCurID)
				{					
					case 0:
					{
						//BIZ_DATA_DBG("sensor alarm 0");
						
						int nChkChnAll 	= pChkChnAllPage0->GetValue();
						int nChkEnAll 	= pChkEnableAllPage0->GetValue();
						int nCmbTypeAll = pComboTypeAllPage0->GetCurSel();
						
						if(nChkChnAll)
						{
							//BIZ_DATA_DBG("sensor alarm 10");
							for(i=0; i<nChMax; i++)
							{
								pTabPara[i].bEnable = nChkEnAll;
								pTabPara[i].nAlarmType = nCmbTypeAll;
							}
							
							for(i=0; i<nRealRow[0]; i++)
							{
								tBasePage0[i].pChkEnable->SetValue(nChkEnAll);
								
								tBasePage0[i].pComboType->SetCurSel(nCmbTypeAll);								
							}
						}	
						int ret = 0;
						for(i=0; i<nChMax; i++)
						{
							bizData_SaveSensorAlarmPara(i, &pTabPara[i]);
						}

						SetInfo("&CfgPtn.SaveConfigSuccess");				
					}
					break;
					case 1:
					{
						int ret = 0;
						int nCmbDlyAll = pComboDelayAllPage1->GetCurSel();
						if(pChkChnAllPage1->GetValue())
						{
							for(i=0; i<nChMax; i++)
							{
								pTabPara[i].nDelayTime = nCmbDlyAll;
							}
							
							for(i=0; i<nRealRow[1]; i++)
							{
								tDealPage1[i].pComboDelay->SetCurSel(nCmbDlyAll);
							}
							
							for(i=0; i<nChMax; i++)
							{
								if(psAlarmDispatchIns)
								{
									memcpy(&psAlarmDispatchIns[i],
										&psAlarmDispatchIns[nChMax],
										sizeof(SBizAlarmDispatch)
									);
								}
							}
						}
						
						SaveDealPara2Cfg(nChMax);

						for(i=0; i<nChMax; i++)
						{
							bizData_SaveSensorAlarmPara(i, &pTabPara[i]);
						}

						SetInfo("&CfgPtn.SaveConfigSuccess");
					}
					break;
					
 #ifndef DISABLE_ALARM_SCH
					case 2:
					{	
						int ret = 0;
						//for(int i=0; i<sizeof(sch)/sizeof(sch[0]); i++)
						for(i = 0; i < nChMax; i++)
						{
							ret = SaveScrStr2Cfg(
								sch[i],
								GSR_CONFIG_ALARM_SENSOR_SCH_SCH, 
								EM_GSR_CTRL_SCH_VAL, 
								i
							);
						}

						if(ret == 0)
						{
							SetInfo("&CfgPtn.SaveConfigSuccess");
						}
						else
						{
							SetInfo("&CfgPtn.SaveConfigFail");
						}
					}
					break;
#endif
				}				
			}
			else if(i==SENSOR_SUBPAGES)
			{
				//default
				
				char tmpbuf[32] = {0};
	
				switch(m_nCurID)
				{
					case 0:
					{
						int ret = 0;
						for(i=0; i<nChMax; i++)
						{
							SSensorAlarmPara sPIns;
							
							bizData_GetSensorAlarmDefault( i, &sPIns );
							pTabPara[i].bEnable = sPIns.bEnable;
							pTabPara[i].nAlarmType = sPIns.nAlarmType;
							strncpy(pTabPara[i].szName, sPIns.szName,sizeof(sPIns.szName));
						}	
						
						if(ret == 0)
						{
							SetInfo("&CfgPtn.LoadDefaultSuccess");
						}
						else
						{
							SetInfo("&CfgPtn.LoadDefaultFail");
						}						

						int nChBase = nCurPage[0]*nRealRow[0];
						for (int i = 0; i<nRealRow[0]; i++)
						{
							tBasePage0[i].pChkEnable->SetValue(pTabPara[nChBase+i].bEnable);
							tBasePage0[i].pComboType->SetCurSel(pTabPara[nChBase+i].nAlarmType);
							tBasePage0[i].pEditName->SetText(pTabPara[nChBase+i].szName);
						}
					}
					break;
					case 1:
					{

						int ret = 0;
						for(i=0; i<nChMax; i++)
						{
							SSensorAlarmPara sPIns;
							
							bizData_GetSensorAlarmDefault( i, &sPIns );
							pTabPara[i].nDelayTime = sPIns.nDelayTime;
						}	

						int nChBase = nCurPage[1]*nRealRow[1];
						
						for (int i = 0; i<nRealRow[1]; i++)
						{	
							tDealPage1[i].pComboDelay->SetCurSel(pTabPara[nChBase+i].nDelayTime);
						}
						
						for(int i=0; i<nChMax; i++)
						{
							ret |= bizData_GetSENSORAlarmDealDefault(i, &psAlarmDispatchIns[i]);
						}

						if(ret == 0)
						{
							SetInfo("&CfgPtn.LoadDefaultSuccess");
						}
						else
						{
							SetInfo("&CfgPtn.LoadDefaultFail");
						}
					}
					break;
					case 2:
					{
						int nCh = pComboChnPage2->GetCurSel();
							
						int ret = GetScrStrSetCurSelDo(
							(void*)&pSCHGridPage2[0],
							GSR_CONFIG_ALARM_SENSOR_SCH_SCH+GSR_DEFAULT_OFFSET, 
							EM_GSR_CTRL_SCH, 
							nCh
						);
						
						uint mask = 0;
						
						for(i=0; i<7; i++)
						{
							pSCHGridPage2[i]->GetMask(&mask);
							//BIZ_DO_DBG("GET msk idx %d mask %x\n", i, mask);
							SetSch(nCh, i, &mask);
						}

						uint mask_cur[7] = {0};
						GetSch(nCh, mask_cur);
						for(int n=0; n<7; n++)
						{
							pSCHGridPage2[n]->SetMask(&mask_cur[n]);
						}

						if(ret == 0)
						{
							SetInfo("&CfgPtn.LoadDefaultSuccess");
						}
						else
						{
							SetInfo("&CfgPtn.LoadDefaultFail");
						}
					}
					break;
				}				
			}
			else 
			{
				//exit
				this->Close();
			}
			
		}

	}
}

void CPageSensorAlarm::AdjHide()
{
	int curID = m_nCurID;

	if(m_nCurID>=2)
	{
		return;
	}
	
	if( nPageNum[curID]-1 == nCurPage[curID] && nPageNum[curID] > 1 )
	{
		VD_BOOL bHide = FALSE;
		
		int firstHide = nRealRow[curID]-nHideRow[curID];
		int lastHide = nRealRow[curID];
		int nRow     = nRealRow[curID];
		int nCol     = TabSzSensorAlarm[curID].nCol;

		switch(curID)
		{
			case 0:
			{
				for(int i=firstHide; i<lastHide; i++)
				{
					tBasePage0[i].pChnIndex->Show(bHide,TRUE);			
					tBasePage0[i].pChkEnable->Show(bHide,TRUE);
					tBasePage0[i].pComboType->Show(bHide,TRUE);
					tBasePage0[i].pEditName->Show(bHide,TRUE);
				}
			} break;
			case 1:
			{	
				for(int i=firstHide; i<lastHide; i++)
				{
					tDealPage1[i].pChnIndex->Show(bHide,TRUE);			
					tDealPage1[i].pComboDelay->Show(bHide,TRUE);
					tDealPage1[i].pBtDeal->Show(bHide,TRUE);
				}
			} break;
		}		
	}
}

void CPageSensorAlarm::OnTrackMove0()
{	
	VD_BOOL bHide = FALSE;
	int 	idx;
	int curID = m_nCurID;

	//printf("PageSensorAlarm curID: %d\n", curID);
	
	if (curID > 1)
		return;
	
	nCurPage[curID] = pScrollbar[curID]->GetPos();

	//printf("nCurPage = %d \n", nCurPage[curID]);

	int firstHide = nRealRow[curID]-nHideRow[curID];
	int lastHide = nRealRow[curID];
	int nRow     = nRealRow[curID];
	int nCurPg   = nCurPage[curID];
	int nPgNum   = nPageNum[curID];
	int nCol     = TabSzSensorAlarm[curID].nCol;
	
	for(int i=0; i<nRow; i++)
	{
		CStatic*	pStatic = (curID==0)?tBasePage0[i].pChnIndex:tDealPage1[i].pChnIndex;
		
		char szID[16] = {0};
		idx = nCurPage[curID]*nRealRow[curID]+i;
		sprintf(szID,"%d",idx+1);
		pStatic->SetText(szID);
	}	
	
	bHide = (nCurPg == nPgNum-1)?FALSE:TRUE;
	
	switch(curID)
	{
		case 0: // get current page, refresh ctrls on current page
		{
			for(int i=0; i<nRow; i++)
			{
				idx = nCurPage[curID]*nRealRow[curID]+i;
				tBasePage0[i].pChkEnable->SetValue(pTabPara[idx].bEnable);
				tBasePage0[i].pComboType->SetCurSel(pTabPara[idx].nAlarmType);
				tBasePage0[i].pEditName->SetText(pTabPara[idx].szName);				
				if(idx >= nChMax - 1)
				{
					break;
				}
			}
			
			for(int i=firstHide; i<lastHide; i++)
			{
				tBasePage0[i].pChnIndex->Show(bHide,TRUE);
				tBasePage0[i].pChkEnable->Show(bHide,TRUE);			
				tBasePage0[i].pComboType->Show(bHide,TRUE);
				tBasePage0[i].pEditName->Show(bHide,TRUE);
			}
		} break;
		case 1:
		{
			// load para
			
			for(int i=0; i<nRow; i++)
			{
				idx = nCurPage[curID]*nRealRow[curID]+i;
				tDealPage1[i].pComboDelay->SetCurSel(pTabPara[idx].nDelayTime);	
				if(idx >= nChMax - 1)
				{
					break;
				}
			}
			
			for(int i=firstHide; i<lastHide; i++)
			{
				tDealPage1[i].pChnIndex->Show(bHide,TRUE);
				tDealPage1[i].pComboDelay->Show(bHide,TRUE);			
				tDealPage1[i].pBtDeal->Show(bHide,TRUE);
			}
		} break;
	}
}

void CPageSensorAlarm::OnClickEnable()
{
	int i = 0;
	BOOL bFind = FALSE;
	CCheckBox* pFocus = (CCheckBox *)GetFocusItem();
	
	for (i = 0; i < nRealRow[0]; i++)
	{
		if (pFocus == tBasePage0[i].pChkEnable)
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		int nChnBase = nCurPage[0]*nRealRow[0]+i;
		
		pTabPara[nChnBase].bEnable = pFocus->GetValue();
	}
}

void CPageSensorAlarm::OnComboBoxPg1()
{
	int i = 0;
	BOOL bFind = FALSE;
	CComboBox* pFocus = (CComboBox *)GetFocusItem();
	
	for (i = 0; i < nRealRow[0]; i++)
	{
		if (pFocus == tBasePage0[i].pComboType)
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		int nChnBase = nCurPage[0]*nRealRow[0]+i;
		
		pTabPara[nChnBase].nAlarmType = pFocus->GetCurSel();
        if (0 == pTabPara[nChnBase].nAlarmType)
        {
            pTabPara[nChnBase].nAlarmType = 1;
        }
        else
        {
            pTabPara[nChnBase].nAlarmType = 0;
        }
	}
}

void CPageSensorAlarm::OnComboBoxPg2()
{
	int i = 0;
	BOOL bFind = FALSE;
	CComboBox* pFocus = (CComboBox *)GetFocusItem();
	
	for (i = 0; i < nRealRow[1]; i++)
	{
		if (pFocus == tDealPage1[i].pComboDelay)
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		int nChnBase = nCurPage[1]*nRealRow[1]+i;
		
		pTabPara[nChnBase].nDelayTime = pFocus->GetCurSel();
	}
}

void CPageSensorAlarm::OnEditName()
{
	int i = 0;
	BOOL bFind = FALSE;
	CEdit* pFocus = (CEdit *)GetFocusItem();
	
	for (i = 0; i < nRealRow[0]; i++)
	{
		if (pFocus == tBasePage0[i].pEditName)
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		int nChnBase = nCurPage[0]*nRealRow[0]+i;
		
		pFocus->GetText(pTabPara[nChnBase].szName, sizeof(pTabPara[nChnBase].szName));
	}
}

void CPageSensorAlarm::InitPage0()
{
	int i = 0;
	CRect rtTab(25, 55+pBmp_tab_normal->height+10, m_Rect.Width()-25, 55+pBmp_tab_normal->height+10 + (CTRL_HEIGHT+1)*7);
	pTabPage0[0] = CreateTableBox(rtTab, this, 4, 7);
	pTabPage0[0]->SetColWidth(0, 54);
	pTabPage0[0]->SetColWidth(1, 92);
	pTabPage0[0]->SetColWidth(2, 148);
	items[0].push_back(pTabPage0[0]);
	
	if(nPageNum[0]>1)
	{
		pScrollbar[0] = CreateScrollBar(CRect(m_Rect.Width()-25-SCROLLBAR,
											55+pBmp_tab_normal->height+10+30,
											m_Rect.Width()-25,
											55+pBmp_tab_normal->height+10+30+29*6), 
											this,
											scrollbarY,
											0,
											(nPageNum[0]-1)*10, 
											1, 
											(CTRLPROC)&CPageSensorAlarm::OnTrackMove0);

		items[0].push_back(pScrollbar[0]);
	}

	char* szTitle[4] = {
		"&CfgPtn.Channel",
		"&CfgPtn.Enable",
		"&CfgPtn.Type",
		"&CfgPtn.Name",
	};

	for (i= 0; i<4; i++)
	{
		CRect rt;
		pTabPage0[0]->GetTableRect(i, 0, &rt);
		pTabTitlePage0[i] = CreateStatic(CRect(rtTab.left+rt.left, rtTab.top+rt.top, rtTab.left+rt.right, rtTab.top+rt.bottom), this, szTitle[i]);
		pTabTitlePage0[i]->SetTextAlign(VD_TA_LEFT);
		pTabTitlePage0[i]->SetBkColor(VD_RGB(67,77,87));
		pTabTitlePage0[i]->SetTextAlign(VD_TA_XLEFT);
		pTabTitlePage0[i]->SetTextColor(VD_GetSysColor(COLOR_CTRLTEXT));
		items[0].push_back(pTabTitlePage0[i]);
	}

	rtTab.top = rtTab.bottom;
	rtTab.bottom = rtTab.top + CTRL_HEIGHT;
	pStaticAllPage0 = CreateStatic(rtTab, this, "&CfgPtn.All");
	pStaticAllPage0->SetTextAlign(VD_TA_LEFT|VD_TA_YCENTER);
	items[0].push_back(pStaticAllPage0);

	rtTab.top = rtTab.bottom;
	rtTab.bottom = rtTab.top + CTRL_HEIGHT+2;
	pTabPage0[1] = CreateTableBox(rtTab, this, 4, 1);
	pTabPage0[1]->SetColWidth(0, 54);
	pTabPage0[1]->SetColWidth(1, 92);
	pTabPage0[1]->SetColWidth(2, 148);
	items[0].push_back(pTabPage0[1]);


	CRect rt;
	pTabPage0[1]->GetTableRect(0, 0, &rt);
	pChkChnAllPage0 = CreateCheckBox(CRect(rtTab.left+rt.left+5, rtTab.top+rt.top+2, rtTab.left+rt.left+5+CTRL_HEIGHT, rtTab.top+rt.bottom), this);
	pChkChnAllPage0->SetValue(0);
	items[0].push_back(pChkChnAllPage0);
	
	pTabPage0[1]->GetTableRect(1, 0, &rt);
	pChkEnableAllPage0 = CreateCheckBox(
		CRect(rtTab.left+rt.left+5, rtTab.top+rt.top+2, rtTab.left+rt.left+5+CTRL_HEIGHT, rtTab.top+rt.bottom), 
		this );//, 
		//styleEditable, (CTRLPROC)&CPageSensorAlarm::OnClickEnableAll);
	items[0].push_back(pChkEnableAllPage0);

	pTabPage0[1]->GetTableRect(2, 0, &rt);
	pComboTypeAllPage0 = CreateComboBox(CRect(rtTab.left+rt.left+1, rtTab.top+rt.top, rtTab.left+rt.right-1, rtTab.top+rt.bottom), this, 
		NULL );//, (CTRLPROC)&CPageSensorAlarm::OnClickTypeAll);
	
	SValue ComboType[CBX_MAX];
	int nItemCount;
	GetSensorAlarmBaseTypeList(ComboType, &nItemCount, CBX_MAX);
	//printf("yg PageSensorAlarm nItemCount: %d\n", nItemCount);
	for(int k=0; k<nItemCount; k++)
	{
		pComboTypeAllPage0->AddString(ComboType[k].strDisplay);
	}
					
	//pComboTypeAllPage0->AddString("Normal Open");
	//pComboTypeAllPage0->AddString("Normal Close");
	items[0].push_back(pComboTypeAllPage0);


	rtTab.left = 25;
	rtTab.top = 55+pBmp_tab_normal->height+10;
	for (i = 0; i< nRealRow[0]; i++)
	{
		char tmpbuf[32] = {0};
		sprintf(tmpbuf, "%d", i+1);
		CRect rttmp;
		pTabPage0[0]->GetTableRect(0, i+1, &rttmp);
		//printf("tab: %d %d %d %d\n", rtTab.left+rttmp.left, rtTab.top+rttmp.top, rtTab.left+rttmp.right, rtTab.top+rttmp.bottom);
		tBasePage0[i].pChnIndex = CreateStatic(CRect(rtTab.left+rttmp.left+2, rtTab.top+rttmp.top, rtTab.left+rttmp.right, rtTab.top+rttmp.bottom), this, tmpbuf);
		tBasePage0[i].pChnIndex->SetTextAlign(VD_TA_LEFT);

 		pTabPage0[0]->GetTableRect(1, i+1, &rttmp);
		tBasePage0[i].pChkEnable = CreateCheckBox(CRect(rtTab.left+rttmp.left+5, rtTab.top+rttmp.top+2, rtTab.left+rttmp.left+5+CTRL_HEIGHT, rtTab.top+rttmp.bottom), 
			this, styleEditable, (CTRLPROC)&CPageSensorAlarm::OnClickEnable);
		tBasePage0[i].pChkEnable->SetValue(1);

 		pTabPage0[0]->GetTableRect(2, i+1, &rttmp);
		tBasePage0[i].pComboType = CreateComboBox(CRect(rtTab.left+rttmp.left+1, rtTab.top+rttmp.top+1, rtTab.left+rttmp.right-1, rtTab.top+rttmp.bottom-1), 
			this, NULL, (CTRLPROC)&CPageSensorAlarm::OnComboBoxPg1, 0);
		
		for(int k=0; k<nItemCount; k++)
		{
			tBasePage0[i].pComboType->AddString(ComboType[k].strDisplay);
		}
		tBasePage0[i].pComboType->SetCurSel(0);

 		pTabPage0[0]->GetTableRect(3, i+1, &rttmp);
		tBasePage0[i].pEditName = CreateEdit(CRect(rtTab.left+rttmp.left+1, rtTab.top+rttmp.top+1, rtTab.left+rttmp.right-1-SCROLLBAR, rtTab.top+rttmp.bottom-1), 
									this, 12, styleEditable, (CTRLPROC)&CPageSensorAlarm::OnEditName);


		GetSensorAlarmBaseNameCur(i, tmpbuf);
		
		//memset(tmpbuf,0,sizeof(tmpbuf));
		//sprintf(tmpbuf, "SENSOR %d", i+1);
		tBasePage0[i].pEditName->SetText(tmpbuf);

 		items[0].push_back(tBasePage0[i].pChnIndex);
 		items[0].push_back(tBasePage0[i].pChkEnable);
 		items[0].push_back(tBasePage0[i].pComboType);
 		items[0].push_back(tBasePage0[i].pEditName);
	}

	ShowSubPage(0, FALSE);

}

#define BUTTON_WIDTH 85
void CPageSensorAlarm::InitPage1()
{
	int firstColWidth = 64;
	int secondColWidth = 220;

	int i= 0;
	CRect rtTab(25, 55+pBmp_tab_normal->height+10, m_Rect.Width()-25, 55+pBmp_tab_normal->height+10 + (CTRL_HEIGHT+1)*7);
	pTabPage1[0] = CreateTableBox(rtTab, this, 3, 7);
	pTabPage1[0]->SetColWidth(0, firstColWidth);
	pTabPage1[0]->SetColWidth(1, secondColWidth);
	items[1].push_back(pTabPage1[0]);

	
	if(nPageNum[1]>1)
	{
		pScrollbar[1] = CreateScrollBar(CRect(m_Rect.Width()-25-SCROLLBAR,
											55+pBmp_tab_normal->height+10+30,
											m_Rect.Width()-25,
											55+pBmp_tab_normal->height+10+30+29*6), 
											this,
											scrollbarY,
											0,
											(nPageNum[1]-1)*10, 
											1, 
											(CTRLPROC)&CPageSensorAlarm::OnTrackMove0);

		items[1].push_back(pScrollbar[1]);
	}
	
	char* szTitle[3] = {
		"&CfgPtn.Channel",
		"&CfgPtn.HoldingTime",
		"&CfgPtn.AlarmHandling",		
	};

	for (i= 0; i<3; i++)
	{
		CRect rt;
		pTabPage1[0]->GetTableRect(i, 0, &rt);
		pTabTitlePage1[i] = CreateStatic(CRect(rtTab.left+rt.left, rtTab.top+rt.top, rtTab.left+rt.right, rtTab.top+rt.bottom), this, szTitle[i]);
		pTabTitlePage1[i]->SetTextAlign(VD_TA_LEFT);
		pTabTitlePage1[i]->SetBkColor(VD_RGB(67,77,87));
		pTabTitlePage1[i]->SetTextAlign(VD_TA_XLEFT);
		pTabTitlePage1[i]->SetTextColor(VD_GetSysColor(COLOR_CTRLTEXT));
		items[1].push_back(pTabTitlePage1[i]);
	}

	rtTab.top = rtTab.bottom;
	rtTab.bottom = rtTab.top + CTRL_HEIGHT;
	pStaticAllPage1 = CreateStatic(rtTab, this, "&CfgPtn.All");
	pStaticAllPage1->SetTextAlign(VD_TA_LEFT|VD_TA_YCENTER);
	items[1].push_back(pStaticAllPage1);

	rtTab.top = rtTab.bottom;
	rtTab.bottom = rtTab.top + CTRL_HEIGHT+3;
	pTabPage1[1] = CreateTableBox(rtTab, this, 3, 1);
	pTabPage1[1]->SetColWidth(0, firstColWidth);
	pTabPage1[1]->SetColWidth(1, secondColWidth);
	items[1].push_back(pTabPage1[1]);


	CRect rt;
	pTabPage1[1]->GetTableRect(0, 0, &rt);
	pChkChnAllPage1 = CreateCheckBox(CRect(rtTab.left+rt.left+5, rtTab.top+rt.top+2, rtTab.left+rt.left+5+CTRL_HEIGHT, rtTab.top+rt.bottom), this);
	
	items[1].push_back(pChkChnAllPage1);

	pTabPage1[1]->GetTableRect(1, 0, &rt);
	pComboDelayAllPage1 = CreateComboBox(CRect(rtTab.left+rt.left+1, rtTab.top+rt.top, rtTab.left+rt.right-1, rtTab.top+rt.bottom), this, NULL);
	GetScrStrInitComboxSelDo(
			(void*)pComboDelayAllPage1,
			GSR_CONFIG_ALARM_SENSOR_ALARMDEAL_DELAYTIME, 
			EM_GSR_COMBLIST, 
			0
		);
	
	items[1].push_back(pComboDelayAllPage1);

#ifdef ALARMDEAL_SET_ALL
	pTabPage1[1]->GetTableRect(2, 0, &rt);
	pBtDealAllPage1 = CreateButton(CRect(rtTab.left+rt.left+5, rtTab.top+rt.top+1, rtTab.left+rt.left+5+BUTTON_WIDTH, rtTab.top+rt.bottom-1), 
		this, "&CfgPtn.Setup", (CTRLPROC)&CPageSensorAlarm::OnClkDealAllPage1, NULL, buttonNormalBmp);
	pBtDealAllPage1->SetBitmap(pBmp_button_normal, pBmp_button_select, pBmp_button_push);
	items[1].push_back(pBtDealAllPage1);
#endif

	rtTab.left = 25;
	rtTab.top = 55+pBmp_tab_normal->height+10;

	for (i = 0; i< nRealRow[1]; i++)
	{
		char tmpbuf[32] = {0};
		sprintf(tmpbuf, "%d", i+1);
		CRect rttmp;
		pTabPage1[0]->GetTableRect(0, i+1, &rttmp);
		
		tDealPage1[i].pChnIndex = CreateStatic(CRect(rtTab.left+rttmp.left, rtTab.top+rttmp.top, rtTab.left+rttmp.right, rtTab.top+rttmp.bottom), this, tmpbuf);
		tDealPage1[i].pChnIndex->SetTextAlign(VD_TA_LEFT);

		pTabPage1[0]->GetTableRect(1, i+1, &rttmp);
		tDealPage1[i].pComboDelay = CreateComboBox(CRect(rtTab.left+rttmp.left+1, rtTab.top+rttmp.top+1, rtTab.left+rttmp.right-1, rtTab.top+rttmp.bottom-1), 
			this, NULL, NULL, (CTRLPROC)&CPageSensorAlarm::OnComboBoxPg2, 0);
	
		GetScrStrInitComboxSelDo(
			(void*)tDealPage1[i].pComboDelay,
			GSR_CONFIG_ALARM_SENSOR_ALARMDEAL_DELAYTIME, 
			EM_GSR_COMBLIST, 
			0
		);

		pTabPage1[0]->GetTableRect(2, i+1, &rttmp);
		tDealPage1[i].pBtDeal = CreateButton(CRect(rtTab.left+rttmp.left+5, rtTab.top+rttmp.top+1, rtTab.left+rttmp.left+5+BUTTON_WIDTH, rtTab.top+rttmp.bottom-1), 
			this, "&CfgPtn.Setup", (CTRLPROC)&CPageSensorAlarm::OnClkAlarmDeal, NULL, buttonNormalBmp);
		tDealPage1[i].pBtDeal->SetBitmap(pBmp_button_normal, pBmp_button_select, pBmp_button_push); 
		
		items[1].push_back(tDealPage1[i].pChnIndex);
 		items[1].push_back(tDealPage1[i].pComboDelay);
		items[1].push_back(tDealPage1[i].pBtDeal);

	}

	ShowSubPage(1, FALSE);
}

void CPageSensorAlarm::OnClkDealAllPage1()
{
	if (m_pPageAlarmDeal)
	{
		m_pPageAlarmDeal->SetAlarmDeal(ALARMDEAL_SENSOR, -1);
		m_pPageAlarmDeal->Open();
	}
}

void CPageSensorAlarm::OnClkAlarmDeal()
{
	int i = 0;
	BOOL bFind = FALSE;
	CButton *pFocusButton = (CButton *)GetFocusItem();
	for (i = 0; i < nRealRow[1]; i++)
	{
		if (pFocusButton == tDealPage1[i].pBtDeal)
		{
			bFind = TRUE;
			break;
		}
	}


	if (bFind)
	{	
		printf("alarm deal %d\n", i);
		if (m_pPageAlarmDeal)
		{
			PRINT_HERE;
			m_pPageAlarmDeal->SetAlarmDeal(ALARMDEAL_SENSOR, i+nCurPage[1]*nRealRow[1]);
			PRINT_HERE;
			m_pPageAlarmDeal->Open();
			PRINT_HERE;
		}
	}

}

void CPageSensorAlarm::InitPage2()
{
	int i= 0;
	CRect rt;
	rt.left = 55;
	rt.top = 55+pBmp_tab_normal->height+10;
	rt.right = rt.left +130;
	rt.bottom = rt.top + TEXT_HEIGHT;

	CStatic* pText1 = CreateStatic(rt, this, "&CfgPtn.Channel");
	pText1->SetTextAlign(VD_TA_LEFT);
	items[2].push_back(pText1);

	rt.left = rt.right;
	rt.right = rt.left + 100;
	rt.bottom = rt.top + CTRL_HEIGHT;
	pComboChnPage2 = CreateComboBox(rt, this, NULL
	, NULL, (CTRLPROC)&CPageSensorAlarm::OnComboBoxChn, 0);
	
	GetScrStrInitComboxSelDo(
		(void*)pComboChnPage2,
		GSR_CONFIG_ALARM_SENSOR_SCH_CHN, 
		EM_GSR_COMBLIST, 
		0
	);
	pComboChnPage2->SetCurSel(0);
	
	items[2].push_back(pComboChnPage2);

	rt.left = 55;
	rt.top = 55+pBmp_tab_normal->height+40;
	rt.right = m_Rect.Width()-55;
	rt.bottom = rt.top + TEXT_HEIGHT*8;

	pTabSCHPage2 = CreateTableBox(rt, this, 2, 8);
	pTabSCHPage2->SetColWidth(0, 100);
	items[2].push_back(pTabSCHPage2);


	for (i = 0 ; i < 6; i++)
	{
		CRect rtTime;
		pTabSCHPage2->GetTableRect(1, 0, &rtTime);

		char szTmp[10] = {0};
		sprintf(szTmp, "|%02d:00", i*4);

		CStatic* pSCHTime = CreateStatic(CRect(rt.left+rtTime.left +i*(rtTime.Width()/6)-3+(i>3?(i-3):0), rt.top+rtTime.top, rt.left+rtTime.right,rt.top+rtTime.bottom), 
			this, szTmp);

		pSCHTime->SetTextColor(VD_GetSysColor(COLOR_CTRLTEXT));
		pSCHTime->SetTextAlign(VD_TA_LEFTTOP);

		items[2].push_back(pSCHTime);
	}

	char* szWeekDay[7] = {
		"&CfgPtn.Sunday",
		"&CfgPtn.Monday",
		"&CfgPtn.Tuesday",
		"&CfgPtn.Wednesday",
		"&CfgPtn.Thursday",
		"&CfgPtn.Friday",
		"&CfgPtn.Saturday"
	};
	
	for (i= 0; i<7; i++)
	{
		CRect rtGrid;
		pTabSCHPage2->GetTableRect(1, i+1, &rtGrid);
		
		pSCHGridPage2[i] = CreateGridCtrl(CRect(rt.left+rtGrid.left, rt.top+rtGrid.top, rt.left+rtGrid.right,rt.top+rtGrid.bottom),
			this, 1, 24);

		pSCHGridPage2[i]->SetColor(VD_RGB(72,110,216), VD_RGB(67,76,80));
		
		items[2].push_back(pSCHGridPage2[i]);

		CRect rtWeek;
		pTabSCHPage2->GetTableRect(0, i+1, &rtWeek);
		CStatic* pWeekDay = CreateStatic(CRect(rt.left+rtWeek.left, rt.top+rtWeek.top, rt.left+rtWeek.right,rt.top+rtWeek.bottom),
			this, szWeekDay[i]);
		items[2].push_back(pWeekDay);
	}

	GetScrStrSetCurSelDo(
		(void*)&pSCHGridPage2[0],
		GSR_CONFIG_ALARM_SENSOR_SCH_SCH, 
		EM_GSR_CTRL_SCH, 
		pComboChnPage2->GetCurSel()
	);
		
	rt.left = 55;
	rt.top = rt.bottom+20;
	rt.right = rt.left+130;
	rt.bottom = rt.top + TEXT_HEIGHT;

	CStatic* pText2 = CreateStatic(rt, this, "&CfgPtn.ApplySettingsTo");
	pText2->SetTextAlign(VD_TA_LEFT);
	items[2].push_back(pText2);

	rt.left = rt.right;
	rt.right = rt.left+ 100;
	rt.bottom = rt.top + CTRL_HEIGHT;
	pComboCopy2ChnPage2 = CreateComboBox(rt, this, NULL);
	
	GetScrStrInitComboxSelDo(
		(void*)pComboCopy2ChnPage2,
		GSR_CONFIG_ALARM_SENSOR_SCH_COPYTO, 
		EM_GSR_COMBLIST, 
		0
	);
	pComboCopy2ChnPage2->SetCurSel(0);
	
	items[2].push_back(pComboCopy2ChnPage2);

	rt.left = rt.right+10;
	rt.right = rt.left+ 120;
	rt.bottom = rt.top + TEXT_HEIGHT;
	CStatic* pText3 = CreateStatic(rt, this, "&CfgPtn.Channel");
	pText3->SetTextAlign(VD_TA_LEFT);
	items[2].push_back(pText3);

	rt.left = rt.right;
	rt.right = rt.left+ BUTTON_WIDTH;
	rt.bottom = rt.top + 25;
	pBtCopyPage2 = CreateButton(rt, this, "&CfgPtn.Copy", (CTRLPROC)&CPageSensorAlarm::OnClkCopy, NULL, buttonNormalBmp);
	pBtCopyPage2->SetBitmap(pBmp_button_normal, pBmp_button_select, pBmp_button_push);
	items[2].push_back(pBtCopyPage2);

	ShowSubPage(2, FALSE);
}


void CPageSensorAlarm::OnClickTypeAll()
{
	int val;
	if(pChkChnAllPage0->GetValue())
	{
		val = pComboTypeAllPage0->GetCurSel();
		for(int i=0;i<nRealRow[0];i++)
		{
			tBasePage0[i].pComboType->SetCurSel(val);
		}
	}
}

void CPageSensorAlarm::OnClickEnableAll()
{
	int val;
	if(pChkChnAllPage0->GetValue())
	{
		val = pChkChnAllPage0->GetValue();
		
		for(int i=0;i<nRealRow[0];i++)
		{
			tBasePage0[i].pChkEnable->SetValue(val);
		}
	}
}

void CPageSensorAlarm::LoadPara()
{
	char tmpbuf[32] = {0};
	int nChBase;
	
	bModified = FALSE;
	
	pChkEnableAllPage0->SetValue(0);
	pComboTypeAllPage0->SetCurSel(1);
	pComboDelayAllPage1->SetCurSel(0);
	
	for(int i=0; i<nChMax; i++)
	{
		bizData_GetSensorAlarmPara( i, &pTabPara[i] );
	}

	//switch(m_nCurID)
	{
		//case 0:
		{
			nChBase = nCurPage[0]*nRealRow[0];
			for (int i = 0; i< nRealRow[0]; i++)
			{
				tBasePage0[i].pChkEnable->SetValue(pTabPara[nChBase+i].bEnable);
				tBasePage0[i].pComboType->SetCurSel(pTabPara[nChBase+i].nAlarmType);
				tBasePage0[i].pEditName->SetText(pTabPara[nChBase+i].szName);
			}
		}
		//break;
		//case 1:
		{
			nChBase = nCurPage[1]*nRealRow[1];
			
			for (int i = 0; i<nRealRow[1]; i++)
			{
				tDealPage1[i].pComboDelay->SetCurSel(pTabPara[nChBase+i].nDelayTime);
			}
			
			for(int i=0; i<nChMax; i++)
			{
				bizData_GetSENSORAlarmDeal(i, &psAlarmDispatchIns[i]);
			}
			memcpy(&psAlarmDispatchIns[nChMax], &psAlarmDispatchIns[0], sizeof(SGuiAlarmDispatch));
		}
	//	break;
	//	case 2:

#ifndef DISABLE_ALARM_SCH
		{
			int nCh = 0;
			//int nChnMax = GetMaxSensorChnNum();
				//pComboChnPage2->GetCurSel();
			for(nCh = 0; nCh < nChMax; nCh++)
			{
				GetScrStrSetCurSelDo(
					(void*)&pSCHGridPage2[0],
					GSR_CONFIG_ALARM_SENSOR_SCH_SCH, 
					EM_GSR_CTRL_SCH, 
					nCh
				);
				
				uint mask = 0;
				
				for(int i=0; i<7; i++)
				{
					pSCHGridPage2[i]->GetMask(&mask);
					BIZ_DO_DBG("GET msk idx %d mask %x\n", i, mask);
					SetSch(nCh, i, &mask);
				}
			}

			uint mask[7] = {0};
			int curIndex = pComboChnPage2->GetCurSel();
			GetSch(curIndex, mask);
			for(int i=0; i<7; i++)
			{
				pSCHGridPage2[i]->SetMask(&mask[i]);
			}
		}
		//break;
#endif
	}
}

VD_BOOL CPageSensorAlarm::UpdateData( UDM mode )
{	
	if (UDM_OPEN == mode)
	{
		LoadPara();
	} 
	else if (UDM_CLOSED == mode)
	{
        ((CPageAlarmCfgFrameWork *)(this->GetParent()))->ReflushItemName();
    }
	
	return TRUE;
}

void CPageSensorAlarm::WriteLogs(int PageIndex)
{
	switch(PageIndex)
	{
		case 0:
		{
			for (int i = 0; i< 4; i++)
			{
				if(tBasePage0[i].pChkEnable->GetValue() != GetSensorAlarmBaseEnableCheckCur(i))
				{
					BizGUiWriteLog(BIZ_LOG_MASTER_SYSCONFIG, BIZ_LOG_SLAVE_CHANGE_SENSOR_ENABLE);
					break;
				}
			}
			for (int i = 0; i< 4; i++)
			{
				if(tBasePage0[i].pComboType->GetCurSel() != GetSensorAlarmBaseTypeCur(i))
				{
					BizGUiWriteLog(BIZ_LOG_MASTER_SYSCONFIG, BIZ_LOG_SLAVE_CHANGE_SENSOR_TYPE);
					break;
				}
			}
		}
		break;
		case 1:
		{
			BizGUiWriteLog(BIZ_LOG_MASTER_SYSCONFIG, BIZ_LOG_SLAVE_CHANGE_SENSOR_ALARMDEAL);
		}
		break;
		case 2:
		{
			BizGUiWriteLog(BIZ_LOG_MASTER_SYSCONFIG, BIZ_LOG_SLAVE_CHANGE_SENSOR_DETECTION_SCHE);
		}
		break;
		default:
		break;
	}
}


void CPageSensorAlarm::GetPara(uchar nCh, SGuiAlarmDispatch *pPara)
{
	int nChMax = GetMaxSensorChnNum();

	if(nCh>=nChMax)
	{
		nCh = nChMax;
	}
	
	if(pPara&&psAlarmDispatchIns&&nCh<=nChMax)
		memcpy( pPara, &psAlarmDispatchIns[nCh], sizeof(SBizAlarmDispatch) );

	//printf("CH %d Sound %d Mail %d\n", nCh, pPara->nFlagBuzz, pPara->nFlagEmail);
	
	return;
}

void CPageSensorAlarm::SavePara(uchar nCh, SGuiAlarmDispatch *pPara)
{
	int nChMax = GetMaxSensorChnNum();
	
	if(nCh>=nChMax)
	{
		nCh = nChMax;
	}

	BIZ_DO_DBG("CH %d Sound %d Mail %d linktype %d linkidx %d\n", 
		nCh, pPara->nFlagBuzz, pPara->nFlagEmail,
		pPara->sAlarmPtz[0].nALaPtzType, pPara->sAlarmPtz[0].nId
	);
	
	//printf("CH %d Sound %d Mail %d ***\n", nCh, pPara->nFlagBuzz, pPara->nFlagEmail);
	
	if(pPara&&psAlarmDispatchIns&&nCh<=nChMax)
		memcpy( &psAlarmDispatchIns[nCh], pPara, sizeof(SBizAlarmDispatch) );
		
	return;
}

void CPageSensorAlarm::SaveDealPara2Cfg(uchar nRealChNum)
{
	if(!psAlarmDispatchIns) return;
	
	int nChMax = GetMaxSensorChnNum();
	
	if(nChMax>nRealChNum)
		nChMax = nRealChNum;
		
	for(int i=0; i<nChMax; i++)
	{
		BIZ_DO_DBG("CH %d Sound %d Mail %d\n", i, psAlarmDispatchIns[i].nFlagBuzz, psAlarmDispatchIns[i].nFlagEmail);
		BIZ_DO_DBG("CH %d Snape %d %d %d %d\n", i, 
 			psAlarmDispatchIns[i].nSnapChn[0], 
 			psAlarmDispatchIns[i].nSnapChn[1],
 			psAlarmDispatchIns[i].nSnapChn[2],
 			psAlarmDispatchIns[i].nSnapChn[3] );
			
		bizData_SaveSENSORAlarmDeal(i, &psAlarmDispatchIns[i]);
	}
}


void CPageSensorAlarm::SetSch( uchar nCh, uchar idx, uint* psch)
{
	sch[nCh][idx] = *psch;
	//memcpy(sch[curID][nCh][idx], sch, sizeof(uint));
}

void CPageSensorAlarm::GetSch( uchar nCh, uint* psch)
{
	 //*sch = sch[curID][nCh][idx];
	memcpy(psch, sch[nCh], sizeof(uint)*7);
}


VD_BOOL CPageSensorAlarm::MsgProc( uint msg, uint wpa, uint lpa )
{
	int px,py;
	int temp;
	uchar nCh;
	CGridCtrl** pGrid;

	int curID = m_nCurID;
	//printf("CPageSensorAlarm::MsgProc msg:%u, wpa:%u, lpa:%u\n", msg, wpa, lpa);
	
	switch(msg)
	{
	case XM_MOUSEWHEEL:   //cw_scrollbar
		{
			int px = VD_HIWORD(lpa);
			int py = VD_LOWORD(lpa);
			CItem* pItem = GetItemAt(px, py);
			if(pItem == NULL)
			{
				if(GetSenSorNum()>6)
				{
					if (curID < TAB_NUM)
					{
						pScrollbar[curID]->MsgProc(msg, wpa, lpa);
					}
				}
			}
			break;
		}
	case XM_LBUTTONUP:
		{
			nCh = GetCurChn();
			pGrid = GetCurGrid();
			
			uint msk = 0;
			
			for(int i=0; i<7; i++)
			{
				pGrid[i]->GetMask(&msk);
				SetSch(nCh, i, &msk);
			}
			
			/*printf("SET %x %x %x %x %x %x %x\n", 
				sch[0][0],
				sch[0][1],
				sch[0][2],
				sch[0][3],
				sch[0][4],
				sch[0][5],
				sch[0][6]
			);*/
		}
		break;
	case XM_MOUSEMOVE:
		{
			px = VD_HIWORD(lpa);
			py = VD_LOWORD(lpa);
			static CItem* last = NULL;
			CItem* temp = GetItemAt(px, py);

			if(temp != last)
			{
		
				if(temp == NULL);
				{
					ClearInfo();
				}

				last = temp;

			}
		}
		break;
	default:
		break;
	}
		
	return 	CPageFrame::MsgProc(msg, wpa, lpa);
}

uchar CPageSensorAlarm::GetCurChn()
{
	uchar nCh = 0;

	nCh = pComboChnPage2->GetCurSel();
	/*
	switch(curID)
	{
		case 0:
		{
			nCh = pComboBox0[0]->GetCurSel();
		} break;
		case 1:
		{
			nCh = pComboBox1[0]->GetCurSel();
		} break;
		case 2:
		{
			nCh = pComboBox2[0]->GetCurSel();
		} break;
		default:
			break;
	}
	*/

	return nCh;
}

CGridCtrl** CPageSensorAlarm::GetCurGrid()
{	
	return pSCHGridPage2;
	/*
	switch(curID)
	{
		case 0:
		{
			return pGridSCH0;
		} break;
		case 1:
		{
			return pGridSCH1;
		} break;
		case 2:
		{
			return pGridSCH2;
		} break;
		default:
			break;
	}

	return NULL; 
	*/
}


void CPageSensorAlarm::OnComboBoxChn()
{
	int nChn = pComboChnPage2->GetCurSel();

	for (int i=0; i<7; i++)
	{
		pSCHGridPage2[i]->SetMask(&sch[nChn][i]);
	}	
	
}

void CPageSensorAlarm::OnClkCopy()
{
	printf("OnClkCopy \n");
	
	uint schIns[7]={0};
	
	uchar nCh = pComboChnPage2->GetCurSel();
	uchar nToCh = pComboCopy2ChnPage2->GetCurSel();

	GetSch(nCh, schIns);
	
	if(nToCh>=GetMaxSensorChnNum())
	{
		for(int j=0; j<GetMaxSensorChnNum(); j++)
		{
			if(j!=nCh)
			{
				for(int i=0; i<7; i++)
				{
					SetSch(j, i, &schIns[i]);
				}
			}
		}
	}
	else
	{
		for(int i=0; i<7; i++)
		{
			SetSch(nToCh, i, &schIns[i]);
		}
	}
}

void CPageSensorAlarm::SetInfo(char* szInfo)
{
	pIconInfo->Show(TRUE);

	pInfoBar->SetText(szInfo);

}

void CPageSensorAlarm::ClearInfo()
{
	pIconInfo->Show(FALSE);
	
	pInfoBar->SetText("");

}


