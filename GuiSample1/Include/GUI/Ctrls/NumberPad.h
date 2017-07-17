//
//  "$Id: NumberPad.h 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __CTRL_NUMBERPAD_H__
#define __CTRL_NUMBERPAD_H__

#define MAX_PAD_ITEMS 30
typedef enum
{
	IPT_NUMBER = 0, //���ְ�
	IPT_UPPERCASE,	//��д��ĸ
	IPT_LOWERCASE,	//Сд��ĸ
	IPT_MARK,		//���÷���
	IPT_TOTAL,
}INPUT_PAD_TYPE;   //����������

typedef enum{
	NPB_PANEL,
	NPB_BUTTON_NORMAL,
	NPB_BUTTON_PUSHED,
	NPB_BUTTON_SELECTED,
	NPB_BUTTON_DISABLED,
}NumberPadBitmap;

class CNumberPad : public CItem
{
	INPUT_PAD_TYPE m_PadType;   //д�ְ�����
	int  m_MaxItems; //�������

	int   m_Cursel;
	CRect m_ButtonRects[MAX_PAD_ITEMS];
	static VD_BITMAP* m_bmpPanel;
	static VD_BITMAP* m_bmpBtnNormal;
	static VD_BITMAP* m_bmpBtnSelected;
	static VD_BITMAP* m_bmpBtnPushed;
	static VD_BITMAP* m_bmpBtnDisabled;

public:
	CNumberPad(VD_PCRECT pRect, CPage * pParent, CItem *pOwner = NULL, INPUT_PAD_TYPE iType = IPT_NUMBER,uint style = 0);
	virtual ~CNumberPad();
	CNumberPad* CreatNumberPad(VD_PCRECT pRect, CPage * pParent, CItem *pOwner = NULL, INPUT_PAD_TYPE iType = IPT_NUMBER,uint style = 0);
	static void GetPadSize(INPUT_PAD_TYPE iType, VD_PSIZE pSize);
	void Draw();
	void DrawSingle(int index);
	int GetAt(int px, int py);
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);

	static void SetDefaultBitmap(NumberPadBitmap region, VD_PCSTR name);
};

#endif //__CTRL_NUMBERPAD_H__

