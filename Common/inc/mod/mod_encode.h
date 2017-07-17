#ifndef _ENCODE_COMMON_H_
#define _ENCODE_COMMON_H_

#include "common_basetypes.h"
#include "common_geometric.h"
#include "common_mutex.h"
#include "common_semaphore.h"
#include "common_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	EM_PIC_LEVEL_BEST = 0,
	EM_PIC_LEVEL_BETTER,
	EM_PIC_LEVEL_GOOD,
	EM_PIC_LEVEL_NORMAL,
	EM_PIC_LEVEL_BAD,
	EM_PIC_LEVEL_WORSE,
	//EM_PIC_LEVEL_WORST,
} EMPICLEVEL;

typedef enum
{
	EM_CHN_VIDEO_SUB = 0, 		//��Ƶ��ͨ��
	EM_CHN_VIDEO_MAIN, 			//��Ƶ��ͨ��
	EM_CHN_AUDIO, 				//��Ƶͨ��
	EM_CHN_TALK, 				//�Խ�ͨ��
	EM_CHN_VIDEO_SNAP, 			//��Ƶץͼ
	//csp modify 20130423
	EM_CHN_VIDEO_THIRD, 		//��Ƶ����·����
} EMCHNTYPE;

typedef enum
{
	//��Ƶ��������
	EM_ENCODE_VIDEO_MJPEG = 26, 		//Motion JPEG
	EM_ENCODE_VIDEO_H261 = 31, 		//H.261
	EM_ENCODE_VIDEO_MP2 = 33, 			//MPEG2 video
	EM_ENCODE_VIDEO_H263 = 34, 		//H.263
	EM_ENCODE_VIDEO_MP4 = 97, 			//MPEG-4
	EM_ENCODE_VIDEO_H264 = 98, 		//H.264
	
	//������������
	EM_ENCODE_AUDIO_PCMU = 0, 			//G.711 ulaw
	EM_ENCODE_AUDIO_PCMA = 8, 			//G.711 Alaw
	EM_ENCODE_AUDIO_G722 = 9, 			//G.722
	EM_ENCODE_AUDIO_G7231 = 4, 		//G.7231
	EM_ENCODE_AUDIO_G728 = 15, 		//G.728
	EM_ENCODE_AUDIO_G729 = 18, 		//G.729
	EM_ENCODE_AUDIO_RAW = 19, 			//raw audio
	EM_ENCODE_AUDIO_ADPCM = 20, 		//adpcm
	EM_ENCODE_AUDIO_ADPCM_HS = 21, 	//��˼adpcm
	EM_ENCODE_AUDIO_MP3 = 96,			//mp3
} EMENCODETYPE;

typedef enum
{
	EM_VIDEO_QCIF = 0,		//PAL:176x144, NTSC:176x112
	EM_VIDEO_CIF = 1,		//PAL:352x288, NTSC:352x240
	EM_VIDEO_HALF_D1 = 2,   //PAL:704*288, NTSC:704x240	
	EM_VIDEO_4CIF = 3,		
	EM_VIDEO_D1 = 4,		//PAL:704x576, NTSC:704x480
	EM_VIDEO_720P = 5,		//1280*720
	EM_VIDEO_1080P = 6,		//1920*1080
	EM_VIDEO_960H = 7,		//1920*1080
	EM_VIDEO_VGA = 8,		//640*480
} EMVIDEORESOLUTION;

typedef enum
{
	EM_FRAME_TYPE_P = 0, /*PSLICE types*/
	EM_FRAME_TYPE_I = 1, /*ISLICE types*/
} EMVIDEOFRAMETYPE;

typedef enum
{
	EM_BITRATE_VARIABLE = 0, 		//������
	EM_BITRATE_CONST, 				//������
} EMBITRATETYPE;

typedef enum
{
	EM_ENCODE_BOARD_COMMON 			= 0,	//ͨ��
	EM_ENCODE_BOARD_HISI3520_16D1 	= 1,	//���hisi3520оƬ16·d1ϵ�� (R9616S)
	//
} EMENCODEBOARDTYPE;

typedef struct
{
	EMENCODETYPE emEncodeType; 				//�ο� EMENCODETYPE
	EMVIDEOFRAMETYPE emVideoFrameType; 		//�ο� EMVIDEOFRAMETYPE
	u64 nTimeStamp; 						//ʱ��� ms
	u32 nLen;
	char* pData;
	EMVIDEORESOLUTION emResol;
	u32 nBitRate;
	//u8 nReserve[8];
	u16 width;
	u16 height;
	u8 nReserve[4];
} SEncodeHead;

typedef void (* PFNEncodeRequest)(SEncodeHead* sEncodeHead, u32 dwContent);

//csp modify 20130106
typedef void (* PFNEncodeEXCEPTION)(u8 nType);

typedef struct
{
	u32 nMainNum; 			//��Ƶ������ͨ����
	u32 nMainMaxFrameSize;	//���������֡��С
	u32 nSubNum; 			//��Ƶ������ͨ����
	u32 nSubMaxFrameSize;	//���������֡��С
	u32 nSnapNum; 			//��Ƶץͼ����ͨ����
	u32 nSnapMaxFrameSize;	//ץͼ���֡��С
	u32 nAudioNum; 			//��Ƶ����ͨ����
	u32 nAudioMaxFrameSize;	//��Ƶ���֡��С
	
	u32 nTalkNum; 			//�Խ�����ͨ����
	u32 nTalkMaxFrameSize;	//�Խ����֡��С
	
	EMENCODEBOARDTYPE emBoardType;//��������
	
	u8 nReserve[4]; 		//�����ֶ�
	
	//csp modify 20130106
	PFNEncodeEXCEPTION EncExceptCB;
	
	//csp modify 20130316
	u8 nVideoStandard;		//��Ƶ��ʽ(EMBIZVIDSTANDARD)
} SEncodeCapbility;

typedef struct 
{
	u32 nBitRate; 							//����λ�� ��KBΪ��λ �����ֽ���ΪnBitRate << 10��
	u32 nFrameRate; 						//����֡��
	u32 nGop;								//�ؼ�֡���
	u32 nMinQP;								//��СQPֵ
	u32 nMaxQP;								//���QPֵ
	EMVIDEORESOLUTION emVideoResolution;	//�ο� EMVIDEORESOLUTION
	EMENCODETYPE emEncodeType; 				//�ο� EMENCODETYPE
	EMBITRATETYPE emBitRateType; 			//�ο� EMBITRATETYPE
	EMPICLEVEL emPicLevel;					//�ο�EMPICLEVEL
	u8 nReserve[8];
} SVideoParam;

typedef struct
{
	
} SVideoSnapParam;

typedef struct
{
	EMENCODETYPE emEncodeType; 			//�ο� EMENCODETYPE
	u32 nSampleRate; 					//��Ƶ������
	u32 nSampleDuration; 				//��Ƶ����ʱ��
	u32 nSampleSize; 					//��Ƶ��������
	u8 nBitWidth; 						//��Ƶλ��
	u8 nReserve[3];
} SAudioParam;

typedef	struct
{
	u8 nForce; //��������ĸı����ֹͣ�����Ƿ�ǿ��ֹͣ //Ĭ��Ҫ����0
	union
	{
		SVideoParam sVideoMainParam; 		//�ο�SVideoMainParam
		SVideoParam sVideoSubParam; 		//�ο�SVideoSubParam
		SVideoSnapParam sVideoSnapParam; 	//�ο�SVideoSnapParam
		SAudioParam sAudioParam; 			//�ο�SAudioParam
	};
} SEncodeParam;

typedef enum
{
	EM_ENCODE_FONTSIZE_SMALL,
	EM_ENCODE_FONTSIZE_NORMAL,
	EM_ENCODE_FONTSIZE_ALL
} EMENCODEFONTSIZE;

typedef enum
{
    EM_ENCODE_OSD_CHNNAME = 0,
    EM_ENCODE_OSD_TIME,
    EM_ENCODE_OSD_EXT1,
    EM_ENCODE_OSD_NUM,
} EMENCODEOSDTYPE;

typedef s32 (* PFNEncodeTextToOsd)(PARAOUT u8* pBuffer, s32 w, s32 h, s8* strOsd, EMENCODEFONTSIZE emSize);

//csp modify 20130423
typedef struct
{
	u8	support;
	u8	wnd_num_threshold;
	EMVIDEORESOLUTION emResolution;
} SThirdStreamProperty;

s32 ModEncodeInit(SEncodeCapbility* psEncodeCap);
s32 ModEncodeDeinit(void);
s32 ModEncodeSetParam(u8 nChn, EMCHNTYPE emChnType, SEncodeParam* psEncodeParam);
s32 ModEncodeGetParam(u8 nChn, EMCHNTYPE emChnType, SEncodeParam* psEncodeParam);
s32 ModEncodeRequestStart(u8 nChn, EMCHNTYPE emChnType, PFNEncodeRequest pfnEncodeCB, u32 nContent);
s32 ModEncodeRequestStop(u8 nChn, EMCHNTYPE emChnType);

s32 ModEncodeOsdInit(u8 nChn, EMENCODEOSDTYPE emOsdType, SRect* pRect, SRgb* pBgColor);
s32 ModEncodeOsdDeinit();
s32 ModEncodeOsdRegistCB(PFNEncodeTextToOsd pfnGetOSDCB);
s32 ModEncodeOsdResolRef(u8 nChn, EMVIDEORESOLUTION emReSolRef);
s32 ModEncodeOsdSet(u8 nChn, EMENCODEOSDTYPE emOsdType, SRect* pRect, EMENCODEFONTSIZE emSize, s8* strOsd);
s32 ModEncodeOsdShow(u8 nChn, EMENCODEOSDTYPE emOsdType, u8 nShow);

s32 ModEncodeGetKeyFrameEx(u8 nChn, SVideoParam* psEncodeParam, u8 bSub);

//csp modify 20130423
s32 ModEncodeSupportThirdStream();

//csp modify 20130423
s32 ModEncodeSetThirdStreamProperty(SThirdStreamProperty *psProperty);
s32 ModEncodeGetThirdStreamProperty(SThirdStreamProperty *psProperty);

//yaogang modify 20150212
s32 ModEncSnapCreateChn(u8 EncChn, int Width, int Height);
s32 ModEncSnapDestroyChn(u8 EncChn);
//s32 ModEncGetSnapshot(u8 StreamChn, u8 EncChn, u8 *pdata, u32 *psize);
s32 ModEncGetSnapshot(u8 EncChn, u16 *pWidth, u16 *pHeight, u8 *pdata, u32 *psize);




#ifdef __cplusplus
}
#endif


#endif

