#ifndef _PAGE_NETWORK_CONFIG_H_
#define _PAGE_NETWORK_CONFIG_H_

#include "PageFrame.h"
//#include "PageChild.h"
#include <vector>
#include "common_basetypes.h"

#define _RZ_P2P_

#ifdef _RZ_P2P_
typedef enum
{
	P2P_TX_HEART = 'h',//"h"��ʾ����֡
	P2P_TX_CMD = 'd',//"d"��ʾͨѶ����֡
	P2P_TX_OVER = 'o',
	P2P_TX_LOST = 'E',
}EmP2PTxType;
#define TX_HEAD_LEN	3
#define TX_DATA_LEN	20
#define TX_TAIL_LEN	2
//ͷ�����ֽ�Ϊ0x58,0x44,0x43,β�����ֽ�Ϊ0x52,0x5A
typedef struct
{
	u8 head[TX_HEAD_LEN];
	u8 leixing;//�ο�EmP2PTxType
	u8 shuju[TX_DATA_LEN];
	u8 jiaoyan;//Ϊ���ͺ����ݲ��ֵĺ͵�ģ256У��
	u8 weibu[TX_TAIL_LEN];
}Tongxun;
#endif

#ifdef _RZ_P2P_
#define NETWORKCFG_SUBPAGES	5
#define NETWORKCFG_BTNNUM	8
#else
#define NETWORKCFG_SUBPAGES	4
#define NETWORKCFG_BTNNUM	7
#endif

/*
#define NETWORKCFG_MAXSUB   2

typedef struct tagMAINFRAME_ITEM
{
	std::vector<CItem*> SubItems[NETWORKCFG_MAXSUB];
	int			cursubindex;
}MAINFRAME_ITEM;
*/

/*
 ���ýӿڴ�ȡ��������ṹ
*/

typedef struct _sGuiNetAdvancePara
{
	char	szSmtpServer[64];
	// ����������
	char	szSendMailBox[64];
	char	szSMailPasswd[64];

    u8		nSSLFlag;

	// �ռ�������
	char	szReceiveMailBox[64];
	char	szReceiveMailBox2[64];
	char	szReceiveMailBox3[64];

	// ����ͼƬ
	char	szSnapPic[64];
	
	// ץͼ���
	u32		nSnapIntvl;
	
} SGuiNetAdvancePara;

typedef struct _sGuiNetConfigPara
{
	// ��������
	u8     	Version[8];			    // 8�ֽڵİ汾��Ϣ
	char   	HostName[16];           // ������
	u32  	HostIP;                 // IP ��ַ
	u32  	Submask;                // ��������
	u32  	GateWayIP;              // ���� IP
	u32  	DNSIP;                  // DNS IP
	u32  	DNSIPAlt;               // DNS IP
	u8 		MAC[18];				// MAC Address

    // �ⲿ�ӿ�
    u32   	AlarmServerIP;          // ��������IP
    u16 	AlarmServerPort;        // �������Ķ˿�
    u32   	SMTPServerIP;           // SMTP server IP
    u16 	SMTPServerPort;         // SMTP server port
	
    u32   	LogServerIP;            // Log server IP
    u16 	LogServerPort;          // Log server port

    // ��������˿�
    u16 	HttpPort;               	// HTTP����˿ں�
    u16 	MobilePort;              	// �ֻ���ض˿ں�
    
    u16 	TCPMaxConn;             	// TCP ���������
    u16 	TCPPort;                	// TCP �����˿�

	u8		nSubStreamMax;				// ������������
	u8		nSendBufNumPerChn;			// ÿͨ������buff��
	u32		nFrameSizeMax;				// ֡buff��С����
	
	// ��·������
	u8		nVideoMediaType;			// ��Ƶ��������
	u8		nAudioMediaType;			// Ԥ����Ƶ��������
	u8		nVoipMediaType;				// �Խ���Ƶ��������
	u8		nAudioSampleMode;			// ��Ƶ����ģʽ

    u16 	UDPPort;                	// UDP �����˿�

    u8		McastFlag;
    u16 	McastPort;              	// �鲥�˿�
    u32 	McastIP;                	// �鲥IP
    
	u8		PPPOEFlag;					// pppoe ʹ�ܿ���
	u8		PPPOEUser[64];
	u8		PPPOEPasswd[64];
	
	u8		DhcpFlag;					// Dhcp ʹ�ܿ���
	
	u8		DDNSFlag;					// DDNS ����λ 0 closed, 1, 2, 3 other active sel
	u8		DDNSDomain[64];
	u8		DDNSUser[64];
	u8		DDNSPasswd[64];
	u16		UpdateIntvl;				// DDNS	IP���¼��
	
    // ����
    u8    	MonMode;                	// ����Э�� TCP|UDP|MCAST
    u8    	PlayMode;               	// �ط�Э�� TCP|UDP|MCAST
    u8    	AlmSvrStat;             	// ��������״̬ <��/��>
	
	// �߼�����
    SGuiNetAdvancePara	sAdancePara;
	
	//csp modify 20130321
	u8		UPNPFlag;
} SGuiNetPara;

//�������
typedef struct 
{
	u32 nBitRate; // λ��(��λΪKb)
	u8 nFrameRate; // ֡��
	u32 nGop; // �ؼ�֡���
	u32 nMinQP; // (�ݲ�֧��)
	u32 nMaxQP; // (�ݲ�֧��)

	u8 nVideoResolution;  // ��Ƶ�ֱ���(EMBIZENCRESOLUTION)
	u8 nEncodeType; // ��Ƶ��������(EMBIZENCTYPE����,��ʱֻ֧�̶ֹ�ȡֵ98,H.264) ��֧������
	u8 nBitRateType; // λ������(EMBIZENCBITRATETYPE)
	u8 nPicLevel; // ͼ������(EMBIZENCPICLEVEL)
} SGuiNetStreamPara;

class CPageNetworkConfig :public CPageFrame
{
public:
	CPageNetworkConfig(VD_PCRECT pRect,VD_PCSTR psz,VD_BITMAP* icon = NULL,CPage * pParent = NULL);
	~CPageNetworkConfig();
	
	VD_PCSTR GetDefualtTitleCenter();
	
	VD_BOOL UpdateData( UDM mode );
	//VD_BOOL MsgProc( uint msg, uint wpa, uint lpa );
	
	void SwitchDHCP();
	void LoadPara( int curID );
	void CtrlEnable(VD_BOOL flag);
	void SetDynNetPara(uint ip, uint submsk, uint gw, uint dns1);
	void SetNetStatus(uchar type, uchar state, uchar rslt); // 0 dhcp 1 pppoe 2 mail 3 ddns; 0 suc, 1 fail 
	int  GetProtocolValue(int sel);//cw_ddns
	
protected:
	
private:
	void LoadMailCfg(SGuiNetPara* pCfg);
	void UpdateDomain();
	BOOL Digital(const char *s);
	void MessgeBox(int index);
	
	void ShowRegistButton( void );
	void TestMailInfo(uchar idx, uchar act, uchar rslt);
	VD_BOOL MsgProc( uint msg, uint wpa, uint lpa );
	
	CStatic* 		pStatus;
	CScrollBar* 	pScrollbar;
	SGuiNetStreamPara*	pTabPara;
	int             nRealRow;
	int             nPageNum;
	int             nHideRow;
	int             nCurPage;
	
	void 			OnTrackMove0();
	void			AdjHide();
	
	int	nChMax;
	
	int	m_nLastSel;
	
	int	curID;
	
	void OnCombox3();
	void OnCheckBox3();
	void OnCombSel();
	//void OnClkConfigItem();
	void SwitchPage( int mainID,int subID);
	void ShowSubPage(int mainID,int subID, BOOL bShow);

	void OnClickSubPage();
	void OnClickBtn();
	void OnTrackMove();

	CButton* pButton[NETWORKCFG_BTNNUM+2];
	CTableBox* pTable;
	CStatic* pInfoBar;
	CStatic* pIconInfo;

	void SetInfo(char* szInfo);
	void ClearInfo();
	//CButton* pItemButton[3];

	//"����"��ҳ��0
	void InitPage00();
	void OnEditChange00();
	void OnTrackMove00();
	int lastPos00;
	CStatic* pStatic00[9];
	CEdit* pEdit00[2+1];//20111024 mobile port
	CIPAddressCtrl* pIpCtrl00[5];
	CCheckBox* pCheckBox00;
	CTableBox* pTable00;
	CTableBox* pTable01;
	CScrollBar* pScrollbar00;
	
	//"����"��ҳ��1
	void InitPage01();
	void OnTest01();
	
	CCheckBox* pCheckBox01;
	CEdit* pEdit01[2];
	
	//csp modify
	//CStatic* pStatic01[2];
	CStatic* pStatic01[3];
	
	CButton* pButton01;
	
	//csp modify
	CStatic* pStaticpppoetitle[4];
	CIPAddressCtrl* pIpCtrlpppoecontext[4];
	//unsigned char pppoe_flag;
	u8 pppoe_flag;
	
	//csp modify 20130321
	CStatic* pStaticupnptitle[1];
	CCheckBox* pCheckBoxupnp[1];
	void OnCheckBoxupnp();
	
	//"����"��ҳ
	void InitPage1();
	CStatic* pStatic1[12+2];
	CComboBox* pComboBox1[25+5*2];
	CCheckBox* pCheckBox1;
	CTableBox* pTable10;
	CTableBox* pTable11;
	void OnCheckBox01();
	
	void OnEditChange2();
	//"Email"��ҳ0
	void InitPage20();
	void OnTest20();
	void OnTrackMove20();
	int lastPos20;
	CStatic* pStatic20[8];
	CEdit* pEdit20[8];
	CCheckBox* pCheckBox20;
	CButton* pButton20;
	CTableBox* pTable20;
	CTableBox* pTable21;
	CScrollBar* pScrollbar20;
	
	//"Email"��ҳ1
	void InitPage21();
	CStatic* pStatic21[3];
	CEdit* pEdit21[2];
	
	//"��������"��ҳ
	void InitPage3();
	void OnTest3();
	void OnEditChange3();
	CStatic* pStatic3[6];
	CCheckBox* pCheckBox3;
	CComboBox* pComboBox3[2];
	CEdit* pEdit3[3];
	CButton* pButton3;
	CTableBox* pTable3;
	void LoadButtomCtl();
	
	unsigned char m_IsLoader;
	
#ifdef _RZ_P2P_
	void InitPage4();
	
	void OnCheckBox4();
	
	void p2pregister();
	
	void modifymac(char *macaddr);
	void setnetwork();
	
	void mtdopmac(char *mac);
	void mtdopuuid(char *uid);
	
	CTableBox* pTable4;
	CCheckBox* pCheckBox4;
	CStatic* pStatic4[7];
	CEdit* pEdit4[3];
	CButton* pButton4;
#endif
};

#endif //_PAGE_NETWORK_CONFIG_H_

