#ifndef _PAGE_BASICCFGFRAMEWORK_H_
#define _PAGE_BASICCFGFRAMEWORK_H_

#include "PageFrame.h"
//#include "PageChild.h"
#include <vector>

#define BASICCFG_SUBPAGES	2
#define BASICCFG_BTNNUM	    5

class CPageInfoBox;
class CPageBasicConfigFrameWork :public CPageFrame
{
public:
	CPageBasicConfigFrameWork(VD_PCRECT pRect,VD_PCSTR psz,VD_BITMAP* icon = NULL,CPage * pParent = NULL);
	~CPageBasicConfigFrameWork();

	VD_PCSTR GetDefualtTitleCenter();
	VD_BOOL UpdateData( UDM mode );
	VD_BOOL MsgProc( uint msg, uint wpa, uint lpa );
	VD_BOOL Open();
	int nThisPageOpened;
	int nEdittingTime;
	int nDateChanged;
	int nTimeChanged;
	void updatetime(int flag);
	void RecvNotifyClose();	//Ծ������CMS remote CTRL

protected:
	void WriteLogs();

private:
	int curID;
	u8 nNVROrDecoder;//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������

	int m_lastVFormat;
	int m_lastOutput;
	int m_lastLanguage;

	//void OnClkConfigItem();
	void SwitchPage( int subID);
	void ShowSubPage(int subID, BOOL bShow);

	void OnClickSubPage();
	//void OnClickCtlBtn();

	CButton* pButton[BASICCFG_BTNNUM+2];
	CTableBox* pTable;
	CStatic* pInfoBar;
	CStatic* pIconInfo;
	//CButton* pItemButton[3];

	void SetInfo(char* szInfo);
	void ClearInfo();
	
	//"ϵͳ"��ҳ��
	void InitPage0();
	void OnEditChange0();
	
	//csp modify
	void OnTrackMove0();
	
	//csp modify
	//CStatic* pStatic0[9];
	//CStatic* pStatic0[10];
	//yaogang modify 20141025 ͨ����Ѳ����ѡ��
	CStatic* pStatic0[11];
	
	CEdit* pEdit0[2];
	//yaogang modify 20140918 
	CCheckBox* pCheckBox0[4];// 3 add ��ʾʵʱ����
	CComboBox* pComboBox0[6];// 5  yaogang modif 20141025 ͨ����Ѳ����ѡ��
	CTableBox* pTable0;
	
	//csp modify
	CScrollBar *pScrollbar0;
	u8 pg0_page;
	CStatic* pStatic01[4];
	CComboBox* pComboBox01[1];
	CEdit* pEdit01[1];
	
	//CTableBox* pTable01;
	
	//"������ʱ��"��ҳ��
	void InitPage1();
	void OnEditChange1();
	void OnSynchronize1();
	void OnSave1();
	void OnComb();
	
	void OnCheckBoxNTPEnable();
	
	//CStatic* pStatic1[9];
	CStatic* pStatic1[10];
	CComboBox* pComboBox1[3];
	//CCheckBox* pCheckBox1[2];
	CCheckBox* pCheckBox1[3];
	CButton* pButton1[2];
	CEdit* pEdit1[2];
	CDateTimeCtrl* pDateTime1[2];
	CTableBox* pTable1;
	
	CPageInfoBox* m_pPageInfoBox;
	
	void OnDateTimeChange0();
	void OnDateTimeChange1();
	void LoadButtomCtl();
	
	unsigned char m_IsLoader;
};

#endif //_PAGE_BASICCFGFRAMEWORK_H_

