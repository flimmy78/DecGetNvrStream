#ifndef __PAGE_DESKTOP_H__
#define __PAGE_DESKTOP_H__


#include "GUI/Ctrls/Page.h"


//#include "System/AppEvent.h"
#include "PageFrame.h"


#include <vector>

//add by liu
#include "MultiTask/Timer.h"

typedef enum 
{
	EM_SPLIT_H,  //��
	EM_SPLIT_V,  //��
}EMSPLITLINETYPE;


typedef enum 
{
	EM_CHNSTATE_HIDE, 
	EM_CHNSTATE_1,	
	EM_CHNSTATE_2,			 
}EMCHNSTATESHOW;

typedef enum
{
	EM_STATE_RECORD, //¼��״̬
	EM_STATE_MD, //�ƶ����
	EM_STATE_ALARM, //����¼��
	EM_STATE_TIMER, //��ʱ¼��
	EM_STATE_MANUAL_REC, //�ֶ�¼��
	EM_STATE_MAX,
}EM_STATE_TYPE;

//add by liu
typedef enum
{
	EM_STATE_SNAP,	//ץͼͼ��
	//EM_STATE_LINK, //����״̬
	//EM_STATE_MKPHONE, //��˷�״̬
	//EM_STATE_SOUND, //����״̬	
	EM_ELSE_STATE_MAX,
}EM_ELSESTATE_TYPE;


typedef struct
{
	int x;
	int y;
	int x2;
	int y2;

} ChnRegion;

class CPageMainFrameWork;
class CPageSplitFrameWork;
class CPagePlayBackFrameWork;
class CPageStartFrameWork;
class CPageInputTray;
class CPageLogin;
class CPageAlarmList;

class CPageDesktop: public CPage
{
public:
	CPageDesktop(VD_PCRECT pRect,VD_PCSTR psz = NULL,VD_BITMAP* icon = NULL,CPage * pParent = NULL);
	~CPageDesktop();
	
	/*�¼�����*/
	//void onAppEvent(appEventCode code, int index, appEventAction action, EVENT_HANDLER *param, const CConfigTable* data = NULL);
	
	VD_BOOL UpdateData(UDM mode);                     //ҳ��ˢ��    
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);  //��Ϣ����

	static void SetLogin(int login);
	void SetCurPreviewMode(EMBIZPREVIEWMODE emMod);//��õ�ǰ��Ԥ��ģʽ	
	void SetCurPreModePage(u32 nPage);
	u32 ClikCurChn(EMBIZPREVIEWMODE emMod, u32 nPage, int x, int y);//���ص�ǰ�����ͨ��
	void SetPageFlag(BOOL bFlag);
	void SetIsDwellFlag(BOOL bFlag);

	int StartZoomVideoArea();
	int ChangZoomArea();
	int DrawZoomArea();
	void ExitFromZoomStatue();
	int ExitFromDrawStatue();
	
	void OnRec();
	
	//���˵��ص�����
#if 0//csp modify
	void OnMenu();
	void On1x1();
	void OnTriangle();
	void On2x2();
	void OnDwellNo();
	void OnZoom();
	void OnAudio();
	void OnPtz();
	void OnColor();
	void OnSnap();
	void OnPlay();
	void OnEdit();
#endif
	
#if 0//csp modify
	void OnLogoShow(int _spt,int _sub);
	void OnTvAjust();//TV��������
	void OnMatrix();/*!< ��Ƶ���� */
	void OnPtzCtrl();//��̨����
	void OnImgConf();//ͼ������
	void OnRecord();//�ֶ�¼�����
	void OnQuickSch();//�طŲ�ѯ
	void OnPlayback();
	void OnBackupOnline();//ʵʱ��¼
	void OnCtrlAlmIn();//��������
	void OnCtrlAlmOut();//�������
	
	void OnMain();//���˵�
	void OnLogout();//ע��
	void OnBackUp(); 
	void OnSystemSilence();//ϵͳ����added by wyf on 090922
	void OnShutDown();
	//void OnShutDown(uint param);
	void OnLocksys();
	
	void OnMenuSplit();
	void OnSetSpt();//ͨ���л�
	
	//�Ӳ˵��ص�����
	void OnSetSpt1Chan();//�л�������
	void OnSetSpt2Chan();//�л�2����
	void OnSetSpt4Chan();
	void OnSetSpt6Chan(void);//�л�6����
	void OnSetSpt8Chan();
	void OnSetSpt9Chan();
	void OnSetPIPChan();
	
#ifdef FUNC_QUICK_PLAY
	void OnQuickPlay(int ch, int RecType, int BackTime);
	void OnQuickPlayDefault();
	void GetQuickPlayMenuCurSel();
#endif
	
	//��ʱˢ������ص�����
	void OnDelay(uint arg);//����ҳ����ʱ����
	void runLButtonDown(uint lpa);//add langzi 2009-11-9
	void OnTimer(uint arg);//��ʱˢ�±����ʱ��ص�����
	//add by yanjun for config data updata
	void UpdataDisplayConfig();
	void OnSwitchVideoOutput(uint param);
	void OnConfigChanged();
	void OnSplitChange(VD_BOOL post);
	void OnTourStateChanged(VD_BOOL started, VD_BOOL locked);
#endif
	
	//���ýṹ����ʱ�Ļص�����
	//void OnConfigMonitorTour(CConfigMonitorTour& config, int& ret);
	//void OnConfigGUISet(CConfigGUISet& config, int& ret);
	//void OnConfigTVAdjust(CConfigTVAdjust& config, int& ret);
	//void OnConfigChannelTitle(CConfigChannelTitle& config, int& ret);
	
private:
	void Draw();
	VD_BOOL AdjustRect();
	
#if 0//csp modify
	void UpdateConfig();
	void SetChannelTitle(VD_BOOL bFlag);
	void SetChannelLogo(VD_BOOL bFlag, VD_BOOL bForced = FALSE);
	void SetChannelState(VD_BOOL recordStatus, VD_BOOL alarmStatus, VD_BOOL bForced = FALSE);
	void SetTimeTitle(VD_BOOL bFlag, VD_BOOL bForced = FALSE);
	//bForced: TRUE ʱ��Ļ���ܷ��������ţ���Ҫ���µ�������
	void SetBitRateInfo(VD_BOOL bFlag, VD_BOOL bForced = FALSE);
	void SetCardInfo(VD_BOOL value); 
	void SetNextSplit();
	VD_BOOL SetSplitSub(int *spt, int *sub);
	void SetSilenceMenu();//�����Ҽ��˵�����Ϊ"����"����added by wyf on 090925
#endif
	
private:
	//yaogang modify 20150324
	u8 nNVROrDecoder;	//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������

	CMenu *pMainMenu;
	EMBIZPREVIEWMODE emCurPreviewMode;//��ǰ�Ĵ���Ԥ��ģʽ
	EMBIZPREVIEWMODE eMaxViewMode;//��ǰ���õ����ͨ����
	EMBIZPREVIEWMODE emLastPreMode;//��һ�ε�Ԥ��ģʽ
	u32 nPreviewPage;
	u32 RowNum;//��ǰģʽ���ж�����
	u32 ColNum;//��ǰģʽ���ж�����
	int nScreanWidth;
	int nScreanHeight;
	int maxChn;
	int px;
	int py;
	CStatic *pAlarmIcon;
	
	CMenu*  pSubMenu[N_SPLIT];
	CMenu*	pPOPMenu; 
	BOOL	bThisPageFlag;
	BOOL	IsDwell;

	BOOL 	m_IsPbElecZoomDrawStatus;
	BOOL 	m_IsElecZoomDrawStatus;
	BOOL 	IsDrawElecZoomArea;
	BOOL 	IsElecZoomBegin;
	BOOL	IsElecZoomStatue;
	BOOL	IsLbuttonDown;
	int 	mouse_eleczoom_x;
	int 	mouse_eleczoom_y;
	int 	start_x;
	int 	start_y;
	int 	end_x;
	int 	end_y;
	int 	zoom_w;
	int 	zoom_h;
	int 	iszoom_x1;
	int 	iszoom_y1;
	int 	iszoom_x2;
	int 	iszoom_y2;
	int 	Is3520;
	int 	Is9616S;
	int 	m_CurChn;
	
	int		CurScreenHeight;//���ӷŴ��׼�ߴ� ��
	int		CurScreenWidth;//���ӷŴ��׼�ߴ�  ��
	
	SBizPreviewElecZoom stCapRect;
	SBizPreviewElecZoom stPbCapRect;
	SBizSystemPara sysPara;
	
	int		mouse_spt_x;
	int		mouse_spt_y;

	//add by liu
	CTimer *m_DeskStreamTimer;

//	CTimer	m_PageMainTimer;//����ҳ�涨ʱ��
	
// 	CTimer	m_UpdateTimer;//ʱ������ͨ������ˢ�¶�ʱ��
// 	CTimer	m_ShutTimer;	//�ػ�ȷ�϶�ʱ��
// 	CTimer	m_SwitchTimer;//�л���Ƶ������Ͷ�ʱ�� 
// 	int		m_ShutCountDown;
	
//	CDevSplit *m_pDevSplit;
// 	CDisplay *m_pDisplay;
// 	CConfigMonitorTour m_ConfigMonitorTour;
// 	CConfigGUISet m_ConfigGUISet;
// 	CConfigTVAdjust m_ConfigTVAdjust;
// 	CConfigChannelTitle m_ConfigChannelTitle;

//	CPageCountDown *pPageCountDown;

//	VD_BITMAP * m_pBMPChannelStates[ALLCHSTATES];

//	CDevVideo *m_pDevVideo;

// 	CStatic* m_pBpsChannel[2];
// 	CStatic* m_pBpsUnit[2];
// 	CStatic* m_ChannelNo[8];
// 	CStatic* m_pStaticShowBps[8];   
// 	int  m_count;  //��ʾ����ʱ����������ˢ�´���

// 	//����������ʾATM���ŵ��ӵ����
// 	CStatic* m_pStaticCardName[N_DESKTOP_INFO_LINE];  //Ҫ���ӵ���Ŀ����,�罻�׽��
// 	CStatic* m_pStaticCardContent[N_DESKTOP_INFO_LINE]; //ÿһ���Ӧ�ľ�������,����彻�׽��Ĵ�С.
// 
// 	char m_CardName[N_DESKTOP_INFO_LINE * N_SYS_CH][DESKTOP_INFO_MAX_LEN]; //��ŵ��ӵ���Ŀ����,ÿͨ����Ӧ8��, ��1ͨ����Ӧm_CardName[0][32]-m_CardName[7][32]
// 	char m_CardContent[N_DESKTOP_INFO_LINE * N_SYS_CH][DESKTOP_INFO_MAX_LEN];
// 
// 	uint m_ShowCardInfo[N_SYS_CH];		/*!< ��ʾ��ǰ�ǲ������ڵ��ӱ�����Ϣ����Ļ�� */

// 	CButton* m_pButtonTour;
// 	VD_RECT	m_rectTour;// ��Ѳ��ť��������
// 	VD_BITMAP * bmp_tour_enable;
// 	VD_BITMAP * bmp_tour_disable;
// 	CMutex	m_MutexState;//����ҳ��״̬

// 	VD_BITMAP * bmp_logo_middle;
// 	VD_BITMAP * bmp_logo_blank;//���������LOGOͼƬadd by nike.xie 09/5/21
 	
// 	VD_COLORREF m_bkColor;
	
	VD_COLORREF m_keyColor;
	VD_RECT m_RectScreen;
	
	CPageInputTray* m_pPageInputTray;
	CPageStartFrameWork* m_pPageStart;

	CPageAlarmList *m_alarmList;
	
	static int isLogin;
	CPageLogin* m_pPageLogin;
 	CPageMainFrameWork* m_pPageMain;
	CPageSplitFrameWork* m_pPageSplit;
//	CPagePlayBackFrameWork* m_pPagePlayBack;//csp modify 20130509
	
// 	CPageFunction* m_pPageFunction;
// 	CPageInputTray* m_pPageInputTray;
	
// 	SYSTEM_TIME m_OldSt;
// 	int m_iTimeFmt;
// 	int m_bTitleFlag;
// 	VD_BITMAP * bmp_systemsilence[2];   //����ָ��  add langzi 2009-11-9
// 	VD_BITMAP * bmp_systemnosilence[2];  //�Ǿ���ָ��  add langzi 2009-11-9	
	
public:
	std::vector<CStatic*> m_vSplitLine;
	std::vector<CStatic*> m_vElecZoomSplitLine;
	std::vector<CStatic*> m_vElecZoomAreaSplitLine;
	std::vector<CStatic*> m_vCoverRect;
	//yaogang modify 20151202
	std::vector<CStatic*> m_vFrameSplitLine;
	int m_maxCH;
	int m_maxRect;
	int* m_curCover;

	BOOL m_bAlarmListStatus;
	void ToggleAlarmIcon(bool isVisible);
	
	//csp modify
	//void InitSplitLine(int maxlines = 16, VD_COLORREF color = VD_RGB(255,255,255));
	void InitSplitLine(int maxlines = 16, VD_COLORREF color = VD_RGB(232,232,232));
	
	void SetSplitLineRect(int index, int x, int y, int length, EMSPLITLINETYPE type);
	void ShowSplitLine(int index, BOOL bShow);
	void SetCurVideoSize();//�ı���ӷŴ��׼�ߴ�
	//yaogang modify 20151202
	void ShowFrameSplitLine(BOOL bShow);
	
	std::vector<CStatic*>m_vChannelName;
	void InitChnName(int maxName = 4);
	void SetChnName(int index, VD_PCRECT pRect, VD_PCSTR psz);
	void EditChnName( int index,  VD_PCSTR psz );
	void EditChnWidth( int index ,int width);
	void ShowChnName(int index, BOOL bShow);

	/*add by liu*/
	std::vector<CStatic*>m_vChannelKbps;
	void InitChnKbps(int maxName = 16);
	void SetChnKbps(int index, VD_PCRECT pRect, VD_PCSTR psz);
	void ShowChnKbps(int index, BOOL bShow);
	std::vector<CStatic*>m_vChnElseState;
	int m_nMaxElseStatetype;


	std::vector<CStatic*>m_vRegion;
	void InitRegion(int maxName);
	void ShowRegion( int index, BOOL bShow );
	ChnRegion nChnRegion[16];
	void SetRegion( int index, VD_PCRECT pRect, VD_PCSTR psz );
	void ShowRegionALl(int hua);
	void DeleteRegion(int index);
	
	std::vector<CStatic*>m_vTimeTile;
	void InitTimeTitle(int max = 1);
	//void SetTimeTitle(int index, VD_PCRECT pRect, VD_PCSTR psz);
	void SetTimeTitleRect(int index, VD_PCRECT pRect);
	void SetTimeTitleString(int index, VD_PCSTR psz);
	void ShowTimeTitle(int index, BOOL bShow);
	
	std::vector<CStatic*>m_vChnState;
	int m_nMaxStatetype;
	
	void SetPbElecZoomstatus(BOOL flag);
	void SetElecZoomChn(int nChn);
	void SetAllElecZoomStatueEmpty();
	int GetElecZoomChn();
	
	std::vector<CStatic*>m_vElecZoomTile;
	void InitElecZoomTile(int max);
	void SetElecZoomstatus(BOOL flag);
	void SetElecZoomRect( int index, VD_PCRECT pRect);
	void SetElecZoomTileString( int index, VD_PCSTR psz );
	void ShowElecZoomTile(int index, BOOL bShow);
	
	std::vector<CStatic*>m_vElecZoomInvalidTile;
	void InitElecZoomInvalidTile(int max);
	void SetElecZoomInvalidRect( int index, VD_PCRECT pRect);
	void SetElecZoomInvalidTileString( int index, VD_PCSTR psz );
	void ShowElecZoomInvalidTile(int index, BOOL bShow);
	
	void InitElecZoomSplitLine(int maxlines = 16, VD_COLORREF color = VD_RGB(255,255,255));
	void SetElecZoomSplitLineRect(int index, int x, int y, int length, EMSPLITLINETYPE type);
	void ShowElecZoomSplitLine(int index, BOOL bShow);
	
	void InitElecZoomAreaSplitLine(int maxlines = 16, VD_COLORREF color = VD_RGB(255,255,255));
	void SetElecZoomAreaSplitLineRect(int index, int x, int y, int length, EMSPLITLINETYPE type);
	void ShowElecZoomAreaSplitLine(int index, BOOL bShow);
	
	void InitChnState(int maxChn = 4, int maxstatetype = 5);
	void SetChnStateBMP(int nChn, int nStatetype, VD_PCSTR pszBmpName1, VD_PCSTR pszBmpName2 );
	void ShowChnState(int nChn, int nStatetype, int x, int y, EMCHNSTATESHOW showtype);
	void ClearAllChnState();

	//add by liu
	VD_BOOL ChnElseStateIsShow(int nChn, int nStatetype);
	void InitChnElseState(int maxChn = 16, int maxstatetype = 3);
	void ShowChnElseState( int nChn, int nStatetype, int x, int y, EMCHNSTATESHOW showtype );
	void SetChnElseStateBMP(int nChn, int nStatetype, VD_PCSTR pszBmpName1, VD_PCSTR pszBmpName2 );

	CStatic *pStaticVerifySNResult;//csp modify
	
	void HideAllOsdWithoutTime(int nChnNum);
	void HideDeskOsdWithoutTime(int nMaxChn);
	
	void InitCoverRect(int maxCh, int maxRect, VD_COLORREF color = VD_RGB(120,120,120));
	void SetCoverRect(int ch, int x, int y, int w, int h);
	void ClearCoverRect(int ch);
	
	void ShowCoverRect();
	void HideCoverRect();
	
	void SetModePlaying();
	void SetModePreviewing();
	
	//add by liu
	int ShowDeskStreamInfo(uint param);
	
	u8 m_FlagPlaying;
	u8 m_nPlayBackFileFormat;
	u8* m_AllChnStatus;
};

#if 0
class CPageCountDown : public CPageFrame
{
public:
	CPageCountDown(VD_PCRECT pRect,VD_PCSTR psz = NULL,VD_BITMAP* icon = NULL,CPage * pParent = NULL);
	~CPageCountDown();
	VD_BOOL UpdateData(UDM mode);
	void SetProgress(uint pos);
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);

private:
	CProgressCtrl*	pProgress;
};
#endif

#endif //__PAGE_DESKTOP_H__

