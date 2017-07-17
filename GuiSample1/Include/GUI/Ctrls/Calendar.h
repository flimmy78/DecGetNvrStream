//
//  "$Id: Calendar.h 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __CTRL_CALENDAR_H__
#define __CTRL_CALENDAR_H__

class CCalendar : public CItem
{
	static std::string m_strMonth;	//�·��ַ���
	static std::string m_strWeeks[7];	//�����ַ���
	static	int	m_weekstart;
	CComboBox	*m_pMonth;	//�·���Ͽ�
	CNumberBox	*m_pYear;	//�����ֿ�
	CButton		*m_pPrev;	//��һ��
	CButton		*m_pNext;	//��һ��
	CButton		*m_pDays[7][6];//
	CGridCtrl	*m_pGrid;
	uint		m_dwMask;	//λ��־
	int			m_nMaxDays;	//��������
	int			m_nStartWeekDay; //���µ�һ������
	int			m_iCursel;	//��ǰѡ�е���,��0��ʼ
	CTRLPROC	m_onMonthChanged;
	CTRLPROC	m_onDaySelected;

public:
	CCalendar(VD_PCRECT pRect, CPage * pParent, CTRLPROC onMonthChanged = NULL, CTRLPROC onDaySelected = NULL,uint style = 0);
	virtual ~CCalendar();
	
	virtual void Show(VD_BOOL flag, VD_BOOL redraw = TRUE);	//��ʾ/���ؿؼ�

	VD_SIZE& GetDefaultSize(void);
	void DrawSingle(int index);
	int GetAt(int px, int py);
	void DrawFrame();
	void Draw();
	void Select(VD_BOOL flag);
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
	void OnPrevMonth();
	void OnNextMonth();
	void OnMonthChanged();
	void OnDaySelected();
	void SetDate(SYSTEM_TIME *p);
	VD_BOOL  GetDate(SYSTEM_TIME *p);
	void SetMask(uint mask);
	int GetDaysOfMonth(int year,int month);
	void UpdateDays();
	static VD_BOOL SetStrings(VD_PCSTR month, VD_PCSTR week);
	static void SetWeekStart(int weekstart);
};
CCalendar* CreateCalendar(VD_PCRECT pRect, CPage * pParent, CTRLPROC onMonthChanged = NULL, CTRLPROC onDaySelected = NULL,uint style = 0);
#endif //__CTRL_CALENDAR_H__

