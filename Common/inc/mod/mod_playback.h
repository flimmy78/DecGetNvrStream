#ifndef _MOD_PLAYBACK_H_
#define _MOD_PLAYBACK_H_

#ifndef PACK_NO_PADDING
#define PACK_NO_PADDING  __attribute__ ((__packed__))
#endif

typedef void* PbMgrHandle;

typedef struct 
{
	u8   nMaskType;
	u32  nMaskChn;
	u32  nStartTime;
	u32  nEndTime;
	void* pReserverInfo;
} SPBSearchPara;

typedef enum
{
	EM_CTL_NULL=0,
	EM_CTL_STOP,
	EM_CTL_PAUSE,
	EM_CTL_RESUME,
	EM_CTL_STEP,
	EM_CTL_PRE_SECT,
	EM_CTL_NXT_SECT,
	EM_CTL_SPEED_DOWN,
	EM_CTL_SPEED_UP,
	EM_CTL_SET_SPEED,
	EM_CTL_SEEK,
	EM_CTL_FORWARD,
	EM_CTL_BACKWARD,
	EM_CTL_MUTE,
	EM_CTL_PBTIME,
	EM_CTL_PBFILE
} EmPBCtlCmd;

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

typedef struct
{
    u8 nChn;
    u8 nType;
    u32 nStartTime;
    u32 nEndTime;
    u8 nFormat;  //N��ʽ�� 10 = D1��13 = CIF  P��ʽ�� 0 = D1�� 3 = CIF
    u8 nStreamFlag; //0:��Ƶ��;1:��Ƶ��
    u32 nSize;
    u32 nOffset;
    u8 nDiskNo;
    u8 nPtnNo;
    u16 nFileNo;
} PACK_NO_PADDING SPBRecfileInfo;

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
} PACK_NO_PADDING SPBRecSnapInfo;



typedef enum										//��������ö��
{
	EM_PLAYRATE_1 = 1,								//��������
	EM_PLAYRATE_2 = 2,								//2x���
	EM_PLAYRATE_4 = 4,								//4x���
	
	//csp modify 20130429
	//EM_PLAYRATE_MAX = 8,							//8x���
	EM_PLAYRATE_8 = 8,								//8x���
	EM_PLAYRATE_16 = 16,							//16x���
	EM_PLAYRATE_32 = 32,							//32x���
	EM_PLAYRATE_64 = 64,							//64x���
	
	EM_PLAYRATE_1_2 = -2,							//1/2x����
	EM_PLAYRATE_1_4 = -4,							//1/4x����
	EM_PLAYRATE_MIN = -8,							//1/8x����
	EM_PLAYRATE_SINGLE = 0,							//֡��
	EM_PLAYRATE_BACK_1 = -1,						//1x�˷�
}EmPlayRate;

typedef struct
{
	BOOL bStop;
	u32 nProg;
	u32 nTotalTime;
	u64 nCurTime;
}SPBCallbackInfo;

typedef struct
{
	u32 nMaxFrameSize;
	u64  nChnMaskOfD1;	
}SPBInitPara;

typedef void (* PBPROGFUNC)(SPBCallbackInfo* cbInfo);
typedef void (* PBREGFUNCCB)(void);

#ifdef __cplusplus
extern "C" 
{
#endif

//��ʼ���ط�ģ��
PbMgrHandle ModPlayBackInit(u32 nMaxChn, void *pContent, u64 nChnMaskOfD1);

//��ʼ��ʱ��ط� 
s32 ModPlayBackByTime(PbMgrHandle hPbMgr, SPBSearchPara* pSearchParam);

//���ļ��ط�
s32 ModPlayBackByFile(PbMgrHandle hPbMgr, SPBRecfileInfo* pFileInfo);

//�طſ��� 
s32 ModPlayBackControl(PbMgrHandle hPbMgr, EmPBCtlCmd emPBCtlcmd, s32 nContext);

//�ͷŻط�ģ��
s32 ModPlayBackDeinit(PbMgrHandle hPbMgr);

//�طŽ���
s32 ModPlayBackProgress(PBPROGFUNC pbProgFunc);

//ע��ص�����,����type: 0,�رյ��ӷŴ�
s32 ModPlayBackRegistFunCB(u8 type, PBREGFUNCCB func); 

//��ȡ�ط�ʱ��ͨ���Ƿ����ļ�Ҫ�ط�
s32 ModPlayBackGetRealPlayChn(PbMgrHandle hPbMgr, u64* pChnMask);

//���õ�ǰ�Ƿ��������Խ�����Ҫ���λطŵ���Ƶ
s32 ModPlayBackSetModeVOIP(PbMgrHandle hPbMgr, u8 nVoip);

/*�ط�ʱ��ͨ���Ŵ�/�ָ� (˵��:nKey<0,�Զ����ηŴ���һ��ͨ��; 
							  nKey>=���ͨ����,�ָ���ͨ���ط�;
							  nKey����,�Ŵ��nKey������)*/
s32 ModPlayBackZoom(PbMgrHandle hPbMgr, s32 nKey);

/*��ȡĳͨ���ļ��ĸ�ʽ �� ����ֵ: 0,D1; 3,cif*/
s32 ModPlayBackGetVideoFormat(PbMgrHandle hPbMgr, u8 nChn);

//yaogang modify 20150112
//s32 ModSnapDisplay(PbMgrHandle hPbMgr, SPBRecSnapInfo* pSnapInfo);
s32 ModSnapDisplay(SPBRecSnapInfo* pSnapInfo);


#ifdef __cplusplus
}
#endif

#endif  //_PLAYBACK_H_

