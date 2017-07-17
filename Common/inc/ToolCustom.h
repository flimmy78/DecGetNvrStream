
#ifndef _TOOL_CUSTOM_H_
#define _TOOL_CUSTOM_H_

//create by lbzhu, 2011-11-23
#include "common_basetypes.h"

//for nVideoResolution ȡֵ
#define VIDENC_RESOL_QCI 0 //PAL:176x144, NTSC:176x112
#define VIDENC_RESOL_CIF 1 //PAL:352x288, NTSC:352x240
#define VIDENC_RESOL_HALF_D1 2  //PAL:704*288, NTSC:704x240	
#define VIDENC_RESOL_4CIF 3 //PAL:704x576, NTSC:704x480
#define VIDENC_RESOL_D1 4 //PAL:704x576, NTSC:704x480

//for nEncodeType ȡֵ
//��Ƶ��������
#define VIDENC_FORMAT_MJPEG 26 //Motion JPEG �ݲ�֧��
#define VIDENC_FORMAT_H261 31 //H.261 �ݲ�֧��
#define VIDENC_FORMAT_MP2 33 //MPEG2 video �ݲ�֧��
#define VIDENC_FORMAT_H263 34 //H.263 �ݲ�֧��
#define VIDENC_FORMAT_MP4 97 //MPEG-4 �ݲ�֧��
#define VIDENC_FORMAT_H264 98 //H.264 

//for nCbrOrVbr ȡֵ
#define VIDENC_VBR 0 //������
#define VIDENC_CBR 1 //������

//for nPicLevel ȡֵ
#define VIDENC_PIC_LEVEL_BEST 0
#define VIDENC_PIC_LEVEL_BETTER 1
#define VIDENC_PIC_LEVEL_GOOD 2
#define VIDENC_PIC_LEVEL_NORMAL 3
#define VIDENC_PIC_LEVEL_BAD 4
#define VIDENC_PIC_LEVEL_WORSE 5
//#define VIDENC_PIC_LEVEL_WORST 6

//for nChnStart/nChnEnd ȡֵ
#define SET_ALLCHN 255 //��ʾ�������ͨ��
//!= SET_ALLCHN ��ʾ��Ծ���ĳ��ͨ��

//================================================
typedef struct
{
	u8 nChnStart; //��ʼĿ��ͨ��//�ο�for nChnStart/nChnEnd
	u8 nChnEnd; //����Ŀ��ͨ��//�ο�for nChnStart/nChnEnd

	u8 nFrameRate; //����֡��
	u8 nReserve0;
	u32 nBitRate; //����λ�� ��KBΪ��λ �����ֽ���ΪnBitRate << 10��
	u8 nVideoResolution;	//�ο� for nVideoResolution ȡֵ
	u8 nEncodeType;  //�ο� for nEncodeType ȡֵ ��ǰ��֧��VIDENC_FORMAT_H264
	u8 nCbrOrVbr;  //�ο� for nCbrOrVbr ȡֵ
	u8 nPicLevel; //�ο�for nPICLEVEL ȡֵ
	u32 nGop; //�ؼ�֡���//�ݲ�֧��
	u32 nMinQP; //��СQPֵ//�ݲ�֧��
	u32 nMaxQP;	 //���QPֵ//�ݲ�֧��
	u8 nReserve[8];
} SVidEncParam;

typedef struct
{
	u8 nChnStart; //��ʼĿ��ͨ��//�ο�for nChnStart/nChnEnd
	u8 nChnEnd; //����Ŀ��ͨ��//�ο�for nChnStart/nChnEnd

	u8 nHue; //��ͨ��ɫ��(���֣�0-255)
	u8 nSaturation; //���Ͷ�(���֣�0-255)
	u8 nContrast; //�Աȶ�(���֣�0-255)
	u8 nBrightness; //����(���֣�0-255)

	u8 nReserve[10];
} SVidPreviewColor;

//for nVideoStandard
#define VIDEO_STANDARD_PAL 12
#define VIDEO_STANDARD_NTSC 10

#define LANGUAGE_SET_NUM_MAX 30

//for nLanguageId ȡֵ
#define LANGUAGE_ID_ENG 0 //Ӣ��
#define LANGUAGE_ID_SCH 1 //����
#define LANGUAGE_ID_TCH 2 //����
#define LANGUAGE_ID_HAN 3 //����
#define LANGUAGE_ID_JAP 4 //����
#define LANGUAGE_ID_GER 5 //����
#define LANGUAGE_ID_RUS 6 //����
#define LANGUAGE_ID_FRE 7 //����
#define LANGUAGE_ID_POR 8 //��������
#define LANGUAGE_ID_TUR 9 //��������
#define LANGUAGE_ID_SPA 10 //��������
#define LANGUAGE_ID_ITA 11 //�������
#define LANGUAGE_ID_POL 12 //����
#define LANGUAGE_ID_POS 13 //��˹
#define LANGUAGE_ID_THAI 14//̩��

//csp modify 20121224
#define LANGUAGE_ID_MAGYAR 15//��������
#define LANGUAGE_ID_SLOVAKIA 16//˹�工����
#define LANGUAGE_ID_VIETNAM 17//Խ����
#define LANGUAGE_ID_GREECE 18//ϣ����
//lcy add
#define LANGUAGE_ID_HEBREW 19//ϣ������

//csp modify 20121224
//#define LANGUAGE_ID_NUM 15 //����֧����
//#define LANGUAGE_ID_NUM 16 //����֧����
//#define LANGUAGE_ID_NUM 17 //����֧����
//#define LANGUAGE_ID_NUM 18 //����֧����
//#define LANGUAGE_ID_NUM 19 //����֧����
#define LANGUAGE_ID_NUM 20 //����֧����

typedef struct
{
	u8 nLanguageIdNum; //ʵ��������
	u8 nLanguageIdDefault; //�ο�for nLanguageId ȡֵ
	u8 nLanguageIdList[LANGUAGE_SET_NUM_MAX]; //�ο�for nLanguageId ȡֵ
	u8 reserve[32];
} SLanguageParam;

//=========��������ͷ�����Ҫ�úù���ֵҪΨһ===

#define EMCUSTOMTYPE int

//EMCUSTOMTYPE Begin=================
#define EM_CUSTOM_BASE 0
#define EM_CUSTOM_LOGO_START 1 //����logo ��������
#define EM_CUSTOM_LOGO_APP 2  //Ӧ��ϵͳlogo ��������
#define EM_CUSTOM_PANEL_REMOTE_CTRL 3 //���ң��������

#define EM_CUSTOM_DEVICE_MODEL 10 //�豸�ͺ�
#define EM_CUSTOM_DEVICE_VIDEOSTAND 11 //�豸��ƵԴ��ʽ
#define EM_CUSTOM_DEVICE_LANGUAGE 12 //���Լ�Ĭ��ֵ��ʡ��
#define EM_CUSTOM_DEVICE_GETFONT 13 //�豸����Ч

//���Է��붨��(50~99)
#define EM_CUSTOM_TRANSLATE_BASE 50
#define EM_CUSTOM_TRANSLATE_ENG (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_ENG) //Ӣ��
#define EM_CUSTOM_TRANSLATE_SCH (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_SCH) //����
#define EM_CUSTOM_TRANSLATE_TCH (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_TCH) //����
#define EM_CUSTOM_TRANSLATE_HAN (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_HAN) //����
#define EM_CUSTOM_TRANSLATE_JAP (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_JAP) //����
#define EM_CUSTOM_TRANSLATE_GER (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_GER) //����
#define EM_CUSTOM_TRANSLATE_RUS (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_RUS) //����
#define EM_CUSTOM_TRANSLATE_FRE (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_FRE) //����
#define EM_CUSTOM_TRANSLATE_POR (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_POR) //��������
#define EM_CUSTOM_TRANSLATE_TUR (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_TUR) //��������
#define EM_CUSTOM_TRANSLATE_SPA (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_SPA) //��������
#define EM_CUSTOM_TRANSLATE_ITA (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_ITA) //�������
#define EM_CUSTOM_TRANSLATE_POL (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_POL) //����
#define EM_CUSTOM_TRANSLATE_POS (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_POS) //��˹
#define EM_CUSTOM_TRANSLATE_THAI (EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_THAI) //̩��

//csp modify 20121224
#define EM_CUSTOM_TRANSLATE_MAGYAR		(EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_MAGYAR) //��������
#define EM_CUSTOM_TRANSLATE_SLOVAKIA	(EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_SLOVAKIA) //˹�工����
#define EM_CUSTOM_TRANSLATE_VIETNAM		(EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_VIETNAM) //Խ����
#define EM_CUSTOM_TRANSLATE_GREECE		(EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_GREECE) //ϣ����
//lcy add
#define EM_CUSTOM_TRANSLATE_HEBREW		(EM_CUSTOM_TRANSLATE_BASE + LANGUAGE_ID_HEBREW) //ϣ������

#define EM_CUSTOM_TRANSLATE_TOP 99

#define EM_CUSTOM_DEVICE_IP 100  //�豸Ĭ��IP
#define EM_CUSTOM_DEVICE_MAC 101 //�豸Ĭ��MAC
#define EM_CUSTOM_DEVICE_GATEWAY 102  //�豸Ĭ������

//for ͨ������
#define EM_CUSTOM_VIDENC_MAIN_BASE 1000  //�������������Ĭ��ֵ
#define EM_CUSTOM_VIDENC_MAIN_TOP 1099  //������������ƺ�
	
#define EM_CUSTOM_VIDENC_SUB_BASE 1100  //�������������Ĭ��ֵ
#define EM_CUSTOM_VIDENC_SUB_TOP 1199  //������������ƺ�
	
#define EM_CUSTOM_PREVIEW_COLOR_BASE 1200  //Ԥ��ͼ�����Ĭ��ֵ
#define EM_CUSTOM_PREVIEW_COLOR_TOP 1299 //���Ԥ��ͼ��������ƺ�

#define EM_CUSTOM_ALL 0xffffffff  //��ʾ���ж�������(����Ҫ�����ж��ƴ���ʱ��Ч)
//EMCUSTOMTYPE End=================

typedef union //���Ʋ���
{
	char strFilePath[256]; //EM_CUSTOM_LOGO_START, EM_CUSTOM_LOGO_APP, EM_CUSTOM_TRANSLATE_XXX
	u8 strData[32]; //EM_CUSTOM_DEVICE_MODEL, EM_CUSTOM_DEVICE_IP, EM_CUSTOM_DEVICE_MAC, EM_CUSTOM_DEVICE_GATEWAY
	u8 nVideoStandard; //EM_CUSTOM_DEVICE_VIDEOSTAND
	SVidEncParam sVidEncParam;//EM_CUSTOM_VIDENC_MAIN_BASE~EM_CUSTOM_VIDENC_MAIN_TOP, EM_CUSTOM_VIDENC_SUB_BASE ~ EM_CUSTOM_VIDENC_SUB_TOP
	SVidPreviewColor sVidPreviewColor; //EM_CUSTOM_PREVIEW_COLOR_BASE ~ EM_CUSTOM_PREVIEW_COLOR_TOP
	SLanguageParam sLanguageParam; //EM_CUSTOM_DEVICE_LANGUAGE
} SCustomData;

//==========���������ṹ���뱣�ֲ���===============
typedef struct
{
	EMCUSTOMTYPE emType; //�ο�EMCUSTOMTYPE ��Ӧ��ʹ�õ�SCustomData�ο�SCustomData
	u32 nOffSet;	//����ڱ����Ʒ�����ƫ����
	u32 nLen; //�����ļ�����
} SCustomInfo;
//================================================

//StrFileNameΪ�����ļ�;�豸��������ռ���
s32 CustomOpen(char* strFileName);
s32 CustomGenerate(EMCUSTOMTYPE emType, SCustomData* psCustomData); //���õ�����������
s32 CustomCancel(EMCUSTOMTYPE emType); //ȡ��ĳ���
s32 CustomGetCount(void);
s32 CustomGetInfo(s32 nIndex, SCustomInfo* psCustomInfo);
s32 CustomGetLogo(s32 nIndex, u32 nMaxLen, u8* pData, u32 *pDataLen); //���logo�ļ�����ʹ��
s32 CustomGetFile(s32 nIndex, char* strFileOut); //�������ݵ��ļ�
s32 CustomGetFile2(EMCUSTOMTYPE emType, char* strFileOut); //�������ݵ��ļ�
s32 CustomGetData(s32 nIndex, SCustomData* psCustomData); //�ýӿڲ�֧��EM_CUSTOM_LOGO_APP/EM_CUSTOM_LOGO_START/EM_CUSTOM_TRANSLATE_XXX��������
s32 CustomGetData2(EMCUSTOMTYPE emType, SCustomData* psCustomData); //�ýӿڲ�֧��EM_CUSTOM_LOGO_APP/EM_CUSTOM_LOGO_START/EM_CUSTOM_TRANSLATE_XXX��������

s32 CustomSave(void); //��������
s32 CustomClose(u8 nSave); //�ر�(nSave�����Ƿ񱣴�Ķ�)
#endif

