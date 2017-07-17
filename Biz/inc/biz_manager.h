#ifndef _BIZ_MANAGER_H_
#define _BIZ_MANAGER_H_

#include "biz.h"
#include "common_msgcmd.h"
#include "mod_playback.h"
#include "mod_alarm.h"
#include "mod_user.h"
#include "biz_syscomplex.h"
#include "biz_preview.h"
#include "biz_encode.h"


#ifdef __cplusplus
extern "C" {
#endif

#define TOURPATH_MAX	32
#define TOURPIONT_MAX	128

//#define TEST_TIMESPAN //zlb0629

//�������úͻ�ýṹ��
typedef struct {
    SBizParaTarget sBizParaTarget;
    void *pData;
} SBizPara;

// �����ļ���Ϣ�б�
typedef struct {
    char* szDir;
	SBizUpdateFileInfoList* spFileList;
} SBizConfigFileList;

//config resume����
typedef struct {
	EMBIZCONFIGPARATYPE emType;
	s32 nId;
	s8 pFilePath[64];
}SBizCfgResume;

//�طſ��Ʋ���
typedef struct {
    EMBIZPLAYCTRLCMD emPBCtlcmd;
    u32 nContext;
} SBizPbControl;

typedef struct
{
    EMBIZPLAYTYPE emBizPlayType;
    void* para;
} SBizPbStart;

typedef struct
{
    u8 nChn;
    EMBIZPTZCMD emCmd;
    s16 nId;
} SBizPtzCtrl;

typedef struct
{
    EMBIZCFGPARALIST emParaType;
    s8* strList[128];
    u8* pnRealListLen;
    u8 nMaxListLen;
    u8 nMaxStrLen;
} SBizConfigParaList;

typedef struct
{
    EMBIZCFGPARALIST emParaType;
    s32 nValue;
    u8* pnIndex;
} SBizConfigParaIndex;

typedef struct
{
    EMBIZCFGPARALIST emParaType;
    u8 nIndex;
    s32* pnValue;
} SBizConfigParaValue;

//������Ϣ����
typedef enum
{
	EM_BIZMSG_GET_PARA = 0,
	EM_BIZMSG_SET_PARA,
	EM_BIZMSG_GET_DEFAULTPARA,
	EM_BIZMSG_BACKUP_CONFIG,
	EM_BIZMSG_RESUME_CONFIG,

	//
	EM_BIZMSG_GET_LOG_INFO = 50,
	EM_BIZMSG_GET_PLAY_SEARCH,
	//
	EM_BIZMSG_START_PREVIEW = 100, //��Ϣ�����ṹ��SBizPreviewPara
	EM_BIZMSG_STOP_PREVIEW,
	EM_BIZMSG_CHN_PREVIEW,
	EM_BIZMSG_RESUME_PREVIEW,
	EM_BIZMSG_NEXT_PREVIEW,
	EM_BIZMSG_LAST_PREVIEW,
	EM_BIZMSG_NEXT_PREVIEW_MOD,
	EM_BIZMSG_LAST_PREVIEW_MOD,
	EM_BIZMSG_PREVIEW_MUTE,
	EM_BIZMSG_PREVIEW_AUDIOCHN,
	EM_BIZMSG_PREVIEW_VOLUME,
	EM_BIZMSG_START_PATROL,
	EM_BIZMSG_STOP_PATROL,

	EM_BIZMSG_START_MANUAL_REC = 150,
	EM_BIZMSG_STOP_MANUAL_REC,
	EM_BIZMSG_START_PLAYBACK,
	EM_BIZMSG_PLAYBACK_CTRL,
	EM_BIZMSG_STOP_REC,
	EM_BIZMSG_STOP_ALLREC,
	EM_BIZMSG_PAUSE_REC,
	EM_BIZMSG_RESUME_REC,
	EM_BIZMSG_SCHDEULE_REC,

	EM_BIZMSG_SYSCOMPLEX_EXIT = 200,
	EM_BIZMSG_SYSCOMPLEX_UPGRADE,
	EM_BIZMSG_SYSCOMPLEX_UPGRADE_FILELIST,
	EM_BIZMSG_SYSCOMPLEX_CONFIG_FILELIST,
	EM_BIZMSG_SYSCOMPLEX_MOUNTED_USB_DIR_LIST,
	EM_BIZMSG_SYSCOMPLEX_BACKUP,
	EM_BIZMSG_SYSCOMPLEX_BREAK_BACKUP,
	EM_BIZMSG_SYSCOMPLEX_DISKMANAGE,
	EM_BIZMSG_SYSCOMPLEX_DISKFORMAT,
	EM_BIZMSG_SYSCOMPLEX_DISKSEARCH,
	EM_BIZMSG_SYSCOMPLEX_DTSET,
	EM_BIZMSG_SYSCOMPLEX_DTGET,
	EM_BIZMSG_SYSCOMPLEX_DTGETSTR,

	EM_BIZMSG_PTZ_CTRL = 250,

	// ��������
	EM_BIZMSG_NET_INIT = 300,
	EM_BIZMSG_NET_WRCFG,
	EM_BIZMSG_NET_RDCFG,
	EM_BIZMSG_NET_REQ_PREVIEW_MAIN_START,
	EM_BIZMSG_NET_REQ_PREVIEW_SUB_START,
	EM_BIZMSG_NET_REQ_PREVIEW_SUB_STOP,
	EM_BIZMSG_NET_REQ_PREVIEW_SUB_MB_START,
	EM_BIZMSG_NET_REQ_PREVIEW_SUB_MB_STOP,
	EM_BIZMSG_NET_REQ_PREVIEW_STOP,
	EM_BIZMSG_NET_REQ_PREVIEW_AUDIO_START,
	EM_BIZMSG_NET_REQ_PREVIEW_AUDIO_STOP,
	EM_BIZMSG_NET_REQ_VOIP_START,
	EM_BIZMSG_NET_REQ_VOIP_STOP,
	EM_BIZMSG_NET_REG_CB,
	EM_BIZMSG_NET_UNREG_CB,
	//csp modify 20130423
	EM_BIZMSG_NET_REQ_PREVIEW_THIRD_START,
	EM_BIZMSG_NET_REQ_PREVIEW_THIRD_STOP,

	EM_BIZMSG_VIDEOMAIN_RECSTATUS= 1000,
	EM_BIZMSG_AUDIO_RECSTATUS,

	EM_BIZMSG_ENCDATA = 2000,

	EM_BIZMSG_EVENT_CB = 10000,
	EM_BIZMSG_EVENT_DISPATCH,

	EM_BIZMSG_GETPARALIST = 20000,
	EM_BIZMSG_GETPARAINDEX,
	EM_BIZMSG_GETPARAVALUE   
} EMBIZMSGTYPE;

//������Ϣͷ�ṹ
typedef struct
{
	EMBIZMSGTYPE emMsgType;
	void* pData;			
} SBizMsgHeader;


//ҵ������ṹ��
typedef struct
{
	SBizInitPara sBizPara; //ҵ����ʼ������
	SMsgCmdHdr psMsgCmdHdr; //ҵ��������Ϣ����(һ��ֻ��ִ��һ��)
	SMsgCmdHdr psMsgCmdInner; //�ڲ���Ϣ
	
	u8 nVideoMainNum;
	u8 nVideoSubnum;
	u8 nAudioNum;
	u8 nSnapNum;
	u8 nSensorNum;
	u8 nAlarmOutNum;
	u8 nBuzzNum;
	u8 nTalkNum;
	
	u8* pnTalkStatus;
	
	u8* pnVideoMainRecStatus;
	u8* pnVideoMainNetStatus;
	
	u8* pnVideoSubNetStatus;
	u8* pnVideoSubNetMbStatus;
	
	//csp modify 20140525
	u8* pnVideoThirdNetStatus;
	u8* pnVideoThirdNetMbStatus;
	
	u8* pnAudioRecStatus;
	u8* pnAudioNetStatus;
	
	u8 *pnSensorStatus;
	u8* pnVMotionStatus;
	u8* pnVLostStatus;
	u8* pnVBlindStatus;
	
	u8 nPtzProtocolNum;
	u8** pProtocolList;
	
	PbMgrHandle hPb;
	AlaHdr hAla;
	UserCenter hUser;
	
	pthread_t thrdDispatch;
	
	u8 nNVROrDecoder;	//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
} SBizManager;

extern SBizManager g_sBizManager;

s32 DoBizInit(SBizManager* psBizManager , SBizInitPara* psBizPara);
s32 DoBizDeinit(SBizManager* psBizManager);
s32 SendBizEvent(SBizEventPara* psBizEventPara);
s32 SendBizInnerMsgCmd(EMBIZMSGTYPE emBizMsgType, void* pData);
s32 SendBizMsgCmd(EMBIZMSGTYPE emBizMsgType, void* pData);

s32 SysMainTain();

s32 IsPAL(void);
void *BizDispatchFxn(void* arg);
void *BizDispatchInnerFxn(void* arg);
void SetNoAuthFuncCB(CHECKNOAUTH func);
u8 BizCheckAllHDDFormated();

//s32 GetTimeTick();

s32 DealCmdWithoutMsg(SBizEventPara* sBizEventPara);

#ifdef __cplusplus
}
#endif

#endif

