#include "GUI/Pages/PageMainFrameWork.h"
#include "GUI/Pages/PageConfigFrameWork.h"
#include "GUI/Pages/PageManualAlarm.h"
#include "GUI/Pages/PageSearch.h"

#include "GUI/Pages/PagePtzTrack.h"
#include "GUI/Pages/PageDataBackup.h"
#include "GUI/Pages/PagePtzPreset.h"
#include "GUI/Pages/PageInfoFrameWork.h"
#include "GUI/Pages/PageDiskMgr.h"
#include "GUI/Pages/PageUpdate.h"
#include "GUI/Pages/PageMessageBox.h"
#include "GUI/Pages/PageMaskSet.h"
#include "GUI/Pages/PageDesktop.h"
#include "GUI/Pages/PageInfoBox.h"
#include "GUI/Pages/PageTest.h"
#include "GUI/Pages/BizData.h"
#include "biz.h"

//#include "GUI/Pages/PageCfgRecParam.h"

//static MAINFRAME_ITEM	items[MAX_MAIN_ITEM];

enum{//�����������һһ��Ӧ
	CONFIG_BUTTON_CONFIG,
	CONFIG_BUTTON_SEARCH,
	CONFIG_BUTTON_BACKUP,
	CONFIG_BUTTON_INFO,	
	//CONFIG_BUTTON_ALARM, ��ʱ�ر��ֶ�����
	CONFIG_BUTTON_DISK,
	CONFIG_BUTTON_RESOURCE,
	CONFIG_BUTTON_LOGOFF,
	CONFIG_BUTTON_SHUTDOWN,
	CONFIG_BUTTON_RESTART,
	//CONFIG_BUTTON_EXIT,
	CONFIG_BUTTON_NUM,
};

char* mainShortcut[CONFIG_BUTTON_NUM] = {
	"&CfgPtn.ConfigMgmt",//"Config",
	//yaogang modify 20141106
	//"&CfgPtn.DataSearch",	
	"&CfgPtn.VideoPlayback",
	"&CfgPtn.DataBackup",
	"&CfgPtn.Information",
	//"&CfgPtn.ManualAlarm", ��ʱ�ر��ֶ�����
	"&CfgPtn.DiskManagement",
	"&CfgPtn.UPGRADE",
	"&CfgPtn.Logoff",
	"&CfgPtn.ShutDown",
	"&CfgPtn.RestartSystem",
	//"&CfgPtn.Exit",
};

char* mainShortcutBmpName[CONFIG_BUTTON_NUM+1][2] = {    
	{DATA_DIR"/temp/config.bmp",	DATA_DIR"/temp/config_f.bmp"},
	{DATA_DIR"/temp/search.bmp",	DATA_DIR"/temp/search_f.bmp"},	
	{DATA_DIR"/temp/backup.bmp",	DATA_DIR"/temp/backup_f.bmp"},
	{DATA_DIR"/temp/info.bmp",		DATA_DIR"/temp/info_f.bmp"},
	//{DATA_DIR"/temp/info.bmp",		DATA_DIR"/temp/info_f.bmp"},
	//{DATA_DIR"/temp/alarm.bmp",	DATA_DIR"/temp/alarm_f.bmp"},��ʱ�ر��ֶ�����
	//{DATA_DIR"/temp/alarm.bmp",	DATA_DIR"/temp/alarm_f.bmp"},
	{DATA_DIR"/temp/disk.bmp",		DATA_DIR"/temp/disk_f.bmp"},
	{DATA_DIR"/temp/resources.bmp",	DATA_DIR"/temp/resources_f.bmp"},
	{DATA_DIR"/temp/logoff.bmp",	DATA_DIR"/temp/logoff_f.bmp"},
	{DATA_DIR"/temp/shutdown.bmp",	DATA_DIR"/temp/shutdown_f.bmp"},
	{DATA_DIR"/temp/restart.bmp",	DATA_DIR"/temp/restart_f.bmp"},
	//{DATA_DIR"/temp/exit3.bmp",	DATA_DIR"/temp/exit3_f.bmp"},
	{DATA_DIR"/temp/main_pic.bmp",	DATA_DIR"/temp/main_pic.bmp"}, //����һ��Ҫ���������һ��
};

//csp modify
//static VD_BITMAP* pBmpButtonNormal[CONFIG_BUTTON_NUM];
static VD_BITMAP* pBmpButtonNormal[CONFIG_BUTTON_NUM+1];

//csp modify
//static VD_BITMAP* pBmpButtonSelect[CONFIG_BUTTON_NUM];
static VD_BITMAP* pBmpButtonSelect[CONFIG_BUTTON_NUM+1];

CPageMainFrameWork::CPageMainFrameWork( VD_PCRECT pRect,VD_PCSTR psz,VD_BITMAP* icon /*= NULL*/,CPage * pParent /*= NULL*/ ):
	CPageFrame(pRect, psz, icon, pParent), curMainItemSel(0)
{
	#define LEFT_POS	(52)
	
	//csp modify
	//#define SPACE		(78+3)
	#define SPACE		(78)
	
	int rd = 10;
	int space = SPACE;
	CRect rtTmp;
	rtTmp.left = LEFT_POS;

	//printf("%s this: %p\n", __func__, this);
	
	#if 1//csp modify
	rtTmp.top =  20;
	#else
	rtTmp.top =  22;
	#endif

	SBizDvrInfo DvrInfo;
	u8 nNVROrDecoder = 1; //Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
	if (BizGetDvrInfo(&DvrInfo) == 0)
	{
		nNVROrDecoder = DvrInfo.nNVROrDecoder;
	}
	
	int i, cnt, line, col, left, top;
	cnt = 0;
	for(i=0; i<CONFIG_BUTTON_NUM; i++)
	{
		pButton[i] = NULL;
		pStatic[i] = NULL;
		//printf("%s 1 i: %d, cnt: %d\n", __func__, i, cnt);
		if (nNVROrDecoder != 1)
		{
			if ( i == CONFIG_BUTTON_SEARCH \
				|| i == CONFIG_BUTTON_BACKUP \
				|| i == CONFIG_BUTTON_DISK )
			{
				continue;
			}
		}

		line = cnt /4;
		col = cnt % 4;
		left = rtTmp.left + (72+space) * col;
		top = rtTmp.top + (40+space) * line;

		pBmpButtonNormal[i] = VD_LoadBitmap(mainShortcutBmpName[i][0]);
		pBmpButtonSelect[i] = VD_LoadBitmap(mainShortcutBmpName[i][1]);
		pButton[i] = CreateButton(CRect(left, top, 
						left+pBmpButtonNormal[i]->width, top+pBmpButtonNormal[i]->height), 
						this, NULL, (CTRLPROC)&CPageMainFrameWork::OnClkMainItem, NULL, buttonNormalBmp);
		pButton[i]->SetBitmap(pBmpButtonNormal[i], pBmpButtonSelect[i], pBmpButtonSelect[i]);
		
		pStatic[i] = CreateStatic(CRect(left-10-30, top+72, 
						left+pBmpButtonNormal[i]->width+10+30, top+102), this, mainShortcut[i]);
		pStatic[i]->SetTextAlign(VD_TA_CENTER);

		cnt++;
	}
	
//yaogang modify 20141106 ɾ����ʾH.264 DVRͼƬ
#if 0//csp modify
	//Modify by Lirl on Feb/22/2012,ɾ����ʾH.264 DVRͼƬ
	//Modify by Lirl on Jan/12/2012
	//�����ʾ���½ǵ�ͼƬ,ͼƬռ�������ؼ���ռ�õĿռ�
	//ͼƬ��С:195*50//194*34
	rtTmp.top = 310;
	rtTmp.top += 12;//csp modify
	rtTmp.left = 205+69+space+pBmpButtonNormal[CONFIG_BUTTON_NUM-1]->width;
	rtTmp.left += 4;//csp modify
	rtTmp.right = rtTmp.left + pBmpButtonNormal[CONFIG_BUTTON_NUM]->width;
	rtTmp.bottom = rtTmp.top + pBmpButtonNormal[CONFIG_BUTTON_NUM]->height;
	printf("CPageMainFrameWork H264DVR pos:(%d,%d,%d,%d),parent pos:(%d,%d,%d,%d)\n",
		rtTmp.left,rtTmp.right,rtTmp.top,rtTmp.bottom,
		m_Rect.left,m_Rect.right,m_Rect.top,m_Rect.bottom);
	pStaticPic = CreateStatic(&rtTmp, this, "");
	//pStaticPic->SetBkColor(VD_RGB(67,77,87));
	pStaticPic->SetBitmap(VD_LoadBitmap(mainShortcutBmpName[CONFIG_BUTTON_NUM][0]));
#endif
	
	//printf("%s 1 this: %p\n", __func__, this);
	m_pPageConfig = new CPageConfigFrameWork(NULL, "&CfgPtn.SETUP", icon_dvr, this);

	//yaogang modify 20141106
	//printf("%s 2 this: %p\n", __func__, this);
	//m_pPageSearch = new CPageSearch(CRect(0, 0, 642, 418), "&CfgPtn.DataSearch", icon_dvr, this);
	//m_pPageSearch = new CPageSearch(CRect(0, 0, 642, 418), "&CfgPtn.VideoPlayback", icon_dvr, this);
	
	//printf("%s 3\n", __func__);
	fflush(stdout);
	m_pPageManualAlarm = new CPageManualAlarm(NULL, "&CfgPtn.ManualAlarm", icon_dvr, NULL);
	
	//printf("%s 4\n", __func__);
	fflush(stdout);
	//m_pPageDataBackup = new CPageDataBackup(NULL, "&CfgPtn.Backup", icon_dvr, this);
	
	//printf("%s 5\n", __func__);
	fflush(stdout);
	m_pPageInfo = new CPageInfoFrameWork(NULL, "&CfgPtn.Information", icon_dvr, this);
	
	//printf("%s 6\n", __func__);
	fflush(stdout);
	m_pPageDskMgr = new CPageDiskMgr(NULL, "&CfgPtn.DiskManagement", icon_dvr, this);
	SetPage(EM_PAGE_DISKMGR, m_pPageDskMgr);
	
	//printf("%s 7\n", __func__);
	fflush(stdout);
	m_pPageUpdate = new CPageUpdate(NULL, "&CfgPtn.UPGRADE", icon_dvr, this);
	SetPage(EM_PAGE_UPDATE,m_pPageUpdate);
	
	//m_pPageTest = new CPageTest(NULL, " Test", icon_dvr, this);
	
	printf("CPageMainFrameWork over\n");
}

CPageMainFrameWork::~CPageMainFrameWork()
{
	
}

VD_PCSTR CPageMainFrameWork::GetDefualtTitleCenter()
{
	return "&CfgPtn.Basic";
}

void CPageMainFrameWork::OnClkMainItem()
{
	//printf("OnClkMenuItem\n");
	
	u32 g_nUserId = GetGUILoginedUserID();
	u32 userid = g_nUserId;
	int i = 0;
	BOOL bFind = FALSE;
	CButton *pFocusButton = (CButton *)GetFocusItem();
	for(i = 0; i < CONFIG_BUTTON_NUM; i++)
	{
		if(pFocusButton == pButton[i])
		{
			bFind = TRUE;
			break;
		}
	}
	if(bFind)
	{
		switch(i)
		{
			case CONFIG_BUTTON_CONFIG:
			{
				EMBIZUSERCHECKAUTHOR author = BizUserCheckAuthority(EM_BIZ_USER_AUTHOR_SYSTEM_SETUP,NULL,g_nUserId);
				if(author == EM_BIZ_USER_AUTHOR_NO)
				{
					MessageBox("&CfgPtn.NoAuthority", "&CfgPtn.WARNING", MB_OK|MB_ICONWARNING);
				}
				else //if(author == EM_BIZ_USER_AUTHOR_YES)
				{
					//printf("CONFIG_BUTTON_CONFIG-1^^^^^^^%s,%d\n",__func__,__LINE__);
					
					//csp modify 20130427
					if(m_pPageConfig->GetFlag(IF_OPENED))
					{
						printf("rz*****pageconfig open error\n");
						m_pPageConfig->Close(UDM_EMPTY);
					}
					
					m_pPageConfig->Open();
					
					//printf("CONFIG_BUTTON_CONFIG-2^^^^^^^%s,%d\n",__func__,__LINE__);
				}
			}break;
			case CONFIG_BUTTON_SEARCH:
			{
				//EMBIZUSERCHECKAUTHOR author = BizUserCheckAuthority(EM_BIZ_USER_AUTHOR_DATA_MANAGE,NULL,g_nUserId);
				if(1)//(author == EM_BIZ_USER_AUTHOR_YES)
				{
					SetSystemLockStatus(1);
					m_pPageSearch->FromMainpage();
					m_pPageSearch->UpdateData(UDM_EMPTY);
					m_pPageSearch->Open();
					SetSystemLockStatus(0);
					BizGUiWriteLog(BIZ_LOG_MASTER_SEARCH, BIZ_LOG_SLAVE_SEARCH_DATA_BYTIME);	
				}
				else //if(author == EM_BIZ_USER_AUTHOR_NO)
				{
					MessageBox("&CfgPtn.NoAuthority", "&CfgPtn.WARNING"  , MB_OK|MB_ICONWARNING);
				}
			}break;
			case CONFIG_BUTTON_BACKUP:
			{
				EMBIZUSERCHECKAUTHOR author = BizUserCheckAuthority(EM_BIZ_USER_AUTHOR_DATA_MANAGE,NULL,g_nUserId);
				if(author == EM_BIZ_USER_AUTHOR_NO)
				{
					MessageBox("&CfgPtn.NoAuthority", "&CfgPtn.WARNING"  , MB_OK|MB_ICONWARNING);
				}
				else //if(author == EM_BIZ_USER_AUTHOR_YES)
				{
					m_pPageDataBackup->Open();
				}
			}
			break;
			case CONFIG_BUTTON_INFO:
			{
				//VD_RECT rtTmp;
				//m_pPageInfo->pStaticPic->GetRect(&rtTmp);
				//printf("CONFIG_BUTTON_INFO-1 H264DVR pos:(%d,%d,%d,%d)\n",
				//	rtTmp.left,rtTmp.right,rtTmp.top,rtTmp.bottom);
				
				m_pPageInfo->Open();
				
				//m_pPageInfo->pStaticPic->GetRect(&rtTmp);
				//printf("CONFIG_BUTTON_INFO-2 H264DVR pos:(%d,%d,%d,%d)\n",
				//	rtTmp.left,rtTmp.right,rtTmp.top,rtTmp.bottom);
				
				BizGUiWriteLog(BIZ_LOG_MASTER_CHECKINFO, 0);	
			}
			break;
			#if 0 // ��ʱ�ر��ֶ�����
			case CONFIG_BUTTON_ALARM:
			{
				//printf("CONFIG_BUTTON_ALARM\n");
				m_pPageManualAlarm->Open();
				
			}break;
			#endif
			case CONFIG_BUTTON_DISK:
			{
				EMBIZUSERCHECKAUTHOR author = BizUserCheckAuthority(EM_BIZ_USER_AUTHOR_DISK_MANAGE,NULL,g_nUserId);
				if(author == EM_BIZ_USER_AUTHOR_YES)
				{
					if(BizSysGetCheckDiskStatue())//��ȡ�����ϴ����̼��״̬
					{
						//usleep(500 * 1000);
						return;
					}
					
					if(BizSysGetInsertDiskStatue())//��ȡ�´��̲���״̬
					{
						//usleep(500 * 1000);
						return;
					}
					
					m_pPageDskMgr->Open();
				}
				else if(author == EM_BIZ_USER_AUTHOR_NO)
				{
					MessageBox("&CfgPtn.NoAuthority", "&CfgPtn.WARNING"  , MB_OK|MB_ICONWARNING);
				}
			}
			break;
			case CONFIG_BUTTON_RESOURCE:
			{
				EMBIZUSERCHECKAUTHOR author = BizUserCheckAuthority(EM_BIZ_USER_AUTHOR_SYSTEM_UPDATE,NULL,g_nUserId);
				if(author == EM_BIZ_USER_AUTHOR_YES)
				{
					m_pPageUpdate->Open();
				}
				else if(author == EM_BIZ_USER_AUTHOR_NO)
				{
					MessageBox("&CfgPtn.NoAuthority", "&CfgPtn.WARNING"  , MB_OK|MB_ICONWARNING);
				}
			}
			break;
			case CONFIG_BUTTON_LOGOFF://ע��
			{
				//printf("LogOut User[%s]/ID[%d]\n", GetGUILoginedUserName(), GetGUILoginedUserID());
				if(0 == BizUserLogout(g_nUserId))
				{
					CPageDesktop::SetLogin(0);
					SetGUIUserIsLogined(0);
					
					Close();
					//printf("LogOut OK......\n");
					
					BizGUiWriteLog(BIZ_LOG_MASTER_SYSCONTROL, BIZ_LOG_SLAVE_LOGOFF);
					
					//BizSysComplexExit( EM_BIZSYSEXIT_LOCK );
				}
			}
			break;
			case CONFIG_BUTTON_SHUTDOWN:
			{
				EMBIZUSERCHECKAUTHOR author = BizUserCheckAuthority(EM_BIZ_USER_AUTHOR_SHUTDOWN,NULL,g_nUserId);
				if(author == EM_BIZ_USER_AUTHOR_YES)
				{
					const char* szInfo = GetParsedString("&CfgPtn.ShutDownNote");
					const char* szType = GetParsedString("&CfgPtn.WARNING");
					UDM ret= MessageBox(szInfo, szType , MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2);

					if(GetRunStatue() == UDM_OK)
					{
						SetSystemLockStatus(1);//cw_lock						
						//MessageBox("&CfgPtn.SaveSetup", "&CfgPtn.Info"  , MB_ICONINFORMATION);

						//BizSysComplexExit(EM_BIZSYSEXIT_POWEROFF);
						//BizSysComplexExit(EM_BIZSYSEXIT_POWEROFF);  //cw_shutdown
						if(FxnKeycodeShut()<0)
						{
							SetSystemLockStatus(0);//cw_lock
						}
					}
                    else
                    {
					    ReflushItemName();
                        return;
                    }
				}
				else if(author == EM_BIZ_USER_AUTHOR_NO)
				{
					MessageBox("&CfgPtn.NoAuthority", "&CfgPtn.WARNING"  , MB_OK|MB_ICONWARNING);
				}
			}
			break;
			case CONFIG_BUTTON_RESTART:
            {
				EMBIZUSERCHECKAUTHOR author = BizUserCheckAuthority(EM_BIZ_USER_AUTHOR_SHUTDOWN,NULL,g_nUserId);
				if(author == EM_BIZ_USER_AUTHOR_YES)
				{
					const char* szInfo = GetParsedString("&CfgPtn.IsRestartSystem");
					const char* szType = GetParsedString("&CfgPtn.WARNING");
					UDM ret = MessageBox(szInfo, szType , MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2);
					
					if(GetRunStatue() == UDM_OK)
					{
						SetSystemLockStatus(1);
						RestartSystem();
					}
					else
					{
					    ReflushItemName();
                        return;
                    }
				}
				else if(author == EM_BIZ_USER_AUTHOR_NO)
				{
					MessageBox("&CfgPtn.NoAuthority", "&CfgPtn.WARNING"  , MB_OK|MB_ICONWARNING);
				}
			}
            break;
			/*
			case CONFIG_BUTTON_EXIT:
			{
				this->Close();
			}
			break;
			*/
			default:
			break;
		}	
	}
}

//add by Lirl on Nov/18/2011,ÿ�λص���ҳ��ʱ����ˢ����������Ŀ���ƣ���ֹ����
void CPageMainFrameWork::ReflushItemName()
{
    int staticIndex = 0;
    for (int i=0; i<3; i++)
	{
		for(int j=0; j<4; j++)
		{
			staticIndex = (i*4+j);
			//���˵�����8����,������Ŀʱ���ֶ�����������,��ֹ��ȫ��ˢ��
			if (staticIndex >= 9) 
			//if (staticIndex >= subItemNum)  //yaogang modify 20150324			
			{
				return;
			}
			//printf("%s staticIndex: %d\n", __func__, staticIndex);
			if (pStatic[staticIndex])
				pStatic[staticIndex]->SetText(pStatic[staticIndex]->GetText());
			//printf("%s staticIndex 1: %d\n", __func__, staticIndex);
		}
	}
}
//end

VD_BOOL CPageMainFrameWork::MsgProc( uint msg, uint wpa, uint lpa )
{
	u8 lock_flag = 0;  //cw_shutdown
	GetSystemLockStatus(&lock_flag);
	if(lock_flag)
	{
		return FALSE;
	}
	
	if(msg == XM_RBUTTONDOWN) 
	{
		//printf(" XM_RBUTTONDOWN cw^^^^^^^%s,%d\n",__func__,__LINE__);//cw_test
		this->Close();
		//return TRUE;
	}
	
    if(msg == XM_RBUTTONUP)
    {
    	//printf(" XM_RBUTTONUP cw^^^^^^^%s,%d\n",__func__,__LINE__);//cw_test
        return TRUE;
    }
	
	return CPage::MsgProc(msg, wpa, lpa);
}

VD_BOOL CPageMainFrameWork::UpdateData(UDM mode)
{
	//printf("CPageMainFrameWork %s 1\n", __func__);
    if(UDM_OPEN == mode) 
    {
    	//printf("MainFrame cw^^^^^^^%s,%d\n",__func__,__LINE__);//cw_test
    }
	
	if(UDM_OPEN == mode || UDM_OPENED == mode) 
    {
		//printf("CPageMainFrameWork %s 2\n", __func__);
        ReflushItemName();
    }
	
	static CPage** page = NULL;
	if(!page)
	{
		page = GetPage();
	}
	//printf("CPageMainFrameWork %s 3\n", __func__);
	if(mode == UDM_CLOSED)
	{
		for(int i=0; i<GetVideoMainNum(); i++)
		{
			(((CPageDesktop*)page[EM_PAGE_DESKTOP])->m_vChannelName[i])->SetText((((CPageDesktop*)page[EM_PAGE_DESKTOP])->m_vChannelName[i])->GetText());
		}
	}
	
	return TRUE;
}

