//
//  "$Id: DateTimeCtrl.h 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __CTRL_DATETIME_H__
#define __CTRL_DATETIME_H__

#define DTC_ITEMS 23

typedef enum{
	DTB_NORMAL,
	DTB_SELECTED,
	DTB_PUSHED,
	DTB_DISABLED,
}DateTimeCtrlBitmap;

/// �����̶�����������޸ģ�ʹ��ʱ����CreateDateTimeCtrl���캯����
/// ������ԣ������ǹ̶�һ��DTS_DATETIME��DTS_DATE��DTS_TIMEҲ������
/// DTS_YEAR, DTS_MONTH, DTS_DAY, DTS_HOUR, DTS_MINUTE, DTS_SECOND��λ��Ľ��
/// ��1��ʾ��λ��Ӧ������ʾ�������ʾ����ʾ
enum DTS_TYPE
{
	DTS_DATETIME = 0x3f,	///< ��6λ��ʾ 
	DTS_DATE = 0x38,		///< 6λ�еĸ���λ
	DTS_TIME = 0x07,		///< 6λ�еĵ���λ
	DTS_YEAR = 0x20,		///< 6λ�е����λ��ʾYear
	DTS_MONTH = 0x10,		
	DTS_DAY	= 0x08,
	DTS_HOUR = 0x04,
	DTS_MINUTE = 0x02,
	DTS_SECOND = 0x01,
}; 

//ʱ���ʾ����
/*enum COLOR_TYPE
{
	COLOR_BK,
	COLOR_TEXT,
};*/

class CDateTimeCtrl : public CItem
{
	CNumberBox *pnb[6];
	CComboBox *pcb;
	VD_BOOL single;
	int datefmt;
	int datesprtr;
	int timefmt;
 	VD_RECT m_sClientRect;
	int m_iDtcNumWidth;
	int m_iDtcLinePos;
	CTRLPROC m_vProc;
	int width;
	uint m_dtStyle;
	VD_COLORREF  m_BkColor;
	VD_BOOL  m_CustomColor;
	static VD_BITMAP* m_bmpNormal;
	static VD_BITMAP* m_bmpSelected;
	static VD_BITMAP* m_bmpPushed;
	static VD_BITMAP* m_bmpDisabled;
public:
	SYSTEM_TIME time;
	
	CDateTimeCtrl(VD_PCRECT pRect, CPage * pParent, CTRLPROC vproc = NULL, uint dwStyle = DTS_DATETIME);
	virtual ~CDateTimeCtrl();
	virtual VD_BOOL Open();//��

	virtual void Show(VD_BOOL flag, VD_BOOL redraw = TRUE);	

	void Draw();
	void DrawFrame();
	void SetBkColor(VD_COLORREF color, VD_BOOL redraw = FALSE );
	void SetTimeFormat(int vdatefmt, int vsprtrfmt, int vtimefmt);
	void SetDateTime(SYSTEM_TIME *p);
	void GetDateTime(SYSTEM_TIME *p);
	void OnDateChanged();
	void OnHourChanged();
	void OnTimeChanging();
	void OnTimeChanging1();
	void Select(VD_BOOL flag);
	void Enable(VD_BOOL flag);
	void Show(VD_BOOL flag);
	void SetRect(VD_PCRECT pRect, VD_BOOL bReDraw = TRUE);
	void DrawSplit();
	VD_SIZE& GetDefaultSize(void);
	static int GetDaysOfMonth(int year,int month);
	static int Hour12To24(int hour12, int ampm);
	static int Hour24To12(int hour24);
	static int Hour24ToAMPM(int hour24);
	static void SetDefaultBitmap(DateTimeCtrlBitmap region, VD_PCSTR name);
};

CDateTimeCtrl* CreateDateTimeCtrl(VD_PCRECT pRect, CPage * pParent, CTRLPROC vproc = NULL, uint dwStyle = DTS_DATETIME);

#endif //__CTRL_DATETIME_H__
