#ifndef __NETCOMM_H_
#define __NETCOMM_H_

#include "common_basetypes.h"
#include "hddcmd.h"
#include "diskmanage.h"
#include "Cmdcallback.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define 
#define FILE_PATH_MAX				128

#define RTSP                             // RTSP ENABLE
#define MOBILE_SERVER2                   // �ֻ����ENABLE
#define DDNS_USER_LEN_MAX			32

// typedef

typedef u32	IPDEF;

// Զ�̸�ʽ��/Զ�������ص�����

//2.4.5
typedef enum
{
	EM_NETFORMAT,
	EM_NETUPDATE,
} EMNETOP;
typedef enum
{
	EM_NETUPGRADE_USB,
	EM_NETUPGRADE_NET,
} EMNETUPGRADEWAY;

// mainboard || panel
typedef enum
{
	EM_NETUPGRADE_MAINBOARD,
	EM_NETUPGRADE_PANEL,
	EM_NETUPGRADE_STARTLOGO,
	EM_NETUPGRADE_APPLOGO,
} EMNETUPGRADETARGET;

typedef struct _SFormat
{
	u8 nIdx;	
} SRFormat;

typedef struct _SUpdate
{
	EMNETUPGRADETARGET 		eTgt;
	EMNETUPGRADEWAY		 	eWay;
	char sfilepath[FILE_PATH_MAX];	
} SRUpdate;

typedef struct
{
	EMNETOP eOp; 	// 0 format, 1 update
	union
	{
		SRFormat Format;
		SRUpdate Update;
	};
} SRemoteOpCB;

typedef s32 (*PFRemoteOp)(SRemoteOpCB * para);

/*
 VOIP ����������ṹ
*/
typedef struct _sVOIPStream
{
	s32 	 nChn;
	u8		 *pData;
	u32		 nLen; // in buffer length; out real data length
	u64		 nTimeStamp;
	BOOL     bRaw;
} SVOIPStream, *PSVOIPStream;

/*
 VOIP ������Ƶ���ص�
*/
typedef void (*PNetCommVOIPIn)(PSVOIPStream* pStream);

/*
 VOIP ���������ص�
*/
typedef void (*PNetCommVOIPOut)(PSVOIPStream* pStream);

/* 
 ������ý��֡����
*/
typedef enum
{
	EM_VIDEO,
	EM_AUDIO,
	EM_TALK,
	EM_THIRD,//csp modify 20130423
	EM_MAX
} EM_STREAM_FRAME;

/*
 Ԥ������Ƶ֡ͷ��Ϣ�ṹ
*/
typedef struct _sNetCommStreamHeader
{
	u8	byChnIndex;       // 
	u32	byMediaType;
	u8	byFrameType;
	u8	byLastSnd;
	u8	bSub; // 0 not sub, 3 - bit0 mb sub, bit1 net sub
	u32	dwlen;
	u32	timeStamp;
	u8	resolution;
	u16	nWidth;
	u16	nHeight;
	u8	nFrameRate;
	EM_STREAM_FRAME eFrameType;
	u32	nAudioMode;
	u32	stamp, stamp1;
} SNetComStmHead, *PSNetComStmHead;

/*
 Զ�̻ط��ļ���������
 
*/
typedef enum
{
	EM_RMTPLAY_OPEN,
	EM_RMTPLAY_SEEK,
	EM_RMTPLAY_GetTimeLength,
	EM_RMTPLAY_GetMediaFormat,
	EM_RMTPLAY_CLOSE,
	EM_RMTPLAY_GETFRAME,	
} EM_RMTPLAY_OP;

/*
 �ļ�����������Դ
 �طţ�����
 
*/
typedef enum
{
	EM_FILEOP_DOWN,
	EM_FILEOP_REPLAY,
	EM_FILEOP_BACKUP,
	
} EM_FILEOP_SRC;

/*
 Զ�̻ط��ļ����������ṹ
*/
typedef struct _sRemotePlayFileOp
{
	// op type
	EM_RMTPLAY_OP eOp;

	// op req src
	EM_FILEOP_SRC eOpSrc;
	
	// operate result
	s32 nOpRslt;
	
	// op param
	union
	{
		struct {
			char szFilePath[FILE_PATH_MAX]; // open file path
			u32  nOffset;         			// seek offset
			void* pFile;
		} OpenSeek;
		
		struct {
			u32 nTotalTime; 
		} GetTimeLen;
		
		struct {
			u16 nWidth;
			u16 nHeight;
		} GetFormat;
		
		struct {
			int nFrameSize;
			u8* pFrameBuf;
			u32 nBufSize;
			u32 nStartTime;
			u8  bKey;
			u64 nPts;
			u8  nMediaType; // 1 audio, 0 video
		} GetFrame;			
	};
		
} SRmtPlayFileOp, *PSRmtPlayFileOp;

/*
 Ԥ����������������
*/
typedef enum
{
	EM_PREQ_MAIN,
	EM_PREQ_SUB,
	EM_PREQ_SUB_MB,
	EM_PREQ_AUDIO,
	EM_PREQ_VOIP,
	EM_PREQ_THIRD,//csp modify 20130423
	EM_PREQ_MAX,
} EM_PREVREQ;

/*
 �������������
*/
typedef enum
{
	EM_START,
	EM_STOP,
} EM_ACT;

/*
 PStreamRequestCB param struct 
*/
typedef struct _sTcpStreamReq
{
	u8 			nChn;
	EM_PREVREQ 	nType; // 0 main, 1 sub, 2 audio, 
	EM_ACT		nAct; // 0 start, 1 stop	
} STcpStreamReq;

/*
 �����û���Ϣ
*/
typedef struct
{
	u8		bOnline; // 0 offline, 1 online
	u8		bUserDel;
	u8		resv[2];
	u32		id;
	u32 	ip;
	s32		sock;
	char	mac[18];	
	char 	name[32];
} PACK_NO_PADDING SNetUser;

/*
	��̬IP, 
	��̬IP,
	PPPoE��������
	..
*/
typedef enum
{
	ECW_STATIC,
	ECW_DHCP,
	ECW_PPPOE,
	ECW_MAX
} E_CONN_WAY;

/*
	�����ӣ�
	δ����
	..
*/
typedef enum
{
	ECS_ONLINE,
	ECS_OFFLINE,
	ECS_MAX
} E_CONN_STATE;

typedef struct _sNetConnSta
{
	E_CONN_WAY 		eWay;
	E_CONN_STATE	eState;
} SNetConnStatus;

/*
 Զ�̻ط��ļ������ص�����
 
 ����:
  void* pFileHandle : �ļ����
  EM_RMTPLAY_OP eOp : �ļ�����
  PSRmtPlayFileOp pParam : ��������
  
 ����: ��pParam���ز������
*/
typedef void (*PRemotePlayCB)(void* pFileHandle, PSRmtPlayFileOp pParam);

/*
 �ṩ����ģ����÷�����Ļص�����,�ûص�������������Ƶ��
 
*/
typedef void (*PStreamRequestCB)(STcpStreamReq* param);

/*
 �ṩ����ģ����÷�����Ļص�����,�ûص���������ؼ�֡
 
*/
typedef void (*PRequestKeyFrameCB)();

// �����ڲ�֧����־����
typedef enum
{
	EM_NETLOG_START_DWELL,
	EM_NETLOG_STOP_DWELL,
	EM_NETLOG_MUTE,
	EM_NETLOG_CHANGE_VOICE,
	EM_NETLOG_CONTROL_PTZ,
	EM_NETLOG_SNAP_SUCCESS,
	EM_NETLOG_START_MANUAL_REC,
	EM_NETLOG_STOP_MANUAL_REC,
	EM_NETLOG_MANUAL_ALARM,
	EM_NETLOG_CLEAR_ALARM,
	EM_NETLOG_START_MD,
	EM_NETLOG_STOP_MD,
	EM_NETLOG_POWER_OFF,
	EM_NETLOG_REBOOT,
	EM_NETLOG_LOGIN,
	EM_NETLOG_LOGOFF,
	EM_NETLOG_FORMAT_SUCCESS,
	EM_NETLOG_FORMAT_FAIL,
	EM_NETLOG_UPDATE,

	EM_NETLOG_ENABLE_PWDCHECK,
	EM_NETLOG_DISABLE_PWDCHECK,
	EM_NETLOG_CHANGE_VO,
	EM_NETLOG_CHANGE_MAX_NETUSER,
	EM_NETLOG_CHANGE_VIDEOFORMAT,
	EM_NETLOG_CHANGE_LANGUAGE,
	EM_NETLOG_CHANGE_ZONE,
	EM_NETLOG_ENABLE_AUTO_TIMESYNC,
	EM_NETLOG_DISABLE_AUTO_TIMESYNC,
	EM_NETLOG_CHANGE_NTP_SERVER,
	EM_NETLOG_DST_CONFIG,
	EM_NETLOG_CHANGE_CHN_NAME,
	EM_NETLOG_CHANGE_REC_ENABLE,
	EM_NETLOG_CHANGE_REC_AUDIO_ENABLE,
	EM_NETLOG_CHANGE_REC_CODESTREAM,
	EM_NETLOG_CHANGE_TIME_BEFORE_ALARM,
	EM_NETLOG_CHANGE_TIME_AFTER_ALARM,
	EM_NETLOG_CHANGE_REC_EXPIRETIME,
	EM_NETLOG_CHANGE_OSD,
	EM_NETLOG_CHANGE_TIMER_RECSCHE,
	EM_NETLOG_CHANGE_MD_RECSCHE,
	EM_NETLOG_CHANGE_SENSORALARM_RECSCHE,
	EM_NETLOG_CHANGE_SENSOR_ENABLE,
	EM_NETLOG_CHANGE_SENSOR_TYPE,
	EM_NETLOG_CHANGE_SENSOR_ALARMDEAL,
	EM_NETLOG_CHANGE_SENSOR_DETECTION_SCHE,
	EM_NETLOG_CHANGE_MD_ENABLE,
	EM_NETLOG_CHANGE_MD_AREA_SETUP,
	EM_NETLOG_CHANGE_MD_ALARMDEAL,
	EM_NETLOG_CHANGE_MD_DETECTION_SCHE,
	EM_NETLOG_CHANGE_VIDEOLOSS_ALARMDEAL,
	EM_NETLOG_CHANGE_ALARMOUT_SCHE,
	EM_NETLOG_ENABLE_SOUND_ALARM,
	EM_NETLOG_DISABLE_SOUND_ALARM,
	EM_NETLOG_CHANGE_IP,
	EM_NETLOG_CHANGE_HTTP_PORT,
	EM_NETLOG_CHANGE_SERVER_PORT,
	EM_NETLOG_CHANGE_NET_CODESTREAM,
	EM_NETLOG_CHANGE_OUTBOX_CONFIG,
	EM_NETLOG_CHANGE_INBOX_CONFIG,
	EM_NETLOG_CHANGE_DNS_CONFIG,
	EM_NETLOG_ADD_USER,
	EM_NETLOG_DELETE_USER,
	EM_NETLOG_CHANGE_USER_AUTH,
	EM_NETLOG_CHANGE_PWD,
	EM_NETLOG_CHANGE_SERIAL_CONFIG,
	EM_NETLOG_CHANGE_CRUISELINE,
	EM_NETLOG_FACTORY_RESET,
	EM_NETLOG_PB_STOP,
	EM_NETLOG_PB_PAUSE,
	EM_NETLOG_PB_RESUME,
	EM_NETLOG_PB_BACKWARD,
	EM_NETLOG_PB_FORWARD,
	EM_NETLOG_START_BACKUP,
	EM_NETLOG_END_BACKUP,
	EM_NETLOG_SEARCH_DATA_BYTIME,
	EM_NETLOG_SEARCH_RECTIME,
	EM_NETLOG_CHECKINFO
} EM_NETLOG;

// ����������־��¼�ص�����
typedef struct _sNetLog
{
	int ip;
	int sockfd;
	EM_NETLOG nLogId;	// ��ѭctrlprotocol��������ID
} SNetLog;

/*
 ��־��¼�ص�
 
*/
typedef void (*PNetLogWriteCB)(SNetLog* pLog);


/*
 ���ýӿڴ�ȡ��������ṹ
*/
typedef struct _sNetAdvPara
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
	
} SNetAdvancePara;

#define MAX_ALARM_UPLOAD_NUM 5

typedef struct
{
	CPHandle g_cph;
	u8 g_state;
}ifly_AlarmUploadCenter_t;

typedef struct _sNetCfgPara
{
	// ��������
	u8     	Version[8];			    // 8�ֽڵİ汾��Ϣ
	char   	HostName[16];           // ������
	u32  	HostIP;                 // IP ��ַ
	u32  	Submask;                // ��������
	u32  	GateWayIP;              // ���� IP
	u32  	DNSIP;                  // DNS IP
	u32  	DNSIPAlt;				// DNS IP
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
	
	u8		DDNSFlag;					// DDNS ����λ
	u8		DDNSDomain[64];
	u8		DDNSUser[64];
	u8		DDNSPasswd[64];
	u16		UpdateIntvl;				// DDNS	IP���¼��
	
    // ����
    u8    	MonMode;                	// ����Э�� TCP|UDP|MCAST
    u8    	PlayMode;               	// �ط�Э�� TCP|UDP|MCAST
    u8    	AlmSvrStat;             	// ��������״̬ <��/��>
	
	// �߼�����
    SNetAdvancePara	sAdancePara;
	
	//csp modify 20130321
	u8		UPNPFlag;
} SNetPara;

// ����״̬��֪ͨ�ص�
typedef enum
{
	EM_NET_STATE_DHCP,
	EM_NET_STATE_PPPOE,
	EM_NET_STATE_MAIL,
	EM_NET_STATE_DDNS,
	EM_NET_STATE_CONN,
	EM_NET_STATE_UPDATEMAINBOARDSTART,
	EM_NET_STATE_UPDATEPANNELSTART,
	EM_NET_STATE_UPDATESTARTLOGOSTART,
	EM_NET_STATE_UPDATEAPPLOGOSTART,
	
	EM_NET_STATE_FORMAT_START,
	EM_NET_STATE_SG,//�ϴ����ƽ̨����״̬
	EM_NET_STATE_SGReqSnap,//���ƽ̨�����ϴ�
} EM_NET_ACT;

typedef struct _sNetStateRslt
{
	u8 state; // 0 start, 1 stop
	u8 nErr; // 0 succ, 1 fail
} SNetStateRslt;

typedef struct _sNetStateNotify
{
	EM_NET_ACT eAct;
	SNetStateRslt sRslt;
	unsigned int SGReqChnMask; //���ƽ̨����������ͨ������
} SNetStateNotify;

typedef void (*PNetCommStateNotify)(void* pNotify);

/*
 ����ģ���ʼ�������ṹ(�ݶ�������������Ҫɾ��)
*/
typedef struct _sNetCommCfg
{
	SNetPara sParaIns;
	
	u16		nAllLinkMax;			  		// ����������
	u8		nSubStreamMax;					// ������������
	u8		nSendBufNumPerChn;				// ÿͨ������buff��
	u32		nFrameSizeMax;					// ֡buff��С����
	u32		yFactor;						// ��ʽ
	u8		nVideoMediaType;
	u8		nAudioMediaType;
	u8		nVideoFrameRate;
	u8		nAudioFrameRate;
	u8		nAudioMode;
	u8		nVoipMode;
	u8		nChnMax;
	//yaogang modify 20150324 //Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
	u8		nNVROrDecoder;
	
	//csp mpdofy
	u16		nVideoMaxWidth;
	u16		nVideoMaxHeight;
	
	void*	pDiskMgr;
	
	// Callback
	PStreamRequestCB 	pReqStreamCB;
	PRequestKeyFrameCB 	pReqKeyCB;
	PRemotePlayCB 		pRpCB;
	PNetCommVOIPOut 	pVOIPOut;
	PNetCommVOIPIn 		pVOIPIn;
	
	PNetLogWriteCB		pWriteLogCB;
	
	PNetCommStateNotify pNotify;
	PFRemoteOp			pRFomat, pRUpdate;
} SNetCommCfg, *PSNetCommCfg;

typedef struct _sNetMultiCastInfo
{
	u32		deviceIP; 						//�豸IP  
	u16		devicePort;						//�豸�˿� 
	char 	device_name[32];				//�豸����
	char 	device_mode[32];				//�豸�ͺ�
	u8		maxChnNum;						//���ͨ����
	u8		maxAduioNum;					//�����Ƶ��
	u8		maxSubstreamNum;				//�����������
	u8		maxPlaybackNum;					//���ط���
	u8		maxAlarmInNum;					//��󱨾�������
	u8		maxAlarmOutNum;					//��󱨾������
	u8		maxHddNum;						//���Ӳ����
	u8 nNVROrDecoder;	//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
} PACK_NO_PADDING SNetMultiCastInfo;

// set net para cmd
typedef enum
{
	EM_NET_SET_IP,
	EM_NET_SET_GW,
	EM_NET_SET_MSK,
	EM_NET_SET_HW,
	EM_NET_SET_DNS,
	EM_NET_ADD_DNS
} EM_NET_SET;

typedef struct _sNetSet
{
	union
	{
		u32 nIP;
		u32 nGateway;
		u32 nSubmask;
		u8	szHW[18];
		u32 nDns;
	};
} SNetSet;

/*
 Network state
*/
typedef enum
{
	EM_NS_CONNECTED = 0,
	EM_NS_DISCONNECTED,
	
} EM_NET_CONN_STATE;

/*
 �ʼ�����
*/
typedef struct _sMailContent
{
	char* pSrcUsr;
	char* pSrcBox;
	char* pToUsr;
	char* pToBox;

	char* pUserName;
	char* pPasswd;

	char* pSmtp;
	char* pSubject;
	char* pBody;

	u8	  bAttach;	
} SMailContent;

// DDNS 
typedef enum
{
#if 1//csp modify
	EM_NET_DDNS_TL = 1,
	EM_NET_DDNS_3322,
	EM_NET_DDNS_DYDNS,
	EM_NET_DDNS_DVRNET,
	EM_NET_DDNS_NOIP,
	EM_NET_DDNS_JMDVR,
	EM_NET_DDNS_CHANGEIP,
	EM_NET_DDNS_JSJDVR,
	
	EM_NET_DDNS_KONLAN,
	
	EM_NET_DDNS_CCTVSVR,
	EM_NET_DDNS_CCTVDVR,
	
	EM_NET_DDNS_MYQSEE,
	EM_NET_DDNS_DYNDDNS,
	EM_NET_DDNS_PEOPLE,
	EM_NET_MAX,
#else
	EM_NET_DDNS_TL = 1,
	EM_NET_DDNS_3322,
	EM_NET_DDNS_DYDNS,
	EM_NET_DDNS_DVRNET,
	EM_NET_DDNS_NOIP,
	EM_NET_DDNS_JMDVR,
	EM_NET_DDNS_CHANGEIP,
	EM_NET_DDNS_JSJDVR,
	EM_NET_DDNS_CCTVSVR,
	EM_NET_DDNS_CCTVDVR,
	EM_NET_DDNS_KONLAN,
	EM_NET_DDNS_MYQSEE,
	EM_NET_DDNS_DYNDDNS,
	EM_NET_DDNS_PEOPLE,
	EM_NET_MAX,
#endif
} EM_NET_DDNS;

typedef struct _sNetDdnsPara
{
	char szUser[DDNS_USER_LEN_MAX];
	char szPasswd[DDNS_USER_LEN_MAX];
	char szDomain[DDNS_USER_LEN_MAX];
} SNetDdnsPara;

typedef enum {
	UPDATE_TYPE_MAINBOARD 	=  0,
	UPDATE_TYPE_PANEL 		=  1,
	UPDATE_TYPE_STARTLOGO 	=  2,
	UPDATE_TYPE_APPLOGO 	=  3
} E_REMOTE_UPDATE;

typedef struct _sMultiCastInfo
{
	u32		deviceIP; 						//�豸IP  
	u16		devicePort;						//�豸�˿� 
	char 	device_name[32];				//�豸����	
} SMultiCastInfo;

//csp modify 20130423
typedef struct
{
	u8	support;
	u8	wnd_num_threshold;
} SNetThirdStreamProperty;

//csp modify 20130423
s32 NetComm_SetThirdStreamProperty(SNetThirdStreamProperty *psProperty);
s32 NetComm_GetThirdStreamProperty(SNetThirdStreamProperty *psProperty);

s32 NetComm_DDNS_Install(EM_NET_DDNS eProt);
s32 NetComm_DDNS_Start(SNetDdnsPara* pStart);
s32 NetComm_DDNS_Stop( void );
s32 NetComm_DDNS_Register(SNetDdnsPara* pReg);

s32 NetComm_SendMail(SMailContent* pMail);

// extern ..
/*
 ����ģ���ʼ��
 
 ������
	pCfg: PSNetCommCfg
	pReqStreamCB: PStreamRequestCB
	pReqKeyCB : PRequestKeyFrameCB 
	
 ����:
  ���� s32
  ˵�� 0 �ɹ�����0 ʧ��
 
*/
s32 NetCommInit(
	PSNetCommCfg pCfg
);

/*
 ����ģ��Ԥ������Ƶ֡���ͺ���,����ģ���յ��������ͨ���˺�������������
 
 ������
	pHead: PSNetComStmHead
	pBuf: u8*
	
 ����: 
  ���� s32
  ˵�� 0 �ɹ��� ��0 ʧ��

*/
s32 NetCommSendPreviewFrame(PSNetComStmHead pHead, u8* pBuf);

/*
 д���������
 ����: 
 	pConfig: SNetSet
 	
 ����:
  ���� s32
  ˵�� 0 �ɹ��� ��0 ʧ��
 	
*/
s32 NetComm_WritePara( SNetPara* pConfig );


/*
 �����������
 ����: 
 	pConfig: SNetPara
 	
 ����:
  ���� s32
  ˵�� 0 �ɹ��� ��0 ʧ��
 	
*/
s32 NetComm_ReadPara(SNetPara* pConfig);
s32 NetComm_ReadParaEx(SNetPara* pConfig);
s32 NetComm_StartDHCP();
s32 NetComm_StopDHCP();
s32 NetComm_PPPoE_Start(char *pszUser, char* pszPasswd);
s32 NetComm_PPPoE_Stop(u8 flag);
u8 NetComm_GetState();
s32 NetComm_WebRestart(u16 nPort);
s32 NetComm_NetUserCtrl(u8 op, SNetUser* pUser);
void NetComm_InitMultiCastInfo(SNetMultiCastInfo* pMultiCastInfo);
s32 NetComm_DDNSIpUpdate(void);
u8 NetComm_GetVoipState( void );
void NetComm_UpdateProgNotify(u8 nProgress);

//csp modify 20130321
s32 NetComm_RestartUPnP(u8 upnp_enable);

void NetComm_GetMultiCastInfo(SMultiCastInfo* pInfo);
void NetComm_SetMultiCastInfo(SMultiCastInfo* pInfo);

void net_write_upload_alarm(u32 key);
void upload_alarm(u32 key);
void SetRefreshDiskStatue(u8 flag);
u8 GetCheckDiskStatue();

#ifdef __cplusplus
}
#endif

#endif // __NETCOMM_H_

