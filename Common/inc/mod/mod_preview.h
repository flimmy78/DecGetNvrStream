#ifndef _PREVIEW_H_
#define _PREVIEW_H_

#include "common_basetypes.h"
#include "common_geometric.h"
#include "common_mutex.h"
#include "common_semaphore.h"
#include "common_thread.h"
#include "common_debug.h"

#define PreviewMode pre_mode_e

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	EM_PREVIEW_1SPLIT = 1,
	EM_PREVIEW_4SPLITS = 4,
	EM_PREVIEW_6SPLITS = 6,
	EM_PREVIEW_8SPLITS = 8,
	EM_PREVIEW_9SPLITS = 9,
	EM_PREVIEW_10SPLITS = 10,
	EM_PREVIEW_12SPLITS = 12,
	EM_PREVIEW_13SPLITS = 13,
	EM_PREVIEW_16SPLITS = 16,
	EM_PREVIEW_25SPLITS = 25,//24,//nvr used
	EM_PREVIEW_24SPLITS = 24,//25,//nvr used
	EM_PREVIEW_36SPLITS = 36,//32,//nvr used
	EM_PREVIEW_32SPLITS = 32,//36,//nvr used
	EM_PREVIEW_64SPLITS = 64,
	EM_PREVIEW_HideItem = 253,//���ؿؼ�
	EM_PREVIEW_PIP = 254,
	EM_PREVIEW_CLOSE = 255,
} EMPREVIEWMODE;

//Ԥ��ɫ�ʲ���
typedef struct
{
	u16 nHue;
	u16 nSaturation;
	u16 nContrast;
	u16 nBrightness;
} SPreviewImagePara;

typedef struct
{
	u8 nBrightness;
	u8 nContrast;
	u8 nSaturation;
	u8 nHue;
} SVoImagePara;

//��Ѳ����
typedef struct
{
    EMPREVIEWMODE emPreviewMode; //��ѲԤ��ģʽ
    u8 nIsPatrol; //Ĭ���Ƿ�����Ѳ
    u8 nPatrolChnNum;//��Ѳͨ����16 32 48 64
    u8 nStops; //��Ѳվ��(��һ��ѭ���ܹ��ж��ٸ���ػ���);
    u16 nInterval; //��Ѳʱ����(s)
    u8 pnStopModePara[255]; //ÿվԤ��ģʽ����;�ο�SBizPreviewPara.nModePara��nStops==0xffʱ��Ч
} SPreviewPatrolPara;

//Ԥ���ص�����
typedef struct
{
    EMPREVIEWMODE emPreviewMode; //Ԥ�����ڷָ�ģʽ
    u8 nModePara; //extend parameter for EMPREVIEWMODE
    u8 nAdioChn;
    u8 nMute;
    u8 nVolume;
} SPreviewCbData;

//Ԥ���ص�����
typedef void (* FNPREVIEWCB)(SPreviewCbData* psPreviewCbData); 

//Ԥ���ص�����
typedef struct
{
    EMPREVIEWMODE emPreviewMode; //Ԥ�����ڷָ�ģʽ
    u8 nModePara; //Preview first chn,  extend parameter for EMPREVIEWMODE 
    
    //�ײ�ʹ�õ�ͨ����(VDEC & VO, 64·�����ʹ��)
    //u8 nModeLowChn;//ģ���ڲ�ά���ı���
} SPreviewPara;

//Ԥ����ʼ������
typedef struct
{
    u8 nVideoNum;
    u8 nAudioNum;
    u8 nMaxStrOsdNum;
    u8 nMaxImgOsdNum;
    u8 nMaxRectOsdNum;
    FNPREVIEWCB pfnPreviewCb;
} SPreviewInitPara;

//���ӷŴ�Ԥ������
typedef struct{
	unsigned int s32X;
	unsigned int s32Y;
	unsigned int u32Width;
	unsigned int u32Height;
}SPreviewElecZoom;

//Ԥ����ʼ��
s32 ModPreviewInit(SPreviewInitPara* psPreviewInitPara);

//����/ֹͣԤ��
s32 ModPreviewStart(SPreviewPara* psPreviewPara);
s32 ModPreviewStop(void);
//Ԥ��ͨ���Ŵ�
s32 ModPreviewZoomChn(u8 nChn);
//���ӷŴ�Ԥ��
s32 ModPreviewElecZoomChn(int flag, u8 nChn, SPreviewElecZoom* stCapRect);
s32 ModPreviewPbElecZoomChn(int flag, u8 nChn, SPreviewElecZoom* stCapRect);
//ͼƬԤ��
s32 ModPreviewSnap(int flag, SPreviewElecZoom* stCapRect);
//yaogang modify 20150212
s32 ModGetSnapshot(u8 chn);


//�ָ�Ԥ�����ϸ�״̬(������Ԥ��ͨ���Ŵ��ط�ֹͣ��)
s32 ModPreviewResume(u8 nForce); //����˵��: 0,��ǿ�ƻָ�,��Ԥ����û�����������κδ���
								 //			 ��0,ǿ�ƻָ�,��Ԥ����û��������ǿ�ƿ���Ԥ�����ٻָ�Ԥ��ģʽ����Ҫ���ڻطŽ�����ָ�Ԥ��
//Ԥ����ҳ
s32 ModPreviewNext(void);
//Ԥ����ҳ
s32 ModPreviewLast(void);

//Ԥ����������
s32 ModPreviewMute(u8 nEnable);
//Ԥ����������
s32 ModPreviewVolume(u8 nVlm);
//Ԥ����Ƶ�����������
s32 ModPreviewSetAudioOut(u8 nChn);

//Ԥ��ͼ��ɫ������/��ȡ
s32 ModPreviewSetImage(u8 nChn, SPreviewImagePara* psImagePara);
s32 ModPlayBackSetImage(SVoImagePara* psVoImagePara);
//s32 ModPreviewGetImage(u8 nChn, PARAOUT SPreviewImagePara* psImagePara);

//Ԥ����Ѳ��������
s32 ModPreviewSetPatrol(SPreviewPatrolPara* psPatrolPara);

//Ԥ����Ѳ��ʼ/����
s32 ModPreviewStartPatrol(void);
s32 ModPreviewStopPatrol(void);

s32 ModPreviewVoip(u8 nEnable);

//Ԥ�������ڸ���ʾ������
s32 ModPreviewRectOsdShow(u8 nChn, u8 nIndex, u8 nShow, SPRect pSRect);

//csp modify
s32 ModPreviewIsOpened(void);
s32 ModPreviewGetMode(void);

#ifdef __cplusplus
}
#endif

#endif

