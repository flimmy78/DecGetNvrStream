#ifndef _BIZ_PLAYBACK_H_
#define _BIZ_PLAYBACK_H_

#include "biz.h"
#include "mod_playback.h"
#include "biz_manager.h"

typedef struct
{
	u8 nMaxChn;
	HDDHDR hDisk;
	u64 nChnMaskOfD1;
} SBizPlayback;

s32 PlaybackInit(u8 nEnable, SBizPlayback* psInitPara);

//yaogang modify 20150112
s32 SnapDisplay(SBizRecSnapInfo *pSnapInfo);

//��ʼ�ط�
s32 PlaybackStart(SBizPbStart *psPbStart);
//�طſ��� 
s32 PlayBackControl(SBizPbControl* psBizPbCtl);

/*�ط�ʱ��ͨ���Ŵ�/�ָ� (˵��:nKey<0,�Զ����ηŴ���һ��ͨ��; 
							  nKey>=���ͨ����,�ָ���ͨ���ط�;
							  nKey����,�Ŵ��nKey������)*/
s32 PlayBackZoom(s32 nKey);

s32 PlayBackGetRealPlayChn(u64* pChnMask);

s32 PlayBackSetModeVOIP(u8 nVoip);

s32 PlayBackGetVideoFormat(u8 nChn);

//ע��ص�����,����type: 0,�رյ��ӷŴ�
s32 PlayBackRegistFunCB(u8 type, PBREGFUNCCB func); 

#endif

