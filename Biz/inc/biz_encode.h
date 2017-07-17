#ifndef _BIZ_ENCODE_H_
#define _BIZ_ENCODE_H_

#include "biz.h"
#include "mod_encode.h"
#include "biz_manager.h"

typedef enum
{
	EM_BIZ_ENCODE_BOARD_COMMON 			= 0,	//ͨ��
	EM_BIZ_ENCODE_BOARD_HISI3520_16D1 	= 1,	//���hisi3520оƬ16·d1ϵ�� (R9616S)
	//
}EMBIZENCODEBOARDTYPE;

typedef struct
{
	u32 nMainNum; 			//��Ƶ������ͨ����
	u32 nMainMaxFrameSize;		//���������֡��С
	u32 nSubNum; 			//��Ƶ������ͨ����
	u32 nSubMaxFrameSize;		//���������֡��С
	u32 nSnapNum; 			//��Ƶץͼ����ͨ����
	u32 nSnapMaxFrameSize;		//ץͼ���֡��С
	u32 nAudioNum; 			//��Ƶ����ͨ����
	u32 nAudioMaxFrameSize;		//��Ƶ���֡��С
	EMBIZENCODEBOARDTYPE emChipType;
} SBizEncode;

typedef struct
{
    u8 nChn;
    EMCHNTYPE emChnType;
    SEncodeHead* psEncodeHead;
} SBizEncodeData;

typedef enum
{
    EM_BIZ_ENCODE_OSD_CHNNAME = 0,
    EM_BIZ_ENCODE_OSD_TIME,
    EM_BIZ_ENCODE_OSD_EXT1,
    EM_BIZ_ENCODE_OSD_NUM,
} EMBizENCODEOSDTYPE;


s32 EncodeInit(u8 nEnable, SBizEncode* psInitPara);
s32 EncodeOsdInit();
s32 EncodeChangeOsdResolution(u8 nChn);
s32 EncodeChangeOsdPosition(u8 nChn, EMBizENCODEOSDTYPE emOsdType, u16 nX, u16 nY);
s32 EncodeOsdShow(u8 nChn, EMBizENCODEOSDTYPE emOsdType, u8 bShow);
s32 VEncMainSetParam(u8 nChn, SBizEncodePara* psEncodeParam);
s32 VEncSubSetParam(u8 nChn, SBizEncodePara* psEncodeParam);
s32 EncodeGetParam(u8 nChn, EMCHNTYPE emChnType, PARAOUT SBizEncodePara* psEncodeParam);
s32 EncodeRequestStart(u8 nChn, EMCHNTYPE emChnType);
s32 EncodeRequestStartWithoutRec(u8 nChn, EMCHNTYPE emChnType);
s32 EncodeRequestStop(u8 nChn, EMCHNTYPE emChnType);
s32 EncodeRequestStopWithoutRec(u8 nChn, EMCHNTYPE emChnType);
s32 EncodeTimeOsdSetAllChn(u8* strTime);
s32 EncodeStrOsdUpdate(u8 chn, u8* strOsd);

#endif

