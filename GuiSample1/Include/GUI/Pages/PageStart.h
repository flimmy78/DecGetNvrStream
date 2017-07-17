

#ifndef __PAGE_START_H__
#define __PAGE_START_H__

#include "GUI/Ctrls/Page.h"

class CPageStart : public CPage
{
public:
	CPageStart(VD_PCRECT pRect,VD_PCSTR psz = NULL,VD_BITMAP* icon = NULL,CPage * pParent = NULL);
	~CPageStart();
	void SetText(VD_PCSTR psz);
	void Draw();
	VD_BOOL UpdateData(UDM mode);
	//Ĭ��Ϊ����ҳ�棬��Ļ������δ��ʱ�����˴�����Ϊ��Ļ����ҳ���ʱӦ��ֵPAGE_APPSCREENPROTECT
	VD_BOOL Open(int state = 0/*CPageStart::PAGE_APPSTART*/);
	VD_BOOL Close();
	
private:
	CStatic*		pText;
	// bitmaps
	VD_BITMAP * bmp_logo;

};
//extern CAlarm	g_Alarm;
//extern CGUI		g_GUI;
#endif //__PAGE_START_H__
