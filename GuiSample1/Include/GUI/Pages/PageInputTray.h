


#ifndef __PAGE_INPUT_TRAY_H__
#define __PAGE_INPUT_TRAY_H__


#include "MultiTask/Timer.h"
#include "GUI/Ctrls/Page.h"
#include "GUI/GDI/IMM.h"

#define CHAR_LIST_NUM 5

class CPageCharList;
class CPageInputTray : public CPage
{
	enum time_t
	{
		TIMER_CHAR,
		TIMER_DELETE,
	};

public:
	CPageInputTray(VD_PCRECT pRect, VD_PCSTR psz = NULL, VD_BITMAP* icon = NULL, CPage * pParent = NULL);
	~CPageInputTray();
	VD_BOOL UpdateData(UDM mode);
	void Show(VD_BOOL flag, VD_BOOL redraw = TRUE);
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
	void Draw();
	void OnTimeOver(uint param);

private:
	void OnInputTypeShift();
	void OnInputTypeChanged();
	void OnInputTypeCheck();

private:
	CButton *m_pButton;
	VD_BITMAP	*bmp_input_control;
	VD_BITMAP	*bmp_input_numeric;
	VD_BITMAP	*bmp_input_lowercase;
	VD_BITMAP	*bmp_input_uppercase;
	VD_BITMAP	*bmp_input_pinyin;
	VD_BITMAP	*bmp_input_mark;
	
	KEY_INPUT_TYPE	m_iInputType;
	int		char_index;		//���ֶ�Ӧ���ַ����
	int		last_num;		//���һ�ΰ�������
	CTimer	m_CharTimer;		//�ַ��л���ʱ��
	CTimer	m_DeleteTimer;		

	CPageCharList *pPageCharList;
};

class CPageCharList  : public CPage
{
	enum page_t
	{
		PAGEUP,		// ���Ϸ�ҳ
		PAGEDOWN,	// ���·�ҳ
		PAGENONE,	// ����ҳ
	};

public:
	CPageCharList(VD_PCRECT pRect,VD_PCSTR psz = NULL,VD_BITMAP* icon = NULL,CPage * pParent = NULL);
	~CPageCharList();
	VD_BOOL UpdateData(UDM mode);
	void OnInputChanged();
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
	void Draw();
	void GenCandidateList();
	void EnterChar();
	int GetCharAt(int x, int y);
	int GetPageAt(int x, int y);
	void UpdatePageButton();
	VD_BOOL WordExist(VD_PCSTR word, int fontStyle = FS_NORMAL);
private:
	CEdit	*m_pEditPinyin;//ƴ��
	CStatic *m_pStaticList[CHAR_LIST_NUM];//CHAR_LIST_NUM����ѡ��

	int		m_iSelected;
	int		m_iPageButton;	//��ҳ��ťѡ��״̬
	int		m_nChars;
	CIMM	m_IMM;
};

#endif //__PAGE_INPUT_TRAY_H__
