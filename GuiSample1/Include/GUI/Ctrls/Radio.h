
#ifndef __CTRL_RADIO_H__
#define __CTRL_RADIO_H__

class CRadio;
typedef std::vector<CRadio *> PRADIOGROUP;

class CRadio : public CItem
{
	//std::string	string;	//�ؼ��ַ���
	CRadio		*m_pHead;	//��һ����ѡ��
	int			m_iIndex;	//����������
	int			m_iCurSel;	//��ǰ��, ֻ�Ե�һ����ѡ����Ч
	PRADIOGROUP m_Group;//��ѡ����, ֻ�Ե�һ����ѡ����Ч
	CTRLPROC	m_onChanged;
	VD_BITMAP* m_pRadioImageNormal;
	VD_BITMAP* m_pRadioImageSelect;
	VD_BITMAP* m_pRadioImagePush;
	VD_BITMAP* m_pRadioImageDisable;

public:
	CRadio(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz = NULL, CTRLPROC onChanged = NULL,uint style = 0);//onChangedֻ����������
	CRadio(VD_PCRECT pRect, CPage * pParent, CRadio* pHead, VD_PCSTR psz = NULL,uint style = 0);
	virtual ~CRadio();

	void Draw();
	void Select(VD_BOOL flag);
	VD_BOOL MsgProc(uint msg, uint wpa, uint lpa);
	int SetCurSel(int sel);
	int GetCurSel();
	void Switch();
	VD_BOOL IsHead();
	VD_SIZE& GetDefaultSize(void);

	VD_BOOL SetBitmap(VD_BITMAP* pBmpNormal = NULL,	VD_BITMAP* pBmpSelect = NULL, VD_BITMAP* pBmpPush = NULL, VD_BITMAP* pBmpDisable = NULL);
	void DrawCtrlRadio(CTRL_STATE state= ctrlStateNormal);
};
CRadio* CreateRadio(VD_PCRECT pRect, CPage * pParent, VD_PCSTR psz = NULL, CTRLPROC onChanged = NULL,uint style = 0);
CRadio* CreateRadio(VD_PCRECT pRect, CPage * pParent, CRadio* pHead, VD_PCSTR psz = NULL,uint style = 0);

#endif //__CTRL_RADIO_H__
