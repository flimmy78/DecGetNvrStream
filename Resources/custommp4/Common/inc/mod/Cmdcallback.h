#ifndef __CMDCALLBACK_H_
#define __CMDCALLBACK_H_

#include "common_basetypes.h"
#include "ctrlprotocol.h"

// define 
#define SEGMENTS_OF_DAY				4
#ifndef MAX_COVER_REGION_NUM
#define MAX_COVER_REGION_NUM		4
#endif
//#define DEBUG_TEMP

// cam speed
#define MAX_SPEED 15
#define HIGH_SPEED MAX_SPEED
#define MIDDLE_SPEED MAX_SPEED/2
#define LOW_SPEED MAX_SPEED/6

// typedef 

/*
 ��ʽ
*/
typedef enum
{
	PAL 	= 12,
	NTSC	= 10
	
} EM_VS;

/*
 PPPOE Զ�̿�������
*/
typedef enum
{
	EM_CMD_PPPOE_PREUP,
	EM_CMD_PPPOE_UP,
	EM_CMD_PPPOE_DOWN,
	EM_CMD_PPPOE_DISCONN,
	
} EM_CMD_PPPOE;

typedef struct _sCmdPPPOE
{
	EM_CMD_PPPOE nCmdType;
} PACK_NO_PADDING SCmdPPPOE;

/*
 Զ�̵�½�������
*/
typedef struct _sRemoteLoginInfo
{
	int nUserNum;
	char szPrivilege[16];
	char szUserName[32];
} PACK_NO_PADDING SRemoteUserInfo;

/*
 ����ڸ���
*/
typedef struct _sMaxMaskNum
{
	u32 num;
} PACK_NO_PADDING SMaxMaskNum;

/*
 ¼���ճ̲���
 
*/
typedef struct _sCPcSch
{
	u32 nChn;
	u32 nWeek;
} PACK_NO_PADDING SCPRecSch;

/*
 ��־��¼����
 
*/
typedef struct _sCPLogInfo
{
	ifly_search_desc_t Desc;
	ifly_logInfo_t* para_log;
} PACK_NO_PADDING SCPLogInfo;

/*
����ͷ��¼����
//xdc
*/
typedef struct _sCPIpcInfo
{
	ifly_search_desc_t Desc;
	ifly_ipc_info_t* para_log;
} PACK_NO_PADDING SCPIpcInfo;
/*
 ¼���ļ��б����
 
*/
typedef struct _sCPRecFile
{
	ifly_recfile_desc_t desc;
	ifly_recfileinfo_t* info;
} PACK_NO_PADDING SCPRecFile;

/*
 ϵͳ����
*/
typedef struct
{
	u16		device_id;
	char	device_name[32];
	u8		flag_overlay;
	u8      fb_show_type;
	u8		kb_lock_time;
	u8      split_image;
	u8		switch_time;
	u8		flag_status;
	u8		video_format;
	u8		vga_resolution;
	u8      transparency;
	u8		language;
	char	device_model[32];
	char	device_serial[32];
	char	version[64];

}PACK_NO_PADDING ifly_sysparam_t;

/*
 Զ������ṹ
*/
typedef struct _sRemoteCmd
{
	// ctrlpotocol handle
	CPHandle 	cph;

	// parameter
	union
	{
		u8						nAlarmInId;
		u8						nChn;
		u8						nAlarmUpload;
		u8						nHddIdx;
		SCmdPPPOE 				sCmdPoeIns;
		SCPRecSch				sCPRecSch;
		ifly_cp_dhcp_t 			cp_dhcp;
		ifly_loginpara_t 		loginpara;
		ifly_RecordParam_t		RecordParam;
		ifly_SubStreamParam_t 	SubStreamParam;
		ifly_ImgParam_t 		ImgParam;
		ifly_PicAdjust_t 		PicAdjust;
		ifly_AlarmInParam_t 	AlarmInParam;
		ifly_AlarmOutParam_t	AlarmOutParam;
		ifly_AlarmNoticeParam_t	AlarmNoticeParam;
		ifly_AlarmEmail_SMTP_t	AlarmEmail_SMTP;
		ifly_recsearch_param_t 	recsearch_param;
		ifly_VideoBlockParam_t	VideoBlockParam;

		ifly_reset_picadjust_t 	reset_picadjust;
		ifly_framerate_list_t 	framerate_list;
		ifly_videoresolu_list_t videoresolu_list;
		ifly_SysParam_t 		SysParam;
		ifly_RecordSCH_t 		RecordSCH;
		ifly_MDParam_t 			MDParam;
		ifly_VideoLostParam_t 	VideoLostParam;
		ifly_NetWork_t 			NetWork;
		ifly_PTZParam_t 		PTZParam;
		ifly_PtzPresetCtr_t 	PtzPresetCtr;
		ifly_PtzCruisePathParam_t PtzCruisePathParam;
		ifly_CruisePathCtr_t 	CruisePathCtr;
		ifly_TrackCtr_t		 	TrackCtr;
		ifly_userMgr_t 			userMgr;
		ifly_sysTime_t 			sysTime;
		ifly_PtzCtrl_t 			PtzCtrl;
		ifly_ManualRecord_t 	ManualRecord;
		ifly_search_log_t 		search_log;
		ifly_RecSchTime_t		RecSchTime;
		//xdc
		ifly_search_ipc_t		search_ipc;
		ifly_ipc_info_t			ipc_info;
		//yaogang modify 20141030
		ifly_patrol_para_t		patrol_para;
		ifly_preview_para_t		preview_para;
		ifly_chn_para_t			chn_para;
	} PACK_NO_PADDING sReq;
	
} PACK_NO_PADDING SRemoteCmdReq, *PSRemoteCmdReq;

/*
 ����ģ������Ҫ��Ϣ������ģ��
 sBasicInfo - ������Ϣ
 sExtInfo   - ������Ϣ
 
*/
typedef struct _sRemoteCmdRslt
{
	// �Ƿ�ʹ�ò�����Ϣ
	BOOL bUseExt;
	
	// ��ȡ/����/���ƵĽ��
	// CTRL_SUCCESS �ɹ�������δ�ɹ���Ӧ�Ĵ���״̬
	u32  nErrCode;

	// basicinfo length
	u32 nBasicInfoLen;
	union
	{
		u8						nChn;
		SCPRecFile				sRecFile;
		SCPLogInfo				sLogInfo;
		SMaxMaskNum				sMaxMskNum;
		ifly_sysparam_t			sysparam;
		// get login info
		SRemoteUserInfo			sUserInfo;
		ifly_DeviceInfo_t 		DeviceInfo;
		ifly_Video_Property_t 	Video_Property;
		ifly_Audio_Property_t 	Audio_Property;
		ifly_VOIP_Property_t 	VOIP_Property;
		ifly_MD_Property_t 		MD_Property;
		ifly_network_t			netInfo;
		ifly_VGA_Solution 		VGA_Solution;
		ifly_SysLangList_t 		SysLangList;
		ifly_SysParam_t 		SysParam;
		ifly_RecordParam_t 		RecordParam;
		ifly_bitRateList_t		bitRateList;
		ifly_SubStreamParam_t 	SubStreamParam;
		ifly_ImgParam_t 		ImgParam;
		ifly_PicAdjust_t 		PicAdjust;
		ifly_AlarmInParam_t 	AlarmInParam;		
		ifly_AlarmOutParam_t	AlarmOutParam;
		ifly_AlarmNoticeParam_t	AlarmNoticeParam;
		ifly_AlarmEmail_SMTP_t	AlarmEmail_SMTP;
		ifly_userNumber_t		userNumber;
		ifly_VideoBlockParam_t	VideoBlockParam;
		ifly_framerate_list_t 	framerate_list;
		ifly_videoresolu_list_t videoresolu_list;
		ifly_RecordSCH_t 		RecordSCH;
		ifly_MDParam_t 			MDParam;
		ifly_VideoLostParam_t 	VideoLostParam;
		ifly_PTZParam_t 		PTZParam;
		ifly_PtzCruisePathParam_t PtzCruisePathParam;
		ifly_hddInfo_t 			hddInfo;
		ifly_sysTime_t 			sysTime;
		ifly_ManualRecord_t 	ManualRecord;
		ifly_utctime_t 			utctime;
		ifly_sysinfo_t			sysinfo;
		ifly_MDSenselist_t 		MDSenselist;
		ifly_MDAlarmDelaylist_t MDAlarmDelaylist;
		ifly_BaudRateList_t 	BaudRateList;
		ifly_PTZProtocolList_t	PTZProtocolList;
		ifly_RecSchTime_t		RecSchTime;
		//xdc
		SCPIpcInfo				sIpcInfo;
		//yaogang modify 20141030
		ifly_patrol_para_t		patrol_para;
		ifly_preview_para_t		preview_para;
	} sBasicInfo;
	
} PACK_NO_PADDING SRemoteCmdRslt, *PSRemoteCmdRslt;



/*
 Զ�������ص����ͣ��ûص������������Ĵ���
 pCmd ���ݵĽ�����ѭctrlprotocol.h���������ͽṹ
 
 ������
	SRemoteCmd*	pCmd : command
	u8* 	pAckBuf : ack buffer should be filled in callback function
	u32* 	pAckLen : ack buffer length
	
 ����: void

*/
typedef void(*PNetCommCommandDeal)(
	SRemoteCmdReq* pCmd,
	SRemoteCmdRslt* pRslt
);

typedef s32 (*PNetCommCommandDeal2)(
	SRemoteCmdReq* pCmd,
	u8* pAckBuf,
	u32* pAckLen
);

/*
 ����ID�����������
 
*/
typedef struct _sCmdCBPair
{
	u32 								nCmdID;
	PNetCommCommandDeal pCB;
} SCmdCBPair, *PSCmdCBPair;


void cmdPro_Init( PNetCommCommandDeal2 p, u8 nChMax );

/*
 �ú�������ע�������ص�����������ģ��

 ����
 	u32 nCmdID : ����id (ȡֵ��ѭctrlprotocol.h����)
 	PNetCommCommandDeal pCB : Զ�������ص�
 	
 ����
    s32 0 �ɹ���1ʧ��
 
*/
s32 NetCommRegCmdCB( u32 nCmdID, PNetCommCommandDeal pCB );


/*
 �ú�������ȡ��ע��������ص�����

 ����
 	u32 nCmdID : ����id (ȡֵ��ѭctrlprotocol.h����)
 	
*/
void NetCommUnregCmdCB(u32 nCmdID);


#endif // __CMDCALLBACK_H_

