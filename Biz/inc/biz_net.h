#ifndef __BIZNET_H_
#define __BIZNET_H_

#include "biz.h"
#include "netcomm.h"
#include "biz_manager.h"
#include "mediastream.h"
#include "CmdCallback.h"
#include "sg_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define 

#define PAL 12
#define NTSC 10

#define AUDIO_SAMPLE_BITWIDTH		16

#define CHN_MAX						16

#define BIZ_NET_ONLINE_MAX 			1024
#define BIZ_NET_USER_NAME_MAX		15

//#define MAX_FRAME_SIZE				(s32)256*1024//zlb20100802

#define CUR_STARDMODE	PAL

// typdef 

typedef struct
{
	u8 		nChNum;
	
	//csp modify 20121130
	//int 	nVs;
	u16		reserved;
	u8		nLangId;
	u8		nVs;
	
	u32 	nFrameSizeMax;
	
	char 	device_name[32];				//�豸����
	char 	device_mode[32];				//�豸�ͺ�
	u8		maxChnNum;						//���ͨ����
	u8		maxAduioNum;					//�����Ƶ��
	u8		maxPlaybackNum;					//���ط���
	u8		maxAlarmInNum;					//��󱨾�������
	u8		maxAlarmOutNum;					//��󱨾������
	u8		maxHddNum;						//���Ӳ����
	u8 		nNVROrDecoder;	//Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
} SNetSysPara;

// extern ..

s32 NetInit(void* pHddMgr, SNetSysPara* pPara);

s32 bizNet_Init();
void bizNet_RequestPreviewCB(STcpStreamReq* param);
void bizNet_remoteplayCB(
	void* pFileHandle,
	PSRmtPlayFileOp pParam
);

typedef struct _sBizNetOnlineUsr
{
	u32  nId;
	u32  nIP;
	u64  nMac;
	char szName[BIZ_NET_USER_NAME_MAX];
	char szPass[BIZ_NET_USER_NAME_MAX];
} SBNOnlineUser;

s32 BizNetAddLoginUser( SBNOnlineUser* pUser );
s32 BizNetDelLoginUser( u32 nId );

s32 BizNetGetLoginUserID( SBNOnlineUser* pUser );
void BizNetUpdateProgressNoitfyCB(u8 nProgress);
void BizNetStopEnc(void);

#ifdef __cplusplus
}
#endif


#endif // __A_H_


