#ifndef __MOD_SYSCOMPLEX_H__
#define __MOD_SYSCOMPLEX_H__

#include "public.h"
#include "common_msgcmd.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USB_DIR_LEN_MAX		64
#define USB_DIR_NUM_MAX		4
#define	DEV_PATH_LEN_MAX	15
#define	UPDATE_FILE_NAME_MAX	64
#define	UPDATE_FILE_EXT_MAX		6
#define SYS_TIME_LEN_MAX	64


// ��ʱ�ṩ����ʱ��ص�����
// ʱ���ʽ��ѭ��ǰ�����ڲ�ά�ֵĸ�ʽ����
typedef void (*PUpdateTimeCB)(char* pstrDateTime);

// Function return errcode
typedef enum
{
	EM_MODSYS_SUCCESS = 0,
	EM_MODSYS_FAILED,
	EM_MODSYSINIT_SUCCESS, 							// ģ���ʼ���ɹ�4	
	EM_MODSYSINIT_FAIL,     						// ģ���ʼ��ʧ��
	EM_MODSYSINIT_NULL,    							// ģ��ص�Ϊ��
	EM_MODSYS_THREAD_READ_CMD_ERR ,
	EM_MODSYS_THREAD_READ_CMD_OK ,
	EM_MODSYS_THREAD_USB_UPDATE ,
	EM_MODSYS_THREAD_FILE_PATH_ERR,
	EM_MODSYS_UPDATE_RUNNING,
} EMMODYSERR;

//2.2 ϵͳ�����¼�ö��
typedef enum
{
	EM_SYSEVENT_LOCK = 0,		//see 2.3 nDelay
	EM_SYSEVENT_RESTART,		//see 2.3 nDelay
	EM_SYSEVENT_POWEROFF,		//see 2.3 nDelay
	EM_SYSEVENT_POWEROFF_MANUAL,	//no parameter by this event
	EM_SYSEVENT_UPGRADE_INIT,	//no parameter by this event
	EM_SYSEVENT_UPGRADE_RUN,	//see SProgress
	EM_SYSEVENT_REMOTEUP_START,//cw_remote
	EM_SYSEVENT_UPGRADE_DONE,	//see EMRESULT
	EM_SYSEVENT_BACKUP_INIT,	//no parameter by this event
	EM_SYSEVENT_BACKUP_RUN,	//see SProgress
	EM_SYSEVENT_BACKUP_DONE,	//see EMRESULT
	EM_SYSEVENT_FORMAT_INIT,	//no parameter by this event
	EM_SYSEVENT_FORMAT_RUN,	//see SProgress
	EM_SYSEVENT_FORMAT_DONE,	//see EMRESULT
	EM_SYSEVENT_DATETIME_STR,
	EM_SYSEVENT_DATETIME_YMD,
	EM_SYSEVENT_GETDMINFO,
	//EM_SYSEVENT_PCBHDD,//xdc
	EM_SYSEVENT_SATARELOAD,
	EM_SYSEVENT_DISKCHANGED,
} EMSYSEVENT;

// Զ����������֪ͨ�ص�
typedef void (*PFNRemoteUpdateProgressCB)(u8 nProgress);

//2.4 ���Ȳ���
typedef struct
{
	u64 lTotalSize;
	u64 lCurSize;
	u8 nProgress;	//0~100
} SProgress;

//2.4.5
typedef enum
{
	EM_UPGRADE_USB,
	EM_UPGRADE_NET,
} EMUPGRADEWAY;

// mainboard || panel
typedef enum
{
	EM_UPGRADE_MAINBOARD,
	EM_UPGRADE_PANEL,
	EM_UPGRADE_STARTLOGO,
	EM_UPGRADE_APPLOGO,
} EMUPGRADETARGET;

//2.5 ���н��ö��
typedef enum
{
	EM_UPGRADE_SUCCESS = 1,//�����ɹ�1
	EM_UPGRADE_RUNNING = -198,
	EM_UPGRADE_FAILED_UNKNOW = -199,// ����ʧ�� -199~-100��������������

	EM_BACKUP_SUCCESS = 2,//���ݳɹ�2
	EM_BACKUP_FAILED_UNKNOW = -299,// ����ʧ�� -299~-200��������������
	EM_BACKUP_FAILED_NOSPACE = -298,

	EM_FORMAT_SUCCESS = 3,//��ʽ���ɹ�3
	EM_FORMAT_FAILED_UNKNOW = -399,// ��ʽ��ʧ�� -399~-300��������������
	
	EM_SEARCH_SUCCESS = 4,//��ʽ���ɹ�3
	EM_SEARCH_FAILED_UNKNOW = -499,// ��ʽ��ʧ�� -399~-300��������������	
} EMRESULT;

// 2.6 ϵͳ�˳�����ö��
typedef enum
{
	EM_SYSEXIT_LOCK,
	EM_SYSEXIT_RESTART,
	EM_SYSEXIT_POWEROFF,
} EMSYSEXITTYPE;

// 2.7 �ļ�����ö��
typedef enum
{
	EM_FILE_REC = 0,
	EM_FILE_SNAP,
} EMFILETYPE;

// 2.8 �ļ�������ʽö��
typedef enum
{
	EM_SEARCH_BY_TIME,
	EM_SEARCH_BY_FILE,
} EMSEARCHWAY;


// 2.10 ��ʱ����������
typedef struct 
{
	u8 nMaskType;
	u32 nMaskChn;
	u32 nStartTime;
	u32 nEndTime;
	void* pReserverInfo;
} SSearchPara;

#if 0
typedef struct
{
	u8   channel_no;//u32  channel_no;//csp modify
	u8   type;
	u32  start_time;
	u32  end_time;
	u8   image_format;//3:cif;4:4cif
	u8   stream_flag;//0:��Ƶ��;1:��Ƶ��
	u32	 size;
	u32  offset;
	u8   disk_no;
	u8   ptn_no;
	u16  file_no;
}PACK_NO_PADDING recfileinfo_t;
#endif

// 2.17  �ļ���Ϣ����
typedef struct
{
	u8 nChn;
	u8 nType;
	u32 nStartTime;
	u32 nEndTime;
	u8 nFormat;      //N��ʽ�� 10 = D1��13 = CIF  P��ʽ�� 0 = D1�� 3 = CIF
	u8 nStreamFlag;  //0:��Ƶ��;1:��Ƶ��
	u32 nSize;
	u32 nOffset;
	u8 nDiskNo;
	u8 nPtnNo;
	u16 nFileNo;
} PACK_NO_PADDING SRecfileInfo;

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
} PACK_NO_PADDING SRecSnapInfo;

// 2.11 �ļ���������ṹ��
typedef struct
{
	u16 nFileNum;
	union
	{
		SRecfileInfo *psRecfileInfo;//���ڴ�����ݵ��ļ���Ϣ����
		SRecSnapInfo *psRecSnapInfo;
	};
} PACK_NO_PADDING SSearchResult;

// 2.12 �ļ���ʽ
typedef enum
{
	EM_FORMAT_ORIGINAL = 0,
	EM_FORMAT_AVI,
	EM_FORMAT_JPEG,
	EM_FORMAT_BMP,
} EMFILEFORMAT;

// backup device type
typedef enum
{
	EM_BACKUP_USB,
	EM_BACKUP_DVDR,
	
} EMBACKUPDEV;

typedef enum
{
	EM_DVDRW_MODE_NEW,
	EM_DVDRW_MODE_ADD,
	
} EMBACKUPDVDRWMODE;

// Backup target attributes
typedef struct _sBackupTargetAttr
{
	EMFILEFORMAT fmt;
	EMBACKUPDEV  devt;
	union
	{
		u8 reserve;
		EMBACKUPDVDRWMODE mode;
	};
} SBackTgtAtt, *PSBackTgtAtt;

// 2.9 ������������
typedef struct
{
	EMSEARCHWAY emSearchWay;
	union
	{
		SSearchPara sSearchPara;
		SSearchResult sSearchResult;
	};
} SSearchCondition;

// 2.13 ¼���豸������
typedef void* DMHDR;

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
} SDevInfo;

// 2.14 ��ô洢�豸������Ϣ
typedef struct
{
	u8 		  nDiskNum;
	SDevInfo  *psDevList;
} SStoreDevManage;


// 2.16 ��ʽ����ʽ
typedef enum
{
	EM_FORMAT_FAT32_IFV,						//��ʽ��Ϊfat32����ʼ��¼���ʽ�ļ�
	EM_FORMAT_FAT32,								//��ʽ��Ϊfat32
} EMFORMATWAY;

// 2.20 ����ʱ���ʽ
typedef enum
{
	EM_DATETIME_YYYYMMDDHHNNSS, //�����ټ�
	EM_DATETIME_MMDDYYYYHHNNSS,
	EM_DATETIME_DDMMYYYYHHNNSS,
	EM_DATETIME_IGNORE,
} EMDATETIMEFORMAT;

// 2.20 ʱ����ʽ
typedef enum
{
	EM_TIME_24HOUR, //�����ټ�
	EM_TIME_12HOUR,
} EMTIMEFORMAT;

// 2.19 ����ʱ��
typedef struct
{
	u16 nYear;
	u8 nMonth;
	u8 nDay;
	u8 nHour;
	u8 nMinute;
	u8 nSecode;
	u8 nWday;
	EMDATETIMEFORMAT emDateTimeFormat;
	EMTIMEFORMAT emTimeFormat;
} SDateTime;

typedef struct
{
	u8 nType;
	u32 nTotal;
	u32 nFree;
	char szMountPoint[32];
	//disk_manager* hddindex;//xdc
} SDMInfo;

//2.3 ϵͳ�����¼����� parameter for EMSYSEVENT
typedef struct
{
	union
	{
		u8				nDelay;//nDelay seconds��support EM_SYSEVENT_LOCK��EM_SYSEVENT_RESTART��EM_SYSEVENT_POWEROFF
 		SProgress		sProgress;//support EM_SYSEVENT_UPGRADE_RUN
		EMRESULT		emResult;//support EM_SYSEVENT_UPGRADE_DONE EM_SYSEVENT_BACKUP_DONE EM_SYSEVENT_FORMAT_DONE
		char			szTime[SYS_TIME_LEN_MAX];
		SDateTime		sDateTime;
		SDMInfo			sDmInfo;
		SStoreDevManage	sStoreMgr;
	};
} SEventPara;

// �����ļ���Ϣ�ṹ��
//
typedef struct _sUpdateFileInfo
{
	char szName[UPDATE_FILE_NAME_MAX];
	char szExt[UPDATE_FILE_EXT_MAX];
	u32  nSize;
	u32  nTimeStamp;
} SUpdateFileInfo;

// �����ļ���Ϣ�б�ṹ��
//
typedef struct _sUpdateFileInfoList
{
	u8 nMax;
	u8 nFileNum;
	SUpdateFileInfo* pInfo;
} SUpdateFileInfoList;

// usb�豸�ҽ�·��
//
typedef struct _sUsbDirInfo 
{
	char szDir[USB_DIR_LEN_MAX];
} SUsbDirInfo;

// usb�豸�ҽ�·���б�
typedef struct _sUsbDirList
{
	SUsbDirInfo sInfo[USB_DIR_NUM_MAX]; // null - no usb dir, else - valid usb mounted dir
} SUsbDirList;

//2.1 ϵͳ״̬�ص�����
typedef void (*PFNSysNotifyCB)(EMSYSEVENT emSysEvent, SEventPara *psEventPara);

// reset�ص�
typedef void (*PFNSysResetCB)(void);

// ��ȡ����LOGO�ߴ�
typedef void (*PFNGetUpdateSize)( char* pJpg, int* width, int* height );

// ʹ��/����ǰ���
typedef void (*PFNEnablePanel)( int bEn );

// ����ģ���ʼ�������ṹ��
typedef struct _sModSysCmplxInit
{
	DMHDR pDMHdr;
	PFNSysNotifyCB pSysNotifyCB;
	PUpdateTimeCB  pUpTimeCB;
	PFNSysResetCB  pSysResetCB;
	PFNSysResetCB  pStopRecCB;
	PFNSysResetCB  pResumeRecCB;
	PFNGetUpdateSize pGetLogoSizeCB;
	PFNEnablePanel pDisablePanelCB;
	
	//csp modify 20131213
	u8 nTimeZone;
} SModSysCmplxInit, *PSModSysCmplxInit;

//1���ӿ���ϸ����
/*
	��������:ģ���ʼ��
	���������
		pSysCmplxInit:
			����:PSModSysCmplxInit
			����:ϵͳ�����ʼ���ṹ������洢�豸���������¼�֪ͨ�ص�����
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexInit( PSModSysCmplxInit pSysCmplxInit );

/*
	��������:ģ�鷴��ʼ��
	�����������
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexDeinit(void);

/*
  ��Ҫ��������������ö�٣�����/����/�رյȣ�����ʱʱ��(s)��u8 delay����
	��������:ϵͳ�˳�
	���������
		emSysExitType:
			����:EMSYSEXITTYPE
			����:�˳���������/����/�ر�ϵͳ
		nDelay
			����:u8
			����:��ʱʱ��(s)
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexExit(EMSYSEXITTYPE emSysExitType, u8 nDelay);

/*
	��������:ϵͳ����
	���������
		emUpdateTarget:
			����:EMUPGRADETARGET
			����:����Ŀ������/ǰ���
		emUpgradeWay:
			����:EMUPGRADEWAY
			����:����;��������/ͨ��Э��/FTP��
		pcUpdateFName��
			����:char *
			����:�����ļ�·����������������Ϊ�գ���������Ϊ�գ�
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexUpgrade(EMUPGRADETARGET emUpdateTarget, EMUPGRADEWAY emUpgradeWay, char *pcUpdateFName);

/*
��������:�ж��ļ�����
*/
s32 ModSysComplexBreakBackup(int type);

/*
	��������:�ļ�����
	���������
		emFileType:
			����:EMFILETYPE
			����:�ļ�����
		psBackTgtAtt:
			����:PSBackTgtAtt
			����:����Ŀ�����			
		psSearchCondition��
			����:SSearchCondition*
			����:��������
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexBackup( EMFILETYPE 	emFileType, 
						 PSBackTgtAtt	psBackTgtAtt, 
						 SSearchCondition* psSearchCondition );

/*
	��������:��ô洢�豸������Ϣ��һ���������豸������Ϣ�ṹ�壨���������豸��Ϣ��
	���������
		psStoreDevManage:
			����:SStoreDevManage
			����:�洢�豸������Ϣ
		MaxDiskNum:
			����:u8
			����:Ӳ�������
			 
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexDMGetInfo(SStoreDevManage *psStoreDevManage, u8 MaxDiskNum);

/*
	��������:��ʽ��
	���������
		pstrDevPath:
			����:char *
			����:�豸·��
		emFormatWay:
			����:EMFORMATWAY
			����:��ʽ����ʽ����ʽ
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexDMFormat(char *pstrDevPath, EMFORMATWAY emFormatWay);

/*
	��������:�ļ�����
	���������
		emFileType:
			����:EMFILETYPE
			����:�ļ�����
		psSearchPara:
			����:SSearchPara
			����:��������
		psSearchResul:
			����:SSearchResult
			����:�������ڴ����������ṹ��
		nMaxFileNum:
			����:u16
			����:psRecfileList��������
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexDMSearch(EMFILETYPE emFileType, SSearchPara* psSearchPara, SSearchResult* psSearchResult, u16 nMaxFileNum);

/*
	��������:����ϵͳ����ʱ��
	���������
		psDateTime:
			����:const SDateTime*
			����:������������ʱ��
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexDTSet(SDateTime* psDateTime);

s32 ModSysComplexSetTimeZone(int nTimeZone);

/*
	��������:���ϵͳ����ʱ��
	���������
		psDateTime:
			����:SDateTime*
			����:���ڻ������ʱ��
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexDTGet(SDateTime* psDateTime);
s32 ModSysComplexDTGet_TZ(SDateTime* psDateTime);
s32 ModSysComplexConvertToDT(time_t time, SDateTime* psDateTime);


/*
	��������:��ø�ʽ��ϵͳ����ʱ���ַ���
	���������
		pstrDateTime:
			����:char*
			����:���ڻ������ʱ���ַ���(�Ȱ�1.0Ĭ��YYYY-MM-DD HH:NN:SS)
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexDTGetString(char* pstrDateTime);


/*
	��������:��ȡ����USB�豸��Ŀ¼�µ������ļ�
	���������
		emTgt:
			����:EMUPGRADETARGET
			����:ָ����ȡ�������ļ�����
		pInfo:
			����:SUpdateFileInfo*
			����:�����ļ���Ϣ
			
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexGetUpdateFileInfo(EMUPGRADETARGET emTgt, SUpdateFileInfoList* pInfo);


/*
	��������:��ȡ����USB�豸�ҽ�·���б�
	���������
		pInfo:
			����:SUsbDirList*
			����:�ҽ�·����Ϣ
			
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexGetUsbDirInfo(SUsbDirList* pInfo);

/*
	��������:��ȡ����USB�豸�ҽ�·���б�
	���������
		szDir:
			����: char*
			����: ָ��·�������������ļ�
		pInfo:
			����:SUpdateFileInfoList*
			����:�����ļ���Ϣ
			
	���أ�
		����:s32
		����:���سɹ�,ʧ��,�����ֵ
		ȡֵ:0�ɹ�;��0ʧ�ܻ����ֵ
*/
s32 ModSysComplexGetConfigFileInfo(char* szDir, SUpdateFileInfoList* pInfo);

time_t read_rtc(int utc);

// regist remote update progress notify
void ModSysComplexRegNetUpdateProgCB(PFNRemoteUpdateProgressCB pCB);

PFNSysResetCB GetRecStopCB();//cw_9508S

#ifdef __cplusplus
}
#endif

#endif // __MOD_SYSCOMPLEX_H__

