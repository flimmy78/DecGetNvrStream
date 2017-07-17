#ifndef _BIZ_TYPES_H_
#define _BIZ_TYPES_H_

///////////////////////ҵ����ʼ������ģ�����//////////////////////////
//param used by SBizEventPara
typedef enum
{
    EM_BIZ_EVENT_UNKNOW = -1,
    EM_BIZ_EVENT_RECORD = 0,  //¼��״̬������
    EM_BIZ_EVENT_ALARM_SENSOR, //����������״̬
    EM_BIZ_EVENT_ALARM_VMOTION, //�ƶ����״̬
    EM_BIZ_EVENT_ALARM_VBLIND,  //�ڵ�״̬
    EM_BIZ_EVENT_ALARM_VLOSS, //��Ƶ��ʧ״̬
    EM_BIZ_EVENT_ALARM_OUT, //�������״̬
    EM_BIZ_EVENT_ALARM_BUZZ,  //������״̬
    EM_BIZ_EVENT_ALARM_IPCEXT,//IPC�ⲿ�����������¼�
    EM_BIZ_EVENT_ALARM_IPCCOVER,
    EM_BIZ_EVENT_ALARM_DISK_LOST,
    EM_BIZ_EVENT_ALARM_DISK_ERR,
    EM_BIZ_EVENT_ALARM_DISK_NONE,
    EM_BIZ_EVENT_ALARM_485EXT,
    //yaogang modify 20160201
	//nvr.cpp ��Զ��NVR ����
	EM_BIZ_EVENT_ALARM_RemoteNVR_VLOST,
	EM_BIZ_EVENT_ALARM_RemoteNVR_DISK_ERR,
    EM_BIZ_EVENT_ALARM_RemoteNVR_DISK_LOST,

    EM_BIZ_EVENT_LOCK = 50,
    EM_BIZ_EVENT_RESTART,		//ϵͳ����nDelay������
    EM_BIZ_EVENT_POWEROFF,		//ϵͳ����nDelay��ػ�
    EM_BIZ_EVENT_POWEROFF_MANUAL,	//��ʾ�����ֶ��ػ���

    EM_BIZ_EVENT_TIMETICK = 90,
    EM_BIZ_EVENT_DATETIME_YMD,
	
    EM_BIZ_EVENT_FORMAT_INIT = 100,  //��ʽ����ʼ��
    EM_BIZ_EVENT_FORMAT_RUN, //��ʽ����
    EM_BIZ_EVENT_FORMAT_DONE, //��ʽ������

    EM_BIZ_EVENT_PLAYBACK_INIT = 150,  //�طų�ʼ��
    EM_BIZ_EVENT_PLAYBACK_RUN, //�ط���
    EM_BIZ_EVENT_PLAYBACK_DONE, //�طŽ���

    EM_BIZ_EVENT_BACKUP_INIT = 200,  //���ݳ�ʼ��
    EM_BIZ_EVENT_BACKUP_RUN, //������
    EM_BIZ_EVENT_BACKUP_DONE, //���ݽ���

    
    EM_BIZ_EVENT_UPGRADE_INIT = 250,  //������ʼ��
    EM_BIZ_EVENT_UPGRADE_RUN, //������
    EM_BIZ_EVENT_UPGRADE_DONE, //��������
    EM_BIZ_EVENT_REMOTEUP_START,//cw_remote
    EM_BIZ_EVENT_GETDMINFO,
    EM_BIZ_EVENT_SATARELOAD,//
    EM_BIZ_EVENT_DISKCHANGED,//

    EM_BIZ_EVENT_PREVIEW_REFRESH = 300,
    EM_BIZ_EVENT_LIVE_REFRESH = 301,
    EM_BIZ_EVENT_SHOWTIME_REFRESH = 302,

	EM_BIZ_EVENT_ENCODE_GETRASTER = 350,

	EM_BIZ_EVENT_NET_STATE_DHCP = 400,
	EM_BIZ_EVENT_NET_STATE_PPPOE,
	EM_BIZ_EVENT_NET_STATE_MAIL,
	EM_BIZ_EVENT_NET_STATE_DDNS,
	EM_BIZ_EVENT_NET_STATE_CONN,
	EM_BIZ_EVENT_NET_STATE_UPDATEMAINBOARDSTART,
	EM_BIZ_EVENT_NET_STATE_UPDATEPANNELSTART,
	EM_BIZ_EVENT_NET_STATE_UPDATESTARTLOGOSTART,
	EM_BIZ_EVENT_NET_STATE_UPDATEAPPLOGOSTART,
	EM_BIZ_EVENT_NET_STATE_FORMATSTART,

	EM_BIZ_EVENT_NET_STATE_DHCP_STOP = 410,
	EM_BIZ_EVENT_NET_STATE_PPPOE_STOP,
	EM_BIZ_EVENT_NET_STATE_MAIL_STOP,
	EM_BIZ_EVENT_NET_STATE_DDNS_STOP,
	EM_BIZ_EVENT_NET_STATE_CONN_STOP,

	EM_BIZ_EVENT_NET_STATE_SGUPLOAD,
	
	EM_BIZ_EVENT_NET_CHANGEPARA_RESET,
	
}EMBIZEVENT;

typedef enum
{
	EM_BIZENCODE_FONTSIZE_SMALL,
	EM_BIZENCODE_FONTSIZE_NORMAL,
	EM_BIZENCODE_FONTSIZE_ALL
} EMBIZNCODEFONTSIZE;

typedef struct
{
	u8 nEnable;
	u8 nMainType;
	u8 nSubType;
	u8 ntime;
} SBizMainTainPara;

typedef struct
{
	u8* pBuffer;
	 s32 w;
	 s32 h;
	 s8* strOsd;
	 EMBIZNCODEFONTSIZE emSize;
} SBizRaster;

typedef struct
{
	u64 lTotalSize;
	u64 lCurSize;
	u8 nProgress;	//0~100
} SBizProgress;

#define	BIZ_UPDATE_FILE_NAME_MAX	64
#define	BIZ_UPDATE_FILE_EXT_MAX		6

typedef struct
{
	char szName[BIZ_UPDATE_FILE_NAME_MAX];
	char szExt[BIZ_UPDATE_FILE_EXT_MAX];
	u32  nSize;
	u32  nTimeStamp;
} SBizUpdateFileInfo;

// �����ļ���Ϣ�б�ṹ��
typedef struct
{
	u8 nMax;
	u8 nFileNum;
	SBizUpdateFileInfo* pInfo;
} SBizUpdateFileInfoList;

typedef enum
{	
	EM_BIZUPGRADE_SUCCESS = 1,//�����ɹ�1
	EM_BIZUPGRADE_RUNNING = -198,
	EM_BIZUPGRADE_FAILED_UNKNOW = -199,// ����ʧ�� -199~-100��������������

	EM_BIZBACKUP_SUCCESS = 2,//���ݳɹ�2
	EM_BIZBACKUP_FAILED_UNKNOW = -299,// ����ʧ�� -299~-200��������������
	EM_BIZBACKUP_FAILED_NOSPACE = -298,// ����ʧ�� -299~-200��������������

	EM_BIZFORMAT_SUCCESS = 3,//��ʽ���ɹ�3
	EM_BIZFORMAT_FAILED_UNKNOW = -399,// ��ʽ��ʧ�� -399~-300��������������
	
	EM_BIZSEARCH_SUCCESS = 4,//��ʽ���ɹ�3
	EM_BIZSEARCH_FAILED_UNKNOW = -499,// ��ʽ��ʧ�� -399~-300��������������	

	EM_BIZ_NET_STATE_SUCCESS = 0,
	EM_BIZ_NET_STATE_FAIL = -599,	// ����ģ�����ʧ��
} EMBIZRESULT;

//for SBizPreviewPara.emBizPreviewMode /SBizPreviewData
typedef enum
{
    EM_BIZPREVIEW_1SPLIT = 1,
    EM_BIZPREVIEW_4SPLITS = 4, //nModePara=0~�����Ƶͨ����- 1; 0~3 for 1-4; 4-7 for 5-8; 8-11 for 9-12 ....  
    EM_BIZPREVIEW_6SPLITS = 6, //nModePara=0~�����Ƶͨ���� - 1; 0~5 for 1-6; 6-11 for 7-12 ��nModePara ����ͨ����
    EM_BIZPREVIEW_8SPLITS = 8, //����EM_BIZPREVIEW_6SPLITS
    EM_BIZPREVIEW_9SPLITS = 9, //����EM_BIZPREVIEW_4SPLITS
    EM_BIZPREVIEW_10SPLITS = 10, //����EM_BIZPREVIEW_4SPLITS
    EM_BIZPREVIEW_12SPLITS = 12, //����EM_BIZPREVIEW_6SPLITS
    EM_BIZPREVIEW_13SPLITS = 13, //����EM_BIZPREVIEW_6SPLITS
    EM_BIZPREVIEW_16SPLITS = 16, //����EM_BIZPREVIEW_4SPLITS
    EM_BIZPREVIEW_25SPLITS = 25,//24,
    EM_BIZPREVIEW_24SPLITS = 24,//25, //����EM_BIZPREVIEW_4SPLITS
    EM_BIZPREVIEW_36SPLITS = 36,//32, //����EM_BIZPREVIEW_6SPLITS
    EM_BIZPREVIEW_32SPLITS = 32,//36, //����EM_BIZPREVIEW_4SPLITS
    EM_BIZPREVIEW_64SPLITS = 64, //����EM_BIZPREVIEW_4SPLITS
    EM_BIZPREVIEW_HideItem = 253,//���ؿؼ�
    EM_BIZPREVIEW_PIP = 254, //
    EM_BIZPREVIEW_CLOSE = 255,   //
} EMBIZPREVIEWMODE;

//Ԥ��״̬���»ص�
typedef struct
{
    EMBIZPREVIEWMODE emBizPreviewMode; //Ԥ�����ڷָ�ģʽ
    u8 nModePara; //extend parameter for EMBIZPREVIEWMODE
    u8 nAdioChn; //��ǰ��Ƶ���ͨ��
    u8 nMute; //�������
    u8 nVolume; //�������
} SBizPreviewData;

typedef enum
{
    EM_BIZREC_MASK_MANUAL = (1 << 0), //�ֶ�¼��
    EM_BIZREC_MASK_TIMING = (1 << 1), //��ʱ¼��
    EM_BIZREC_MASK_MOTION = (1 << 2), //�ƶ����¼��
    EM_BIZREC_MASK_SENSORALARM = (1 << 3), //����������¼��
} EMBIZRECSTATUSMASK;

typedef struct
{
    u8 nChn; //ͨ����
    u8 nLastStatus; //�ϴ�״̬ // >0 ��ʾ����¼��ÿλ����һ�����ͣ���ο�EMBIZRECSTATUSMASK
    u8 nCurStatus; //��ǰ״̬ // >0 ��ʾ����¼��ÿλ����һ�����ͣ���ο�EMBIZRECSTATUSMASK
} SBizRecStatus;

typedef struct
{
    u8 nChn; //ͨ���Ż��ߴ�����id
    u8 nLastStatus; //�ϴ�״̬ // >0 ��ʾ�б���, 0��ʾ��
    u8 nCurStatus; //�ϴ�״̬ // >0 ��ʾ�б���, 0��ʾ��
    time_t nTime; //���һ�α���ʱ��
    //yaogang modify 20160201
	//nvr.cpp ��Զ��NVR ����
    u32 nvrIP;
} SBizAlarmStatus;

// 2.20 ����ʱ���ʽ
typedef enum
{
	//�����ټ�
	EM_BIZDATETIME_YYYYMMDDHHNNSS, //year-month-day
	EM_BIZDATETIME_MMDDYYYYHHNNSS,
	EM_BIZDATETIME_DDMMYYYYHHNNSS //day-month-year
} EMBIZDATETIMEFORMAT;

typedef enum
{
	//�����ټ�
	EM_BIZTIME_24HOUR, 
	EM_BIZTIME_12HOUR,
} EMBIZTIMEFORMAT;

// 2.19 ����ʱ��
typedef struct
{
	u16 nYear;
	u8 nMonth;
	u8 nDay;
	u8 nHour;
	u8 nMinute;
	u8 nSecond;
	u8 nWday;
	EMBIZDATETIMEFORMAT emDateTimeFormat;
	EMBIZTIMEFORMAT emTimeFormat;
} SBizDateTime;

typedef struct
{
	u8  nType; // 0 hdd, 1 usb
	u32 nTotal;
	u32 nFree;
	char szMountPoint[32];
} SBizDMInfo;

// remote format
typedef struct 
{
	u8 nDiskIdx;
	EMBIZRESULT emBizResult;
} SBizRemoteFormat;

// 2.15 �豸��Ϣ
#ifndef HDD_UNFORMAT
#define HDD_UNFORMAT 0 //δ��ʽ��
#endif
#ifndef HDD_FORMATTING
#define HDD_FORMATTING 1 //��ʽ����
#endif
#ifndef HDD_INUSE
#define HDD_INUSE 2 //ʹ����
#endif
#ifndef HDD_IDLE
#define HDD_IDLE 3 //����
#endif
#ifndef HDD_SLEEP
#define HDD_SLEEP 4 //����
#endif
#ifndef HDD_ERROR
#define HDD_ERROR 5 //����
#endif

typedef struct
{
	u8 nType;//0 hdd; 1 usb
	char strDevPath[32];
	u8 nPartitionNo;
	u32 nStatus;//u8 nStatus;//0 ok; 1 unformated; 2 err
	u32 nTotal;
	u32 nFree;
	//u8 storage_type;//'s'->sata, 'u'->'usb'
	u8 disk_physical_idx;//������Ӳ�̵�������
	u8 disk_logic_idx;//������Ӳ�̵��߼����
	u8 disk_system_idx;//ϵͳ��⵽��Ӳ�̱��sda->0, sdb->1...
	//char disk_name[32];//Ӳ���豸��/dev/sda, /dev/sdb...
	char disk_sn[64];//Ӳ�����кš�Ψһ
} SBizDevInfo;

// 2.14 ��ô洢�豸������Ϣ
typedef struct
{
	u8 		  nDiskNum;
	SBizDevInfo*  psDevList;
} SBizStoreDevManage;

//param used by FPBIZEVENTCB
typedef struct
{
	EMBIZEVENT emType;
	union
	{
		u16 nDelay;
		SBizProgress sBizProgress; //����
		EMBIZRESULT emBizResult; //ִ�н��ö��
		SBizPreviewData sPreviewData; //��ǰԤ��ģʽ
		SBizRecStatus sBizRecStatus;
		SBizAlarmStatus sBizAlaStatus;
		s8* pstrDateTime;
		SBizDateTime sBizDateTime;
		SBizRaster sBizRaster;
		SBizDMInfo sDmInfo;
		SBizStoreDevManage sStoreMgr;
		SBizRemoteFormat sBizFormat;
	};   
} SBizEventPara;

//param used by  SBizInitPara.pfnBizEventCb
//typedef void (* FNBIZEVENTCB)(SBizEventPara* sBizEventPara); //ҵ����¼��ص�����
typedef int (* FNBIZEVENTCB)(SBizEventPara* sBizEventPara); //ҵ����¼��ص�����

//ģ������ȡֵ����
#define MASK_BIZ_MOD_NONE 0 //������ģ��
//******mask definition for each module start******
#define MASK_BIZ_MOD_PREVIEW (1 << 0) //Ԥ��
#define MASK_BIZ_MOD_ENCODE (1 << 1) //����
#define MASK_BIZ_MOD_RECORD (1 << 2) //¼��
#define MASK_BIZ_MOD_PLAYBACK (1 << 3) //�ط�
#define MASK_BIZ_MOD_ALARM (1 << 4) //����
#define MASK_BIZ_MOD_CONFIG (1 << 5) //����
#define MASK_BIZ_MOD_SYSCOMPLEX (1 <<6) //ϵͳ����
#define MASK_BIZ_MOD_PTZ (1 <<7) //ϵͳ����
#define MASK_BIZ_MOD_USER (1 <<8) //�û�
#define MASK_BIZ_MOD_LOG (1 <<9) //��־
#define MASK_BIZ_MOD_NET (1 << 15) //����
//yaogang modify 20150324
#define MASK_BIZ_MOD_SNAP (1 << 16) //ץͼ--biz_snap.cpp

//******mask definition for each module end******
#define MASK_BIZ_MOD_ALL 0xffffffff //��������ģ��

//ģ������nModMask
//param used by BizInit
typedef struct
{
	u32 nModMask;
	FNBIZEVENTCB pfnBizEventCb;
} SBizInitPara;

///////////////////////Ԥ�����//////////////////////////
//for BizElectronicZoom
typedef struct{
	unsigned int s32X;
	unsigned int s32Y;
	unsigned int u32Width;
	unsigned int u32Height;
}SBizPreviewElecZoom;
//for BizStartPreview
typedef struct
{
    EMBIZPREVIEWMODE emBizPreviewMode; //Ԥ�����ڷָ�ģʽ
    u8 nModePara; //extend parameter for EMBIZPREVIEWMODE
} SBizPreviewPara;

// for EMBIZPARATYPE.EM_BIZ_PATROL
typedef struct
{
    u8 nIsPatrol; //��Ѳ�Ƿ�����(����:0=��;1=��)
    u8 nPatrolChnNum;//��Ѳͨ����16 32 48 64
    u8 nInterval; //��Ѳ�л�ʱ��(����:��λ��s)
    u8 nPatrolMode; //��Ѳʱ���õ�Ԥ��ģʽ(����:�ο�Mode)
    u8 nStops; //��Ѳվ��(����:һ���ֻ�ͣ������)
    u8 pnStopModePara[255]; //��Ѳÿվ��ʹ�õ�Ԥ����չ����(�����б�:�ο�ModePara)
} SBizCfgPatrol;

///////////////////////�ط�ģ�����//////////////////////////
typedef enum
{
	EM_BIZPLAY_TYPE_FILE = 0,
	EM_BIZPLAY_TYPE_TIME,
} EMBIZPLAYTYPE;

typedef enum
{
	EM_BIZCTL_NULL=0,
	EM_BIZCTL_STOP,
	EM_BIZCTL_PAUSE,
	EM_BIZCTL_RESUME,
	EM_BIZCTL_STEP,
	EM_BIZCTL_PRE_SECT,
	EM_BIZCTL_NXT_SECT,
	EM_BIZCTL_SPEED_DOWN,
	EM_BIZCTL_SPEED_UP,
	EM_BIZCTL_SET_SPEED,
	EM_BIZCTL_SEEK,
	EM_BIZCTL_FORWARD,
	EM_BIZCTL_BACKWARD,
	EM_BIZCTL_MUTE,
	EM_BIZCTL_PBTIME,
	EM_BIZCTL_PBFILE
} EMBIZPLAYCTRLCMD;

///////////////////////��̨ģ����ؿ�������//////////////////////////
typedef enum
{
	EM_BIZPTZ_CMD_START_TILEUP = 0,
	EM_BIZPTZ_CMD_START_TILEDOWN,
	EM_BIZPTZ_CMD_START_PANLEFT,
	EM_BIZPTZ_CMD_START_PANRIGHT,
	EM_BIZPTZ_CMD_START_LEFTUP,
	EM_BIZPTZ_CMD_START_LEFTDOWN,
	EM_BIZPTZ_CMD_START_RIGHTUP,
	EM_BIZPTZ_CMD_START_RIGHTDOWN,
	EM_BIZPTZ_CMD_START_ZOOMWIDE,
	EM_BIZPTZ_CMD_START_ZOOMTELE,
	EM_BIZPTZ_CMD_START_FOCUSNEAR,
	EM_BIZPTZ_CMD_START_FOCUSFAR,
	EM_BIZPTZ_CMD_START_IRISSMALL,
	EM_BIZPTZ_CMD_START_IRISLARGE,
	EM_BIZPTZ_CMD_START_WIPEROPEN,
	EM_BIZPTZ_CMD_START_WIPERCLOSE,
	//
	EM_BIZPTZ_CMD_STOP_TILEUP = 30,
	EM_BIZPTZ_CMD_STOP_TILEDOWN,
	EM_BIZPTZ_CMD_STOP_PANLEFT,
	EM_BIZPTZ_CMD_STOP_PANRIGHT,
	EM_BIZPTZ_CMD_STOP_LEFTUP,
	EM_BIZPTZ_CMD_STOP_LEFTDOWN,
	EM_BIZPTZ_CMD_STOP_RIGHTUP,
	EM_BIZPTZ_CMD_STOP_RIGHTDOWN,
	EM_BIZPTZ_CMD_STOP_ZOOMWIDE,
	EM_BIZPTZ_CMD_STOP_ZOOMTELE,
	EM_BIZPTZ_CMD_STOP_FOCUSNEAR,
	EM_BIZPTZ_CMD_STOP_FOCUSFAR,
	EM_BIZPTZ_CMD_STOP_IRISSMALL,
	EM_BIZPTZ_CMD_STOP_IRISLARGE,
	EM_BIZPTZ_CMD_STOP_WIPEROPEN,
	EM_BIZPTZ_CMD_STOP_WIPERCLOSE,
	
	EM_BIZPTZ_CMD_STOP_ALL = 99,

	EM_BIZPTZ_CMD_PRESET_SET = 100,
	//
	EM_BIZPTZ_CMD_PRESET_GOTO = 102,
	//
	EM_BIZPTZ_CMD_AUTOSCAN_ON = 110,
	EM_BIZPTZ_CMD_AUTOSCAN_OFF,

	EM_BIZPTZ_CMD_START_TOUR = 130,
	EM_BIZPTZ_CMD_STOP_TOUR,
	EM_BIZPTZ_CMD_START_TOUR_AUTO = 134,
	EM_BIZPTZ_CMD_STOP_TOUR_AUTO,
	
	//
	EM_BIZPTZ_CMD_START_PATTERN = 140,
	EM_BIZPTZ_CMD_STOP_PATTERN,
	EM_BIZPTZ_CMD_STARTREC_PATTERN,
	EM_BIZPTZ_CMD_STOPREC_PATTERN,

	//
	EM_BIZPTZ_CMD_AUX_ON = 150,
	EM_BIZPTZ_CMD_AUX_OFF,
	//
	EM_BIZPTZ_CMD_LIGHT_ON = 160,
	EM_BIZPTZ_CMD_LIGHT_OFF,
	//
	EM_BIZPTZ_CMD_SETSPEED = 253,
	EM_BIZPTZ_CMD_NULL = 254,
}EMBIZPTZCMD;

///////////////////////����ģ�����//////////////////////////
// 2.6 ϵͳ�˳�����ö��
typedef enum
{
	EM_BIZSYSEXIT_LOCK,
	EM_BIZSYSEXIT_RESTART,
	EM_BIZSYSEXIT_POWEROFF,
} EMBIZSYSEXITTYPE;

// mainboard || panel
typedef enum
{
	EM_BIZUPGRADE_MAINBOARD,
	EM_BIZUPGRADE_PANEL,
	EM_BIZUPGRADE_STARTLOGO,
	EM_BIZUPGRADE_APPLOGO,
} EMBIZUPGRADETARGET;

// 2.7 �ļ�����ö��
typedef enum
{
	EM_BIZFILE_REC = 0,
	EM_BIZFILE_SNAP,
} EMBIZFILETYPE;

// 2.12 �ļ���ʽ
typedef enum
{
	EM_BIZFORMAT_ORIGINAL = 0,
	EM_BIZFORMAT_AVI,
	EM_BIZFORMAT_JPEG,
	EM_BIZFORMAT_BMP,
} EMBIZFILEFORMAT;

// backup device type
typedef enum
{
	EM_BIZBACKUP_USB,
	EM_BIZBACKUP_DVDR,
	
} EMBIZBACKUPDEV;

typedef enum
{
	EM_BIZDVDRW_MODE_NEW,
	EM_BIZDVDRW_MODE_ADD,
	
} EMBIZBACKUPDVDRWMODE;

// Backup target attributes
typedef struct
{
	EMBIZFILEFORMAT fmt;
	EMBIZBACKUPDEV  devt;
	union
	{
		u8 reserve;
		EMBIZBACKUPDVDRWMODE mode;
	};
} SBizBackTgtAtt, *PSBizBackTgtAtt;

// 2.8 �ļ�������ʽö��
typedef enum
{
	EM_BIZSEARCH_BY_TIME,
	EM_BIZSEARCH_BY_FILE,
} EMBIZSEARCHWAY;

// 2.10 ��ʱ����������
typedef struct 
{
	u8 nMaskType;
	u32 nMaskChn;
	u32 nStartTime;
	u32 nEndTime;
	void* pReserverInfo;
} SBizSearchPara;

// 2.17  �ļ���Ϣ����
typedef struct
{
	u8 nChn;
	u8 nType;
	u32 nStartTime;
	u32 nEndTime;
	u8 nFormat;  //N��ʽ�� 10 = D1��13 = CIF  P��ʽ�� 0 = D1�� 3 = CIF    
	u8 nStreamFlag;  //0:��Ƶ��;1:��Ƶ��
	u32 nSize;
	u32 nOffset;
	u8 nDiskNo;
	u8 nPtnNo;
	u16 nFileNo;
} PACK_NO_PADDING SBizRecfileInfo;
typedef struct
{
	u8 nChn;
	u8 nType;
	u8 nFormat;  //��ʽ,0-jpeg,1-bmp,Ĭ��jpeg 
	u8 nReserver;
	u16 	width;
	u16	height;
	u32 nSize;
	u32 nOffset;
	u8 nDiskNo;
	u8 nPtnNo;
	u16 nFileNo;
	u32 nStartTime;
	u32 nStartTimeus;
} PACK_NO_PADDING SBizRecSnapInfo;

// 2.11 �ļ���������ṹ��
typedef struct
{
	u16 nFileNum;
	union
	{
		SBizRecfileInfo *psRecfileInfo;//���ڴ�����ݵ��ļ���Ϣ����
		SBizRecSnapInfo *psRecSnapInfo;
	};
} PACK_NO_PADDING SBizSearchResult;

// 2.9 ������������
typedef struct
{
	EMBIZSEARCHWAY emSearchWay;
	union
	{
		SBizSearchPara sSearchPara;
		SBizSearchResult sSearchResult;
	};
} SBizSearchCondition;


// 2.16 ��ʽ����ʽ
typedef enum
{
	EM_BIZFORMAT_FAT32_IFV,	//��ʽ��Ϊfat32����ʼ��¼���ʽ�ļ�
	EM_BIZFORMAT_FAT32,	//��ʽ��Ϊfat32
} EMBIZFORMATWAY;


// 2.18 ϵͳ��Ϣ
typedef struct
{
	
} SBizSysComplexInfo; //��������

///////////////////////����ģ�����//////////////////////////
typedef enum
{
	EM_BIZCONFIG_PARA_SYSTEM = 0,
	EM_BIZCONFIG_PARA_PREVIEW = 100,
	EM_BIZCONFIG_PARA_IMAGE = 200,
	EM_BIZCONFIG_PARA_MAIN_OSD = 300,
	EM_BIZCONFIG_PARA_STR_OSD = 330,
	EM_BIZCONFIG_PARA_RECT_OSD = 360,	
	EM_BIZCONFIG_PARA_SENSOR = 400,
	EM_BIZCONFIG_PARA_VMOTION = 500,
	EM_BIZCONFIG_PARA_VLOST = 600,
	EM_BIZCONFIG_PARA_VBLIND = 700,
	EM_BIZCONFIG_PARA_ALARMOUT = 800,
	EM_BIZCONFIG_PARA_BUZZ = 900,
	EM_BIZCONFIG_PARA_SENSOR_SCH = 1000,
	EM_BIZCONFIG_PARA_VMOTION_SCH = 1100,
	EM_BIZCONFIG_PARA_VLOST_SCH = 1200,
	EM_BIZCONFIG_PARA_VBLIND_SCH = 1300,
	EM_BIZCONFIG_PARA_ALARMOUT_SCH = 1400,
	EM_BIZCONFIG_PARA_BUZZ_SCH = 1500,	
	//
	EM_BIZCONFIG_PARA_ENC_MAIN = 4000,
	EM_BIZCONFIG_PARA_ENC_SUB = 4100,
	//
	EM_BIZCONFIG_PARA_RECORD = 6000,
	EM_BIZCONFIG_PARA_TIMER_REC_SCH = 6100,
	EM_BIZCONFIG_PARA_SENSOR_REC_SCH = 6200,
	EM_BIZCONFIG_PARA_VMOTION_REC_SCH = 6300,
	EM_BIZCONFIG_PARA_VLOST_REC_SCH = 6400,
	EM_BIZCONFIG_PARA_VBLIND_REC_SCH = 6500,	
	//
	EM_BIZCONFIG_PARA_USER = 8000,
	EM_BIZCONFIG_PARA_PTZ = 8100,
	EM_BIZCONFIG_PARA_NETWORK = 8200,	
	//
	EM_BIZCONFIG_PARA_DVR_PROPERTY = 9000,
	//
	EM_BIZCONFIG_PARA_ALL = 10000,
} EMBIZCONFIGPARATYPE;

///////////////////////����ģ�����//////////////////////////
//��������(������/������)
typedef enum
{
	EM_BIZENC_BITRATE_VARIABLE = 0, 		
	EM_BIZENC_BITRATE_CONST, 				
}EMBIZENCBITRATETYPE;

//ͼ��ֱ��� 0=QCIF/1=CIF/2=HALF_D1/3=4Cif/4=D1
typedef enum
{
	EM_BIZENC_RESOLUTION_QCIF = 0,
	EM_BIZENC_RESOLUTION_CIF = 1,
	EM_BIZENC_RESOLUTION_HD1 = 2,
	EM_BIZENC_RESOLUTION_4CIF = 3,
	EM_BIZENC_RESOLUTION_D1 = 4,
	EM_BIZENC_RESOLUTION_720P = 5,
	EM_BIZENC_RESOLUTION_1080P = 6,
	EM_BIZENC_RESOLUTION_960H = 7,
}EMBIZENCRESOLUTION;

//ͼ������(�����б�:0=���/1=����/2=��/3=һ��/4=��/5=����/6=���)
typedef enum
{
	EM_BIZENC_PICLEVEL_BEST = 0, 		
	EM_BIZENC_PICLEVEL_BETTER = 1,
	EM_BIZENC_PICLEVEL_GOOD = 2,
	EM_BIZENC_PICLEVEL_NORMAL =3,
	EM_BIZENC_PICLEVEL_BAD =4,
	EM_BIZENC_PICLEVEL_WORSE = 5,
	EM_BIZENC_PICLEVEL_WORST = 6,
} EMBIZENCPICLEVEL;

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
} SBizEncodePara;

//����ʱ���
typedef struct 
{
	u32 nStartTime;	//ÿ��ʱ��ε���ʼʱ��
	u32 nStopTime;	//����ʱ��
} SBizSchTime;

//�������ͣ�Ŀǰ��֧��EM_ALARM_SCH_WEEK_DAY
typedef enum					//��������ö��
{
	EM_BIZ_SCH_WEEK_DAY = 0,		//ÿ�ܵ��ļ���
	EM_BIZ_SCH_MONTH_DAY,		//ÿ�µ��ļ���
	EM_BIZ_SCH_EVERY_DAY, 	//ÿ��
	EM_BIZ_SCH_ONCE_DAY,		//ֻһ����Ч / ����
} EMBIZSCHTYPE;

#define MAX_SCHEDULE_SEGMENTS 12	//ÿ������õ�ʱ��ε������Ŀ

typedef struct 
{
	u8 nSchType; //������ʱ������	EMBIZSCHTYPE
	SBizSchTime sSchTime[31][MAX_SCHEDULE_SEGMENTS];	//���β���ʱ��
} SBizSchedule;

///////////////////////��̨ģ�����//////////////////////////
//SBizPtzPara
typedef enum
{
	EM_BIZ_BAUDRATE_110 	= 110,
	EM_BIZ_BAUDRATE_300 	= 300,
	EM_BIZ_BAUDRATE_600 	= 600,
	EM_BIZ_BAUDRATE_1200 	= 1200,
	EM_BIZ_BAUDRATE_2400 	= 2400,
	EM_BIZ_BAUDRATE_4800 	= 4800,
	EM_BIZ_BAUDRATE_9600 	= 9600,
	EM_BIZ_BAUDRATE_19200 	= 19200,
	EM_BIZ_BAUDRATE_38400 	= 38400,
	EM_BIZ_BAUDRATE_57600 	= 57600,
	EM_BIZ_BAUDRATE_115200 	= 115200,
	EM_BIZ_BAUDRATE_230400 	= 230400,
	EM_BIZ_BAUDRATE_460800 	= 460800,
	EM_BIZ_BAUDRATE_921600 	= 921600,
} EMBIZBAUDRATE;

//SBizPtzPara
typedef enum
{
	EM_BIZ_DATABIT_6 = 6,
	EM_BIZ_DATABIT_7 = 7,
	EM_BIZ_DATABIT_8 = 8,
	//
} EMBIZZDATABIT;

//SBizPtzPara
typedef enum
{
	EM_BIZ_STOPBIT_1 = 1,
	EM_BIZ_STOPBIT_2 = 2,
	//
} EMBIZSTOPBIT;

//SBizPtzPara
typedef enum
{
	EM_BIZ_CRCCHECK_TYPE_NONE = 0,	//无校�?
	EM_BIZ_CRCCHECK_TYPE_ODD,		//奇校�?
	EM_BIZ_CRCCHECK_TYPE_EVEN,		//偶校�?
	//
} EMBIZCRCCHECKTYPE;

//SBizPtzPara
typedef enum
{
	EM_BIZ_FLOWCTRL_NONE = 0,
	EM_BIZ_FLOWCTRL_HARDWARE,
	EM_BIZ_FLOWCTRL_XON_XOFF,
	//
} EMBIZFLOWCTRL;

//SBizTourPath
typedef struct
{
    u8 nPresetPos; //Ԥ�õ��
    u16 nDwellTime; //����ʱ��
    u8 nSpeed; //�ٶȹ�10��
} SBizTourPoint;

#define TOUR_POINT_MAX	128
#define TOUR_PATH_MAX 	32
#define PRESET_MAX		128

//SBizPtzAdvancedPara
typedef struct
{
    u8 nPathNo;
    SBizTourPoint sTourPoint[TOUR_POINT_MAX];
} SBizTourPath;

//SBizPtzPara
typedef struct
{
	u8 nIsPresetSet[PRESET_MAX]; //Ԥ�õ�λ�����128��//0��ʾ������ ��1��ʾ����
	SBizTourPath sTourPath[TOUR_PATH_MAX]; //sTourPath[128];
} SBizPtzAdvancedPara;

//EM_BIZ_PTZPARA
typedef struct
{
	u8 nCamAddr; // ��̨��ַ (0-255)
	u32 nBaudRate; // ������EMBIZBAUDRATE
	u8 nDataBit; // ����λEMBIZZDATABIT
	u8 nStopBit; // ֹͣλEMBIZSTOPBIT
	u8 nCheckType; // У������EMBIZCRCCHECKTYPE
	u8 nFlowCtrlType; // ���ط�ʽEMBIZFLOWCTRL
	u8 nProtocol; // Э��index
	u8 nEnable; // �Ƿ�����(����:0=��;1=��)
    SBizPtzAdvancedPara sAdvancedPara;
} SBizPtzPara;

///////////////////////����ģ�����//////////////////////////
#define MAX_ALANAME_LEN 16 

//���������/������������ƽ(�����б�:0=����/�͵�ƽ;1=����/�ߵ�ƽ)
typedef enum
{
    	EM_BIZ_ALARM_LOW_LEVEL = 0, //�͵�ƽʱ����
    	EM_BIZ_ALARM_HIGH_LEVEL = 1, //�ߵ�ƽʱ����
    	EM_BIZ_ALARM_NORMAL_CLOSE = 0, //�պ�ʱΪ��̬��������
    	EM_BIZ_ALARM_NORMAL_OPEN = 1, //�Ͽ�ʱΪ��̬��������
} EMBIZALARMLEVELTYPE;

//EM_BIZ_SENSORPARA
typedef struct
{
	u8 nEnable; //�Ƿ�����(0=��;1=��)
	u8 nType; // ����(EMBIZALARMLEVELTYPE)
	u16 nDelay; // ��ʱʱ�� 
	u8 nSchType; // ��������(EMBIZSCHTYPE)
	s8 name[MAX_ALANAME_LEN + 1];
} SBizAlarmSensorPara;

//yaogang modify 20141010
//EM_BIZ_IPCEXTSENSORPARA
typedef struct
{
	u8 nEnable; //�Ƿ�����(0=��;1=��)
	u16 nDelay; // ��ʱʱ�� 
} SBizAlarmIPCExtSensorPara;
typedef struct
{
	u8 nEnable; //�Ƿ�����(0=��;1=��)
	//u16 nDelay; // ��ʱʱ�� 
} SBizAlarmHDDPara;


//EM_BIZ_VMOTIONPARA
typedef struct
{
    u8 nEnable; //�Ƿ�����
    u8 nSensitivity; //������
    u16 nDelay; //��ʱ
    u8 nSchType; // ��������(EMBIZSCHTYPE)
    u64 nBlockStatus[64]; //��Ӧ�����״̬
} SBizAlarmVMotionPara;

//EM_BIZ_VLOSTPARA
typedef struct
{
	u8 nEnable;	
	u16 nDelay;
	u8 nSchType; // ��������(EMBIZSCHTYPE)
} SBizAlarmVLostPara;

//EM_BIZ_VBLINDPARA
typedef struct
{
	u8 nEnable;
	u16 nDelay;
	u8 nSchType; // ��������(EMBIZSCHTYPE)
} SBizAlarmVBlindPara;

//EM_BIZ_ALARMOUTPARA
typedef struct
{
    u8 nType; //EMBIZALARMLEVELTYPE
    u8 nEnable;	//�Ƿ�����
    u16 nDelay; //��ʱ	
    s8 name[MAX_ALANAME_LEN + 1];//�Լ���������
    u8 nSchType; // ��������(EMBIZSCHTYPE)
} SBizAlarmOutPara;

//EM_BIZ_ALARMBUZZPARA
typedef struct
{
    u8 nEnable; //�Ƿ�����
    u16 nDelay; //��ʱ	
} SBizAlarmBuzzPara;

//��̨��������ѡ��(0=��;1=Ԥ�õ�;2=Ѳ��;3=�켣)
typedef enum
{
	EM_BIZALARM_PTZ_NULL = 0, //������
	EM_BIZALARM_PTZ_PRESET, //����Ԥ�õ�
	EM_BIZALARM_PTZ_PATROL, //Ѳ����
	EM_BIZALARM_PTZ_LOCUS, //�켣
} EMBIZALARMPTZTYPE;

//SBizAlarmDispatch
typedef struct
{
	u8 nALaPtzType; //��̨��������EMBIZALARMPTZTYPE
	u8 nChn; //ͨ����
	u8 nId; //��ʾԤ�õ�id/Ѳ����·id/�켣(��Ч)
} SBizAlarmPtz;

//EM_BIZ_DISPATCH_SENSOR
//EM_BIZ_DISPATCH_VMOTION
//EM_BIZ_DISPATCH_VBLIND
//EM_BIZ_DISPATCH_VLOST
typedef struct
{
	//;;;�¼���������
	u8 nFlagBuzz; // ��������(����:0=��;1=��)
	u8 nZoomChn; // ���汨��ͨ����(255 ��ʾ��Ч)
	u8 nFlagEmail; // �ʼ�(0=��;1=��)
	u8 nSnapChn[64]; // ��Ŵ���ץͼ��ͨ���� (���ͬʱ����64��,255 ��ʾ��Ч)
	u8 nAlarmOut[64]; // �������� (���ͬʱ����64��,255 ��ʾ��Ч)
	u8 nRecordChn[64]; // ����¼��ͨ�� (���ͬʱ����64��,255 ��ʾ��Ч)
	SBizAlarmPtz sAlarmPtz[64]; //���ͬʱ����64������
} SBizAlarmDispatch;

typedef SBizSchedule SBizAlarmSchedule;

//��Ƶ��ʽ(���� : 12-PAL; 10-NTSC)
typedef enum
{
	EM_BIZ_NTSC = 10,
	EM_BIZ_PAL = 12,
} EMBIZVIDSTANDARD;

//���(���� : 0-CVBS;1-VGA 800X600;2-VGA 1024X768;3-VGA 1280X1024)
typedef enum
{
	EM_BIZ_OUTPUT_CVBS = 0,
	EM_BIZ_OUTPUT_VGA_800X600 = 1,
	EM_BIZ_OUTPUT_VGA_1024X768 = 2,
	EM_BIZ_OUTPUT_VGA_1280X1024 = 3,
	//
	EM_BIZ_OUTPUT_HDMI_1280X720 = 4,
	EM_BIZ_OUTPUT_HDMI_1920X1080 = 5,
} EMBIZOUTPUT;

//���� 
typedef enum
{
	EM_BIZ_LANG_ENG = 0, //Ӣ��
	EM_BIZ_LANG_SCH = 1, //���ļ���
	EM_BIZ_LANG_TCH = 2, //���ķ���
} EMBIZLANGUAGE;

 //����ʱ���ʽ(���� : 0=������;1=������;2=������)
typedef enum
{
	EM_BIZ_DATEFMT_YYMMDD = 0,
	EM_BIZ_DATEFMT_MMDDYY = 1,
	EM_BIZ_DATEFMT_DDMMYY = 2,
} EMBIZDATEFORMAT;

 //ʱ����ʽ(���� : 0=24Сʱ��;1=12Сʱ��)
 typedef enum
{
	EM_BIZ_TIMESTD_24H = 0,
	EM_BIZ_TIMESTD_12H = 1,
} EMBIZTIMESTANDARD;

//EM_BIZ_SYSTEMPARA
typedef struct
{
    s8 strDevName[32]; //�豸��(�ַ���)
    u16 nDevId; //�豸��(���� : 0-65535)
    u8 nManual;
    u8 nVideoStandard; //��Ƶ��ʽ(EMBIZVIDSTANDARD)
    u8 nAuthCheck; //�Ƿ�У��Ȩ��(���� : 0-��У��; 1-У��)
    u8 nOutput; //���(EMBIZOUTPUT)
    u8 nLangId; //����(EMBIZLANGUAGE)
    u8 nShowGuide; //�Ƿ���ʾ������(���� : 0-��; 1-��)
    u16 nLockTime; //��¼�޲�����ʱ����ʱ��(����:0��ʾ����; ��λ��s)
    u8 nDateFormat; //����ʱ���ʽ(EMBIZDATEFORMAT)
    u8 nTimeStandard; //ʱ����ʽ(EMBIZTIMESTANDARD)
    u8 nCyclingRecord; //�Ƿ�ѭ��¼��(����:0=��;1=��)
    u8 nShowFirstMenue;
	//csp modify
	s8 sn[32];
	//QYJ
	u8 nTimeZone;
	u8 nSyscCheck;
	s8 strNtpServerAdress[32];
	u8 nSyncTimeToIPC;
	u8 P2PEnable;
	//QYJ
	u8 reserved[256-36];//QYJ
} SBizSystemPara;

typedef struct
{
	unsigned char channel_no;
	unsigned char enable;
	unsigned char stream_type;
	unsigned char trans_type;
	unsigned int protocol_type;
	unsigned int dwIp;
	unsigned short wPort;
	unsigned char force_fps;
	unsigned char frame_rate;
	char user[32];
	char pwd[32];
	char name[32];
	char uuid[64];
	char address[64];//onvifʹ��
	char ipc_type;
	//char reserved[63];
	//yaogang modify for yuetian private	
	char main_audio_sw;//��������Ƶ���أ�NVR���ͨ����
	char sub_audio_sw;//��������Ƶ���أ�NVR�Լ���ͨ���ţ�ָʾҪ����NVR ����һ·����
	char reserved[61];
} SBizIPCameraPara;

//EM_BIZ_PREVIEWPARA
typedef struct
{
    u8 nMode; //Ԥ��ģʽ(EMBIZPREVIEWMODE)
    u8 nModePara; //Ԥ��ģʽ��չ����(����:��ģʽ�µ����Ͻ�ͨ����)

    u8 nAudioOutChn; //��Ƶ���ͨ�� (����:255��ʾ�Զ�ѡ��)
    u8 nVolume; //��Ƶ�������(����: 0~100)
    u8 nMute; //Ԥ������(���� : 0=�Ǿ���;1=����)

    u8 nShowTime; // ����ʾʱ��(�����б�:0=��;1=��)
    //yaogang modify 20140918
    u8 nShowChnKbps;// �Ƿ���ʾͨ����ʵʱ��������
    SPoint sTimePos; // ʱ��XY����
    u8 nShowRecState; // ¼��״̬��ʾ���(�����б�:0=��;1=��)

    u8 nVideoSrcType[128];	//��Ƶ����Դ������
    
} SBizCfgPreview;

//������/��Ƶ��(0=��Ƶ��/1=������)
typedef enum
{
	EM_BIZ_REC_VID_ONLY = 0,
	EM_BIZ_REC_VID_AUDIO,
} EMBIZRECSTREAMTYPE;

//¼���Ӧ�ı���ͨ������ (0=������/1=������)
typedef enum
{
	EM_BIZ_CHN_VIDEO_SUB = 0, 		//��Ƶ��ͨ��
	EM_BIZ_CHN_VIDEO_MAIN, 			//��Ƶ��ͨ��
} EMBIZCHNTYPE;

//EM_BIZ_RECPARA
typedef struct
{
	u8 bRecording;  //�Ƿ�����¼��
	u8 nEnable; //�������
	u8 nStreamType; //������/��Ƶ��(EMBIZRECSTREAMTYPE)
	u8 nEncChn; //¼���Ӧ�ı���ͨ������
	u8 nEncChnType; //¼���Ӧ�ı���ͨ������ (EMBIZCHNTYPE)
	u8 nPreTime; //Ԥ¼ʱ��
	u16 nRecDelay; //��ʱʱ�� 
	u8 nRecExpire; // : ¼��������� (255��ʾ��������)
	u8 nSchRecSchtype; //��ʱ¼�񲼷�����(EMBIZSCHTYPE)
	u8 nMDRecSchtype; //�ƶ����¼�񲼷�����(EMBIZSCHTYPE)
	u8 nSensorRecSchtype; //����������¼�񲼷�����(EMBIZSCHTYPE)
} SBizRecPara;

//EM_BIZ_IMAGEPARA
typedef struct
{
    u16 nHue; //��ͨ��ɫ��(���֣�0-255)
    u16 nSaturation; //���Ͷ�
    u16 nContrast; //�Աȶ�
    u16 nBrightness; //����
} SBizPreviewImagePara;

typedef struct
{
	u8 nBrightness;
	u8 nContrast;
	u8 nSaturation;
	u8 nHue;
} SBizVoImagePara;

//EM_BIZ_STROSD
typedef struct { 
    s8 strChnName[32];  // ��ͨ��ͨ����
    u8 nShowChnName;  // ��ͨ���Ƿ���ʾͨ����(�����б�:0=��;1=��)
    SPoint sChnNamePos; // ��ͨ����XY����
    u8 nEncShowChnName; // �����ͨ���Ƿ���ʾͨ����(�����б�:0=��;1=��)
    SPoint sEncChnNamePos; // �����ͨ����XY����(��D1Ϊ��׼)
    u8 nEncShowTime; // �����ͨ������ʾʱ��(�����б�:0=��;1=��)
    SPoint sEncTimePos; // �����ͨ��ʱ��XY����(��D1Ϊ��׼)
} SBizCfgStrOsd;

//EM_BIZ_RECTOSD
typedef struct { 
    u8 nShowRectOsd; // ��ͨ���Ƿ���ʾ�ڸ�(�����б�:0=��;1=��)
    u8 nRealNum; //�����ڸǸ���
    u8 nMaxNum; //���ɷ��صľ����ڸǸ���;��ò���ʱ��
    SRect* psRectOsd; // ��ͨ���ڸ��ڸǸ�����XYWH���꼰���(��D1Ϊ��׼)
} SBizCfgRectOsd;

//Ԥ��osdʵ�ַ�ʽ :0=��Ƶ��;1=����״̬��OSD;2=�����ʵ��
typedef enum
{
    EM_BIZ_OSD_OPERATOR_VIDLAYER = 0,
    EM_BIZ_OSD_OPERATOR_STATUSLAYER,
    EM_BIZ_OSD_OPERATOR_UILAYER,
} EMBIZOSDOPERATOR;

 // ������ϰ��
typedef enum
{
	//��Ƶ��������
	 EM_BIZ_ENCODE_VIDEO_MJPEG = 26, 		//Motion JPEG
	 EM_BIZ_ENCODE_VIDEO_H261 = 31, 		//H.261
	 EM_BIZ_ENCODE_VIDEO_MP2 = 33, 			//MPEG2 video
	 EM_BIZ_ENCODE_VIDEO_H263 = 34, 		//H.263
	 EM_BIZ_ENCODE_VIDEO_MP4 = 97, 			//MPEG-4
	 EM_BIZ_ENCODE_VIDEO_H264 = 98, 		//H.264
	
	 //������������
	 EM_BIZ_ENCODE_AUDIO_PCMU = 0, 			//G.711 ulaw
	 EM_BIZ_ENCODE_AUDIO_PCMA = 8, 			//G.711 Alaw
	 EM_BIZ_ENCODE_AUDIO_G722 = 9, 			//G.722
	 EM_BIZ_ENCODE_AUDIO_G7231 = 4, 		//G.7231
	 EM_BIZ_ENCODE_AUDIO_G728 = 15, 		//G.728
	 EM_BIZ_ENCODE_AUDIO_G729 = 18, 		//G.729
	 EM_BIZ_ENCODE_AUDIO_RAW = 19, 			//raw audio
	 EM_BIZ_ENCODE_AUDIO_ADPCM = 20, 		//adpcm
	 EM_BIZ_ENCODE_AUDIO_ADPCM_HS = 21, 	//��˼adpcm
	 EM_BIZ_ENCODE_AUDIO_MP3 = 96,			//mp3
} EMBIZENCTYPE;

//�˵����ͬ��(���� : 0=Vga to Cvbs; 1=CvbsToVga; 2=ͬʱֻ��һ�����; 3=�ֱ����;4=��VGA(CVBS�Զ�);5=��CVBS(VGA�Զ�))
typedef enum
{
	EM_BIZ_OUTPUT_VGA2CVBS = 0, 
	EM_BIZ_OUTPUT_CVBS2VGA,
	EM_BIZ_OUTPUT_VGAORCVBS,
	EM_BIZ_OUTPUT_VGAANDCVBS,
	EM_BIZ_OUTPUT_VGA_CVBSAUTO,
	EM_BIZ_OUTPUT_CVBS_VGAAUTO,
} EMBIZOUTPUTSYNC;

////////////////////////�û�
#define MOD_BIZ_MAX_GROUP_NUM 16			//��֧�ֵ������û�����Ŀ
#define MOD_BIZ_MAX_USER_NUM 128			//��֧�ֵ������û���Ŀ

typedef enum						//return value
{
	EM_BIZ_USER_AUTHOR_ERROR = -1,	//check failed
	EM_BIZ_USER_AUTHOR_NO = 0,		//user has no author
	EM_BIZ_USER_AUTHOR_YES = 1,		//user has author
}EMBIZUSERCHECKAUTHOR;

typedef enum						//return value from UserCheckLoginPara()
{
	EM_BIZ_LOGIN_ERR_NOTHISUSER = -2,
	EM_BIZ_LOGIN_ERR_WRONGPASSWD = -3,
	EM_BIZ_LOGIN_ERR_NOLOGINTWINCE = -4,
	EM_BIZ_LOGIN_ERR_IP_NOPERMIT = -5,
	EM_BIZ_LOGIN_ERR_MAC_NOPERMIT = -6,
	//
}EMBIZUSERLOGINERR;

typedef enum						//return value from ModUserAddUser()
{
	EM_BIZ_ADDUSER_INVALID_NAME = -2,
	EM_BIZ_ADDUSER_EXIST_NAME = -3,		
	EM_BIZ_ADDUSER_TOMAXNUM = -4,	
}EMBIZADDUSERERR;

typedef enum						//return value from ModUserDeleteUser
{
	EM_BIZ_DELUSER_INVALID_NAME = -2,
	EM_BIZ_DELUSER_USER_LOGINED = -3,	
}EMBIZDELUSERERR;

typedef enum
{
	EM_BIZ_USER_AUTHOR_SHUTDOWN = 0,
	EM_BIZ_USER_AUTHOR_SYSTEM_RESET,
	EM_BIZ_USER_AUTHOR_SYSTEM_UPDATE,
	EM_BIZ_USER_AUTHOR_SYSTEM_MAINTAIN,
	EM_BIZ_USER_AUTHOR_USER_MANAGE,	
	EM_BIZ_USER_AUTHOR_AUDIO_TALK,
	EM_BIZ_USER_AUTHOR_DISK_MANAGE,
	EM_BIZ_USER_AUTHOR_DATA_MANAGE,
	EM_BIZ_USER_AUTHOR_SYSINFO_VIEW,
	EM_BIZ_USER_AUTHOR_LOG_VIEW,
	EM_BIZ_USER_AUTHOR_LOG_DELETE,
	EM_BIZ_USER_AUTHOR_LOG_BACKUP,	
	EM_BIZ_USER_AUTHOR_REMOTE_LOGIN,
	EM_BIZ_USER_AUTHOR_CHN_NAME_SETUP,
	EM_BIZ_USER_AUTHOR_SYSTEM_SETUP,
	EM_BIZ_USER_AUTHOR_NORMAL_SETUP,
	EM_BIZ_USER_AUTHOR_RECORD_SETUP,
	EM_BIZ_USER_AUTHOR_ENCODE_SETUP,
	EM_BIZ_USER_AUTHOR_PTZ_SETUP,
	EM_BIZ_USER_AUTHOR_ALARM_SETUP,
	EM_BIZ_USER_AUTHOR_NETWORK_SETUP,
	EM_BIZ_USER_AUTHOR_VIDEOMOTION_SETUP,
	//
	EM_BIZ_USER_AUTHOR_LIVEVIEW = 50,
	EM_BIZ_USER_AUTHOR_REMOTEVIEW,
	EM_BIZ_USER_AUTHOR_RECORD,
	EM_BIZ_USER_AUTHOR_PLAYBACK,
	EM_BIZ_USER_AUTHOR_RECFILE_BACKUP,
	EM_BIZ_USER_AUTHOR_PTZCTRL,
	//
	EM_BIZ_USER_AUTHOR_NULL = 65,
}EMBIZUSEROPERATION;

typedef enum
{
	EM_BIZUSER_LOGINFROM_GUI = 0,
	EM_BIZUSER_LOGINFROM_WEB,
	EM_BIZUSER_LOGINFROM_CONSOLE,
	//
}EMBIZUSERLOGINFROM;

typedef struct
{
	u8 UserName[15];
	u8 Password[15];
	EMBIZUSERLOGINFROM emFrom;
	u32 nPcIp;
	u64 nPcMac;
}SBizUserLoginPara;

typedef struct
{
	u8 nAuthor[64];
	u64 nLiveView[4];
	u64 nRemoteView[4];
	u64 nRecord[4];
	u64 nPlayback[4];
	u64 nBackUp[4];
	u64 nPtzCtrl[4];
}SBizUserAuthority;

typedef struct
{
	u8 GroupName[15];
	u8 GroupDescript[30];
	u8 nDoWithUser;	//��������޸ĺ�ɾ��ʱ���Ƿ�ʹ����Ӧ�õ������µ������û���[0]:��[��0]:��
	SBizUserAuthority GroupAuthor;
}SBizUserGroupPara;

typedef struct
{
	u8 UserName[15];
	u8 Password[15];
	u8 GroupBelong[15];
	u8 UserDescript[30];
	u8 emLoginTwince;
	u8 emIsBindPcMac;
	u64 PcMacAddress;
	SBizUserAuthority UserAuthor;
}SBizUserUserPara;

typedef struct
{
	u64 nFlagGroupSetted;
	u64 nFlagUserSetted[2];
	SBizUserUserPara sUserPara[MOD_BIZ_MAX_USER_NUM];
	SBizUserGroupPara sGroupPara[MOD_BIZ_MAX_GROUP_NUM];
}SBizUserPara;

typedef struct 
{
	s8** list;
	s32 nLength;
	s32 nRealLength;
}SBizUserListPara;

//��־

//ϵͳ����
#define BIZ_LOG_MASTER_SYSCONTROL     					0x01      
#define BIZ_LOG_SLAVE_START_DWELL      					0x01
#define BIZ_LOG_SLAVE_STOP_DWELL       					0x02
#define BIZ_LOG_SLAVE_MUTE            					0x03
#define BIZ_LOG_SLAVE_CHANGE_VOICE    					0x04
#define BIZ_LOG_SLAVE_CONTROL_PTZ      					0x05
#define BIZ_LOG_SLAVE_SNAP_SUCCESS     					0x06
#define BIZ_LOG_SLAVE_START_MANUAL_REC 					0x07
#define BIZ_LOG_SLAVE_STOP_MANUAL_REC  					0x08
#define BIZ_LOG_SLAVE_MANUAL_ALARM     					0x09
#define BIZ_LOG_SLAVE_CLEAR_ALARM      					0x0a

#define BIZ_LOG_SLAVE_POWER_OFF        					0x0b
#define BIZ_LOG_SLAVE_REBOOT   	       					0x0c
#define BIZ_LOG_SLAVE_LOGIN            					0x0d
#define BIZ_LOG_SLAVE_LOGOFF           					0x0e
#define BIZ_LOG_SLAVE_FORMAT_SUCCESS   				0x0f
#define BIZ_LOG_SLAVE_FORMAT_FAIL      				0x10
#define BIZ_LOG_SLAVE_UPDATE           					0x11
//csp modify
#define BIZ_LOG_SLAVE_POWER_ON        					0x12
//yg modify 20140814
#define BIZ_LOG_SLAVE_START_MD         					0x13
#define BIZ_LOG_SLAVE_STOP_MD          					0x14

#define BIZ_LOG_SLAVE_VIDEO_LOST        				0x15
#define BIZ_LOG_SLAVE_VIDEO_RESUME        				0x16

#define BIZ_LOG_SLAVE_START_LOCAL_ALARM        		0x17
#define BIZ_LOG_SLAVE_STOP_LOCAL_ALARM        			0x18

#define BIZ_LOG_SLAVE_START_IPCEXT        				0x19
#define BIZ_LOG_SLAVE_STOP_IPCEXT        				0x1a

#define BIZ_LOG_SLAVE_START_485EXT        				0x1b
#define BIZ_LOG_SLAVE_STOP_485EXT        				0x1c


#define BIZ_LOG_SLAVE_DISK_WRERR        				0x1d

#define BIZ_LOG_SLAVE_DISK_NONE	        				0x1e

#define BIZ_LOG_SLAVE_IPCCOVER        					0x1f
#define BIZ_LOG_SLAVE_IPCCOVER_RESUME        			0x20

#define BIZ_LOG_SLAVE_DISK_LOST        					0x21





//ϵͳ����
#define BIZ_LOG_MASTER_SYSCONFIG 						0x02
#define BIZ_LOG_SLAVE_ENABLE_PWDCHECK                   0x01
#define BIZ_LOG_SLAVE_DISABLE_PWDCHECK                  0x02
#define BIZ_LOG_SLAVE_CHANGE_VO                         0x03
#define BIZ_LOG_SLAVE_CHANGE_MAX_NETUSER                0x04
#define BIZ_LOG_SLAVE_CHANGE_VIDEOFORMAT                0x05
#define BIZ_LOG_SLAVE_CHANGE_LANGUAGE                   0x06
#define BIZ_LOG_SLAVE_CHANGE_ZONE                       0x07
#define BIZ_LOG_SLAVE_ENABLE_AUTO_TIMESYNC              0x08
#define BIZ_LOG_SLAVE_DISABLE_AUTO_TIMESYNC             0x09
#define BIZ_LOG_SLAVE_CHANGE_NTP_SERVER                 0x0a
#define BIZ_LOG_SLAVE_DST_CONFIG                        0x0b
#define BIZ_LOG_SLAVE_CHANGE_CHN_NAME                   0x0c
#define BIZ_LOG_SLAVE_CHANGE_REC_ENABLE                 0x0d
#define BIZ_LOG_SLAVE_CHANGE_REC_AUDIO_ENABLE           0x0e
#define BIZ_LOG_SLAVE_CHANGE_REC_CODESTREAM             0x0f
#define BIZ_LOG_SLAVE_CHANGE_TIME_BEFORE_ALARM          0x10
#define BIZ_LOG_SLAVE_CHANGE_TIME_AFTER_ALARM           0x11
#define BIZ_LOG_SLAVE_CHANGE_REC_EXPIRETIME             0x12
#define BIZ_LOG_SLAVE_CHANGE_OSD                        0x13
#define BIZ_LOG_SLAVE_CHANGE_TIMER_RECSCHE              0x14
#define BIZ_LOG_SLAVE_CHANGE_MD_RECSCHE                 0x15
#define BIZ_LOG_SLAVE_CHANGE_SENSORALARM_RECSCHE        0x16
#define BIZ_LOG_SLAVE_CHANGE_SENSOR_ENABLE              0x17
#define BIZ_LOG_SLAVE_CHANGE_SENSOR_TYPE                0x18
#define BIZ_LOG_SLAVE_CHANGE_SENSOR_ALARMDEAL           0x19
#define BIZ_LOG_SLAVE_CHANGE_SENSOR_DETECTION_SCHE      0x1a
#define BIZ_LOG_SLAVE_CHANGE_MD_ENABLE                  0x1b
#define BIZ_LOG_SLAVE_CHANGE_MD_AREA_SETUP              0x1c
#define BIZ_LOG_SLAVE_CHANGE_MD_ALARMDEAL               0x1d
#define BIZ_LOG_SLAVE_CHANGE_MD_DETECTION_SCHE          0x1f
#define BIZ_LOG_SLAVE_CHANGE_VIDEOLOSS_ALARMDEAL        0x20
#define BIZ_LOG_SLAVE_CHANGE_ALARMOUT_SCHE              0x21
#define BIZ_LOG_SLAVE_ENABLE_SOUND_ALARM                0x22
#define BIZ_LOG_SLAVE_DISABLE_SOUND_ALARM               0x23
#define BIZ_LOG_SLAVE_CHANGE_IP                         0x24
#define BIZ_LOG_SLAVE_CHANGE_HTTP_PORT                  0x25
#define BIZ_LOG_SLAVE_CHANGE_SERVER_PORT                0x26
#define BIZ_LOG_SLAVE_CHANGE_NET_CODESTREAM             0x27
#define BIZ_LOG_SLAVE_CHANGE_OUTBOX_CONFIG              0x28
#define BIZ_LOG_SLAVE_CHANGE_INBOX_CONFIG               0x29
#define BIZ_LOG_SLAVE_CHANGE_DNS_CONFIG                 0x2a
#define BIZ_LOG_SLAVE_ADD_USER                          0x2b
#define BIZ_LOG_SLAVE_DELETE_USER                       0x2c
#define BIZ_LOG_SLAVE_CHANGE_USER_AUTH                  0x2d
#define BIZ_LOG_SLAVE_CHANGE_PWD                        0x2e
#define BIZ_LOG_SLAVE_CHANGE_SERIAL_CONFIG              0x2f
#define BIZ_LOG_SLAVE_CHANGE_CRUISELINE                 0x30
#define BIZ_LOG_SLAVE_FACTORY_RESET                     0x31

//�ط�
#define BIZ_LOG_MASTER_PLAYBACK 						0x03
#define BIZ_LOG_SLAVE_PB_STOP 							0x01
#define BIZ_LOG_SLAVE_PB_PAUSE 							0x02
#define BIZ_LOG_SLAVE_PB_RESUME 						0x03
#define BIZ_LOG_SLAVE_PB_BACKWARD 						0x04
#define BIZ_LOG_SLAVE_PB_FORWARD 						0x05
#define BIZ_LOG_SLAVE_PB_START 							0x06  //cw_log

//����
#define BIZ_LOG_MASTER_BACKUP 							0x04
#define BIZ_LOG_SLAVE_START_BACKUP 						0x01
#define BIZ_LOG_SLAVE_END_BACKUP 						0x02

//����
#define BIZ_LOG_MASTER_SEARCH 							0x05
#define BIZ_LOG_SLAVE_SEARCH_DATA_BYTIME 				0x01
#define BIZ_LOG_SLAVE_SEARCH_RECTIME 					0x02

//�鿴��Ϣ
#define BIZ_LOG_MASTER_CHECKINFO 						0x06

//����
#define BIZ_LOG_MASTER_ERROR 							0x07
#define BIZ_LOG_SLAVE_ERROR_CRASH 						0x01  //cw_log
#define BIZ_LOG_SLAVE_ERROR_LOGINPWD 					0x02  //cw_log
#define BIZ_LOG_SLAVE_ERROR_HDD_ABNORMAL				0x03  //Ӳ���쳣
#define BIZ_LOG_SLAVE_ERROR_ENC_ABNORMAL				0x04  //�����쳣
#define BIZ_LOG_SLAVE_ERROR_FILE_ABNORMAL 				0x05  //¼���ļ��쳣

typedef struct
{
	u32 nMaxLogs;
	void* pHddMgr;
}SBizLogInitPara;

typedef struct
{
	u8  nMode;									//��ѯ��ʽ
	u16 nMasterMask;							//��ѯ����
	time_t nStart;								//��ʼʱ��
	time_t nEnd;								//����ʱ��
}SBizLogSearchPara;

typedef struct
{
	s8  nMasterType;							//����������
	s8  nSlaveType;								//���ظ�����
	u8  nchn;									//����Դ����Ż�ͨ��
	char aIp[16];								//IP��ַ
	char aUsername[32];							//�û���
}SBizLogInfo;

typedef struct
{
	u16  nType;									//��������
	u8  nchn;									//����Դ����Ż�ͨ��
	time_t  nTime;								//������ʼʱ��
	char aIp[16];								//IP��ַ
	char aUsername[32];							//�û���
	u32 logId;									//cw_log
}SBizLogInfoResult;

typedef struct
{
	u32 nRealNum;								//���ص���־��
	u32 nMaxLogs;
	SBizLogInfoResult* psLogList;	    		//������־��ѯ�Ľ��
}SBizLogResult;

typedef void (*BIZLOGEXPORTCB)(SBizLogInfoResult* pLog, u8* strLog);
typedef void (*BIZPBREGFUNCCB)(void);

//EM_BIZ_DVRINFO
typedef struct
{
    s8 strVersion[64]; //�汾��
    s8 strModel[32]; //�ͺ�
    s8 sproductnumber[32];//��Ʒ��  9624
    u8 nPreviewNum; // Ԥ��ͨ����(����)
    u8 nVidMainNum; // ������ͨ����(����)
    u8 nVidSubNum; // ������ͨ����(����)
    u8 nAudNum; // ��Ƶͨ����(����)
    u8 nSnapNum; // ץͼͨ����(����)
    u8 nVoipNum; // �����Խ�ͨ����(����)
    u8 nRecNum; // ¼��ͨ����(����)
    u8 nSensorNum; // ������ͨ����(����)
    u8 nAlarmoutNum; // �������ͨ����(����)
    u8 nBuzzNum; // ����������(����)
    u8 nMaxHddNum; // Ӳ��֧�ָ���(����)
    u32 nMaxMainFrameSize; // ���������֡��С(����:ʮ������)
    u32 nMaxSubFrameSize; // ���������֡��С(����:ʮ������)
    u32 nMaxAudFrameSize; // �����Ƶ֡��С(����:ʮ������)
    u32 nMaxSnapFrameSize; // ���ץͼ�ļ���С(����:ʮ������)

    u8 nOsdOperator; // Ԥ��osdʵ�ַ�ʽ �ο�(EMBIZOSDOPERATOR)
    u8 nStrOsdNum; // Ԥ��֧�ֵ��ַ���osd��(����)
    u8 nImgOsdNum; // Ԥ��֧�ֵ�ͼƬosd��(����)
    u8 nRectOsdNum; // Ԥ��֧�ֵľ����ڸ�osd��(����)
    SRgb sRectOsdRgb; // Ԥ�������ڸ�osdɫ��(ʮ������rgb:��ff ff ff)
    u8 nSplitLineNum; // Ԥ��֧�ֵķָ���osd��(����:����+����)
    u16 nRefWidth; // Osd�ο���
    u16 nRefHeight; // Osd�ο���

    u8 nAudioEncType; // ��Ƶ������ϰ��(EMBIZENCTYPE)
    u32 nSampleRate; // ������(����:8000/16000/32000)
    u8 nSampleDuration; // ����ʱ��(����,��λ����)
    u32 nSampleSize; // ������С(����)
    u8 nBitWidth; // ����λ��(����8/16)

    u8 nVoipEncType; // ��Ƶ������ϰ��(EMBIZENCTYPE)
    u32 nVoipSampleRate; // ������(����:8000/16000/32000)
    u8 nVoipSampleDuration; // ����ʱ��(����,��λ����)
    u32 nVoipSampleSize; // ������С(����)
    u8 nVoipBitWidth; // ����λ��(����8/16)

    u8 nMDAreaRows; // �ƶ������������
    u8 nMDAreaCols; // �ƶ������������
    u8 nMDSenseMin; // �ƶ������������Сֵ
    u8 nMDSenseMax; // �ƶ�������������ֵ
    
    u8 nSchSegments; //����ʱ��������ֵ
    u8 nOutputSync; //�˵����ͬ��(EMBIZOUTPUTSYNC)

	u8 nBuzzDuration; //����ʱ��ʱ�����㽫�Ǽ�Ъʽ����(��λs) BuzzDurationͬʱ�������Ч
	u8 nBuzzInterval; //����ʱ�������㽫�Ǽ�Ъʽ����(��λs)
	u8 nNVROrDecoder;	//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
} SBizDvrInfo;

/*
 ���ýӿڴ�ȡ��������ṹ
*/

typedef struct _sNetAdvancePara
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
	
} SBizNetAdvancePara;

typedef struct _sNetConfigPara
{
	// ��������
	u8     	Version[8];			    // 8�ֽڵİ汾��Ϣ
    char   	HostName[16];           // ������
    u32  	HostIP;                 // IP ��ַ
    u32  	Submask;                // ��������
    u32  	GateWayIP;              // ���� IP
    u32  	DNSIP;                  // DNS IP
    u32  	DNSIPAlt;                 // DNS IP
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
    SBizNetAdvancePara	sAdancePara;
	
	//csp modify 20130321
	u8		UPNPFlag;
} SBizNetPara;

/*
 Network state
*/
typedef enum
{
	EM_BIZNS_CONNECTED = 0,
	EM_BIZNS_DISCONNECTED,
	
} EM_BIZNET_STATE;


/*
 �ʼ�����
*/
typedef struct _sBizMailContent
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
} SBizMailContent;


#define BIZ_DDNS_USER_LEN_MAX	32

/*
 DDNS action
*/
typedef enum
{
	EM_BIZDDNS_START,
	EM_BIZDDNS_REGIST,
	EM_BIZDDNS_STOP,
} EM_BIZ_DDNS_ACT;

/*
 DDNS type
*/
typedef enum
{
	EM_BIZDDNS_PROT_TL = 1,
	EM_BIZDDNS_PROT_3322,
	EM_BIZDDNS_PROT_DYDNS,
	EM_BIZDDNS_PROT_DVRNET,
	EM_BIZDDNS_PROT_NOIP,
	EM_BIZDDNS_PROT_JMDVR,
	EM_BIZDDNS_PROT_CHANGEIP,
	EM_BIZDDNS_PROT_JSJDVR,
	EM_BIZDDNS_PROT_CCTVSVR,
	EM_BIZDDNS_PROT_CCTVDVR,
	EM_BIZDDNS_PROT_KONLAN,
	EM_BIZDDNS_PROT_MYQSEE,
	EM_BIZDDNS_PROT_DYNDDNS,
	EM_BIZDDNS_PROT_PEOPLE,
	EM_BIZDDNS_PROT_MAX
} EM_BIZ_DDNS_PROT;

/*
 DDNS control para
*/
typedef struct _sBizNetDdnsPara
{
	EM_BIZ_DDNS_PROT	eProt;

	char szDomain[BIZ_DDNS_USER_LEN_MAX];
	char szUser[BIZ_DDNS_USER_LEN_MAX];
	char szPasswd[BIZ_DDNS_USER_LEN_MAX];
	
} sBizNetDdnsPara;


// ����״̬��֪ͨ�ص�
typedef enum
{
	EM_BIZNET_STATE_DHCP,
	EM_BIZNET_STATE_PPPOE,
	EM_BIZNET_STATE_MAIL,
	EM_BIZNET_STATE_DDNS,
	EM_BIZNET_STATE_CONN,
} EM_BIZNET_ACT;

typedef struct _sBizNetStateRslt
{
	u8 nErr; // 0 succ, 1 fail
} SBizNetStateRslt;

typedef struct _sBizNetStateNotify
{
	EM_BIZNET_ACT eAct;
	SBizNetStateRslt sRslt;
} SBizNetStateNotify;

/*
	��̬IP, 
	��̬IP,
	PPPoE��������
	..
*/
typedef enum
{
	BIZ_ECW_STATIC,
	BIZ_ECW_DHCP,
	BIZ_ECW_PPPOE,
	BIZ_ECW_MAX
} BIZ_E_CONN_WAY;

/*
	�����ӣ�
	δ����
	..
*/
typedef enum
{
	BIZ_ECS_ONLINE,
	BIZ_ECS_OFFLINE,
	BIZ_ECS_MAX
} BIZ_E_CONN_STATE;

typedef struct _sBizNetConnSta
{
	BIZ_E_CONN_WAY 		eWay;
	BIZ_E_CONN_STATE	eState;
} SBizNetConnStatus;


///////////////////////�������ȡ�������ü���������ģ�����//////////////////////////
//for BizSetPara/BizGetPara
typedef enum
{
	//ϵͳ�Ͳ���
	EM_BIZ_DVRINFO = 0, //dvr�豸����;ֻ��
	EM_BIZ_SYSTEMPARA, //�豸��Ϣ;SBizSystemPara
	EM_BIZ_PREVIEWPARA, //���Ԥ������;�ο�SBizCfgPreview
	EM_BIZ_PATROLPARA, //��Ƶ��Ѳ��Ѳ��;�ο�SBizCfgPatrol
	EM_BIZ_SYSTEMMAINTAIN,//ϵͳά����Ϣ

	EM_BIZ_USERPARA=10,  //�û�����
	EM_BIZ_USER_GETUSERLIST,//�õ��û��б�
	EM_BIZ_USER_GETUSERINFO,//�õ�ĳ���û���Ϣ
	EM_BIZ_USER_ADDUSER,//����û�
	EM_BIZ_USER_MODIFYUSER,//�޸��û�
	EM_BIZ_USER_DELETEUSER,//ɾ���û�
	EM_BIZ_USER_GETGROUPLIST,//�õ��û����б�
	EM_BIZ_USER_GETGROUPINFO,//�õ�ĳ���û�����Ϣ
	
	EM_BIZ_SYSCOMPLEXPARA=30, //ϵͳ�������

	//ͨ���Ͳ���
	EM_BIZ_IMAGEPARA = 100,  //Ԥ��ɫ��SBizPreviewImagePara
	EM_BIZ_STROSD,  //�ַ���osd����SBizCfgStrOsd
	EM_BIZ_RECTOSd,  //����osd����SBizCfgRectOsd
	EM_BIZ_ENCMAINPARA,  //�������������SBizEncodePara
	EM_BIZ_ENCSUBPARA, //�������������SBizEncodePara
	EM_BIZ_ENCMOBPARA, //�ֻ������������SBizEncodePara
	EM_BIZ_RECPARA, //¼�����SBizRecPara
	EM_BIZ_SENSORPARA, //����������SBizAlarmSensorPara
	//yaogang modify 20141010
	EM_BIZ_IPCCOVERPARA,
	EM_BIZ_IPCEXTSENSORPARA, //IPC�ⲿ����������SBizAlarmIPCExtSensorPara
	EM_BIZ_485EXTSENSORPARA,
	EM_BIZ_HDDPARA,
	
	EM_BIZ_VMOTIONPARA, //�ƶ����SBizAlarmVMotionPara
	EM_BIZ_VBLINDPARA, //�ڵ���������SBizAlarmVBlindPara
	EM_BIZ_VLOSTPARA, //��Ƶ��ʧ����SBizAlarmVLostPara
	EM_BIZ_ALARMOUTPARA, //�����������SBizAlarmOutPara
	EM_BIZ_ALARMBUZZPARA, //����������SBizAlarmBuzzPara
   	 //һ�㱨���¼�������������
	EM_BIZ_DISPATCH_SENSOR, //SBizAlarmDispatch
	EM_BIZ_DISPATCH_IPCCOVER,
	EM_BIZ_DISPATCH_IPCEXTSENSOR, //SBizAlarmDispatch
	EM_BIZ_DISPATCH_485EXTSENSOR, //SBizAlarmDispatch
	EM_BIZ_DISPATCH_HDD, 
	
	EM_BIZ_DISPATCH_VMOTION, //SBizAlarmDispatch
	EM_BIZ_DISPATCH_VBLIND, //SBizAlarmDispatch
	EM_BIZ_DISPATCH_VLOST, //SBizAlarmDispatch
	//һ�㱨���¼���������
	EM_BIZ_SCHEDULE_SENSOR, //����������SBizAlarmSchedule
	EM_BIZ_SCHEDULE_IPCEXTSENSOR,
	EM_BIZ_SCHEDULE_VMOTION, //�ƶ���ⱨ������SBizAlarmSchedule
	EM_BIZ_SCHEDULE_VBLIND, //�ڵ���������SBizAlarmSchedule
	EM_BIZ_SCHEDULE_VLOST, //��Ƶ��ʧ��������SBizAlarmSchedule
	//��������¼���������
	EM_BIZ_SCHEDULE_ALARMOUT, //�����������SBizAlarmSchedule
	EM_BIZ_SCHEDULE_BUZZ, //��ʱ��֧��;����������SBizAlarmSchedule
	//�ط�ɫ�ʿ���
	EM_BIZ_VO_IMAGEPARA, //SBizPreviewImagePara
	EM_BIZ_SNAP_PARA, //yaogang modify 20150105
	
	//NVR used
	//IPCamera��������
	EM_BIZ_IPCAMERAPARA = 200,//
	//yaogang modify 20141209
	//���ƽ̨����
	EM_BIZ_SGPARA,
	EM_BIZ_SGALARMPARA,
	EM_BIZ_SGDAILYPARA,

	EM_BIZ_SCHEDULE_RECTIMING = 1000, //��ʱ¼�񲼷�SBizSchedule
	EM_BIZ_SCHEDULE_RECSENSOR, //����������¼�񸴷�SBizSchedule
	EM_BIZ_SCHEDULE_RECMOTION, //�ƶ����¼�񲼷�SBizSchedule
    
	EM_BIZ_PTZPARA = 2000, //��̨����//SBizPtzPara

	EM_BIZ_NETPARA = 3000, // ������� //SBizNetPara
} EMBIZPARATYPE;

typedef struct
{
    EMBIZPARATYPE emBizParaType; //��������
    u8 nChn; //��ͨ���Ͳ�����Ч
} SBizParaTarget;

//����ѡ���б�ö��
typedef enum
{
    //**********************�����б�ֻ����
    EM_BIZ_CFG_PARALIST_BASE = 0,
    EM_BIZ_CFG_PARALIST_LANGSUP, // ֧�ֵ������б�
    EM_BIZ_CFG_PARALIST_LANGSRC, // ֧�ֵ�������Դ�ļ��б� ;��֧���ַ����б�
    EM_BIZ_CFG_PARALIST_PREVIEWMODE, // ֧�ֵ�Ԥ��ģʽ
    EM_BIZ_CFG_PARALIST_BITRATE, // ֧�ֵ�λ���б�
    EM_BIZ_CFG_PARALIST_BITTYPE, // λ�������б�
    EM_BIZ_CFG_PARALIST_PICLEVEL, // ͼ�������б�
    EM_BIZ_CFG_PARALIST_VMAINRESOLH_NUM, // ������ͨ��֧�ֵķֱ����б�//cw_9508S
    EM_BIZ_CFG_PARALIST_VMAINRESOL, // ������ͨ��֧�ֵķֱ����б�
    EM_BIZ_CFG_PARALIST_VMAINRESOL2, // ������ͨ��֧�ֵķֱ����б�
    EM_BIZ_CFG_PARALIST_VSUBRESOL, // ������ͨ��֧�ֵķֱ����б�
    EM_BIZ_CFG_PARALIST_VMOBRESOL, // �ֻ�����ͨ��֧�ֵķֱ����б�
    EM_BIZ_CFG_PARALIST_FPSPAL, // P�Ƴ�����֧�ֵ�֡���б�
    EM_BIZ_CFG_PARALIST_FPSNTSC, // N�Ƴ�����֧�ֵ�֡���б�
    EM_BIZ_CFG_PARALIST_FPSPALD1, // P��D1��֧�ֵ�֡���б�
    EM_BIZ_CFG_PARALIST_FPSNTSCD1, // N��D1��֧�ֵ�֡���б�
    EM_BIZ_CFG_PARALIST_FPSPALCIF, // P��Cif��֧�ֵ�֡���б�
    EM_BIZ_CFG_PARALIST_FPSNTSCCIF, // N��Cif��֧�ֵ�֡���б�
    EM_BIZ_CFG_PARALIST_STREAMTYPE, // ¼�����������б�
    EM_BIZ_CFG_PARALIST_VIDEOSTANDARD, // ��Ƶ��ʽ�б�
    EM_BIZ_CFG_PARALIST_OUTPUT, // ��Ƶ����б�
    EM_BIZ_CFG_PARALIST_SWITCHPICTURE, //��Ѳ����ѡ��
    EM_BIZ_CFG_PARALIST_LOCKTIME, // ������ʱ��ѡ���б�
    EM_BIZ_CFG_PARALIST_DATEFORMAT, // ���ڸ�ʽ�б�
    EM_BIZ_CFG_PARALIST_TIMESTANDARD, // ʱ����ʽ�б�
    EM_BIZ_CFG_PARALIST_SITEMAINOUT, // �ֳ�������б�
    EM_BIZ_CFG_PARALIST_INTERVAL, // ʱ�����б�
    EM_BIZ_CFG_PARALIST_SENSORTYPE, // �����������б�
    EM_BIZ_CFG_PARALIST_ALARMOUTTYPE, // ������������б�
    EM_BIZ_CFG_PARALIST_DELAY, // ��ʱʱ��ѡ���б�
    EM_BIZ_CFG_PARALIST_PRETIME, // Ԥ¼ʱ��ѡ���б�
    EM_BIZ_CFG_PARALIST_RECDELAY, // ¼����ʱʱ��ѡ���б�
    EM_BIZ_CFG_PARALIST_RECEXPIRE, // ¼���������ѡ���б�
    EM_BIZ_CFG_PARALIST_SCHEDULETYP, // ��������ѡ���б�
    EM_BIZ_CFG_PARALIST_PTZLINKTYPE, // ��̨��������ѡ���б�
    EM_BIZ_CFG_PARALIST_BAUDRATE, // ������ѡ���б�
    EM_BIZ_CFG_PARALIST_DATABIT, // ����λ����ѡ���б�
    EM_BIZ_CFG_PARALIST_STOPBIT, // ֹͣλ����ѡ���б�
    EM_BIZ_CFG_PARALIST_CHECKTYPE, // У����������ѡ���б�
    EM_BIZ_CFG_PARALIST_FLOWCTRLTYPE, // ���ط�ʽ����ѡ���б�
    EM_BIZ_CFG_PARALIST_DDNSDOMAIN, // DDNSDomain
    EM_BIZ_CFG_PARALIST_DDNSIPUPDATEITVL, // DDNS IP update 
    EM_BIZ_CFG_PARALIST_VIDEOSRCTYPE, // ��ƵԴ����ѡ���б�
	
    //
    EM_BIZ_CFG_PARALIST_PTZPROTOCOL, // ��̨Э��ѡ���б�
    //yaogang modify 20141210 ShenGuang
    EM_BIZ_CFG_PARALIST_RECORD_SNAP_RES,//ץͼ�ֱ���
    EM_BIZ_CFG_PARALIST_RECORD_SNAP_QUALITY,//ץͼ����
    EM_BIZ_CFG_PARALIST_RECORD_SNAP_INTERVAL,//ץͼʱ����
    EM_BIZ_CFG_PARALIST_RepairName,//����ϵͳ����
    EM_BIZ_CFG_PARALIST_RepairType,//����ϵͳ����
    EM_BIZ_CFG_PARALIST_Maintain,//ά��ά��
    EM_BIZ_CFG_PARALIST_Test,//����
    EM_BIZ_CFG_PARALIST_AlarmInput, //����ͼƬ����: �������
} EMBIZCFGPARALIST;

//yaogang modify 20141209
//���ƽ̨����
typedef struct
{
	char AgentID[100];
	char CenterIP[100];
	u8 RunEnable;//����״̬����
	u8 AlarmEnable;//������������
	u8 DailyPicEnable;//�ճ�ͼ������
	u8 normal_exit;// 1: ϵͳ�����ػ�0: �쳣�˳�
} SBizSGParam;

//����ͼƬ���ã�ÿ������ͨ��һ���ṹ
//m·���أ�n·IPC�ⲿ����(m 16--n 32)
typedef struct
{
	u8 PreTimes;		//ǰ��ʱ��
	u8 Interval;		//���ʱ��
	u8 StillTimes;		//��¼ʱ��
	u8 AlarmInput;	//�������
	u32 IpcChn;		//ÿ��IPCͨ����ռһλ��0xffff ffff��ʾ����ͨ��
} SBizAlarmPicCFG;

//�ճ�ͼƬ����
typedef struct
{
	u8 TimeEnable;	//��ʱ�ϴ�ʱ��ʹ��
	u8 hour;
	u8 min;
	u8 sec;
} SBizSGTime;

typedef struct
{
	SBizSGTime Time1;		//��ʱ�ϴ�ʱ��1 
	SBizSGTime Time2;		//��ʱ�ϴ�ʱ��2
	u32 IpcChn1;		//ÿ��IPCͨ����ռһλ��0xffff ffff��ʾ����ͨ��
	u32 IpcChn2;		//ÿ��IPCͨ����ռһλ��0xffff ffff��ʾ����ͨ��
} SBizDailyPicCFG;

#if 0
typedef struct
{
	u8 TimeEnable;	//��ʱ�ϴ�ʱ��ʹ��
	u8 hour;
	u8 min;
	u8 sec;
} SBizSGTime;
typedef struct
{
	u8 PreTimes;		//ǰ��ʱ��
	u8 Interval;		//���ʱ��
	u8 StillTimes;		//��¼ʱ��
	u8 AlarmInput;	//�������
	u32 IpcChn;		//16��IPCͨ����ռһλ��0x1000 0000��ʾ����ͨ��
} SBizAlarmPicCFG;
typedef struct
{
	SBizSGTime Time1;		//��ʱ�ϴ�ʱ��1 
	SBizSGTime Time2;		//��ʱ�ϴ�ʱ��2
	u32 IpcChn1;		//16��IPCͨ����ռһλ��0x1000 0000��ʾ����ͨ��
	u32 IpcChn2;		//16��IPCͨ����ռһλ��0x1000 0000��ʾ����ͨ��
} SBizDailyPicCFG;
typedef struct
{
	char AgentID[100];
	char CenterIP[100];
	u8 RunEnable;//����״̬����
	u8 AlarmEnable;//������������
	u8 DailyPicEnable;//�ճ�ͼ������
	SBizAlarmPicCFG SAlarmPicParam;//����ͼƬ���ò���
	SBizDailyPicCFG SDailyPicParam;//�ճ�ͼƬ���ò���
} SBizSGParam;
#endif

//yaogang modify 20150105
typedef struct 
{
	u8 Enable;	//ʹ��
	u8 Res;		//�ֱ���
	u8 Quality;	//ͼƬ����
	u8 Interval;	//ʱ����
} SBizSnapPara;
typedef struct 
{
	SBizSnapPara TimeSnap;		//��ʱץͼ
	SBizSnapPara EventSnap;	//�¼�ץͼ
} SBizSnapChnPara;

#endif

