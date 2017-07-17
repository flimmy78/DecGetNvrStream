#ifndef _PAGE_ALARMDEAL_H_
#define _PAGE_ALARMDEAL_H_

#include "PageFrame.h"
#include "PagePtzTypes.h"

#define ALARMDEAL_SUBPAGES		3
#define ALARMDEAL_COMMBUTTON (ALARMDEAL_SUBPAGES+2)  //tabs + OK,exit

#define LINKAGE_COLS 3
#define LINKAGE_ROWS 8
#define TOURPATH_MAX	32

typedef enum 
{
	ALARMDEAL_NULL,
	ALARMDEAL_SENSOR,
	ALARMDEAL_MOTION,
	ALARMDEAL_VIDEOLOSS,
	ALARMDEAL_IPCEXTSENSOR,
	ALARMDEAL_485EXTSENSOR,
	ALARMDEAL_HDD,
}EMALARMDEAL_TYPE;


//SBizAlarmDispatch
typedef struct
{
	uchar nALaPtzType; //��̨��������EMBIZALARMPTZTYPE
	uchar nChn; //ͨ����
	uchar nId; //��ʾԤ�õ�id/Ѳ����·id/�켣(��Ч)
} SGuiAlarmPtz;

typedef struct
{
	//;;;�¼���������
	uchar nFlagBuzz; // ��������(����:0=��;1=��)
	uchar nZoomChn; // ���汨��ͨ����(255 ��ʾ��Ч)
	uchar nFlagEmail; // �ʼ�(0=��;1=��)
	uchar nSnapChn[64]; // ��Ŵ���ץͼ��ͨ���� (���ͬʱ����64��,255 ��ʾ��Ч)
	uchar nAlarmOut[64]; // �������� (���ͬʱ����64��,255 ��ʾ��Ч)
	uchar nRecordChn[64]; // ����¼��ͨ�� (���ͬʱ����64��,255 ��ʾ��Ч)
	SGuiAlarmPtz sAlarmPtz[64]; //���ͬʱ����64������
} SGuiAlarmDispatch;

typedef struct 
{
	uchar bEnable;
	//int  nAlarmType;
	//char szName[32];
	//int  nDelayTime;
} SHDDAlarmPara;


class CPageAlarmDeal:public CPageFrame
{
public:
	CPageAlarmDeal(VD_PCRECT pRect,VD_PCSTR psz,VD_BITMAP* icon = NULL,CPage * pParent = NULL, uchar nParentLabel=0);
	~CPageAlarmDeal();
	
	void SetAlarmDeal(EMALARMDEAL_TYPE type, int nChn);
	VD_BOOL MsgProc( uint msg, uint wpa, uint lpa );  //cw_scroll
	uchar nParentLabel; // 0 sensor,1 videolost,2 motiondetect, 3 IPCExtSensor, 4HDD
	VD_BOOL UpdateData(UDM mode);
	
private:
	//yaogang modify 20150324
	u8 nNVROrDecoder;//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
	void OnTriAllChn();
	void OnSnapAllChn();
	
	void SetPara();
	void GetPara();
	CPage *pParent;
	SHDDAlarmPara sHDDAlarmPara;
	SGuiAlarmDispatch sAlarmDispatchIns;
	
	SGuiPtzAdvancedPara *psGuiPtzPara;
	sGuiPtzAdvanPara *psGuiPtzCount;
	
	EMALARMDEAL_TYPE m_emDealType;
	int m_nDealChn;
	int m_nCurID;
	int nChMax;

	void SwitchPage( int subID);
	void ShowSubPage(int subID, BOOL bShow);
	void OnClickSubPage();

	CButton* pButton[ALARMDEAL_COMMBUTTON];
	CTableBox* pTabFrame;
	CStatic* pInfoBar;

	void SetInfo(char* szInfo);
	void ClearInfo();

	void InitPage0();	
	CTableBox* pTabPG0;
	//yaogang modify 20141010
	CCheckBox* pChkEable;
	CCheckBox* pChkSoundPG0;
	CComboBox* pComboBigVideoPG0;
	//CCheckBox* pChkTriggerAlarmPG0[1];
	CCheckBox* pChkTriggerAlarmPG0[4];
	CCheckBox* pChkMailPG0;
	CCheckBox* pChkSnapAllPG0;
	CCheckBox* pChkSnapChnPG0[4];	

	void InitPage1();
	CTableBox* pTabPG1;
	CCheckBox** pChkTriggerAllPG1;
	CCheckBox** pChkTriggerChnPG1;	

	void InitPage2();
	void OnTrackMove2();
	void OnComboTypeChange2();
	void OnComboIndexChange2();
	void AdjustLinkageRows();
	int m_page2;
	int m_maxPage2;
	int* type;
	int* id;
	CTableBox* pTabPG2;
	CStatic* pStaticIdPG2[LINKAGE_ROWS];
	CComboBox* pComboTypePG2[LINKAGE_ROWS];
	CComboBox* pComboIndexPG2[LINKAGE_ROWS];	
	CScrollBar* pScroll2;

	std::vector<CItem*>	items[ALARMDEAL_SUBPAGES];
};

#endif

