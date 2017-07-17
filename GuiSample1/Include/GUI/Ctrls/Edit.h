//
//  "$Id: Edit.h 117 2008-11-27 09:44:43Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//    Description:    
//    Revisions:        Year-Month-Day  SVN-Author  Modification
//

#ifndef __CTRL_EDIT_H__
#define __CTRL_EDIT_H__

typedef enum
{
    #if defined SHBELL || defined ZXBELL
    ETS_NORMAL        = 0x0000,
    #endif
    editPassword    = 0x0001,
    
	edit_KI_NUMERIC = 0x0002,			//����
	edit_KI_UPPERCASE = 0x0004,	//��д��ĸ
	edit_KI_LOWERCASE = 0x0008,	//Сд��ĸ
	edit_KI_MARK = 0x0010,		//���÷���
	edit_KI_PINYIN = 0x0020,		//ƴ������
	edit_KI_CONTROL = 0x0040,		//ATMԭʼ���Ƽ�
	edit_KI_NOCH = 0x8000,		//������
//    styleNoBorder    = 0x0002,
}EDITSTYLE;

struct EDIT_ITEM
{
    uchar len;                //�ַ����ַ����ֽ���
};

typedef enum{
    ETB_NORMAL,
    ETB_SELECTED,
    ETB_DISABLED,
}EditBitmap;

typedef std::vector<EDIT_ITEM> EDIT_ITEM_VECTOR;

class CEdit : public CItem
{
    int     curpos;            //��ǰλ��
    int     showpos;            //��ʼ��ʾ��λ��
    int     word_pos;            //��ǰ�ʵ�λ��
    int     len_max;            //�ַ�������
    EDIT_ITEM_VECTOR items;    //����Ϣ
    std::string input;        //�û�����
    CNumberPad* numpad;        //���������
    int caret_cnt;            //�������˸����
    int caret_pos;            //�����λ��
    int last_char_with;        //���һ���ַ��Ŀ��
    VD_RECT caret_rect;        //�����������
    VD_BOOL modifying;            //�Ƿ������޸�
    VD_COLORREF    m_BkColor;    //������ɫ
static    KEY_INPUT_TYPE input_type;//��ǰ���뷨����
static    VD_BOOL m_bMarkPadShown;    //������屻�Զ���
    CTRLPROC m_onChanged;
        
private:
    void DrawCaret();
    void UpdateCaretPos(VD_BOOL redraw = FALSE);
    int GetTextExtent(int pos, int len = 1024);
protected:
    static VD_BITMAP* bmp_etb_normal;
    static VD_BITMAP* bmp_etb_selected;
    static VD_BITMAP* bmp_etb_disabled;

public:
	VD_BOOL IsPadOpen();
    //CEdit�ؼ��������Զ��۽�������,--by wangqin 20070316
    CEdit(VD_PCRECT pRect, CPage * pParent, int vlen=6, uint vstyle = 0, CTRLPROC onChanged = NULL);
    virtual ~CEdit();

    void ShowDropDown(VD_BOOL show);
    void SetFocus(VD_BOOL flag);
    void Draw();
    VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
    void Select(VD_BOOL flag);
    void Empty();
    int GetText(char *buf, int vlen);
    void SetText(const char *psz);
    void SetBkColor(VD_COLORREF color, VD_BOOL redraw = FALSE);
    uint GetStyle();
    VD_SIZE& GetDefaultSize(void);

    static void SetDefaultBitmap(EditBitmap region, VD_PCSTR name);
};
CEdit* CreateEdit(VD_PCRECT pRect, CPage * pParent, int vlen=6, uint vstyle = 0, CTRLPROC onChanged = NULL);

//add by nike.xie 20090907  //�������༭�������⴦��
class CDDNSEdit: public CEdit
{
public:
    CDDNSEdit(VD_PCRECT pRect, CPage * pParent, int vlen=6, uint vstyle = 0, CTRLPROC onChanged = NULL);
    virtual ~CDDNSEdit();

    VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
};
//end

#endif //__CTRL_EDIT_H__
